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
/* Constructors and Destructor                                               */
/*****************************************************************************/

FireCAMCapture::FireCAMCapture(size_t bufferSize) :
  bufferSize(bufferSize) {
}

FireCAMCapture::FireCAMCapture(const FireCAMCapture& src) :
  bufferSize(src.bufferSize) {
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

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

FireCAMCapture& FireCAMCapture::operator=(const FireCAMCapture& src) {
  bufferSize = src.bufferSize;
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
    else
      FireCAMUtils::error("Bad capture option: ", option);
  }
}

void FireCAMCapture::save(std::ostream& stream) const {
  stream << "capture.buffer_size = " << bufferSize << std::endl;
}

void FireCAMCapture::writeParameters(dc1394camera_t* device) const {
  FireCAMUtils::assert("Failed to setup capture",
    dc1394_capture_setup(device, bufferSize, DC1394_CAPTURE_FLAGS_DEFAULT));
}
