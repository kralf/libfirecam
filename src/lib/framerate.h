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

#ifndef FIRECAM_FRAMERATE_H
#define FIRECAM_FRAMERATE_H

/** \file framerate.h
  * \brief FireCAM framerate definition
  */

#include <dc1394/dc1394.h>

class FireCAMFramerate {
friend class FireCAMCamera;
public:
  /** Construct a FireCAM framerate object
    */
  FireCAMFramerate(double framesPerSecond = 0.0);
  FireCAMFramerate(const FireCAMFramerate& src);

  /** Destroy a FireCAM framerate object
    */
  virtual ~FireCAMFramerate();

  /** Access the number of frames per second
    */
  void setFramesPerSecond(double framesPerSecond);
  double getFramesPerSecond() const;

  /** FireCAM framerate assignments
    */
  FireCAMFramerate& operator=(const FireCAMFramerate& src);

  /** FireCAM framerate comparisons
    */
  bool operator==(const FireCAMFramerate& framerate) const;
  bool operator!=(const FireCAMFramerate& framerate) const;
  bool operator<(const FireCAMFramerate& framerate) const;

  /** Write framerate information to the given stream
    */
  void write(std::ostream& stream) const;

  /** Load framerate configuration from the given stream
    */
  void load(std::istream& stream);
  /** Save framerate configuration to the given stream
    */
  void save(std::ostream& stream) const;
protected:
  dc1394framerate_t rate;

  double framesPerSecond;

  /** Construct a FireCAM framerate object
    */
  FireCAMFramerate(dc1394camera_t* device);
  FireCAMFramerate(dc1394framerate_t rate);

  /** Read the framerate parameters
    */
  void readParameters();
  /** Write the framerate parameters to the specified device
    */
  void writeParameters(dc1394camera_t* device) const;
};

#endif
