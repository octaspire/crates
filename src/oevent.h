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
#ifndef OEVENT_H_INCLUSION
#define OEVENT_H_INCLUSION

#include "otypes.h"

enum OEvent {
  OEVENT_UP, OEVENT_DOWN, OEVENT_LEFT, OEVENT_RIGHT, OEVENT_OK, OEVENT_EXIT,
  OEVENT_A,
  OEVENT_B,
  OEVENT_C,
  OEVENT_D,
  OEVENT_E,
  OEVENT_F,
  OEVENT_G,
  OEVENT_H,
  OEVENT_I,
  OEVENT_J,
  OEVENT_K,
  OEVENT_L,
  OEVENT_M,
  OEVENT_N,
  OEVENT_O,
  OEVENT_P,
  OEVENT_Q,
  OEVENT_R,
  OEVENT_S,
  OEVENT_T,
  OEVENT_U,
  OEVENT_V,
  OEVENT_W,
  OEVENT_X,
  OEVENT_Y,
  OEVENT_Z,
  OEVENT_0,
  OEVENT_1,
  OEVENT_2,
  OEVENT_3,
  OEVENT_4,
  OEVENT_5,
  OEVENT_6,
  OEVENT_7,
  OEVENT_8,
  OEVENT_9,
  OEVENT_BACKSPACE
};
extern const ochar *oevent_itoa[];

#endif
