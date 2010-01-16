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
#include "osettings.h"
#include "oerror.h"
#include "odir.h"
#include "oscriptmanager.h"
#include <errno.h>

static void getsettingi(OScriptManager *scriptmanager, const ochar *fieldname, oint32 *value, oboolean warnings, oint32 minval, oint32 maxval)
{
  oint32 tmp;
  oscriptmanager_gettablefieldva(scriptmanager, "config", fieldname, 'i', &tmp, warnings);
  if (tmp >= minval && tmp <= maxval)
    *value = tmp;
  else if (warnings)
    oerror_warning("Setting \"%s\" has incorrect value \"%d\". Allowed are values %d-%d", fieldname, tmp, minval, maxval);
}

static void getsettingui(OScriptManager *scriptmanager, const ochar *fieldname, ouint32 *value, oboolean warnings, ouint32 minval, ouint32 maxval)
{
  oint32 tmp;
  oscriptmanager_gettablefieldva(scriptmanager, "config", fieldname, 'i', &tmp, warnings);

  if (tmp < 0)
  {
    oerror_warning("Setting \"%s\" has incorrect value \"%d\". Value must be non-negative and between values %u-%u", fieldname, tmp, minval, maxval);
    return;
  }

  if (tmp >= minval && tmp <= maxval)
    *value = tmp;
  else if (warnings)
    oerror_warning("Setting \"%s\" has incorrect value \"%d\". Allowed are values %d-%d", fieldname, tmp, minval, maxval);
}

static void getsettingb(OScriptManager *scriptmanager, const ochar *fieldname, oboolean *value, oboolean warnings)
{
  oscriptmanager_gettablefieldva(scriptmanager, "config", fieldname, 'b', value, warnings);
}

static void getsettingd(OScriptManager *scriptmanager, const ochar *fieldname, onumber *value, oboolean warnings, onumber minval, onumber maxval)
{
  onumber tmp;
  oscriptmanager_gettablefieldva(scriptmanager, "config", fieldname, 'd', &tmp, warnings);
  if (tmp >= minval && tmp <= maxval)
    *value = tmp;
  else if (warnings)
    oerror_warning("Setting \"%s\" has incorrect value \"%f\". Allowed are values %f-%f", fieldname, tmp, minval, maxval);
}

OSettings *osettings_new(OScriptManager *scriptmanager)
{
  ochar path[] = "resources/config.lua";
  ochar logfilename[] = "resources/log.txt";
  OSettings *settings = (OSettings*)oerror_malloc(sizeof(OSettings));
  settings->screenwidth                       = 0;
  settings->screenheight                      = 0;
  settings->screenbpp                         = 0;
  settings->verbose                           = 0;
  settings->filelog                           = 0;
  settings->fps                               = 65;
  settings->fullscreen                        = 0;
  settings->controllers                       = 1;
  settings->sounds                            = 1;
  settings->mixchannels                       = 16;
  settings->music                             = 1;
  settings->musicvolume                       = 1;
  settings->soundvolume                       = 1;
  settings->soundstereo                       = 0;
  settings->soundchunksize                    = 256;
  settings->controllernthtouse                = 1;
  settings->controllerok                      = 1;
  settings->controllerexit                    = 2;
  settings->controllerrestart                 = 10;
  settings->controllerhatup                   = 1;
  settings->controllerhatdown                 = 4;
  settings->controllerhatleft                 = 8;
  settings->controllerhatright                = 2;
  settings->controlleraxisupdown1             = 1;
  settings->controlleraxisupdown2             = 3;
  settings->controlleraxisrightleft1          = 0;
  settings->controlleraxisrightleft2          = 2;
  settings->controlleraxistriggerdownrightmin = 32000;
  settings->controlleraxistriggerdownrightmax = 32769;
  settings->controlleraxistriggerupleftmin    = -32769;
  settings->controlleraxistriggerupleftmax    = -32000;
  settings->controllerbuttonrepeatdelay       = 30;
  settings->controllerbuttonrepeatinterval    = 8;
  settings->grabinputfullscreen               = 1;
  settings->backfaceculling                   = 1;
  settings->lighting                          = 1;
  settings->verticalsynchronization           = 1;
  oscriptmanager_loadscript(scriptmanager, odir_convertpath(path));

  getsettingb(scriptmanager,  "filelog",                                &settings->filelog,                           1);
  if (settings->filelog)
    if (freopen(odir_convertpath(logfilename), "w", stderr) == 0)
      fprintf(stdout, "warning: redirection of stderr to file \"%s\" failed (%s)\n", logfilename, strerror(errno));

  getsettingui(scriptmanager, "screen_width",                           &settings->screenwidth,                       0, 320, 2048);
  getsettingui(scriptmanager, "screen_height",                          &settings->screenheight,                      0, 240, 2048);
  getsettingui(scriptmanager, "screen_bpp",                             &settings->screenbpp,                         0, 0,   32);
  getsettingui(scriptmanager, "verbose",                                &settings->verbose,                           1, 0,   2);
  getsettingui(scriptmanager, "fps",                                    &settings->fps,                               1, 16,  256);
  getsettingb(scriptmanager,  "fullscreen",                             &settings->fullscreen,                        1);
  getsettingb(scriptmanager,  "controllers",                            &settings->controllers,                       1);
  getsettingb(scriptmanager,  "sounds",                                 &settings->sounds,                            1);
  getsettingui(scriptmanager, "mix_channels",                           &settings->mixchannels,                       1, 8,   2048);
  getsettingb(scriptmanager,  "music",                                  &settings->music,                             1);
  getsettingd(scriptmanager,  "music_volume",                           &settings->musicvolume,                       1, 0,   1);
  getsettingd(scriptmanager,  "sound_volume",                           &settings->soundvolume,                       1, 0,   1);
  getsettingb(scriptmanager,  "sound_stereo",                           &settings->soundstereo,                       1);
  getsettingui(scriptmanager, "sound_chunksize",                        &settings->soundchunksize,                    1, 16,  65536);
  getsettingi(scriptmanager,  "controller_nth_to_use",                  &settings->controllernthtouse,                1, 1,   128);
  getsettingi(scriptmanager,  "controller_ok",                          &settings->controllerok,                      1, 1,   64);
  getsettingi(scriptmanager,  "controller_exit",                        &settings->controllerexit,                    1, 1,   64);
  getsettingi(scriptmanager,  "controller_restart",                     &settings->controllerrestart,                 1, 1,   64);
  getsettingi(scriptmanager,  "controller_hat_up",                      &settings->controllerhatup,                   1, -65536, 65536);
  getsettingi(scriptmanager,  "controller_hat_down",                    &settings->controllerhatdown,                 1, -65536, 65536);
  getsettingi(scriptmanager,  "controller_hat_left",                    &settings->controllerhatleft,                 1, -65536, 65536);
  getsettingi(scriptmanager,  "controller_hat_right",                   &settings->controllerhatright,                1, -65536, 65536);
  getsettingi(scriptmanager,  "controller_axis_up_down_1",              &settings->controlleraxisupdown1,             1, -65536, 65536);
  getsettingi(scriptmanager,  "controller_axis_up_down_2",              &settings->controlleraxisupdown2,             1, -65536, 65536);
  getsettingi(scriptmanager,  "controller_axis_right_left_1",           &settings->controlleraxisrightleft1,          1, -65536, 65536);
  getsettingi(scriptmanager,  "controller_axis_right_left_2",           &settings->controlleraxisrightleft2,          1, -65536, 65536);
  getsettingi(scriptmanager,  "controller_axis_trigger_down_right_min", &settings->controlleraxistriggerdownrightmin, 1, -65536, 65536);
  getsettingi(scriptmanager,  "controller_axis_trigger_down_right_max", &settings->controlleraxistriggerdownrightmax, 1, -65536, 65536);
  getsettingi(scriptmanager,  "controller_axis_trigger_up_left_min",    &settings->controlleraxistriggerupleftmin,    1, -65536, 65536);
  getsettingi(scriptmanager,  "controller_axis_trigger_up_left_max",    &settings->controlleraxistriggerupleftmax,    1, -65536, 65536);
  getsettingi(scriptmanager,  "controller_button_repeat_delay",         &settings->controllerbuttonrepeatdelay,       1, 0, 65536);
  getsettingi(scriptmanager,  "controller_button_repeat_interval",      &settings->controllerbuttonrepeatinterval,    1, 0, 65536);
  getsettingb(scriptmanager,  "grabinputfullscreen",                    &settings->grabinputfullscreen,               1);
  getsettingb(scriptmanager,  "backfaceculling",                        &settings->backfaceculling,                   1);
  getsettingb(scriptmanager,  "lighting",                               &settings->lighting,                          1);
  getsettingb(scriptmanager,  "vertical_synchronization",               &settings->verticalsynchronization,           1);

  return settings;
}

void osettings_release(OSettings *settings)
{
  if (!settings)
    return;

  memset(settings, 0, sizeof(OSettings));
  oerror_free(settings);
}

INLINE ouint32 osettings_getscreenwidth(const OSettings *settings)
{
  return settings->screenwidth;
}

INLINE ouint32 osettings_getscreenheight(const OSettings *settings)
{
  return settings->screenheight;
}

INLINE ouint32 osettings_getscreenbpp(const OSettings *settings)
{
  return settings->screenbpp;
}

INLINE oboolean osettings_isverbose(const OSettings *settings)
{
  return settings->verbose;
}

INLINE ouint32 osettings_getfps(const OSettings *settings)
{
  return  settings->fps;
}

INLINE oboolean osettings_isfullscreen(const OSettings *settings)
{
  return settings->fullscreen;
}

INLINE oboolean osettings_iscontrollers(const OSettings *settings)
{
  return settings->controllers;
}

INLINE oboolean osettings_issounds(const OSettings *settings)
{
  return settings->sounds;
}

INLINE ouint32 osettings_getsoundchannels(const OSettings *settings)
{
  return (settings->soundstereo ? 2 : 1);
}

INLINE ouint32 osettings_getmixchannels(const OSettings *settings)
{
  return settings->mixchannels;
}

INLINE oboolean osettings_ismusic(const OSettings *settings)
{
  return settings->music;
}

INLINE oboolean osettings_isaudio(const OSettings *settings)
{
  return (settings->sounds || settings->music);
}

INLINE onumber osettings_getmusicvolume(const OSettings *settings)
{
  return settings->musicvolume;
}

INLINE onumber osettings_getsoundvolume(const OSettings *settings)
{
  return settings->soundvolume;
}

INLINE ouint32 osettings_getcontrollerok(const OSettings *settings)
{
  return settings->controllerok;
}

INLINE ouint32 osettings_getcontrollerexit(const OSettings *settings)
{
  return settings->controllerexit;
}

INLINE ouint32 osettings_getcontrollerrestart (const OSettings *settings)
{
  return settings->controllerrestart;
}

INLINE oboolean osettings_isverticalsynchronization(const OSettings *settings)
{
  return settings->verticalsynchronization;
}
