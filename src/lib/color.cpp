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

#include "color.h"

#include "utils.h"

/*****************************************************************************/
/* Statics                                                                   */
/*****************************************************************************/

const FireCAMColor::CodingStrings FireCAMColor::codingStrings;
const FireCAMColor::CodingPresets FireCAMColor::codingPresets;

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

FireCAMColor::CodingStrings::CodingStrings() {
  (*this)[mono8] = "mono8";
  (*this)[yuv411] = "yuv411";
  (*this)[yuv422] = "yuv422";
  (*this)[yuv444] = "yuv444";
  (*this)[rgb8] = "rgb8";
  (*this)[mono16] = "mono16";
  (*this)[rgb16] = "rgb16";
  (*this)[mono16s] = "mono16s";
  (*this)[rgb16s] = "rgb16s";
  (*this)[raw8] = "raw8";
  (*this)[raw16] = "raw16";
}

FireCAMColor::CodingPresets::CodingPresets() {
  (*this)[mono8] = DC1394_COLOR_CODING_MONO8;
  (*this)[yuv411] = DC1394_COLOR_CODING_YUV411;
  (*this)[yuv422] = DC1394_COLOR_CODING_YUV422;
  (*this)[yuv444] = DC1394_COLOR_CODING_YUV444;
  (*this)[rgb8] = DC1394_COLOR_CODING_RGB8;
  (*this)[mono16] = DC1394_COLOR_CODING_MONO16;
  (*this)[rgb16] = DC1394_COLOR_CODING_RGB16;
  (*this)[mono16s] = DC1394_COLOR_CODING_MONO16S;
  (*this)[rgb16s] = DC1394_COLOR_CODING_RGB16S;
  (*this)[raw8] = DC1394_COLOR_CODING_RAW8;
  (*this)[raw16] = DC1394_COLOR_CODING_RAW16;
}

FireCAMColor::FireCAMColor(Coding coding) :
  coding(coding) {
}

FireCAMColor::FireCAMColor(const FireCAMColor& src) :
  coding(src.coding) {
}

FireCAMColor::~FireCAMColor() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

size_t FireCAMColor::getDepth() const {
  uint32_t depth;
  FireCAMUtils::assert("Failed to query color depth",
    dc1394_get_color_coding_bit_size(
    FireCAMUtils::convert(coding, codingPresets), &depth));

  return depth;
}

bool FireCAMColor::isMonochrome() const {
  dc1394bool_t color;
  FireCAMUtils::assert("Failed to query color information",
    dc1394_is_color(FireCAMUtils::convert(coding, codingPresets), &color));

  return !color;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

FireCAMColor& FireCAMColor::operator=(const FireCAMColor& src) {
  coding = src.coding;
  return *this;
}

bool FireCAMColor::operator==(const FireCAMColor& color) const {
  return (coding == color.coding);
}

bool FireCAMColor::operator!=(const FireCAMColor& color) const {
  return (coding != color.coding);
}

bool FireCAMColor::operator<(const FireCAMColor& color) const {
  return (coding < color.coding);
}

void FireCAMColor::write(std::ostream& stream) const {
  stream << FireCAMUtils::convert(coding, codingStrings);
}
