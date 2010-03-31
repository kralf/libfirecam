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

#include "videomode.h"
#include "colorfilter.h"
#include "framerate.h"
#include "feature.h"
#include "capture.h"

class FireCAMConfiguration {
friend class FireCAMCamera;
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
  /** Access the framerate
    */
  void setFramerate(const FireCAMFramerate& framerate);
  const FireCAMFramerate& getFramerate() const;
  /** Access the features
    */
  void setFeature(const FireCAMFeature& feature);
  const FireCAMFeature& getFeature(const char* name) const;

  /** Access the color filter
    */
  void setColorFilter(const FireCAMColorFilter& colorFilter);
  const FireCAMColorFilter& getColorFilter() const;
  /** Access the capture
    */
  void setCapture(const FireCAMCapture& capture);
  const FireCAMCapture& getCapture() const;

  /** Load the FireCAM configuration from the file with the specified
    * filename
    */
  void load(const char* filename);

  /** Load the FireCAM configuration from the given stream
    */
  void load(std::istream& stream);
  /** Save the FireCAM configuration to the given stream
    */
  void save(std::ostream& stream) const;
protected:
  FireCAMVideoMode videoMode;
  FireCAMFramerate framerate;
  std::map<std::string, FireCAMFeature> features;

  FireCAMColorFilter colorFilter;
  FireCAMCapture capture;
};

#endif
