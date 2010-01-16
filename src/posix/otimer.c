/*
  Crates - 3D puzzle game.
  Copyright (C) 2008-2010  Octaspire games

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "../otimer.h"
#include "../otypes.h"
#include "../oerror.h"
#include <sys/time.h>

static onumber otimer_getseconds()
{
  static struct timeval tm;
  static struct timezone tz;
  gettimeofday(&tm, &tz);

  return ((onumber)(tm.tv_sec) + ((onumber)(tm.tv_usec) * 0.000001));
}

OTimer *otimer_new(void)
{
  OTimer *timer = (OTimer*)oerror_malloc(sizeof(OTimer));
  timer->startseconds = otimer_getseconds();
  otimer_resetintermediate(timer);
  return timer;
}

void otimer_release(OTimer *timer)
{
  if (!timer)
    return;

  memset(timer, 0, sizeof(OTimer));
  oerror_free(timer);
}

onumber otimer_gettotalseconds(OTimer *timer)
{
  return otimer_getseconds() - timer->startseconds;
}

void otimer_resetintermediate(OTimer *timer)
{
  timer->intermediateseconds = otimer_getseconds();
}

onumber otimer_getintermediateseconds(OTimer *timer)
{
  return otimer_getseconds() - timer->intermediateseconds;
}
