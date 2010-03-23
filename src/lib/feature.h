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

#ifndef FIRECAM_FEATURE_H
#define FIRECAM_FEATURE_H

/** \file feature.h
  * \brief FireCAM feature definition
  */

#include <vector>

#include <dc1394/dc1394.h>

class FireCAMFeature {
friend class FireCAMCamera;
public:
  /** Types and subclasses
    */
  enum Mode {
    manual,
    permanentAuto,
    onePushAuto
  };

  /** Construct a FireCAM feature object
    */
  FireCAMFeature(size_t value = 0, bool enabled = true, Mode mode = manual);
  FireCAMFeature(const std::vector<size_t>& values, bool enabled = true,
    Mode mode = manual);
  FireCAMFeature(const FireCAMFeature& src);

  /** Destroy a FireCAM feature object
    */
  virtual ~FireCAMFeature();

  /** Access the name of the feature
    */
  const char* getName() const;
  /** Access the values of the feature
    */
  void setValue(size_t value);
  void setValues(const std::vector<size_t>& values);
  const std::vector<size_t>& getValues() const;
  /** Access the enabled flag of the feature
    */
  void setEnabled(bool enabled);
  bool isEnabled() const;
  /** Access the mode of the feature
    */
  void setMode(Mode mode);
  Mode getMode() const;

  /** FireCAM feature assignments
    */
  FireCAMFeature& operator=(const FireCAMFeature& src);

  /** Write feature information to the given stream
    */
  void write(std::ostream& stream) const;
protected:
  dc1394feature_t feature;

  std::vector<size_t> values;
  bool enabled;
  Mode mode;

  /** Construct a FireCAM feature object
    */
  FireCAMFeature(dc1394camera_t* camera, dc1394feature_t feature);
};

#endif
