/*
  Crates ‐ extensible 3D puzzle game.
  Copyright (C) 2008‐2010  Octaspire (www.octaspire.com)

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
#ifndef OMESSAGESTACK_H
#define OMESSAGESTACK_H

#include "otextmanager.h"
#include "otypes.h"
#include "ovector.h"
#include <stdio.h>

typedef struct {
  OPtrVector   *messages;
  onumber       showtime;
  onumber       timeleft;
  oint32        basey;
  OTextManager *textmanager;
} OMessageStack;

OMessageStack *omessagestack_new         (OTextManager *textmanager);
void           omessagestack_release     (OMessageStack *messagestack);
void           omessagestack_push        (OMessageStack *messagestack, const ochar *message);
void           omessagestack_pop         (OMessageStack *messagestack);
void           omessagestack_update      (OMessageStack *messagestack, const onumber tpf);
void           omessagestack_render      (OMessageStack *messagestack);
void           omessagestack_setshowtime (OMessageStack *messagestack, const oint32 showtime);

#endif
