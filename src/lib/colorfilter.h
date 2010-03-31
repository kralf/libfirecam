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

#include "frame.h"

class FireCAMColorFilter {
public:
  /** Types and subclasses
    */
  enum Tile {
    rggb,
    gbrg,
    grbg,
    bggr
  };

  enum Method {
    nearest,
    simple,
    bilinear,
    hq_linear,
    downsample,
    edge_sense,
    vng,
    ahd
  };

  class TileStrings :
    public std::map<Tile, std::string> {
  public:
    /** Construct a color filter tile string object
      */
    TileStrings();
  };

  class MethodStrings :
    public std::map<Method, std::string> {
  public:
    /** Construct a color filter method string object
      */
    MethodStrings();
  };

  class TilePresets :
    public std::map<Tile, dc1394color_filter_t> {
  public:
    /** Construct a color filter tile preset object
      */
    TilePresets();
  };

  class MethodPresets :
    public std::map<Method, dc1394bayer_method_t> {
  public:
    /** Construct a color filter method preset object
      */
    MethodPresets();
  };

  /** Construct a FireCAM color filter object
    */
  FireCAMColorFilter(bool enabled = false, Tile tile = rggb, Method
    method = nearest);
  FireCAMColorFilter(const FireCAMColorFilter& src);

  /** Destroy a FireCAM color filter object
    */
  virtual ~FireCAMColorFilter();

  /** Access the color filter's enabled flag
    */
  void setEnabled(bool enabled);
  bool isEnabled() const;
  /** Access the color filter tile
    */
  void setTile(Tile tile);
  Tile getTile() const;
  /** Access the color filter method
    */
  void setMethod(Method method);
  Method getMethod() const;

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

  /** Filter the given input frame
    */
  void filter(const FireCAMFrame& inputFrame, FireCAMFrame& outputFrame,
    const FireCAMColor& color) const;

  static const TileStrings tileStrings;
  static const MethodStrings methodStrings;
  static const TilePresets tilePresets;
  static const MethodPresets methodPresets;
protected:
  bool enabled;

  Tile tile;
  Method method;
};

#endif
