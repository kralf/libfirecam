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

#include <sstream>
#include <stdexcept>

#include "feature.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

FireCAMFeature::FireCAMFeature(size_t value, bool enabled, Mode mode) :
  enabled(enabled),
  mode(mode) {
  values.push_back(value);
}

FireCAMFeature::FireCAMFeature(const std::vector<size_t>& values,
    bool enabled, Mode mode) :
  values(values),
  enabled(enabled),
  mode(mode) {
}

FireCAMFeature::FireCAMFeature(dc1394camera_t* camera, dc1394feature_t
    feature) :
  feature(feature),
  enabled(true),
  mode(manual) {
  dc1394error_t error;

  dc1394bool_t readable;
  error = dc1394_feature_is_readable(camera, feature, &readable);
  if (error != DC1394_SUCCESS) {
    std::ostringstream what;
    what << "Failed to query feature readability: " <<
      dc1394_error_get_string(error);
    throw std::runtime_error(what.str());
  }

  if (readable) {
    if (feature == DC1394_FEATURE_TEMPERATURE) {
      uint32_t target, current;
      error = dc1394_feature_temperature_get_value(camera, &target, &current);
      values.push_back(target);
    }
    else if (feature == DC1394_FEATURE_WHITE_BALANCE) {
      uint32_t ub, vr;
      error = dc1394_feature_whitebalance_get_value(camera, &ub, &vr);
      values.push_back(ub);
      values.push_back(vr);
    }
    else if (feature == DC1394_FEATURE_WHITE_SHADING) {
      uint32_t r, g, b;
      error = dc1394_feature_whiteshading_get_value(camera, &r, &g, &b);
      values.push_back(r);
      values.push_back(g);
      values.push_back(b);
    }
    else {
      uint32_t value;
      error = dc1394_feature_get_value(camera, feature, &value);
      values.push_back(value);
    }
    if (error != DC1394_SUCCESS) {
      std::ostringstream what;
      what << "Failed to query feature value: " <<
        dc1394_error_get_string(error);
      throw std::runtime_error(what.str());
    }
  }

  dc1394bool_t switchable;
  error = dc1394_feature_is_switchable(camera, feature, &switchable);
  if (error != DC1394_SUCCESS) {
    std::ostringstream what;
    what << "Failed to query feature switchability: " <<
      dc1394_error_get_string(error);
    throw std::runtime_error(what.str());
  }
  if (switchable) {
    dc1394switch_t enabled;
    error = dc1394_feature_get_power(camera, feature, &enabled);
    if (error != DC1394_SUCCESS) {
      std::ostringstream what;
      what << "Failed to query feature power: " <<
        dc1394_error_get_string(error);
      throw std::runtime_error(what.str());
    }
    this->enabled = enabled;
  }

  dc1394feature_mode_t mode;
  error = dc1394_feature_get_mode(camera, feature, &mode);
  if (error != DC1394_SUCCESS) {
    std::ostringstream what;
    what << "Failed to query feature mode: " <<
      dc1394_error_get_string(error);
    throw std::runtime_error(what.str());
  }
  if (mode == DC1394_FEATURE_MODE_AUTO)
    this->mode = permanentAuto;
  else if (mode == DC1394_FEATURE_MODE_ONE_PUSH_AUTO)
    this->mode = onePushAuto;
}

FireCAMFeature::FireCAMFeature(const FireCAMFeature& src) :
  feature(src.feature),
  values(src.values),
  enabled(src.enabled),
  mode(src.mode) {
}

FireCAMFeature::~FireCAMFeature() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

const char* FireCAMFeature::getName() const {
  return dc1394_feature_get_string(feature);
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

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

FireCAMFeature& FireCAMFeature::operator=(const FireCAMFeature& src) {
  feature = src.feature;

  values = src.values;
  enabled = src.enabled;
  mode = src.mode;

  return *this;
}

void FireCAMFeature::write(std::ostream& stream) const {
  stream << dc1394_feature_get_string(feature) << ": ";

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
  stream << ")";
}
