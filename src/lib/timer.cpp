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

#include "timer.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

FireCAMTimer::FireCAMTimer() :
  started(false),
  elapsed(0.0) {
  startTime.tv_sec = 0;
  startTime.tv_usec = 0;
}

FireCAMTimer::~FireCAMTimer() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

bool FireCAMTimer::isStarted() const {
  return started;
}

double FireCAMTimer::getElapsed() const {
  if (started) {
    struct timeval stopTime;
    gettimeofday(&stopTime, 0);

    double t1 = (double)startTime.tv_sec+(double)startTime.tv_usec*1e-6;
    double t2 = (double)stopTime.tv_sec+(double)stopTime.tv_usec*1e-6;

    double elapsed = t2-t1;

    return elapsed;
  }
  else
    return elapsed;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

void FireCAMTimer::start() {
  gettimeofday(&startTime, 0);
  started = true;
}

double FireCAMTimer::stop() {
  gettimeofday(&stopTime, 0);

  double t1 = (double)startTime.tv_sec+(double)startTime.tv_usec*1e-6;
  double t2 = (double)stopTime.tv_sec+(double)stopTime.tv_usec*1e-6;

  started = false;
  elapsed = t2-t1;

  return elapsed;
}

double FireCAMTimer::toggle() {
  gettimeofday(&stopTime, 0);

  double t1 = (double)startTime.tv_sec+(double)startTime.tv_usec*1e-6;
  double t2 = (double)stopTime.tv_sec+(double)stopTime.tv_usec*1e-6;

  elapsed = t2-t1;

  start();

  return elapsed;
}

bool FireCAMTimer::toggleElapsed(double minElapsed) {
  gettimeofday(&stopTime, 0);

  double t1 = (double)startTime.tv_sec+(double)startTime.tv_usec*1e-6;
  double t2 = (double)stopTime.tv_sec+(double)stopTime.tv_usec*1e-6;

  double elapsed = t2-t1;

  if (elapsed >= minElapsed) {
    start();
    return true;
  }
  else
    return false;
}
