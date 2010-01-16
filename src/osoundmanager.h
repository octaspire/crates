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
#ifndef OSOUNDMANAGER_H
#define OSOUNDMANAGER_H

#include "osettings.h"
#include "ostring.h"
#include "otypes.h"
#include "optrmap.h"
#include "SDL_mixer.h"

typedef struct {
  OPtrMap         *name2sound;
  const OSettings *settings;
} OSoundManager;

OSoundManager *osoundmanager_new          (const OSettings *settings);
void           osoundmanager_release      (OSoundManager *soundmanager);
void           osoundmanager_play         (OSoundManager *soundmanager, const ouint32 id);
void           osoundmanager_stop         (OSoundManager *soundmanager, const oint32 channel);
oint32         osoundmanager_getidforname (OSoundManager *soundmanager, const ochar *name);

#endif
