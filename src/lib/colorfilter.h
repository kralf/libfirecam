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

#ifndef FIRECAM_COLORFILTER_H
#define FIRECAM_COLORFILTER_H

/** \file colorfilter.h
  * \brief FireCAM color filter definition
  */

#include <map>
#include <string>

#include <dc1394/dc1394.h>

class FireCAMColorFilter {
public:
  /** Types and subclasses
    */
  enum Pattern {
    rggb,
    gbrg,
    grbg,
    bggr
  };

  class PatternStrings :
    public std::map<Pattern, std::string> {
  public:
    /** Construct a color filter pattern string object
      */
    PatternStrings();
  };

  /** Construct a FireCAM color filter object
    */
  FireCAMColorFilter(bool enabled = true, Pattern pattern = rggb);
  FireCAMColorFilter(const FireCAMColorFilter& src);

  /** Destroy a FireCAM color filter object
    */
  virtual ~FireCAMColorFilter();

  /** Access the color filter's enabled flag
    */
  void setEnabled(bool enabled);
  bool isEnabled() const;
  /** Access the color filter pattern
    */
  void setPattern(Pattern pattern);
  Pattern getPattern() const;

  /** FireCAM color filter assignments
    */
  FireCAMColorFilter& operator=(const FireCAMColorFilter& src);

  /** Write color filter information to the given stream
    */
  void write(std::ostream& stream) const;

  /** Load color filter configuration from the given stream
    */
  void load(std::istream& stream);
  /** Save color filter configuration to the given stream
    */
  void save(std::ostream& stream) const;
protected:
  static PatternStrings patternStrings;

  dc1394color_filter_t filter;

  bool enabled;
  Pattern pattern;
};

#endif
