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
#ifndef OAUDIOMANAGER_H
#define OAUDIOMANAGER_H

#include "osoundmanager.h"
#include "omusicmanager.h"
#include "osettings.h"
#include "otypes.h"
#include "SDL_mixer.h"

typedef struct {
  OSoundManager   *soundmanager;
  OMusicManager   *musicmanager;
  const OSettings *settings;
} OAudioManager;

OAudioManager *oaudiomanager_new     (const OSettings *settings);
void           oaudiomanager_release (OAudioManager *audiomanager);

#endif
