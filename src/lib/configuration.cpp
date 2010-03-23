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
  parameters(src.parameters),
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

void FireCAMConfiguration::setParameters(const FireCAMParameters& parameters) {
  this->parameters = parameters;
}

const FireCAMParameters& FireCAMConfiguration::getParameters() const {
  return parameters;
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
  parameters = src.parameters;

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

  if (!file.is_open()) {
    std::ostringstream what;
    what << "Error opening file: " << filename;
    throw std::runtime_error(what.str());
  }
  else
    read(file);
}

void FireCAMConfiguration::read(std::istream& stream) {
  while (!stream.eof()) {
    std::string line;
    std::getline(stream, line);
    std::string::iterator it = std::remove(line.begin(), line.end(), ' ');
    line.erase(it, line.end());

    if (line[0] != '#') {
      std::istringstream lineStream(line);
      std::string key, value;

      std::getline(lineStream, key, '=');
      std::getline(lineStream, value);

      if (key == "video_mode.resolution.width")
        videoMode.setWidth(readValue<size_t>(value));
      else if (key == "video_mode.resolution.height")
        videoMode.setHeight(readValue<size_t>(value));
      else if (key == "video_mode.resolution.depth")
        videoMode.setDepth(readValue<size_t>(value));
      else if (key == "video_mode.resolution.color")
        videoMode.setColor(readValue<bool>(value));
      else if (key == "video_mode.resolution.scalable")
        videoMode.setScalable(readValue<bool>(value));

      else if (key == "frame_rate.fps")
        framerate.setFramesPerSecond(readValue<double>(value));

      else if (key == "parameters.hue_enabled")
        parameters.setHueEnabled(readValue<bool>(value));
      else if (key == "parameters.saturation_enabled")
        parameters.setSaturationEnabled(readValue<bool>(value));
      else if (key == "parameters.gamma_enabled")
        parameters.setGammaEnabled(readValue<bool>(value));
      else if (key == "parameters.auto_gain")
        parameters.setAutoGain(readValue<bool>(value));
      else if (key == "parameters.auto_shutter")
        parameters.setAutoShutter(readValue<bool>(value));
      else if (key == "parameters.auto_white_balance")
        parameters.setAutoWhiteBalance(readValue<bool>(value));
      else if (key == "parameters.auto_exposure")
        parameters.setAutoExposure(readValue<bool>(value));
      else if (key == "parameters.gain")
        parameters.setGain(readValue<size_t>(value));
      else if (key == "parameters.shutter")
        parameters.setShutter(readValue<size_t>(value));
      else if (key == "parameters.white_balance_blue")
        parameters.setWhiteBalanceBlue(readValue<size_t>(value));
      else if (key == "parameters.white_balance_red")
        parameters.setWhiteBalanceRed(readValue<size_t>(value));
      else if (key == "parameters.brightness")
        parameters.setBrightness(readValue<size_t>(value));
      else if (key == "parameters.hue")
        parameters.setHue(readValue<size_t>(value));
      else if (key == "parameters.saturation")
        parameters.setSaturation(readValue<size_t>(value));
      else if (key == "parameters.gamma")
        parameters.setGamma(readValue<size_t>(value));
      else if (key == "parameters.exposure")
        parameters.setExposure(readValue<size_t>(value));
      else if (key == "parameters.pan")
        parameters.setPan(readValue<size_t>(value));
      else if (key == "parameters.tilt")
        parameters.setTilt(readValue<size_t>(value));
      else if (key == "parameters.sharpness")
        parameters.setSharpness(readValue<size_t>(value));

      else if (key == "color_filter.enabled")
        colorFilter.setEnabled(readValue<bool>(value));
      else if (key == "color_filter.pattern")
        colorFilter.setPattern(
          (FireCAMColorFilter::Pattern)readValue<int>(value));

      else if (!key.empty()) {
        std::ostringstream what;
        what << "Bad configuration key: " << key;
        throw std::runtime_error(what.str());
      }
    }
  }
}

void FireCAMConfiguration::write(std::ostream& stream) const {
  stream << "video_mode.resolution.width: " <<
    videoMode.getWidth() << std::endl;
  stream << "video_mode.resolution.height: " <<
    videoMode.getHeight() << std::endl;
  stream << "video_mode.resolution.depth: " <<
    videoMode.getDepth() << std::endl;
  stream << "video_mode.resolution.color: " <<
    videoMode.isColor() << std::endl;
  stream << "video_mode.resolution.scalable: " <<
    videoMode.isScalable() << std::endl;

  stream << "frame_rate.fps: " <<
    framerate.getFramesPerSecond() << std::endl;

  stream << "parameters.hue_enabled: " <<
    parameters.isHueEnabled() << std::endl;
  stream << "parameters.saturation_enabled: " <<
    parameters.isSaturationEnabled() << std::endl;
  stream << "parameters.gamma_enabled: " <<
    parameters.isGammaEnabled() << std::endl;
  stream << "parameters.auto_gain: " <<
    parameters.isAutoGain() << std::endl;
  stream << "parameters.auto_shutter: " <<
    parameters.isAutoShutter() << std::endl;
  stream << "parameters.auto_white_balance: " <<
    parameters.isAutoWhiteBalance() << std::endl;
  stream << "parameters.auto_exposure: " <<
    parameters.isAutoExposure() << std::endl;
  stream << "parameters.gain: " <<
    parameters.getGain() << std::endl;
  stream << "parameters.shutter: " <<
    parameters.getShutter() << std::endl;
  stream << "parameters.white_balance_blue: " <<
    parameters.getWhiteBalanceBlue() << std::endl;
  stream << "parameters.white_balance_red: " <<
    parameters.getWhiteBalanceRed() << std::endl;
  stream << "parameters.brightness: " <<
    parameters.getBrightness() << std::endl;
  stream << "parameters.hue: " <<
    parameters.getHue() << std::endl;
  stream << "parameters.saturation: " <<
    parameters.getSaturation() << std::endl;
  stream << "parameters.gamma: " <<
    parameters.getGamma() << std::endl;
  stream << "parameters.exposure: " <<
    parameters.getExposure() << std::endl;
  stream << "parameters.pan: " <<
    parameters.getPan() << std::endl;
  stream << "parameters.tilt: " <<
    parameters.getTilt() << std::endl;
  stream << "parameters.sharpness: " <<
    parameters.getSharpness() << std::endl;

  stream << "color_filter.enabled: " <<
    colorFilter.isEnabled() << std::endl;
  stream << "color_filter.pattern: " <<
    colorFilter.getPattern() << std::endl;
}
