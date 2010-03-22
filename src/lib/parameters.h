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

#ifndef FIRECAM_PARAMETERS_H
#define FIRECAM_PARAMETERS_H

/** \file parameters.h
  * \brief FireCAM parameter definition
  */

#include <stdlib.h>

class FireCAMParameters {
public:
  /** Construct a FireCAM parameter object
    */
  FireCAMParameters(
    bool hueEnabled = false,
    bool saturationEnabled = false,
    bool gammaEnabled = false,
    bool autoGain = false,
    bool autoShutter = false,
    bool autoWhiteBalance = false,
    bool autoExposure = false,
    size_t gain = 80,
    size_t shutter = 100,
    size_t whiteBalanceBlue = 150,
    size_t whiteBalanceRed = 100,
    size_t brightness = 375,
    size_t hue = 0,
    size_t saturation = 0,
    size_t gamma = 0,
    size_t exposure = 100,
    size_t pan = 50,
    size_t tilt = 40,
    size_t sharpness = 1
  );
  FireCAMParameters(const FireCAMParameters& src);

  /** Destroy a FireCAM parameter object
    */
  virtual ~FireCAMParameters();

  /** Access the hue correction enabled flag
    */
  void setHueEnabled(bool hueEnabled);
  bool isHueEnabled() const;
  /** Access the saturation correction enabled flag
    */
  void setSaturationEnabled(bool saturationEnabled);
  bool isSaturationEnabled() const;
  /** Access the gamma correction enabled flag
    */
  void setGammaEnabled(bool gammaEnabled);
  bool isGammaEnabled() const;

  /** Access the gain auto adjustment flag
    */
  void setAutoGain(bool autoGain);
  bool isAutoGain() const;
  /** Access the shutter auto adjustment flag
    */
  void setAutoShutter(bool autoShutter);
  bool isAutoShutter() const;
  /** Access the white balance auto adjustment flag
    */
  void setAutoWhiteBalance(bool autoWhiteBalance);
  bool isAutoWhiteBalance() const;
  /** Access the exposure auto adjustment flag
    */
  void setAutoExposure(bool autoExposure);
  bool isAutoExposure() const;

  /** Access the gain value
    */
  void setGain(size_t gain);
  size_t getGain() const;
  /** Access the shutter value
    */
  void setShutter(size_t shutter);
  size_t getShutter() const;
  /** Access the white balance blue value
    */
  void setWhiteBalanceBlue(size_t whiteBalanceBlue);
  size_t getWhiteBalanceBlue() const;
  /** Access the white balance red value
    */
  void setWhiteBalanceRed(size_t whiteBalanceRed);
  size_t getWhiteBalanceRed() const;

  /** Access the brightness value
    */
  void setBrightness(size_t brightness);
  size_t getBrightness() const;
  /** Access the hue value
    */
  void setHue(size_t hue);
  size_t getHue() const;
  /** Access the saturation value
    */
  void setSaturation(size_t saturation);
  size_t getSaturation() const;
  /** Access the gamma value
    */
  void setGamma(size_t gamma);
  size_t getGamma() const;
  /** Access the exposure value
    */
  void setExposure(size_t exposure);
  size_t getExposure() const;

  /** Access the pan value
    */
  void setPan(size_t pan);
  size_t getPan() const;
  /** Access the tilt value
    */
  void setTilt(size_t tilt);
  size_t getTilt() const;
  /** Access the sharpness value
    */
  void setSharpness(size_t sharpness);
  size_t getSharpness() const;

  /** FireCAM parameter assignments
    */
  FireCAMParameters& operator=(const FireCAMParameters& src);
protected:
  bool hueEnabled;
  bool saturationEnabled;
  bool gammaEnabled;

  bool autoGain;
  bool autoShutter;
  bool autoWhiteBalance;
  bool autoExposure;

  size_t gain;
  size_t shutter;
  size_t whiteBalanceBlue;
  size_t whiteBalanceRed;

  size_t brightness;
  size_t hue;
  size_t saturation;
  size_t gamma;
  size_t exposure;

  size_t pan;
  size_t tilt;
  size_t sharpness;
};

#endif
