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

#include <stdexcept>

#include "firecam.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

FireCAM::FireCAM() :
  context(dc1394_new()) {
  rescan();
}

FireCAM::~FireCAM() {
  dc1394_free(context);
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

FireCAM& FireCAM::getInstance() {
  static FireCAM fireCAM;
  return fireCAM;
}

dc1394_t* FireCAM::getContext() const {
  return context;
}

const std::map<uint64_t, FireCAMCamera>& FireCAM::getCameras() const {
  return cameras;
}

const FireCAMCamera& FireCAM::getCamera(uint64_t guid) const {
  std::map<uint64_t, FireCAMCamera>::const_iterator it = cameras.find(guid);

  if (it == cameras.end()) {
    std::ostringstream what;
    what << "Bad camera GUID: " << std::hex << guid;
    throw std::runtime_error(what.str());
  }
  else
    return it->second;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

size_t FireCAM::rescan() {
  cameras.clear();

  dc1394camera_list_t* list;
  dc1394error_t error = dc1394_camera_enumerate(context, &list);
  if (error != DC1394_SUCCESS) {
    std::ostringstream what;
    what << "Failed to query cameras: " << dc1394_error_get_string(error);
    throw std::runtime_error(what.str());
  }

  for (int i = 0; i < list->num; ++i) {
    FireCAMCamera camera(context, list->ids[i].guid);
    cameras.insert(std::make_pair(list->ids[i].guid, camera));
  }
  dc1394_camera_free_list(list);

  return cameras.size();
}
