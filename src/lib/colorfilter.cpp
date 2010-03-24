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

FireCAMColorFilter::PatternStrings FireCAMColorFilter::patternStrings;

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

FireCAMColorFilter::PatternStrings::PatternStrings() {
  (*this)[rggb] = "rggb";
  (*this)[gbrg] = "gbrg";
  (*this)[grbg] = "grbg";
  (*this)[bggr] = "bggr";
}

FireCAMColorFilter::FireCAMColorFilter(bool enabled, Pattern pattern) :
  enabled(enabled) {
  setPattern(pattern);
}

FireCAMColorFilter::FireCAMColorFilter(const FireCAMColorFilter& src) :
  filter(src.filter),
  enabled(src.enabled),
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

  if (pattern == gbrg)
    filter = DC1394_COLOR_FILTER_GBRG;
  else if (pattern == grbg)
    filter = DC1394_COLOR_FILTER_GRBG;
  else if (pattern == bggr)
    filter = DC1394_COLOR_FILTER_BGGR;
  else
    filter = DC1394_COLOR_FILTER_RGGB;
}

FireCAMColorFilter::Pattern FireCAMColorFilter::getPattern() const {
  return pattern;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

FireCAMColorFilter& FireCAMColorFilter::operator=(
    const FireCAMColorFilter& src) {
  filter = src.filter;

  enabled = src.enabled;
  pattern = src.pattern;

  return *this;
}

void FireCAMColorFilter::write(std::ostream& stream) const {
  stream << patternStrings[pattern];
}

void FireCAMColorFilter::load(std::istream& stream) {
  std::string module, option, value;
  std::getline(stream, module, '.');
  std::getline(stream, option, '=');
  std::getline(stream, value);

  if (module == "color_filter") {
    if (option == "enabled")
      enabled = FireCAMUtils::convert<bool>(value);
    else if (option == "pattern")
      setPattern(FireCAMUtils::convert(value, patternStrings));
    else
      FireCAMUtils::error("Bad color filter option: ", option);
  }
}

void FireCAMColorFilter::save(std::ostream& stream) const {
  stream << "color_filter.enabled = " << enabled << std::endl;
  stream << "color_filter.pattern = " << patternStrings[pattern] << std::endl;
}
