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

#ifndef FIRECAM_H
#define FIRECAM_H

/** \file firecam.h
  * \brief FireCAM device driver class
  */

#include <map>

#include "camera.h"

class FireCAM {
public:
  /** Access the device driver's static instance
    */
  static FireCAM& getInstance();

  /** Access the FireCAM device driver context
    */
  dc1394_t* getContext() const;

  /** Access all available cameras
    */
  const std::map<uint64_t, FireCAMCamera>& getCameras() const;
  /** Access the camera with the specified GUID
    */
  const FireCAMCamera& getCamera(uint64_t guid) const;

  /** Rescan the bus for available cameras
    */
  size_t rescan();
protected:
  std::map<uint64_t, FireCAMCamera> cameras;

  dc1394_t* context;

  /** Construct a FireCAM device driver object
    */
  FireCAM();

  /** Destroy a FireCAM device driver object
    */
  virtual ~FireCAM();
};

#endif
