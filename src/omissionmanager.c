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
#include "omissionmanager.h"
#include "oerror.h"
#include "odir.h"
#include <stdlib.h>

static void omissionmanager_loadercallback(const ochar *path, const ochar *filename, void *userdata)
{
  OMission *mission = omission_new(filename, path);
  optrvector_pushback(((OMissionManager*)userdata)->missions, mission);
}

static oint32 omissionmanager_mnamecmp(const void* a, const void* b)
{
  OMission* m1 = *(OMission**)a;
  OMission* m2 = *(OMission**)b;
  return ostring_compare(m1->name, m2->name);
}

OMissionManager *omissionmanager_new(void)
{
  ochar path[] = "resources/missions";
  OMissionManager *missionmanager = (OMissionManager*)oerror_malloc(sizeof(OMissionManager));
  missionmanager->current = 0;
  missionmanager->missions = optrvector_new(0, (OPtrVectorElementCallback)omission_release);
  odir_foreachentry(odir_convertpath(path), omissionmanager_loadercallback, missionmanager);
  optrvector_qsort(missionmanager->missions, omissionmanager_mnamecmp);
  return missionmanager;
}

void omissionmanager_release(OMissionManager *missionmanager)
{
  if (!missionmanager)
    return;

  optrvector_release(missionmanager->missions);
  oerror_free(missionmanager);
}

ouint32 omissionmanager_getnummissions(const OMissionManager *missionmanager)
{
  return optrvector_length(missionmanager->missions);
}

OMission *omissionmanager_getmissionptrat(const OMissionManager *missionmanager, const ouint32 index)
{
  return (OMission*)optrvector_getelementptrat(missionmanager->missions, index);
}

OMission *omissionmanager_getmissionptrnamed(const OMissionManager *missionmanager, const ochar *name)
{
  ouint32 i;
  OMission *mission;
  ouint32 len = omissionmanager_getnummissions(missionmanager);

  for (i = 0; i < len; ++i)
  {
    mission = omissionmanager_getmissionptrat(missionmanager, i);
    if (ostring_comparestr(mission->name, name) == 0)
      return mission;
  }

  return 0;
}

const ochar *omissionmanager_getmissionnameat(const OMissionManager *missionmanager, const ouint32 index)
{
  OMission *mission = omissionmanager_getmissionptrat(missionmanager, index);

  if (mission == 0)
    return 0;

  return ostring_tocstr(mission->name);
}

const ochar *omissionmanager_getmissionpathat(const OMissionManager *missionmanager, const ouint32 index)
{
  OMission *mission = omissionmanager_getmissionptrat(missionmanager, index);

  if (mission == 0)
    return 0;

  return ostring_tocstr(mission->path);
}
