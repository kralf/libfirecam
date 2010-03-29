/***************************************************************************
 *   Copyright (C) 2010 by Ralf Kaestner and Luciano Spinello              *
 *   ralf.kaestner@gmail.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "capture.h"

#include "utils.h"

/*****************************************************************************/
/* Statics                                                                   */
/*****************************************************************************/

FireCAMCapture::ModeStrings FireCAMCapture::modeStrings;
FireCAMCapture::ModePresets FireCAMCapture::modePresets;
FireCAMCapture::SpeedPresets FireCAMCapture::speedPresets;

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

FireCAMCapture::ModeStrings::ModeStrings() {
  (*this)[legacy] = "legacy";
  (*this)[fast] = "fast";
}

FireCAMCapture::ModePresets::ModePresets() {
  (*this)[legacy] = DC1394_OPERATION_MODE_LEGACY;
  (*this)[fast] = DC1394_OPERATION_MODE_1394B;
}

FireCAMCapture::SpeedPresets::SpeedPresets() {
  (*this)[100] = DC1394_ISO_SPEED_100;
  (*this)[200] = DC1394_ISO_SPEED_200;
  (*this)[400] = DC1394_ISO_SPEED_400;
  (*this)[800] = DC1394_ISO_SPEED_800;
  (*this)[1600] = DC1394_ISO_SPEED_1600;
  (*this)[3200] = DC1394_ISO_SPEED_3200;
}

FireCAMCapture::FireCAMCapture(size_t bufferSize, Mode mode, size_t speed) :
  bufferSize(bufferSize),
  mode(mode),
  speed(speed) {
}

FireCAMCapture::FireCAMCapture(const FireCAMCapture& src) :
  bufferSize(src.bufferSize),
  mode(src.mode),
  speed(src.speed) {
}

FireCAMCapture::~FireCAMCapture() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

void FireCAMCapture::setBufferSize(size_t bufferSize) {
  this->bufferSize = bufferSize;
}

size_t FireCAMCapture::getBufferSize() const {
  return bufferSize;
}

void FireCAMCapture::setMode(Mode mode) {
  this->mode = mode;
}

FireCAMCapture::Mode FireCAMCapture::getMode() const {
  return mode;
}

void FireCAMCapture::setSpeed(size_t speed) {
  this->speed = speed;
}

size_t FireCAMCapture::getSpeed() const {
  return speed;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

FireCAMCapture& FireCAMCapture::operator=(const FireCAMCapture& src) {
  bufferSize = src.bufferSize;
  mode = src.mode;
  speed = src.speed;

  return *this;
}

void FireCAMCapture::load(std::istream& stream) {
  std::string module, value, option;
  std::getline(stream, module, '.');
  std::getline(stream, option, '=');
  std::getline(stream, value);

  if (module == "capture") {
    if (option == "buffer_size")
      bufferSize = FireCAMUtils::convert<size_t>(value);
    else if (option == "mode")
      mode = FireCAMUtils::convert(value, modeStrings);
    else if (option == "speed")
      speed = FireCAMUtils::convert<size_t>(value);
    else
      FireCAMUtils::error("Bad capture option: ", option);
  }
}

void FireCAMCapture::save(std::ostream& stream) const {
  stream << "capture.buffer_size = " << bufferSize << std::endl;
  stream << "capture.speed = " << speed << std::endl;
}

void FireCAMCapture::writeParameters(dc1394camera_t* device) const {
  FireCAMUtils::assert("Failed to setup capture buffer",
    dc1394_capture_setup(device, bufferSize, DC1394_CAPTURE_FLAGS_DEFAULT));
  FireCAMUtils::assert("Failed to setup capture mode",
    dc1394_video_set_operation_mode(device, modePresets[mode]));
  FireCAMUtils::assert("Failed to setup capture speed",
    dc1394_video_set_iso_speed(device, FireCAMUtils::convert(
    speed, speedPresets)));
}
