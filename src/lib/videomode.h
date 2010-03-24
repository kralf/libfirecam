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

#ifndef FIRECAM_VIDEOMODE_H
#define FIRECAM_VIDEOMODE_H

/** \file videomode.h
  * \brief FireCAM video mode definition
  */

#include <iostream>

#include <dc1394/dc1394.h>

class FireCAMVideoMode {
friend class FireCAMCamera;
public:
  /** Construct a FireCAM video mode object
    */
  FireCAMVideoMode(size_t width = 0, size_t height = 0, size_t depth = 0,
    bool color = false, bool scalable = false);
  FireCAMVideoMode(const FireCAMVideoMode& src);

  /** Destroy a FireCAM video mode object
    */
  virtual ~FireCAMVideoMode();

  /** Access the video modes horizontal resolution
    */
  void setWidth(size_t width);
  size_t getWidth() const;
  /** Access the video modes vertical resolution
    */
  void setHeight(size_t height);
  size_t getHeight() const;
  /** Access the video color resolution
    */
  void setDepth(size_t depth);
  size_t getDepth() const;
  /** Access the video modes color flag
    */
  void setColor(bool Color);
  bool isColor() const;
  /** Access the video modes scalable flag
    */
  void setScalable(bool scalable);
  bool isScalable() const;

  /** FireCAM video mode assignments
    */
  FireCAMVideoMode& operator=(const FireCAMVideoMode& src);

  /** FireCAM video mode comparisons
    */
  bool operator==(const FireCAMVideoMode& videoMode) const;
  bool operator!=(const FireCAMVideoMode& videoMode) const;
  bool operator<(const FireCAMVideoMode& videoMode) const;

  /** Write video mode information to the given stream
    */
  void write(std::ostream& stream) const;

  /** Load video mode configuration from the given stream
    */
  void load(std::istream& stream);
  /** Save video mode configuration to the given stream
    */
  void save(std::ostream& stream) const;
protected:
  dc1394video_mode_t mode;
  dc1394color_coding_t coding;

  size_t width;
  size_t height;
  size_t depth;

  bool color;
  bool scalable;

  /** Construct a FireCAM video mode object
    */
  FireCAMVideoMode(dc1394camera_t* device);
  FireCAMVideoMode(dc1394camera_t* device, dc1394video_mode_t mode,
    dc1394color_coding_t coding);

  /** Read the video mode parameters from the device
    */
  void readParameters(dc1394camera_t* device);
  /** Write the video mode parameters to the device
    */
  void writeParameters(dc1394camera_t* device) const;
};

#endif
