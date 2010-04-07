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

#include <map>
#include <vector>
#include <list>
#include <string>

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

  class ModeStrings :
    public std::map<Mode, std::string> {
  public:
    /** Construct a feature mode string object
      */
    ModeStrings();
  };

  class ModePresets :
    public std::map<Mode, dc1394feature_mode_t> {
  public:
    /** Construct a feature mode preset object
      */
    ModePresets();
  };

  /** Construct a FireCAM feature object
    */
  FireCAMFeature(const char* name = 0, size_t value = 0, bool enabled = true,
    Mode mode = manual);
  FireCAMFeature(const char* name, const std::vector<size_t>& values, bool
    enabled = true, Mode mode = manual);
  FireCAMFeature(const FireCAMFeature& src);

  /** Destroy a FireCAM feature object
    */
  virtual ~FireCAMFeature();

  /** Access the name of the feature
    */
  void setName(const char* name);
  const std::string& getName() const;
  /** Access the values of the feature
    */
  void setValue(size_t value);
  void setValues(const std::vector<size_t>& values);
  const std::vector<size_t>& getValues() const;
  size_t operator[](int i) const;
  /** Access the enabled flag of the feature
    */
  void setEnabled(bool enabled);
  bool isEnabled() const;
  /** Access the mode of the feature
    */
  void setMode(Mode mode);
  Mode getMode() const;

  /** Access the feature's readability flag
    */
  bool isReadable() const;
  /** Access the feature's switchability flag
    */
  bool isSwitchable() const;
  /** Access the supported modes of the feature
    */
  const std::list<Mode>& getModes() const;
  /** Access the boundaries of the feature
    */
  const std::vector<size_t>& getBoundaries() const;

  /** FireCAM feature assignments
    */
  FireCAMFeature& operator=(const FireCAMFeature& src);

  /** FireCAM feature comparisons
    */
  bool operator==(const FireCAMFeature& feature) const;
  bool operator!=(const FireCAMFeature& feature) const;
  bool operator<(const FireCAMFeature& feature) const;

  /** Write feature information to the given stream
    */
  void write(std::ostream& stream) const;

  /** Load feature configuration from the given stream
    */
  void load(std::istream& stream);
  /** Save feature configuration to the given stream
    */
  void save(std::ostream& stream) const;

  static const ModeStrings modeStrings;
  static const ModePresets modePresets;
protected:
  dc1394feature_t feature;

  std::string name;
  std::vector<size_t> values;
  bool enabled;
  Mode mode;

  bool readable;
  bool switchable;
  std::list<Mode> modes;
  std::vector<size_t> boundaries;

  /** Construct a FireCAM feature object
    */
  FireCAMFeature(dc1394camera_t* device, dc1394feature_info_t feature);
  FireCAMFeature(dc1394camera_t* device, dc1394feature_t feature);

  /** Read the feature parameters from the device
    */
  void readParameters(dc1394camera_t* device);
  /** Write the feature parameters to the device
    */
  void writeParameters(dc1394camera_t* device, const FireCAMFeature& feature)
    const;
};

#endif
