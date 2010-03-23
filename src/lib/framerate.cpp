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

#include <iostream>

#include "framerate.h"

#include "utils.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

FireCAMFramerate::FireCAMFramerate(double framesPerSecond) :
  framesPerSecond(framesPerSecond) {
}

FireCAMFramerate::FireCAMFramerate(dc1394camera_t* device) {
  FireCAMUtils::assert("Failed to query framerate",
    dc1394_video_get_framerate(device, &rate));

  readParameters();
}

FireCAMFramerate::FireCAMFramerate(dc1394framerate_t rate) :
    rate(rate) {
  readParameters();
}

FireCAMFramerate::FireCAMFramerate(const FireCAMFramerate& src) :
  rate(src.rate),
  framesPerSecond(src.framesPerSecond) {
}

FireCAMFramerate::~FireCAMFramerate() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

void FireCAMFramerate::setFramesPerSecond(double framesPerSecond) {
  this->framesPerSecond = framesPerSecond;
}

double FireCAMFramerate::getFramesPerSecond() const {
  return framesPerSecond;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

FireCAMFramerate& FireCAMFramerate::operator=(const FireCAMFramerate& src) {
  rate = src.rate;

  framesPerSecond = src.framesPerSecond;

  return *this;
}

void FireCAMFramerate::write(std::ostream& stream) const {
  stream << framesPerSecond << "fps";
}

void FireCAMFramerate::load(std::istream& stream) {
  std::string module, value, option;
  std::getline(stream, module, '.');
  std::getline(stream, option, '=');
  std::getline(stream, value);

  if (module == "frame_rate") {
    if (option == "fps")
      framesPerSecond = FireCAMUtils::convert<double>(value);
    else
      FireCAMUtils::error("Bad framerate option: ", option);
  }
}

void FireCAMFramerate::save(std::ostream& stream) const {
  stream << "frame_rate.fps = " << framesPerSecond << std::endl;
}

void FireCAMFramerate::readParameters() {
  float framesPerSecond;
  FireCAMUtils::assert("Failed to query framerate",
    dc1394_framerate_as_float(rate, &framesPerSecond));

  this->framesPerSecond = framesPerSecond;
}
