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

#include <sstream>
#include <stdexcept>

#include "videomode.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

FireCAMVideoMode::FireCAMVideoMode(size_t width, size_t height, size_t depth,
    bool color, bool scalable) :
  width(width),
  height(height),
  depth(depth),
  color(color),
  scalable(scalable) {
}

FireCAMVideoMode::FireCAMVideoMode(dc1394camera_t* device, dc1394video_mode_t
    mode, dc1394color_coding_t coding) :
  mode(mode),
  coding(coding) {
  dc1394error_t error;

  uint32_t width, height;
  error = dc1394_get_image_size_from_video_mode(device, mode, &width,
    &height);
  if (error != DC1394_SUCCESS) {
    std::ostringstream what;
    what << "Failed to query image size: " <<
      dc1394_error_get_string(error);
    throw std::runtime_error(what.str());
  }
  this->width = width;
  this->height = height;

  uint32_t depth;
  error = dc1394_get_color_coding_data_depth(coding, &depth);
  if (error != DC1394_SUCCESS) {
    std::ostringstream what;
    what << "Failed to query color depth: " <<
      dc1394_error_get_string(error);
    throw std::runtime_error(what.str());
  }
  this->depth = depth;

  dc1394bool_t color;
  error = dc1394_is_color(coding, &color);
  if (error != DC1394_SUCCESS) {
    std::ostringstream what;
    what << "Failed to query color information: " <<
      dc1394_error_get_string(error);
    throw std::runtime_error(what.str());
  }
  this->color = color;

  this->scalable = dc1394_is_video_mode_scalable(mode);
}

FireCAMVideoMode::FireCAMVideoMode(const FireCAMVideoMode& src) :
  mode(src.mode),
  coding(src.coding),
  width(src.width),
  height(src.height),
  depth(src.depth),
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

void FireCAMVideoMode::setDepth(size_t depth) {
  this->depth = depth;
}

size_t FireCAMVideoMode::getDepth() const {
  return depth;
}

void FireCAMVideoMode::setColor(bool Color) {
  this->color = Color;
}

bool FireCAMVideoMode::isColor() const {
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
  depth = src.depth;

  color = src.color;
  scalable = src.scalable;

  return *this;
}

bool FireCAMVideoMode::operator==(const FireCAMVideoMode& videoMode) const {
  return ((width == videoMode.width) && (height == videoMode.height) &&
    (depth == videoMode.depth) && (color == videoMode.color) &&
    (scalable == videoMode.scalable));
}

bool FireCAMVideoMode::operator!=(const FireCAMVideoMode& videoMode) const {
  return !operator==(videoMode);
}

bool FireCAMVideoMode::operator<(const FireCAMVideoMode& videoMode) const {
  return (!(color >= videoMode.color) || !(scalable >= videoMode.scalable) ||
    !(width*height*depth >= videoMode.width*videoMode.height*videoMode.depth));
}

void FireCAMVideoMode::write(std::ostream& stream) const {
  stream << width << "x" << height << "x" << depth << "bpp";
  stream << " (";
  if (color)
    stream << "color";
  else
    stream << "mono";
  if (scalable)
    stream << ", scalable";
  stream << ")";
}
