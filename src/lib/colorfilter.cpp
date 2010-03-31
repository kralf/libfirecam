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

#include "colorfilter.h"

#include "utils.h"

/*****************************************************************************/
/* Statics                                                                   */
/*****************************************************************************/

const FireCAMColorFilter::TileStrings FireCAMColorFilter::tileStrings;
const FireCAMColorFilter::MethodStrings FireCAMColorFilter::methodStrings;
const FireCAMColorFilter::TilePresets FireCAMColorFilter::tilePresets;
const FireCAMColorFilter::MethodPresets FireCAMColorFilter::methodPresets;

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

FireCAMColorFilter::TileStrings::TileStrings() {
  (*this)[rggb] = "rggb";
  (*this)[gbrg] = "gbrg";
  (*this)[grbg] = "grbg";
  (*this)[bggr] = "bggr";
}

FireCAMColorFilter::MethodStrings::MethodStrings() {
  (*this)[nearest] = "nearest";
  (*this)[simple] = "simple";
  (*this)[bilinear] = "bilinear";
  (*this)[hq_linear] = "hq_linear";
  (*this)[downsample] = "downsample";
  (*this)[edge_sense] = "edge_sense";
  (*this)[vng] = "vng";
  (*this)[ahd] = "ahd";
}

FireCAMColorFilter::TilePresets::TilePresets() {
  (*this)[rggb] = DC1394_COLOR_FILTER_RGGB;
  (*this)[gbrg] = DC1394_COLOR_FILTER_GBRG;
  (*this)[grbg] = DC1394_COLOR_FILTER_GRBG;
  (*this)[bggr] = DC1394_COLOR_FILTER_BGGR;
}

FireCAMColorFilter::MethodPresets::MethodPresets() {
  (*this)[nearest] = DC1394_BAYER_METHOD_NEAREST;
  (*this)[simple] = DC1394_BAYER_METHOD_SIMPLE;
  (*this)[bilinear] = DC1394_BAYER_METHOD_BILINEAR;
  (*this)[hq_linear] = DC1394_BAYER_METHOD_HQLINEAR;
  (*this)[downsample] = DC1394_BAYER_METHOD_DOWNSAMPLE;
  (*this)[edge_sense] = DC1394_BAYER_METHOD_EDGESENSE;
  (*this)[vng] = DC1394_BAYER_METHOD_VNG;
  (*this)[ahd] = DC1394_BAYER_METHOD_AHD;
}

FireCAMColorFilter::FireCAMColorFilter(bool enabled, Tile tile, Method
    method) :
  enabled(enabled),
  tile(tile),
  method(method) {
}

FireCAMColorFilter::FireCAMColorFilter(const FireCAMColorFilter& src) :
  enabled(src.enabled),
  tile(src.tile),
  method(src.method) {
}

FireCAMColorFilter::~FireCAMColorFilter() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

void FireCAMColorFilter::setEnabled(bool enabled) {
  this->enabled = enabled;
}

bool FireCAMColorFilter::isEnabled() const {
  return enabled;
}

void FireCAMColorFilter::setTile(Tile tile) {
  this->tile = tile;
}

FireCAMColorFilter::Tile FireCAMColorFilter::getTile() const {
  return tile;
}

void FireCAMColorFilter::setMethod(Method method) {
  this->method = method;
}

FireCAMColorFilter::Method FireCAMColorFilter::getMethod() const {
  return method;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

FireCAMColorFilter& FireCAMColorFilter::operator=(
    const FireCAMColorFilter& src) {
  enabled = src.enabled;

  tile = src.tile;
  method = src.method;

  return *this;
}

void FireCAMColorFilter::write(std::ostream& stream) const {
  if (enabled)
    stream << "on (" << FireCAMUtils::convert(tile, tileStrings) << ", " <<
    FireCAMUtils::convert(method, methodStrings) << ")";
  else
    stream << "off";
}

void FireCAMColorFilter::load(std::istream& stream) {
  std::string module, option, value;
  std::getline(stream, module, '.');
  std::getline(stream, option, '=');
  std::getline(stream, value);

  if (module == "color_filter") {
    if (option == "enabled")
      enabled = FireCAMUtils::convert<bool>(value);
    else if (option == "tile")
      tile = FireCAMUtils::convert(value, tileStrings);
    else if (option == "method")
      method = FireCAMUtils::convert(value, methodStrings);
    else
      FireCAMUtils::error("Bad color filter option: ", option);
  }
}

void FireCAMColorFilter::save(std::ostream& stream) const {
  stream << "color_filter.enabled = " << enabled << std::endl;
  stream << "color_filter.tile = " << FireCAMUtils::convert(tile,
    tileStrings) << std::endl;
  stream << "color_filter.method = " << FireCAMUtils::convert(method,
    methodStrings) << std::endl;
}

void FireCAMColorFilter::filter(const FireCAMFrame& inputFrame, FireCAMFrame&
    outputFrame, const FireCAMColor& color) const {
  if (enabled) {
    outputFrame.clear();

    outputFrame.width = inputFrame.width;
    outputFrame.height = inputFrame.height;
    outputFrame.color = FireCAMColor::rgb8;

    outputFrame.image = new unsigned char[outputFrame.getSize()];
    FireCAMUtils::assert("Failed to apply color filter",
      dc1394_bayer_decoding_8bit(inputFrame.image, outputFrame.image,
      inputFrame.width, inputFrame.height, FireCAMUtils::convert(tile,
      tilePresets), FireCAMUtils::convert(method, methodPresets)));

    outputFrame.timestamp = inputFrame.timestamp;
  }
  else
    outputFrame.convert(inputFrame, color);
}
