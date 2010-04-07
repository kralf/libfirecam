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

#ifndef FIRECAM_FRAME_H
#define FIRECAM_FRAME_H

/** \file frame.h
  * \brief FireCAM frame class
  */

#include <iostream>

#include "color.h"

class FireCAMFrame {
friend class FireCAMCamera;
friend class FireCAMColorFilter;
public:
  /** Types and subclasses
    */
  enum ResizeMethod {
    nearest
  };

  /** Construct a FireCAM frame object
    */
  FireCAMFrame(size_t width = 640, size_t height = 480,
    const FireCAMColor& color = FireCAMColor::mono8);
  FireCAMFrame(const FireCAMFrame& src);

  /** Destroy a FireCAM camera object
    */
  virtual ~FireCAMFrame();

  /** Access the frame's width
    */
  size_t getWidth() const;
  /** Access the frame's height
    */
  size_t getHeight() const;
  /** Access the frame's color
    */
  const FireCAMColor& getColor() const;
  /** Access the frame's image data
    */
  const unsigned char* getImage() const;

  /** Access the frame's timestamp
    */
  double getTimestamp() const;
  /** Access the frame's size
    */
  size_t getSize() const;
  /** Access the frame's buffered flag
    */
  bool isBuffered() const;
  /** Access the frame's empty flag
    */
  bool isEmpty() const;

  /** FireCAM frame assignments
    */
  FireCAMFrame& operator=(const FireCAMFrame& src);

  /** Write frame information to the given stream
    */
  void write(std::ostream& stream) const;

  /** Load the FireCAM frame from the given stream or file
    */
  void load(std::istream& stream);
  void load(const std::string& filename);
  /** Save the FireCAM frame to the given stream or file
    */
  void save(std::ostream& stream) const;
  void save(const std::string& filename) const;

  /** Dump the FireCAM frame into the given directory
    */
  std::string dump(const std::string& directory = "") const;

  /** Resize the frame
    */
  FireCAMFrame& resize(size_t width, size_t height, ResizeMethod
    method = nearest);

  /** Perform color conversion of a frame
    */
  FireCAMFrame& convert(const FireCAMFrame& src, const FireCAMColor& color);

  /** Clear the frame
    */
  void clear();
protected:
  dc1394video_frame_t* frame;

  size_t width;
  size_t height;
  FireCAMColor color;

  unsigned char* image;

  double timestamp;

  bool buffered;

  /** Construct a FireCAM frame object
    */
  FireCAMFrame(dc1394camera_t* device);

  /** Read the frame parameters
    */
  void readParameters();
};

#endif
