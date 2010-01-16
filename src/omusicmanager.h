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
#ifndef OMUSICMANAGER_H
#define OMUSICMANAGER_H

#include "osettings.h"
#include "ostring.h"
#include "otypes.h"
#include "optrmap.h"
#include "SDL_mixer.h"

typedef struct {
  OPtrMap         *name2music;
  const OSettings *settings;
} OMusicManager;

OMusicManager *omusicmanager_new          (const OSettings *settings);
void           omusicmanager_release      (OMusicManager *musicmanager);
void           omusicmanager_play         (OMusicManager *musicmanager, const ouint32 id, const oint32 loops);
void           omusicmanager_playrandom   (OMusicManager *musicmanager, const oint32 loops);
void           omusicmanager_stop         (OMusicManager *musicmanager);
oint32         omusicmanager_getidforname (OMusicManager *musicmanager, const ochar *name);

#endif
