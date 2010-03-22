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

/*****************************************************************************/
/* Statics                                                                   */
/*****************************************************************************/

const FireCAMColorFilter::Presets FireCAMColorFilter::presets;

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

FireCAMColorFilter::Presets::Presets() {
  (*this)[rggb] = DC1394_COLOR_FILTER_RGGB;
  (*this)[gbrg] = DC1394_COLOR_FILTER_GBRG;
  (*this)[grbg] = DC1394_COLOR_FILTER_GRBG;
  (*this)[bggr] = DC1394_COLOR_FILTER_BGGR;
}

FireCAMColorFilter::FireCAMColorFilter(bool enabled, Pattern pattern) :
  enabled(enabled),
  pattern(pattern) {
}

FireCAMColorFilter::FireCAMColorFilter(const FireCAMColorFilter& src) :
  pattern(src.pattern) {
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

void FireCAMColorFilter::setPattern(Pattern pattern) {
  this->pattern = pattern;
}

FireCAMColorFilter::Pattern FireCAMColorFilter::getPattern() const {
  return pattern;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

FireCAMColorFilter& FireCAMColorFilter::operator=(
    const FireCAMColorFilter& src) {
  pattern = src.pattern;
  return *this;
}

FireCAMColorFilter::operator dc1394color_filter_t() const {
  return presets.find(pattern)->second;
}
