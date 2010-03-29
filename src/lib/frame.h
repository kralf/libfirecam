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

#include <dc1394/dc1394.h>

class FireCAMFrame {
friend class FireCAMCamera;
friend class FireCAMColorFilter;
public:
  /** Construct a FireCAM frame object
    */
  FireCAMFrame(size_t width = 640, size_t height = 480);
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
  /** Access the frame's color resolution
    */
  size_t getDepth() const;
  /** Access the frame's image data
    */
  const unsigned char* getImage() const;

  /** Access the frame's timestamp
    */
  double getTimestamp() const;
  /** Access the frame's color flag
    */
  bool isColor() const;
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
protected:
  dc1394video_frame_t* frame;

  bool buffered;

  /** Construct a FireCAM frame object
    */
  FireCAMFrame(dc1394camera_t* device);
};

#endif
