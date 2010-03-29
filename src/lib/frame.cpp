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

FireCAMFrame::FireCAMFrame(size_t width, size_t height) :
  frame(new dc1394video_frame_t()),
  buffered(false) {
  frame->size[0] = width;
  frame->size[1] = height;
  frame->data_depth = 24;

  frame->color_coding = DC1394_COLOR_CODING_RGB8;

  frame->image_bytes = frame->size[0]*frame->size[1]*frame->data_depth/8;
  frame->image = new unsigned char[frame->image_bytes];
}

FireCAMFrame::FireCAMFrame(dc1394camera_t* device) :
  buffered(true) {
  FireCAMUtils::assert("Failed to dequeue frame",
    dc1394_capture_dequeue(device, DC1394_CAPTURE_POLICY_WAIT, &frame));
}

FireCAMFrame::FireCAMFrame(const FireCAMFrame& src) :
  frame(new dc1394video_frame_t()),
  buffered(false) {
  frame->size[0] = src.frame->size[0];
  frame->size[1] = src.frame->size[1];
  frame->data_depth = 24;

  frame->color_coding = DC1394_COLOR_CODING_RGB8;

  frame->image_bytes = frame->size[0]*frame->size[1]*frame->data_depth/8;
  frame->image = new unsigned char[frame->image_bytes];

  operator=(src);
}

FireCAMFrame::~FireCAMFrame() {
  if (!buffered) {
    delete [] frame->image;
    delete frame;
  }
  else
    FireCAMUtils::assert("Failed to enqueue frame",
      dc1394_capture_enqueue(frame->camera, frame));
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

size_t FireCAMFrame::getWidth() const {
  return frame->size[0];
}

size_t FireCAMFrame::getHeight() const {
  return frame->size[1];
}

size_t FireCAMFrame::getDepth() const {
  return frame->data_depth;
}

const unsigned char* FireCAMFrame::getImage() const {
  return frame->image;
}

bool FireCAMFrame::isColor() const {
  dc1394bool_t color;
  FireCAMUtils::assert("Failed to query color information",
    dc1394_is_color(frame->color_coding, &color));

  return color;
}

bool FireCAMFrame::isBuffered() const {
  return buffered;
}

double FireCAMFrame::getTimestamp() const {
  return frame->timestamp*1e-6;
}

bool FireCAMFrame::isEmpty() const {
  return !frame->image_bytes;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

FireCAMFrame& FireCAMFrame::operator=(const FireCAMFrame& src) {
  if (!buffered) {
    if ((frame->size[0] == src.frame->size[0]) &&
        (frame->size[1] == src.frame->size[1]))
      FireCAMUtils::assert("Failed to convert frame",
        dc1394_convert_frames(src.frame, frame));
    else
      FireCAMUtils::error("Bad frame assignment",
        "Frame sizes mismatch");
  }
  else
    FireCAMUtils::error("Bad frame assignment",
      "Attempt to overwrite capture buffer");
}

void FireCAMFrame::write(std::ostream& stream) const {
  if (!isEmpty()) {
    stream << getWidth() << "x" << getHeight() << "x" << getDepth() << "bpp";
    stream << " (";
    if (isColor())
      stream << "color";
    else
      stream << "mono";
    stream << ")";
  }
  else
    stream << "(empty)";
}
