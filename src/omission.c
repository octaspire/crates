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
#include "omission.h"
#include "oerror.h"
#include "ofile.h"
#include "ogame.h"
#include <stdio.h>
#include <time.h>

extern OGame    *game;
extern OMission *parsingMission;

OMission *omission_new(const ochar *name, const ochar *path)
{
  OMission *mission = (OMission*)oerror_malloc(sizeof(OMission));
  mission->levels = optrvector_new(0, (OPtrVectorElementCallback)olevel_release);
  mission->name = ostring_newstrn(name, strstr(name, ".lua") - name);
  mission->path = ostring_newstr(path);
  mission->current = 0;
  mission->numpassed = 0;
  omission_resettimer(mission);
  return mission;
}

void omission_release(OMission *mission)
{
  if (!mission)
    return;

  optrvector_release(mission->levels);
  mission->current = 0;
  mission->numpassed = 0;
  mission->starttime = 0;
  ostring_release(mission->name);
  ostring_release(mission->path);
  memset(mission, 0, sizeof(OMission));
  oerror_free(mission);
}

void omission_reset(OMission *mission)
{
  mission->current = 0;
  mission->numpassed = 0;
  omission_resettimer(mission);
}

void omission_resettimer(OMission *mission)
{
  mission->starttime = time(0);
  mission->endtime = 0;
}

void omission_stoptimer(OMission *mission)
{
  mission->endtime = time(0);
}

ouint32 omission_getnumlevels(const OMission *mission)
{
  return optrvector_length(mission->levels);
}

ouint32 omission_getnumpassed(const OMission *mission)
{
  return mission->numpassed;
}

ouint32 omission_getcurrentlevelnum(const OMission *mission)
{
  return mission->current;
}

void omission_setcurrentlevelnum(OMission *mission, const ouint32 num)
{
  mission->current = num;
}

OLevel *omission_getlevelptrat(OMission *mission, const ouint32 index)
{
  return (OLevel*)optrvector_getelementptrat(mission->levels, index);
}

OLevel *omission_getlevelwithpassword(OMission *mission, const ochar *password)
{
  ouint32 i;
  OLevel *level;
  ouint32 len = optrvector_length(mission->levels);

  for (i = 0; i < len; ++i)
  {
    level = (OLevel*)optrvector_getelementptrat(mission->levels, i);
    if (ostring_comparestr(level->password, password) == 0)
      return level;
  }

  return 0;
}

OLevel *omission_getcurrentlevelptr(OMission *mission)
{
  ouint32 len = optrvector_length(mission->levels);

  if (mission->current >= len)
    return 0;

  return omission_getlevelptrat(mission, mission->current);
}

void omission_nextlevel(OMission *mission)
{
  ouint32 len = optrvector_length(mission->levels);

  if (mission->current >= len)
    return;

  mission->current++;
  mission->numpassed++;
}

oboolean omission_hasnextlevel(OMission *mission)
{
  ouint32 len = optrvector_length(mission->levels);
  return ((mission->current+1) < len);
}

oboolean omission_haslevelwithpassword(OMission *mission, const ochar *password)
{
  ouint32 i;
  OLevel *level;
  ouint32 len = optrvector_length(mission->levels);

  for (i = 0; i < len; ++i)
  {
    level = (OLevel*)optrvector_getelementptrat(mission->levels, i);
    if (ostring_comparestr(level->password, password) == 0)
      return 1;
  }

  return 0;
}

void omission_setcurrentlevelfrompassword(OMission *mission, const ochar *password)
{
  ouint32 i;
  OLevel *level;
  ouint32 len = optrvector_length(mission->levels);

  for (i = 0; i < len; ++i)
  {
    level = (OLevel*)optrvector_getelementptrat(mission->levels, i);
    if (ostring_comparestr(level->password, password) == 0)
    {
      mission->current = i;
      return;
    }
  }
}

OLevel *omission_pushbacknewlevel(OMission *mission)
{
  OLevel *level = olevel_new();
  optrvector_pushback(mission->levels, level);
  return (OLevel*)optrvector_peekback(mission->levels);
}

void omission_parse(OMission *mission)
{
  parsingMission = mission;
  oscriptmanager_loadscript(game->scriptmanager, ostring_tocstr(mission->path));
}

void omission_pushbacklinetolastlevel(OMission *mission, const ochar *line)
{
  olevel_pushbackline((OLevel*)optrvector_peekback(mission->levels), line);
}

void omission_fprintf(OMission *mission, FILE *out)
{
  ouint32 i;
  ouint32 len = optrvector_length(mission->levels);

  for (i = 0; i < len; ++i)
  {
    olevel_fprintf((OLevel*)optrvector_getelementptrat(mission->levels, i), out);
    fprintf(out, "%s", "\n");
  }
}

void omission_printf(OMission *mission)
{
  omission_fprintf(mission, stdout);
}

ouint32 omission_getdurationinseconds(OMission *mission)
{
  if (mission->endtime == 0 || mission->starttime == 0 || mission->starttime > mission->endtime)
    oerror_warning("mission timer has wrong values (starttime=%d, endtime=%d)", mission->starttime, mission->endtime);

  return (mission->endtime - mission->starttime);
}