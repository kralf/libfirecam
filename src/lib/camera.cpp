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

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

FireCAMCamera::FireCAMCamera(dc1394_t* context, uint64_t guid) :
  context(context),
  device(dc1394_camera_new(context, guid)) {
  dc1394video_modes_t modes;
  dc1394error_t error = dc1394_video_get_supported_modes(device, &modes);
  if (error != DC1394_SUCCESS) {
    std::ostringstream what;
    what << "Failed to query video modes: " <<
      dc1394_error_get_string(error);
    throw std::runtime_error(what.str());
  }

  for (int i = 0; i < modes.num; ++i) {
    if (dc1394_is_video_mode_scalable(modes.modes[i])) {
      dc1394color_codings_t codings;
      error = dc1394_format7_get_color_codings(device, modes.modes[i],
        &codings);
      if (error != DC1394_SUCCESS) {
        std::ostringstream what;
        what << "Failed to query color codings: " <<
          dc1394_error_get_string(error);
        throw std::runtime_error(what.str());
      }
      for (int j = 0; j < codings.num; ++j)
        videoModes.push_back(FireCAMVideoMode(device, modes.modes[i],
          codings.codings[j]));
    }
    else {
      dc1394color_coding_t coding;
      error = dc1394_get_color_coding_from_video_mode(device, modes.modes[i],
        &coding);
      if (error != DC1394_SUCCESS) {
        std::ostringstream what;
        what << "Failed to query color coding: " <<
          dc1394_error_get_string(error);
        throw std::runtime_error(what.str());
      }
      FireCAMVideoMode videoMode(device, modes.modes[i], coding);
      videoModes.push_back(videoMode);

      dc1394framerates_t framerates;
      error = dc1394_video_get_supported_framerates(device, modes.modes[i],
        &framerates);
      if (error != DC1394_SUCCESS) {
        std::ostringstream what;
        what << "Failed to query framerates: " <<
          dc1394_error_get_string(error);
        throw std::runtime_error(what.str());
      }
      for (int j = 0; j < framerates.num; ++j)
        this->framerates[videoMode].push_back(FireCAMFramerate(
          framerates.framerates[j]));
    }
  }
}

FireCAMCamera::FireCAMCamera(const FireCAMCamera& src) :
  context(0),
  device(0) {
  operator=(src);
}

FireCAMCamera::~FireCAMCamera() {
  if (device)
    dc1394_camera_free(device);
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

dc1394camera_t* FireCAMCamera::getDevice() const {
  return device;
}

uint64_t FireCAMCamera::getGUID() const {
  return device->guid;
}

const char* FireCAMCamera::getModel() const {
  return device->model;
}

const char* FireCAMCamera::getVendor() const {
  return device->vendor;
}

const std::list<FireCAMVideoMode>& FireCAMCamera::getVideoModes() const {
  return videoModes;
}

const std::list<FireCAMFramerate>& FireCAMCamera::getFramerates(const
    FireCAMVideoMode& videoMode) const {
  std::map<FireCAMVideoMode, std::list<FireCAMFramerate> >::const_iterator
    it = framerates.find(videoMode);

  if (it == framerates.end()) {
    std::ostringstream what;
    what << "Bad video mode in framerate query: ";
    videoMode.write(what);
    throw std::runtime_error(what.str());
  }
  else
    return it->second;
}

void FireCAMCamera::setConfiguration(const FireCAMConfiguration&
    configuration) {
  this->configuration = configuration;
}

const FireCAMConfiguration& FireCAMCamera::getConfiguration() const {
  return configuration;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

FireCAMCamera& FireCAMCamera::operator=(const FireCAMCamera& src) {
  if (device)
    dc1394_camera_free(device);

  context = src.context;
  device = dc1394_camera_new(src.context, src.device->guid);

  configuration = src.configuration;

  videoModes = src.videoModes;
  framerates = src.framerates;

  return *this;
}

bool FireCAMCamera::connect() {
  return false;
}

void FireCAMCamera::disconnect() {
}

void FireCAMCamera::write(std::ostream& stream) const {
  stream << "GUID: " << std::hex << device->guid << std::endl;
  stream << "Vendor: " << device->vendor << std::endl;
  stream << "Model: " << device->model << std::endl;
}
