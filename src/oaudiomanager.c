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
#include "oaudiomanager.h"
#include "oerror.h"
#include <stdlib.h>

OAudioManager *oaudiomanager_new(const OSettings *settings)
{
  OAudioManager *audiomanager = (OAudioManager*)oerror_malloc(sizeof(OAudioManager));
  audiomanager->settings = settings;
  if(Mix_OpenAudio(22050, AUDIO_S16LSB, osettings_getsoundchannels(settings), settings->soundchunksize) == -1 )
    oerror_fatal("mixer cannot be initialized (%s)", Mix_GetError());

  Mix_AllocateChannels(osettings_getmixchannels(settings));

  if (osettings_issounds(settings))
    audiomanager->soundmanager = osoundmanager_new(settings);

  if (osettings_ismusic(settings))
    audiomanager->musicmanager = omusicmanager_new(settings);

  return audiomanager;
}

void oaudiomanager_release(OAudioManager *audiomanager)
{
  if (!audiomanager)
    return;

  osoundmanager_release(audiomanager->soundmanager);
  omusicmanager_release(audiomanager->musicmanager);
  Mix_CloseAudio();
  memset(audiomanager, 0, sizeof(OAudioManager));
  oerror_free(audiomanager);
}
