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

#include "frame.h"

#include "utils.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

FireCAMFrame::FireCAMFrame(size_t width, size_t height, const FireCAMColor&
    color) :
  frame(0),
  width(width),
  height(height),
  color(color),
  image(0),
  timestamp(0.0),
  buffered(false) {
  image = new unsigned char[getSize()];
}

FireCAMFrame::FireCAMFrame(dc1394camera_t* device) :
  frame(0),
  width(0),
  height(0),
  image(0),
  timestamp(0.0),
  buffered(true) {
  FireCAMUtils::assert("Failed to dequeue frame",
    dc1394_capture_dequeue(device, DC1394_CAPTURE_POLICY_WAIT, &frame));
  readParameters();
}

FireCAMFrame::FireCAMFrame(const FireCAMFrame& src) :
  frame(0),
  width(0),
  height(0),
  image(0),
  timestamp(0.0),
  buffered(false) {
  operator=(src);
}

FireCAMFrame::~FireCAMFrame() {
  clear();
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

size_t FireCAMFrame::getWidth() const {
  return width;
}

size_t FireCAMFrame::getHeight() const {
  return height;
}

const FireCAMColor& FireCAMFrame::getColor() const {
  return color;
}

const unsigned char* FireCAMFrame::getImage() const {
  return image;
}

double FireCAMFrame::getTimestamp() const {
  return timestamp;
}

size_t FireCAMFrame::getSize() const {
  return width*height*color.getDepth()/8;
}

bool FireCAMFrame::isBuffered() const {
  return frame;
}

bool FireCAMFrame::isEmpty() const {
  return !image;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

FireCAMFrame& FireCAMFrame::operator=(const FireCAMFrame& src) {
  convert(src, src.color);
}

void FireCAMFrame::write(std::ostream& stream) const {
  if (!isEmpty()) {
    stream << width << "x" << height << "x" << color.getDepth() << "bpp";
    stream << " (";
    if (color.isMonochrome())
      stream << "mono";
    else
      stream << "color";
    stream << ")";
  }
  else
    stream << "(empty)";
}

void FireCAMFrame::load(std::istream& stream) {
  clear();

  try {
    stream >> width >> height;
    color.load(stream);
    stream >> timestamp;

    image = new unsigned char[getSize()];
    for (int i = 0; i < getSize(); ++i)
      stream >> image[i];
  }
  catch (const std::exception& exception) {
    FireCAMUtils::error("Failed to load frame", exception.what());
  }
}

void FireCAMFrame::save(std::ostream& stream) const {
  stream << width << height;
  color.save(stream);
  stream << timestamp;

  for (int i = 0; i < getSize(); ++i)
    stream << image[i];
}

FireCAMFrame& FireCAMFrame::convert(const FireCAMFrame& src, const
    FireCAMColor& color) {
  clear();

  if (!src.frame && (color == src.color)) {
    width = src.width;
    height = src.height;
    this->color = color;

    image = new unsigned char[getSize()];
    for (int i = 0; i < getSize(); ++i)
      image[i] = src.image[i];

    timestamp = src.timestamp;
  }
  else if (src.frame) {
    frame = new dc1394video_frame_t();
    frame->color_coding = FireCAMUtils::convert(color.coding,
      FireCAMColor::codingPresets);

    FireCAMUtils::assert("Failed to convert frame",
      dc1394_convert_frames(src.frame, frame));

    readParameters();
  }
  else
    FireCAMUtils::error("Failed to convert frame",
      "Color codings mismatch");

  return *this;
}

void FireCAMFrame::clear() {
  if (!buffered) {
    if (image)
      delete [] image;
    if (frame)
      delete frame;
  }
  else {
    FireCAMUtils::assert("Failed to enqueue frame",
      dc1394_capture_enqueue(frame->camera, frame));
  }

  frame = 0;

  width = 0;
  height = 0;
  color = FireCAMColor::mono8;

  image = 0;

  timestamp = 0.0;

  buffered = false;
}

void FireCAMFrame::readParameters() {
  if (frame) {
    width = frame->size[0];
    height = frame->size[1];
    color.coding = FireCAMUtils::convert(frame->color_coding,
      FireCAMColor::codingPresets);

    image = frame->image;

    timestamp = frame->timestamp*1e-6;
  }
  else
    FireCAMUtils::error("Failed to read frame parameters",
      "Frame not in capture buffer");
}
