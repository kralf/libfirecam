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

#ifndef FIRECAM_UTILS_H
#define FIRECAM_UTILS_H

/** \file utils.h
  * \brief FireCAM utility functions
  */

#include <sstream>
#include <stdexcept>
#include <map>

#include <dc1394/dc1394.h>

class FireCAMUtils {
public:
  /** Driver library error assertion
    */
  static void assert(const char* description, dc1394error_t error);

  /** Templated error handling using runtime exceptions
    */
  template <typename T> static void error(const char* message,
      const T& value) {
    std::ostringstream what;
    what << message << ": " << value;
    throw std::runtime_error(what.str());
  };

  /** Templated string to value conversions
    */
  template <typename T> static T convert(const std::string& string) {
    std::istringstream stream(string);
    T value;

    if (!(stream >> value))
      error("Bad conversion", string);
    else
      return value;
  };

  /** Templated string to enumeratable conversions
    */
  template <typename T> static T convert(const std::string& string,
      const std::map<T, std::string>& strings) {
    for (typename std::map<T, std::string>::const_iterator it =
        strings.begin(); it != strings.end(); ++it) {
      if (string == it->second)
        return it->first;
    }

    error("Bad conversion", string);
  };
};

#endif
