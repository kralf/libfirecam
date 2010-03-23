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

#include <algorithm>

#include "feature.h"

#include "utils.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

FireCAMFeature::FireCAMFeature(const char* name, size_t value, bool enabled,
    Mode mode) :
  name(name ? name : ""),
  enabled(enabled),
  mode(mode),
  readable(true),
  switchable(true) {
  values.push_back(value);

  modes.push_back(manual);
  modes.push_back(permanentAuto);
  modes.push_back(onePushAuto);
}

FireCAMFeature::FireCAMFeature(const char* name, const std::vector<size_t>&
    values, bool enabled, Mode mode) :
  name(name),
  values(values),
  enabled(enabled),
  mode(mode),
  readable(true),
  switchable(true) {
  modes.push_back(manual);
  modes.push_back(permanentAuto);
  modes.push_back(onePushAuto);
}

FireCAMFeature::FireCAMFeature(dc1394camera_t* device, dc1394feature_info_t
    info) :
  feature(info.id),
  enabled(true),
  mode(manual),
  readable(false),
  switchable(false) {
  name = dc1394_feature_get_string(feature);

  for (int i = 0; i < name.size(); ++i)
    name[i] = std::tolower(name[i]);
  std::replace(name.begin(), name.end(), ' ', '_');
}

FireCAMFeature::FireCAMFeature(dc1394camera_t* device, dc1394feature_t
    feature) :
  feature(feature) {
  name = dc1394_feature_get_string(feature);

  for (int i = 0; i < name.size(); ++i)
    name[i] = std::tolower(name[i]);
  std::replace(name.begin(), name.end(), ' ', '_');

  readParameters(device);
}

FireCAMFeature::FireCAMFeature(const FireCAMFeature& src) :
  name(src.name),
  feature(src.feature),
  values(src.values),
  enabled(src.enabled),
  mode(src.mode),
  readable(src.readable),
  switchable(src.switchable),
  modes(src.modes) {
}

FireCAMFeature::~FireCAMFeature() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

void FireCAMFeature::setName(const char* name) {
  this->name = name;
}

const std::string& FireCAMFeature::getName() const {
  return name;
}

void FireCAMFeature::setValue(size_t value) {
  values.clear();
  values.push_back(value);
}

void FireCAMFeature::setValues(const std::vector<size_t>& values) {
  this->values = values;
}

const std::vector<size_t>& FireCAMFeature::getValues() const {
  return values;
}

void FireCAMFeature::setEnabled(bool enabled) {
  this->enabled = enabled;
}

bool FireCAMFeature::isEnabled() const {
  return enabled;
}

void FireCAMFeature::setMode(Mode mode) {
  this->mode = mode;
}

FireCAMFeature::Mode FireCAMFeature::getMode() const {
  return mode;
}

bool FireCAMFeature::isReadable() const {
  return readable;
}

bool FireCAMFeature::isSwitchable() const {
  return switchable;
}

const std::list<FireCAMFeature::Mode>& FireCAMFeature::getModes() const {
  return modes;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

FireCAMFeature& FireCAMFeature::operator=(const FireCAMFeature& src) {
  feature = src.feature;

  name = src.name;
  values = src.values;
  enabled = src.enabled;
  mode = src.mode;

  readable = src.readable;
  switchable = src.switchable;
  modes = src.modes;

  return *this;
}

void FireCAMFeature::write(std::ostream& stream) const {
  stream << name << ": ";

  for (int i = 0; i < values.size(); ++i) {
    if (i)
      stream << ", ";
    stream << values[i];
  }
  stream << " (";
  if (enabled)
    stream << "on";
  else
    stream << "off";
  stream << ", ";
  if (mode == permanentAuto)
    stream << "auto";
  else if (mode == onePushAuto)
    stream << "one_push_auto";
  else
    stream << "manual";
  if (readable)
    stream << ", readable";
  if (switchable)
    stream << ", switchable";
  stream << ")";
}

void FireCAMFeature::load(std::istream& stream) {
  std::string module, name, value, option;
  std::getline(stream, module, '.');
  std::getline(stream, name, '.');
  std::getline(stream, option, '=');
  std::getline(stream, value);

  if (module == "features") {
    this->name = name;

    if (option == "enabled")
      enabled = FireCAMUtils::convert<bool>(value);
    else if (option == "values") {
      values.clear();
      std::istringstream valueStream(value);

      while (!valueStream.eof()) {\
        std::getline(valueStream, value, ',');
        values.push_back(FireCAMUtils::convert<size_t>(value));
      }
    }
    else if (option == "mode") {
      if (value == "auto")
        mode = permanentAuto;
      else if (value == "one_push_auto")
        mode = onePushAuto;
      else
        mode = manual;
    }
    else
      FireCAMUtils::error("Bad feature option: ", option);
  }
}

void FireCAMFeature::save(std::ostream& stream) const {
  if (switchable)
    stream << "features." << name << ".enabled = " << enabled << std::endl;

  if (!values.empty()) {
    stream << "features." << name << ".values = ";
    for (int i = 0; i < values.size(); ++i) {
      if (i)
        stream << ", ";
      stream << values[i];
    }
    stream << std::endl;
  }

  if (modes.size() > 1) {
    stream << "features." << name << ".mode = ";
    if (mode == permanentAuto)
      stream << "auto";
    else if (mode == onePushAuto)
      stream << "one_push_auto";
    else
      stream << "manual";
    stream << std::endl;
  }
}

void FireCAMFeature::readParameters(dc1394camera_t* device) {
  dc1394bool_t readable;
  FireCAMUtils::assert("Failed to query feature readability",
    dc1394_feature_is_readable(device, feature, &readable));
  this->readable = readable;

  dc1394bool_t switchable;
  FireCAMUtils::assert("Failed to query feature switchability",
    dc1394_feature_is_switchable(device, feature, &switchable));
  this->switchable = switchable;

  if (this->readable) {
    if (feature == DC1394_FEATURE_TEMPERATURE) {
      uint32_t target, current;
      FireCAMUtils::assert("Failed to query temperature values",
        dc1394_feature_temperature_get_value(device, &target, &current));
      values.push_back(target);
    }
    else if (feature == DC1394_FEATURE_WHITE_BALANCE) {
      uint32_t ub, vr;
      FireCAMUtils::assert("Failed to query white balance values",
        dc1394_feature_whitebalance_get_value(device, &ub, &vr));
      values.push_back(ub);
      values.push_back(vr);
    }
    else if (feature == DC1394_FEATURE_WHITE_SHADING) {
      uint32_t r, g, b;
      FireCAMUtils::assert("Failed to query white shading values",
        dc1394_feature_whiteshading_get_value(device, &r, &g, &b));
      values.push_back(r);
      values.push_back(g);
      values.push_back(b);
    }
    else {
      uint32_t value;
      FireCAMUtils::assert("Failed to query feature value",
        dc1394_feature_get_value(device, feature, &value));
      values.push_back(value);
    }
  }

  if (this->switchable) {
    dc1394switch_t enabled;
    FireCAMUtils::assert("Failed to query feature power state",
      dc1394_feature_get_power(device, feature, &enabled));
    this->enabled = enabled;
  }

  dc1394feature_mode_t mode;
  FireCAMUtils::assert("Failed to query feature mode",
    dc1394_feature_get_mode(device, feature, &mode));
  if (mode == DC1394_FEATURE_MODE_AUTO)
    this->mode = permanentAuto;
  else if (mode == DC1394_FEATURE_MODE_ONE_PUSH_AUTO)
    this->mode = onePushAuto;

  modes.clear();
  dc1394feature_modes_t modes;
  FireCAMUtils::assert("Failed to query feature modes",
    dc1394_feature_get_modes(device, feature, &modes));
  for (int i = 0; i < modes.num; ++i) {
    if (modes.modes[i] == DC1394_FEATURE_MODE_AUTO)
      this->modes.push_back(permanentAuto);
    else if (modes.modes[i] == DC1394_FEATURE_MODE_ONE_PUSH_AUTO)
      this->modes.push_back(onePushAuto);
    else
      this->modes.push_back(manual);
  }
}
