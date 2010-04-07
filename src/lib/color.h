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

#ifndef FIRECAM_COLOR_H
#define FIRECAM_COLOR_H

/** \file color.h
  * \brief FireCAM color definition
  */

#include <map>
#include <string>

#include <dc1394/dc1394.h>

class FireCAMColor {
friend class FireCAMVideoMode;
friend class FireCAMFrame;
public:
  /** Types and subclasses
    */
  enum Coding {
    mono8,
    yuv411,
    yuv422,
    yuv444,
    rgb8,
    mono16,
    rgb16,
    mono16s,
    rgb16s,
    raw8,
    raw16
  };

  class CodingStrings :
    public std::map<Coding, std::string> {
  public:
    /** Construct a color coding string object
      */
    CodingStrings();
  };

  class CodingPresets :
    public std::map<Coding, dc1394color_coding_t> {
  public:
    /** Construct a color coding preset object
      */
    CodingPresets();
  };

  /** Construct a FireCAM color object
    */
  FireCAMColor(Coding coding = mono8);
  FireCAMColor(const FireCAMColor& src);

  /** Destroy a FireCAM color object
    */
  virtual ~FireCAMColor();

  /** Access the color's depth
    */
  size_t getDepth() const;
  /** Access the color's size in byte
    */
  size_t getSize() const;
  /** Access the color's monochrome flag
    */
  bool isMonochrome() const;

  /** FireCAM color assignments
    */
  FireCAMColor& operator=(const FireCAMColor& src);

  /** FireCAM color comparisons
    */
  bool operator==(const FireCAMColor& color) const;
  bool operator!=(const FireCAMColor& color) const;
  bool operator<(const FireCAMColor& color) const;

  /** Write color information to the given stream
    */
  void write(std::ostream& stream) const;

  static const CodingStrings codingStrings;
  static const CodingPresets codingPresets;
protected:
  Coding coding;
};

#endif
