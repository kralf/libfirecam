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

#include <fstream>
#include <algorithm>

#include "configuration.h"

#include "utils.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

FireCAMConfiguration::FireCAMConfiguration(const char* filename) :
  imageWidth(0),
  imageHeight(0),
  capture(false),
  transmit(false),
  numDMABuffers(64) {
  if (filename)
    load(filename);
}

FireCAMConfiguration::FireCAMConfiguration(const FireCAMConfiguration& src) :
  videoMode(src.videoMode),
  framerate(src.framerate),
  features(src.features),
  colorFilter(src.colorFilter),
  imageDirectory(src.imageDirectory),
  imageBasename(src.imageBasename),
  imageExtension(src.imageExtension),
  imageWidth(src.imageWidth),
  imageHeight(src.imageHeight),
  capture(src.capture),
  transmit(src.transmit),
  numDMABuffers(src.numDMABuffers) {
}

FireCAMConfiguration::~FireCAMConfiguration() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

void FireCAMConfiguration::setVideoMode(const FireCAMVideoMode& videoMode) {
  this->videoMode = videoMode;
}

const FireCAMVideoMode& FireCAMConfiguration::getVideoMode() const {
  return videoMode;
}

const FireCAMColorFilter& FireCAMConfiguration::getColorFilter() const {
  return colorFilter;
}

void FireCAMConfiguration::setFramerate(const FireCAMFramerate& framerate) {
  this->framerate = framerate;
}

const FireCAMFramerate& FireCAMConfiguration::getFramerate() const {
  return framerate;
}

void FireCAMConfiguration::setFeature(const FireCAMFeature& feature) {
  features[feature.getName()] = feature;
}

const FireCAMFeature& FireCAMConfiguration::getFeature(const char* name)
    const {
  std::map<std::string, FireCAMFeature>::const_iterator it =
    features.find(name);

  if (it == features.end())
    FireCAMUtils::error("Bad device feature", name);
  else
    return it->second;
}

void FireCAMConfiguration::setColorFilter(const FireCAMColorFilter&
    colorFilter) {
  this->colorFilter = colorFilter;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

FireCAMConfiguration& FireCAMConfiguration::operator=(
    const FireCAMConfiguration& src) {
  videoMode = src.videoMode;
  framerate = src.framerate;
  features = src.features;

  colorFilter = src.colorFilter;
  imageDirectory = src.imageDirectory;
  imageBasename = src.imageBasename;
  imageExtension = src.imageExtension;
  numDMABuffers = src.numDMABuffers;

  imageWidth = src.imageWidth;
  imageHeight = src.imageHeight;
  capture = src.capture;
  transmit = src.transmit;

  return *this;
}

void FireCAMConfiguration::load(const char* filename) {
  std::ifstream file(filename);

  if (!file.is_open())
    FireCAMUtils::error("Error opening configuration file", filename);
  else
    load(file);
}

void FireCAMConfiguration::load(std::istream& stream) {
  while (!stream.eof()) {
    std::string line;
    std::getline(stream, line);
    std::string::iterator it = std::remove(line.begin(), line.end(), ' ');
    line.erase(it, line.end());

    if (line[0] != '#') {
      std::istringstream lineStream(line);
      std::string module;
      std::getline(lineStream, module, '.');

      if (module == "video_mode")
        videoMode.load(lineStream.seekg(0));
      else if (module == "frame_rate")
        framerate.load(lineStream.seekg(0));
      else if (module == "features") {
        std::string name;
        std::getline(lineStream, name, '.');

        features[name].load(lineStream.seekg(0));
      }
      else if (module == "color_filter")
        colorFilter.load(lineStream.seekg(0));
      else if (!module.empty()) {
        std::ostringstream what;
        what << "Bad configuration module: " << module;
        throw std::runtime_error(what.str());
      }
    }
  }
}

void FireCAMConfiguration::save(std::ostream& stream) const {
  videoMode.save(stream);
  framerate.save(stream);
  for (std::map<std::string, FireCAMFeature>::const_iterator it =
      features.begin(); it != features.end(); ++it)
    it->second.save(stream);
  colorFilter.save(stream);
}
