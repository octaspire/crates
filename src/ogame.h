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
#ifndef OGAME_H
#define OGAME_H

#include "SDL.h"
#include "oentity.h"
#include "oerror.h"
#include "oevent.h"
#include "otimer.h"
#include "ostate.h"
#include "oscriptmanager.h"
#include "otexturemanager.h"
#include "omeshmanager.h"
#include "omissionmanager.h"
#include "osettings.h"
#include "oaudiomanager.h"
#include "otextmanager.h"
#include "oentitymanager.h"

typedef struct {
  SDL_Surface     *screen;
  SDL_Joystick    *controller;
  SDL_Event        event;
  oboolean         quit;
  oboolean         active;
  OEntityManager  *entitymanager;
  OMeshManager    *meshmanager;
  OMissionManager *missionmanager;
  OScriptManager  *scriptmanager;
  OTextManager    *textmanager;
  OTextureManager *texturemanager;
  OAudioManager   *audiomanager;
  OState          *state;
  OSettings       *settings;
  OTimer          *fps;
  onumber          intermedsec;
  onumber          tpf;
  onumber          delay;
  oboolean         controllerbuttonrepeat;
  oint32           controllerbutton;
  oint32           ctrlbuttonnrepeatdelay;
  oint32           ctrlbuttonnrepeattrigger;
  OString         *nextstatename;
  OString         *nextstateargument;
} OGame;

OGame *ogame_new     (void);
void   ogame_release (OGame *game);
void   ogame_loop    (OGame *game);
void   ogame_quit    (OGame *game);

#endif
