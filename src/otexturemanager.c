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
#include "otexturemanager.h"
#include "odir.h"
#include "oerror.h"
#include "opngtotexture.h"
#include <stdlib.h>
#include "SDL.h"

#define OTEXTUREMANAGER_MAXSKINNAMELEN 128

void otexturemanager_loadtexture(const ochar *path, const ochar *fileName, void *userdata)
{
  GLuint textureID = 0;
  OTexture *texture = opngtotexture_new(path);

  if (texture)
  {
    glGenTextures(1, &textureID);
    omap_put(((OTextureManager*)userdata)->name2textureid, fileName, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, texture->internalformat, texture->width, texture->height, 0, texture->format, GL_UNSIGNED_BYTE, texture->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    opngtotexture_release(texture);
    texture = 0;
  }
}

OTextureManager *otexturemanager_new(const OSettings *settings)
{
  OString *path = ostring_newstr("resources/textures/");
  OString *skinname = 0;
  OTextureManager *texturemanager = (OTextureManager*)oerror_malloc(sizeof(OTextureManager));
  texturemanager->settings = settings;
  texturemanager->name2textureid = omap_new(sizeof(GLuint), 0);
  skinname = osettings_getskin(texturemanager->settings);

  if (skinname == 0)
    oerror_fatal("otexturemanager: this should not have happened: skinname is NULL");

  if (ostring_length(skinname) > OTEXTUREMANAGER_MAXSKINNAMELEN)
    oerror_fatal("otexturemanager: skinname is too long (%d characters). Maximum length is %d", ostring_length(skinname), OTEXTUREMANAGER_MAXSKINNAMELEN);

  if (path == 0)
    oerror_fatal("otexturemanager: this should not have happened: path is NULL");

  ostring_concat(path, skinname);

  if (path->characters == 0 || path->characters->elements == 0)
    oerror_fatal("otexturemanager: this should not have happened: path->characters->elements is not available");

  /* TODO direct access to path->characters and path->characters->elements is ugly */
  /* TODO check that directory exists and use default value otherwise */
  odir_foreachentry(odir_convertpath(path->characters->elements), otexturemanager_loadtexture, texturemanager);
  ostring_release(path);
  path = 0;
  return texturemanager;
}

void otexturemanager_release(OTextureManager *texturemanager)
{
  if (!texturemanager)
    return;

  omap_release(texturemanager->name2textureid);
  memset(texturemanager, 0, sizeof(OTextureManager));
  oerror_free(texturemanager);
}

GLuint otexturemanager_getidforname(OTextureManager *texturemanager, const ochar *name)
{
  GLuint *result = (GLuint*)omap_get(texturemanager->name2textureid, name);

  if (result)
    return *result;

  return 0;
}
