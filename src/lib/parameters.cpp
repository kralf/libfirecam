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

#include "parameters.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

FireCAMParameters::FireCAMParameters(
    bool hueEnabled,
    bool saturationEnabled,
    bool gammaEnabled,
    bool autoGain,
    bool autoShutter,
    bool autoWhiteBalance,
    bool autoExposure,
    size_t gain,
    size_t shutter,
    size_t whiteBalanceBlue,
    size_t whiteBalanceRed,
    size_t brightness,
    size_t hue,
    size_t saturation,
    size_t gamma,
    size_t exposure,
    size_t pan,
    size_t tilt,
    size_t sharpness) :
  hueEnabled(hueEnabled),
  saturationEnabled(saturationEnabled),
  gammaEnabled(gammaEnabled),
  autoGain(autoGain),
  autoShutter(autoShutter),
  autoWhiteBalance(autoWhiteBalance),
  autoExposure(autoExposure),
  gain(gain),
  shutter(shutter),
  whiteBalanceBlue(whiteBalanceBlue),
  whiteBalanceRed(whiteBalanceRed),
  brightness(brightness),
  hue(hue),
  saturation(saturation),
  gamma(gamma),
  exposure(exposure),
  pan(pan),
  tilt(tilt),
  sharpness(sharpness) {
}

FireCAMParameters::FireCAMParameters(const FireCAMParameters& src) :
  hueEnabled(src.hueEnabled),
  saturationEnabled(src.saturationEnabled),
  gammaEnabled(src.gammaEnabled),
  autoGain(src.autoGain),
  autoShutter(src.autoShutter),
  autoWhiteBalance(src.autoWhiteBalance),
  autoExposure(src.autoExposure),
  gain(src.gain),
  shutter(src.shutter),
  whiteBalanceBlue(src.whiteBalanceBlue),
  whiteBalanceRed(src.whiteBalanceRed),
  brightness(src.brightness),
  hue(src.hue),
  saturation(src.saturation),
  gamma(src.gamma),
  exposure(src.exposure),
  pan(src.pan),
  tilt(src.tilt),
  sharpness(src.sharpness) {
}

FireCAMParameters::~FireCAMParameters() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

void FireCAMParameters::setHueEnabled(bool hueEnabled) {
  this->hueEnabled = hueEnabled;
}

bool FireCAMParameters::isHueEnabled() const {
  return hueEnabled;
}

void FireCAMParameters::setSaturationEnabled(bool saturationEnabled) {
  this->saturationEnabled = saturationEnabled;
}

bool FireCAMParameters::isSaturationEnabled() const {
  return saturationEnabled;
}

void FireCAMParameters::setGammaEnabled(bool gammaEnabled) {
  this->gammaEnabled = gammaEnabled;
}

bool FireCAMParameters::isGammaEnabled() const {
  return gammaEnabled;
}

void FireCAMParameters::setAutoGain(bool autoGain) {
  this->autoGain = autoGain;
}

bool FireCAMParameters::isAutoGain() const {
  return autoGain;
}

void FireCAMParameters::setAutoShutter(bool autoShutter) {
  this->autoShutter = autoShutter;
}

bool FireCAMParameters::isAutoShutter() const {
  return autoShutter;
}

void FireCAMParameters::setAutoWhiteBalance(bool autoWhiteBalance) {
  this->autoWhiteBalance = autoWhiteBalance;
}

bool FireCAMParameters::isAutoWhiteBalance() const {
  return autoWhiteBalance;
}

void FireCAMParameters::setAutoExposure(bool autoExposure) {
  this->autoExposure = autoExposure;
}

bool FireCAMParameters::isAutoExposure() const {
  return autoExposure;
}

void FireCAMParameters::setGain(size_t gain) {
  this->gain = gain;
}

size_t FireCAMParameters::getGain() const {
  return gain;
}

void FireCAMParameters::setShutter(size_t shutter) {
  this->shutter = shutter;
}

size_t FireCAMParameters::getShutter() const {
  return shutter;
}

void FireCAMParameters::setWhiteBalanceBlue(size_t whiteBalanceBlue) {
  this->whiteBalanceBlue = whiteBalanceBlue;
}

size_t FireCAMParameters::getWhiteBalanceBlue() const {
  return whiteBalanceBlue;
}

void FireCAMParameters::setWhiteBalanceRed(size_t whiteBalanceRed) {
  this->whiteBalanceRed = whiteBalanceRed;
}

size_t FireCAMParameters::getWhiteBalanceRed() const {
  return whiteBalanceRed;
}

void FireCAMParameters::setBrightness(size_t brightness) {
  this->brightness = brightness;
}

size_t FireCAMParameters::getBrightness() const {
  return brightness;
}

void FireCAMParameters::setHue(size_t hue) {
  this->hue = hue;
}

size_t FireCAMParameters::getHue() const {
  return hue;
}

void FireCAMParameters::setSaturation(size_t saturation) {
  this->saturation = saturation;
}

size_t FireCAMParameters::getSaturation() const {
  return saturation;
}

void FireCAMParameters::setGamma(size_t gamma) {
  this->gamma = gamma;
}

size_t FireCAMParameters::getGamma() const {
  return gamma;
}

void FireCAMParameters::setExposure(size_t exposure) {
  this->exposure = exposure;
}

size_t FireCAMParameters::getExposure() const {
  return exposure;
}

void FireCAMParameters::setPan(size_t pan) {
  this->pan = pan;
}

size_t FireCAMParameters::getPan() const {
  return pan;
}

void FireCAMParameters::setTilt(size_t tilt) {
  this->tilt = tilt;
}

size_t FireCAMParameters::getTilt() const {
  return tilt;
}

void FireCAMParameters::setSharpness(size_t sharpness) {
  this->sharpness = sharpness;
}

size_t FireCAMParameters::getSharpness() const {
  return sharpness;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

FireCAMParameters& FireCAMParameters::operator=(const FireCAMParameters& src) {
  hueEnabled = src.hueEnabled;
  saturationEnabled = src.saturationEnabled;
  gammaEnabled = src.gammaEnabled;

  autoGain = src.autoGain;
  autoShutter = src.autoShutter;
  autoWhiteBalance = src.autoWhiteBalance;
  autoExposure = src.autoExposure;

  gain = src.gain;
  shutter = src.shutter;
  whiteBalanceBlue = src.whiteBalanceBlue;
  whiteBalanceRed = src.whiteBalanceRed;
  brightness = src.brightness;
  hue = src.hue;
  saturation = src.saturation;
  gamma = src.gamma;
  exposure = src.exposure;

  pan = src.pan;
  tilt = src.tilt;
  sharpness = src.sharpness;

  return *this;
}
