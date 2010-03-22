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

#ifndef FIRECAM_CONFIGURATION_H
#define FIRECAM_CONFIGURATION_H

/** \file configuration.h
  * \brief FireCAM configuration class
  */

#include <sstream>
#include <stdexcept>

#include "videomode.h"
#include "colorfilter.h"
#include "framerate.h"
#include "parameters.h"

class FireCAMConfiguration {
public:
  /** Construct a FireCAM configuration object
    */
  FireCAMConfiguration(const char* filename = 0);
  FireCAMConfiguration(const FireCAMConfiguration& src);

  /** Destroy a FireCAM configuration object
    */
  virtual ~FireCAMConfiguration();

  /** FireCAM configuration assignments
    */
  FireCAMConfiguration& operator=(const FireCAMConfiguration& src);

  /** Access the video mode
    */
  void setVideoMode(const FireCAMVideoMode& videoMode);
  const FireCAMVideoMode& getVideoMode() const;
  /** Access the color filter
    */
  void setColorFilter(const FireCAMColorFilter& colorFilter);
  const FireCAMColorFilter& getColorFilter() const;
  /** Access the framerate
    */
  void setFramerate(const FireCAMFramerate& framerate);
  const FireCAMFramerate& getFramerate() const;
  /** Access the parameters
    */
  void setParameters(const FireCAMParameters& parameters);
  const FireCAMParameters& getParameters() const;

  /** Load the FireCAM configuration from the file with the specified
    * filename
    */
  void load(const char* filename);

  /** Read the FireCAM configuration from the given stream
    */
  void read(std::istream& stream);
  /** Write the FireCAM configuration to the given stream
    */
  void write(std::ostream& stream) const;
protected:
  FireCAMVideoMode videoMode;
  FireCAMColorFilter colorFilter;
  FireCAMFramerate framerate;
  FireCAMParameters parameters;

  std::string imageDirectory;
  std::string imageBasename;
  std::string imageExtension;
  size_t numDMABuffers;

  size_t imageWidth;
  size_t imageHeight;
  bool capture;
  bool transmit;

  template <typename T> T readValue(const std::string& string) const {
    std::istringstream stream(string);
    T value;

    if (!(stream >> value)) {
      std::ostringstream what;
      what << "Bad configuration value: " << string;
      throw std::runtime_error(what.str());
    }
    else
      return value;
  };
};

#endif
