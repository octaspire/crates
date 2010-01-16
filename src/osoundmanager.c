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
#include "osoundmanager.h"
#include "odir.h"
#include "oerror.h"
#include <stdlib.h>
#include "SDL_mixer.h"

void osoundmanager_loadsound(const ochar *path, const ochar *fileName, void *userdata)
{
  Mix_Chunk *tmp = Mix_LoadWAV(path);

  if (tmp)
  {
    optrmap_put(((OSoundManager*)userdata)->name2sound, fileName, tmp);
    Mix_VolumeChunk(tmp, MIX_MAX_VOLUME * osettings_getsoundvolume(((OSoundManager*)userdata)->settings));
  }
  else
    oerror_warning("error loading sound %s at %s (%s)", fileName, path, Mix_GetError());
}

static void osoundmanager_releasesound(void *element)
{
  Mix_FreeChunk((Mix_Chunk*)element);
}

OSoundManager *osoundmanager_new(const OSettings *settings)
{
  ochar path[] = "resources/sounds";
  OSoundManager *soundmanager = (OSoundManager*)oerror_malloc(sizeof(OSoundManager));
  soundmanager->settings = settings;
  soundmanager->name2sound = optrmap_new(osoundmanager_releasesound);
  odir_foreachentry(odir_convertpath(path), osoundmanager_loadsound, soundmanager);
  return soundmanager;
}

void osoundmanager_release(OSoundManager *soundmanager)
{
  if (!soundmanager)
    return;

  osoundmanager_stop(soundmanager, -1);
  optrmap_release(soundmanager->name2sound);
  memset(soundmanager, 0, sizeof(OSoundManager));
  oerror_free(soundmanager);
}

void osoundmanager_play(OSoundManager *soundmanager, const ouint32 id)
{
  Mix_Chunk *ptr;

  if (!soundmanager)
    return;

  ptr = (Mix_Chunk*)optrvector_getelementptrat(soundmanager->name2sound->values, id);

  if (ptr == 0)
  {
    oerror_warning("no soundeffect %d", id);
    return;
  }

  if (Mix_PlayChannel(-1, ptr, 0) == -1)
    oerror_warning("cannot play soundeffect %d (%s)", id, Mix_GetError());
}

void osoundmanager_stop(OSoundManager *soundmanager, const oint32 channel)
{
  if (!soundmanager)
    return;

  Mix_HaltChannel(channel);
}

oint32 osoundmanager_getidforname(OSoundManager *soundmanager, const ochar *name)
{
  return optrmap_getindex(soundmanager->name2sound, name);
}
