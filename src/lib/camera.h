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

#ifndef FIRECAM_CAMERA_H
#define FIRECAM_CAMERA_H

/** \file camera.h
  * \brief FireCAM camera class
  */

#include <set>

#include "configuration.h"

class FireCAMCamera {
friend class FireCAM;
public:
  /** Construct a FireCAM camera object
    */
  FireCAMCamera(const FireCAMCamera& src);

  /** Destroy a FireCAM camera object
    */
  virtual ~FireCAMCamera();

  /** Access the camera's raw device
    */
  dc1394camera_t* getDevice() const;

  /** Access the camera's GUID
    */
  uint64_t getGUID() const;
  /** Access the camera model
    */
  const char* getModel() const;
  /** Access the camera vendor
    */
  const char* getVendor() const;

  /** Access the camera's video modes
    */
  const std::set<FireCAMVideoMode>& getVideoModes() const;
  /** Access the camera's framerates for the given video mode
    */
  const std::set<FireCAMFramerate>& getFramerates(const FireCAMVideoMode&
    videoMode) const;
  /** Access the camera's features
    */
  const std::set<FireCAMFeature>& getFeatures() const;

  /** Access the camera's configuration
    */
  void setConfiguration(const FireCAMConfiguration& configuration);
  const FireCAMConfiguration& getConfiguration() const;

  /** Access the camera's transmission flag
    */
  bool isTransmitting() const;

  /** FireCAM camera assignments
    */
  FireCAMCamera& operator=(const FireCAMCamera& src);

  /** Connect or disconnect the camera
    */
  void connect();
  void disconnect();

  /** Reset camera to factory defaults
    */
  void reset();

  /** Write camera information to the given stream
    */
  void write(std::ostream& stream) const;
protected:
  dc1394_t* context;
  dc1394camera_t* device;

  std::set<FireCAMVideoMode> videoModes;
  std::map<FireCAMVideoMode, std::set<FireCAMFramerate> > framerates;
  std::set<FireCAMFeature> features;

  FireCAMConfiguration configuration;

  /** Construct a FireCAM camera object
    */
  FireCAMCamera(dc1394_t* context, uint64_t guid);

  /** Read the supported video modes from the device
    */
  void readVideoModes();
  /** Read the supported framerates from the device
    */
  void readFramerates();
  /** Read the supported features from the device
    */
  void readFeatures();

  /** Read the configuration from the camera
    */
  void readConfiguration();
  /** Write the configuration to the camera
    */
  void writeConfiguration();
};

#endif
