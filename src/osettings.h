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
#ifndef OSETTINGS_H
#define OSETTINGS_H

#include "oscriptmanager.h"
#include "ostring.h"
#include "otypes.h"

typedef struct {
  ouint32  screenwidth;
  ouint32  screenheight;
  ouint32  screenbpp;
  ouint32  verbose;
  oboolean filelog;
  ouint32  fps;
  oboolean fullscreen;
  oboolean controllers;
  oboolean sounds;
  ouint32  mixchannels;
  oboolean music;
  onumber  musicvolume;
  onumber  soundvolume;
  oboolean soundstereo;
  ouint32  soundchunksize;
  oint32   controllernthtouse;
  oint32   controllerok;
  oint32   controllerexit;
  oint32   controllerrestart;
  oint32   controllerhatup;
  oint32   controllerhatdown;
  oint32   controllerhatleft;
  oint32   controllerhatright;
  oint32   controlleraxisupdown1;
  oint32   controlleraxisupdown2;
  oint32   controlleraxisrightleft1;
  oint32   controlleraxisrightleft2;
  oint32   controlleraxistriggerdownrightmin;
  oint32   controlleraxistriggerdownrightmax;
  oint32   controlleraxistriggerupleftmin;
  oint32   controlleraxistriggerupleftmax;
  oint32   controllerbuttonrepeatdelay;
  oint32   controllerbuttonrepeatinterval;
  oboolean grabinputfullscreen;
  oboolean backfaceculling;
  oboolean lighting;
  oboolean verticalsynchronization;
  OString  *skin;
} OSettings;

OSettings *osettings_new                       (OScriptManager *scriptmanager);
void       osettings_release                   (OSettings *settings);
ouint32    osettings_getscreenwidth            (const OSettings *settings);
ouint32    osettings_getscreenheight           (const OSettings *settings);
ouint32    osettings_getscreenbpp              (const OSettings *settings);
oboolean   osettings_isverbose                 (const OSettings *settings);
ouint32    osettings_getfps                    (const OSettings *settings);
oboolean   osettings_isfullscreen              (const OSettings *settings);
oboolean   osettings_iscontrollers             (const OSettings *settings);
oboolean   osettings_issounds                  (const OSettings *settings);
ouint32    osettings_getsoundchannels          (const OSettings *settings);
ouint32    osettings_getmixchannels            (const OSettings *settings);
oboolean   osettings_ismusic                   (const OSettings *settings);
oboolean   osettings_isaudio                   (const OSettings *settings);
ouint32    osettings_getcontrollerok           (const OSettings *settings);
onumber    osettings_getmusicvolume            (const OSettings *settings);
onumber    osettings_getsoundvolume            (const OSettings *settings);
ouint32    osettings_getcontrollerexit         (const OSettings *settings);
ouint32    osettings_getcontrollerrestart      (const OSettings *settings);
oboolean   osettings_isverticalsynchronization (const OSettings *settings);
OString   *osettings_getskin                   (const OSettings *settings);

#endif
