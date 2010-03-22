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

#ifndef FIRECAM_TIMER_H
#define FIRECAM_TIMER_H

/** \file timer.h
  * \brief FireCAM timer functions
  */

#include <sys/time.h>

class FireCAMTimer {
public:
  /** Construct a FireCAM timer object
    */
  FireCAMTimer();

  /** Destroy a FireCAM timer object
    */
  virtual ~FireCAMTimer();

  /** Access if the timer has been started
    */
  bool isStarted() const;
  /** Access the number of elapsed seconds
    */
  double getElapsed() const;

  /** Start and stop the timer
    */
  void start();
  double stop();

  /** Toggle the timer's state
    */
  double toggle();
  bool toggleElapsed(double minElapsed);
private:
  struct timeval startTime;
  struct timeval stopTime;

  bool started;
  double elapsed;
};

#endif
