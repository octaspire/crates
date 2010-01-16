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
#include "omusicmanager.h"
#include "odir.h"
#include "oerror.h"
#include <stdlib.h>
#include "SDL_mixer.h"

void omusicmanager_loadmusic(const ochar *path, const ochar *fileName, void *userdata)
{
  Mix_Music *tmp = Mix_LoadMUS(path);

  if (tmp)
    optrmap_put(((OMusicManager*)userdata)->name2music, fileName, tmp);
  else
    oerror_warning("error loading music %s at %s (%s)", fileName, path, Mix_GetError());
}

static void omusicmanager_releasemusic(void *element)
{
  Mix_FreeMusic((Mix_Music*)element);
}

OMusicManager *omusicmanager_new(const OSettings *settings)
{
  ochar path[] = "resources/musics/";
  OMusicManager *musicmanager = (OMusicManager*)oerror_malloc(sizeof(OMusicManager));
  musicmanager->settings = settings;
  Mix_VolumeMusic(MIX_MAX_VOLUME * osettings_getmusicvolume(settings));
  musicmanager->name2music = optrmap_new(omusicmanager_releasemusic);
  odir_foreachentry(odir_convertpath(path), omusicmanager_loadmusic, musicmanager);
  return musicmanager;
}

void omusicmanager_release(OMusicManager *musicmanager)
{
  if (!musicmanager)
    return;

  omusicmanager_stop(musicmanager);
  optrmap_release(musicmanager->name2music);
  memset(musicmanager, 0, sizeof(OMusicManager));
  oerror_free(musicmanager);
}

void omusicmanager_play(OMusicManager *musicmanager, const ouint32 id, const oint32 loops)
{
  Mix_Music *ptr;

  if (!musicmanager)
    return;

  if (Mix_PlayingMusic())
    return;

  ptr = (Mix_Music*)optrvector_getelementptrat(musicmanager->name2music->values, id);

  if (ptr == 0)
  {
    oerror_warning("no music %d", id);
    return;
  }

  if (Mix_PlayMusic(ptr, loops) == -1)
    oerror_warning("cannot play music %d (%s)", id, Mix_GetError());
}

void omusicmanager_playrandom(OMusicManager *musicmanager, const oint32 loops)
{
  Mix_Music *ptr;

  if (!musicmanager)
    return;

  if (Mix_PlayingMusic())
    return;

  ptr = (Mix_Music*)optrvector_getrandomelementptr(musicmanager->name2music->values);

  if (ptr == 0)
  {
    oerror_warning("cannot find random music");
    return;
  }

  if (Mix_PlayMusic(ptr, loops) == -1)
    oerror_warning("cannot play random music (%s)", Mix_GetError());
}

void omusicmanager_stop(OMusicManager *musicmanager)
{
  if (!musicmanager)
    return;

  if (!Mix_PlayingMusic())
    return;

  Mix_HaltMusic();
}

oint32 omusicmanager_getidforname(OMusicManager *musicmanager, const ochar *name)
{
  return optrmap_getindex(musicmanager->name2music, name);
}
