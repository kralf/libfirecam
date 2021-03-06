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

#include "videomode.h"

#include "utils.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

FireCAMVideoMode::FireCAMVideoMode(size_t width, size_t height,
    const FireCAMColor& color, bool scalable) :
  width(width),
  height(height),
  color(color),
  scalable(scalable) {
}

FireCAMVideoMode::FireCAMVideoMode(dc1394camera_t* device) {
  FireCAMUtils::assert("Failed to query video mode",
    dc1394_video_get_mode(device, &mode));
  FireCAMUtils::assert("Failed to query color coding",
    dc1394_get_color_coding_from_video_mode(device, mode, &coding));

  readParameters(device);
}

FireCAMVideoMode::FireCAMVideoMode(dc1394camera_t* device, dc1394video_mode_t
    mode, dc1394color_coding_t coding) :
  mode(mode),
  coding(coding) {
  readParameters(device);
}

FireCAMVideoMode::FireCAMVideoMode(const FireCAMVideoMode& src) :
  mode(src.mode),
  coding(src.coding),
  width(src.width),
  height(src.height),
  color(src.color),
  scalable(src.scalable) {
}

FireCAMVideoMode::~FireCAMVideoMode() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

void FireCAMVideoMode::setWidth(size_t width) {
  this->width = width;
}

size_t FireCAMVideoMode::getWidth() const {
  return width;
}

void FireCAMVideoMode::setHeight(size_t height) {
  this->height = height;
}

size_t FireCAMVideoMode::getHeight() const {
  return height;
}

void FireCAMVideoMode::setColor(const FireCAMColor& color) {
  this->color = color;
}

const FireCAMColor& FireCAMVideoMode::getColor() const {
  return color;
}

void FireCAMVideoMode::setScalable(bool scalable) {
  this->scalable = scalable;
}

bool FireCAMVideoMode::isScalable() const {
  return scalable;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

FireCAMVideoMode& FireCAMVideoMode::operator=(const FireCAMVideoMode& src) {
  mode = src.mode;
  coding = src.coding;

  width = src.width;
  height = src.height;
  color = src.color;

  scalable = src.scalable;

  return *this;
}

bool FireCAMVideoMode::operator==(const FireCAMVideoMode& videoMode) const {
  return ((width == videoMode.width) && (height == videoMode.height) &&
    (color == videoMode.color) && (scalable == videoMode.scalable));
}

bool FireCAMVideoMode::operator!=(const FireCAMVideoMode& videoMode) const {
  return !operator==(videoMode);
}

bool FireCAMVideoMode::operator<(const FireCAMVideoMode& videoMode) const {
  return ((width < videoMode.width) || (width == videoMode.width) &&
    (height < videoMode.height) || (height == videoMode.height) &&
    (color < videoMode.color) || (color == videoMode.color) &&
    (scalable < videoMode.scalable));
}

void FireCAMVideoMode::write(std::ostream& stream) const {
  stream << width << "x" << height << "x" << color.getDepth() << "bpp";
  stream << " (";
  if (color.isMonochrome())
    stream << "mono";
  else
    stream << "color";
  if (scalable)
    stream << ", scalable";
  stream << ")";
}

void FireCAMVideoMode::load(std::istream& stream) {
  std::string module, value, option;
  std::getline(stream, module, '.');
  std::getline(stream, option, '=');
  std::getline(stream, value);

  if (module == "video_mode") {
    if (option == "width")
      width = FireCAMUtils::convert<size_t>(value);
    else if (option == "height")
      height = FireCAMUtils::convert<size_t>(value);
    else if (option == "color")
      color.coding = FireCAMUtils::convert(value, FireCAMColor::codingStrings);
    else if (option == "scalable")
      scalable = FireCAMUtils::convert<bool>(value);
    else
      FireCAMUtils::error("Bad video mode option: ", option);
  }
}

void FireCAMVideoMode::save(std::ostream& stream) const {
  stream << "video_mode.width = " << width << std::endl;
  stream << "video_mode.height = " << height << std::endl;
  stream << "video_mode.color = " << FireCAMUtils::convert(color.coding,
    FireCAMColor::codingStrings) << std::endl;
  stream << "video_mode.scalable = " << scalable << std::endl;
}

void FireCAMVideoMode::readParameters(dc1394camera_t* device) {
  uint32_t width, height;
  FireCAMUtils::assert("Failed to query image size",
    dc1394_get_image_size_from_video_mode(device, mode, &width, &height));
  this->width = width;
  this->height = height;

  color = FireCAMUtils::convert(coding, FireCAMColor::codingPresets);

  scalable = dc1394_is_video_mode_scalable(mode);
}

void FireCAMVideoMode::writeParameters(dc1394camera_t* device) const {
  FireCAMUtils::assert("Failed to set video mode",
    dc1394_video_set_mode(device, mode));

  if (scalable) {
    FireCAMUtils::assert("Failed to set image size",
      dc1394_format7_set_image_size(device, mode, width, height));
    FireCAMUtils::assert("Failed to set color coding",
      dc1394_format7_set_color_coding(device, mode, coding));
  }
}
