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
#include "oentitymanager.h"
#include "oerror.h"
#include "odir.h"
#include "ofile.h"
#include "oscriptmanager.h"
#include <stdlib.h>
#include <string.h>

void oentitymanager_checkentity(const ochar *path, const ochar *filename, void *userdata)
{
  ochar name[64];
  const ouint32 maxnlen = 40;

  if (strlen(filename) > maxnlen)
    oerror_fatal("entity name \"%s\" is too long (longer than %d)", filename, maxnlen);

  strncpy(name, filename, maxnlen);
  *strchr(name, '.') = '\0';
  oscriptmanager_loadscript(((OEntityManager*)userdata)->scriptmanager, path);
  oscriptmanager_calltablefunctionva(((OEntityManager*)userdata)->scriptmanager, name, "register", ">");
}

void oentitymanager_init(OEntityManager *entitymanager)
{
  ochar path[] = "resources/entities";
  odir_foreachentry(odir_convertpath(path), oentitymanager_checkentity, entitymanager);
}

OEntityManager *oentitymanager_new(OScriptManager *scriptmanager)
{
  OEntityManager *entitymanager = (OEntityManager*)oerror_malloc(sizeof(OEntityManager));
  entitymanager->scriptmanager = scriptmanager;
  entitymanager->symbol2name = optrmap_new((OPtrMapValueCallback)ostring_release);
  return entitymanager;
}

void oentitymanager_release(OEntityManager *entitymanager)
{
  if (!entitymanager)
    return;

  optrmap_release(entitymanager->symbol2name);
  memset(entitymanager, 0, sizeof(OEntityManager));
  oerror_free(entitymanager);
}

OString *oentitymanager_getnameforsymbol(OEntityManager *entitymanager, const ochar *symbol)
{
  return (OString*)optrmap_get(entitymanager->symbol2name, symbol);
}

oboolean oentitymanager_registersymbolforname(OEntityManager *entitymanager, const ochar *symbol, const ochar *name)
{
  if (optrmap_put(entitymanager->symbol2name, symbol, ostring_newstr(name)))
    return 1;

  return 0;
}
