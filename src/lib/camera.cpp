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

#include "camera.h"

#include "firecam.h"
#include "utils.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

FireCAMCamera::FireCAMCamera() :
  context(0),
  device(0) {
}

FireCAMCamera::FireCAMCamera(dc1394_t* context, uint64_t guid) :
  context(context),
  device(dc1394_camera_new(context, guid)) {
  readVideoModes();
  readFramerates();
  readFeatures();

  readConfiguration();
}

FireCAMCamera::FireCAMCamera(const FireCAMCamera& src) :
  context(0),
  device(0) {
  operator=(src);
}

FireCAMCamera::~FireCAMCamera() {
  if (isTransmitting())
    disconnect();

  if (device)
    dc1394_camera_free(device);
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

uint64_t FireCAMCamera::getGUID() const {
  return device->guid;
}

const char* FireCAMCamera::getModel() const {
  return device->model;
}

const char* FireCAMCamera::getVendor() const {
  return device->vendor;
}

const std::set<FireCAMVideoMode>& FireCAMCamera::getVideoModes() const {
  return videoModes;
}

const std::set<FireCAMFramerate>& FireCAMCamera::getFramerates(const
    FireCAMVideoMode& videoMode) const {
  std::map<FireCAMVideoMode, std::set<FireCAMFramerate> >::const_iterator
    it = framerates.find(videoMode);

  if (it == framerates.end()) {
    std::ostringstream stream;
    videoMode.write(stream);

    FireCAMUtils::error("Bad video mode in framerate query", stream.str());
  }
  else
    return it->second;
}

void FireCAMCamera::setConfiguration(const FireCAMConfiguration&
    configuration) {
  if (isTransmitting()) {
    this->configuration = configuration;
    writeConfiguration();
  }
  else
    this->configuration = configuration;
}

const std::set<FireCAMFeature>& FireCAMCamera::getFeatures() const {
  return features;
}

const FireCAMConfiguration& FireCAMCamera::getConfiguration() const {
  return configuration;
}

bool FireCAMCamera::isTransmitting() const {
  if (device) {
    dc1394switch_t transmitting;
    FireCAMUtils::assert("Failed to query transmission status",
      dc1394_video_get_transmission(device, &transmitting));

    return transmitting;
  }
  else
    return false;
}

bool FireCAMCamera::hasVideoMode(const FireCAMVideoMode& videoMode) const {
  std::set<FireCAMVideoMode>::const_iterator it = videoModes.find(videoMode);
}

bool FireCAMCamera::hasFeature(const FireCAMFeature& feature) const {
  std::set<FireCAMFeature>::const_iterator it = features.find(feature);
  return (it != features.end());
}

double FireCAMCamera::getBandwidthUsage() const {
  uint32_t bandwidth;
  FireCAMUtils::assert("Failed to query bandwidth usage",
    dc1394_video_get_bandwidth_usage(device, &bandwidth));

  return bandwidth/4915.0;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

FireCAMCamera& FireCAMCamera::operator=(const FireCAMCamera& src) {
  if (device) {
    if (isTransmitting())
      disconnect();
    dc1394_camera_free(device);
  }

  if (!src.device)
    FireCAMUtils::error("Failed to assign camera", "No such device");

  context = src.context;
  device = dc1394_camera_new(src.context, src.device->guid);

  videoModes = src.videoModes;
  framerates = src.framerates;
  features = src.features;

  configuration = src.configuration;

  return *this;
}

void FireCAMCamera::connect() {
  if (!isTransmitting()) {
    FireCAMUtils::assert("Failed to release channels",
      dc1394_iso_release_all(device));
    writeConfiguration();
    FireCAMUtils::assert("Failed to start transmission",
      dc1394_video_set_transmission(device, DC1394_ON));
  }
}

void FireCAMCamera::disconnect() {
  if (isTransmitting()) {
    FireCAMUtils::assert("Failed to stop transmission",
      dc1394_video_set_transmission(device, DC1394_OFF));
    dc1394_capture_stop(device);
  }
}

void FireCAMCamera::capture(FireCAMFrame& frame) {
  FireCAMFrame rawFrame(device);
  configuration.colorFilter.filter(rawFrame, frame, rawFrame.getColor());
}

void FireCAMCamera::capture(FireCAMFrame& frame, const FireCAMColor& color) {
  FireCAMFrame rawFrame(device);
  configuration.colorFilter.filter(rawFrame, frame, color);
}

void FireCAMCamera::reset() {
  FireCAMUtils::assert("Failed to reset camera",  dc1394_camera_reset(device));
  readConfiguration();
}

void FireCAMCamera::write(std::ostream& stream) const {
  stream << "GUID: " << std::hex << device->guid << std::endl;
  stream << "Vendor: " << device->vendor << std::endl;
  stream << "Model: " << device->model << std::endl;
}

void FireCAMCamera::readVideoModes() {
  videoModes.clear();

  dc1394video_modes_t modes;
  FireCAMUtils::assert("Failed to query video modes",
    dc1394_video_get_supported_modes(device, &modes));

  for (int i = 0; i < modes.num; ++i) {
    if (dc1394_is_video_mode_scalable(modes.modes[i])) {
      dc1394color_codings_t codings;
      FireCAMUtils::assert("Failed to query color codings",
        dc1394_format7_get_color_codings(device, modes.modes[i], &codings));
      for (int j = 0; j < codings.num; ++j)
        videoModes.insert(FireCAMVideoMode(device, modes.modes[i],
          codings.codings[j]));
    }
    else {
      dc1394color_coding_t coding;
      FireCAMUtils::assert("Failed to query color coding",
        dc1394_get_color_coding_from_video_mode(device, modes.modes[i],
          &coding));
      FireCAMVideoMode videoMode(device, modes.modes[i], coding);
      videoModes.insert(videoMode);
    }
  }
}

void FireCAMCamera::readFramerates() {
  framerates.clear();

  for (std::set<FireCAMVideoMode>::const_iterator it = videoModes.begin();
      it != videoModes.end(); ++it) {
    if (!it->isScalable()) {
      dc1394framerates_t framerates;
      FireCAMUtils::assert("Failed to query framerates",
        dc1394_video_get_supported_framerates(device, it->mode, &framerates));
      for (int j = 0; j < framerates.num; ++j)
        this->framerates[*it].insert(FireCAMFramerate(
          framerates.framerates[j]));
    }
  }
}

void FireCAMCamera::readFeatures() {
  features.clear();

  dc1394featureset_t features;
  FireCAMUtils::assert("Failed to query features",
    dc1394_feature_get_all(device, &features));

  for (int i = 0; i < DC1394_FEATURE_NUM; ++i) {
    dc1394bool_t present;
    FireCAMUtils::assert("Failed to query feature presence",
      dc1394_feature_is_present(device, features.feature[i].id, &present));
    if (present)
      this->features.insert(FireCAMFeature(device, features.feature[i].id));
  }
}

void FireCAMCamera::readConfiguration() {
  configuration.videoMode = FireCAMVideoMode(device);
  configuration.framerate = FireCAMFramerate(device);
  for (std::set<FireCAMFeature>::const_iterator it = features.begin();
      it != features.end(); ++it)
    configuration.features[it->name] = FireCAMFeature(device, it->feature);
}

void FireCAMCamera::writeConfiguration() {
  std::set<FireCAMVideoMode>::const_iterator itVideoMode = videoModes.find(
    configuration.videoMode);
  if (itVideoMode == videoModes.end()) {
    std::ostringstream stream;
    configuration.videoMode.write(stream);
    FireCAMUtils::error("Video mode not supported", stream.str());
  }
  else
    itVideoMode->writeParameters(device);

  std::set<FireCAMFramerate>::const_iterator itFramerate =
    framerates[*itVideoMode].find(configuration.framerate);
  if (itFramerate == framerates[*itVideoMode].end()) {
    std::ostringstream stream;
    configuration.framerate.write(stream);
    FireCAMUtils::error("Framerate not supported", stream.str());
  }
  else
    itFramerate->writeParameters(device);

  readFeatures();

  for (std::map<std::string, FireCAMFeature>::const_iterator
      itConfigurationFeature = configuration.features.begin();
      itConfigurationFeature != configuration.features.end();
      ++itConfigurationFeature) {
    std::set<FireCAMFeature>::const_iterator itFeature =
      features.find(itConfigurationFeature->second);
    if (itFeature == features.end()) {
      std::ostringstream stream;
      itConfigurationFeature->second.write(stream);
      FireCAMUtils::error("Feature not supported", stream.str());
    }
    else
      itFeature->writeParameters(device, itConfigurationFeature->second);
  }

  if (!isTransmitting())
    configuration.capture.writeParameters(device);
}
