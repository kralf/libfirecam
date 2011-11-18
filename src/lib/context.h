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

#ifndef FIRECAM_CONTEXT_H
#define FIRECAM_CONTEXT_H

/** \file context.h
  * \brief FireCAM context class
  */

#include <dc1394/dc1394.h>

class FireCAMContext {
public:
  /** Construct a FireCAM context object
    */
  FireCAMContext();

  /** Destroy a FireCAM context object
    */
  virtual ~FireCAMContext();

  /** FireCAM context conversion
    */
  operator dc1394_t*() const;
protected:
  dc1394_t* context;
private:
  /** Construct a FireCAM camera object
    */
  FireCAMContext(const FireCAMContext& src);

  /** FireCAM context assignment
    */
  FireCAMContext& operator=(const FireCAMContext& src);
};

#endif
