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

#include "firecam.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << "GUID [FILENAME]" << std::endl;
    return -1;
  }

  std::istringstream stream(argv[1]);
  uint64_t guid;
  stream >> std::hex >> guid;

  FireCAMCamera& camera = FireCAM::getInstance().getCamera(guid);

  if (argc == 3) {
    std::cout << "Parsing configuration file " << argv[2] << std::endl;
    FireCAMConfiguration configuration(argv[2]);
    camera.setConfiguration(configuration);
  }

  std::cout << "Connecting camera... ";
  std::cout.flush();
  if (camera.connect()) {
    std::cout << "success" << std::endl;
    camera.disconnect();
  }
  else
    std::cout << "failure" << std::endl;

  camera.getConfiguration().save(std::cout);

  return 0;
}
