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
#include "omeshmanager.h"
#include "odir.h"
#include "oerror.h"
#include <stdlib.h>
#include "SDL.h"

void omeshmanager_loadmesh(const ochar *path, const ochar *fileName, void *userdata)
{
  GLuint listID = glGenLists(1);
  omap_put(((OMeshManager*)userdata)->name2listid, fileName, &listID);

  glNewList(listID, GL_COMPILE);
    glBegin(GL_QUADS);
      oscriptmanager_loadscript(((OMeshManager*)userdata)->scriptmanager, path);
    glEnd();
  glEndList();
}

static void omeshmanager_releaselistid(void *element)
{
  glDeleteLists(*((GLuint*)element), 1);
}

OMeshManager *omeshmanager_new(OScriptManager *scriptmanager)
{
  ochar path[] = "resources/meshes";
  OMeshManager *meshmanager = (OMeshManager*)oerror_malloc(sizeof(OMeshManager));
  meshmanager->scriptmanager = scriptmanager;
  meshmanager->name2listid = omap_new(sizeof(GLuint), omeshmanager_releaselistid);
  odir_foreachentry(odir_convertpath(path), omeshmanager_loadmesh, meshmanager);
  return meshmanager;
}

void omeshmanager_release(OMeshManager *meshmanager)
{
  if (!meshmanager)
    return;

  omap_release(meshmanager->name2listid);
  oerror_free(meshmanager);
}

GLuint omeshmanager_getidforname(OMeshManager *meshmanager, const ochar *name)
{
  GLuint *result = (GLuint*)omap_get(meshmanager->name2listid, name);

  if (result)
    return *result;

  return 0;
}
