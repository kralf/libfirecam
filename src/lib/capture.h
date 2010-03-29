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

#ifndef FIRECAM_CAPTURE_H
#define FIRECAM_CAPTURE_H

/** \file capture.h
  * \brief FireCAM capture definition
  */

#include <iostream>
#include <map>
#include <string>

#include <dc1394/dc1394.h>

class FireCAMCapture {
friend class FireCAMCamera;
public:
  /** Types and subclasses
    */
  enum Mode {
    legacy,
    fast
  };

  class ModeStrings :
    public std::map<Mode, std::string> {
  public:
    /** Construct a capture mode string object
      */
    ModeStrings();
  };

  class ModePresets :
    public std::map<size_t, dc1394operation_mode_t> {
  public:
    /** Construct a capture mode preset object
      */
    ModePresets();
  };

  class SpeedPresets :
    public std::map<size_t, dc1394speed_t> {
  public:
    /** Construct a capture speed preset object
      */
    SpeedPresets();
  };

  /** Construct a FireCAM capture object
    */
  FireCAMCapture(size_t bufferSize = 64, Mode mode = legacy,
    size_t speed = 400);
  FireCAMCapture(const FireCAMCapture& src);

  /** Destroy a FireCAM capture object
    */
  virtual ~FireCAMCapture();

  /** Access the capture buffer size
    */
  void setBufferSize(size_t bufferSize);
  size_t getBufferSize() const;
  /** Access the capture mode
    */
  void setMode(Mode mode);
  Mode getMode() const;
  /** Access the capture speed
    */
  void setSpeed(size_t speed);
  size_t getSpeed() const;

  /** FireCAM capture assignments
    */
  FireCAMCapture& operator=(const FireCAMCapture& src);

  /** Load capture configuration from the given stream
    */
  void load(std::istream& stream);
  /** Save capture configuration to the given stream
    */
  void save(std::ostream& stream) const;
protected:
  static ModeStrings modeStrings;
  static ModePresets modePresets;
  static SpeedPresets speedPresets;

  size_t bufferSize;
  Mode mode;
  size_t speed;

  /** Write the capture parameters to the specified device
    */
  void writeParameters(dc1394camera_t* device) const;
};

#endif
