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
#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"

void otexturemanager_loadtexture(const ochar *path, const ochar *fileName, void *userdata)
{
  GLuint textureID = 0;
  oint32 mustlock = 0;
  SDL_Surface *imagesurface = IMG_Load(path);

  if (imagesurface)
  {
    glGenTextures(1, &textureID);
    omap_put(((OTextureManager*)userdata)->name2textureid, fileName, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);
    mustlock = SDL_MUSTLOCK(imagesurface);

    if (mustlock != 0)
      SDL_LockSurface(imagesurface);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, imagesurface->w, imagesurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, imagesurface->pixels);

    if (mustlock != 0)
      SDL_UnlockSurface(imagesurface);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(imagesurface);
    imagesurface = 0;
  }
}

OTextureManager *otexturemanager_new(void)
{
  ochar path[] = "resources/textures";
  OTextureManager *texturemanager = (OTextureManager*)oerror_malloc(sizeof(OTextureManager));
  texturemanager->name2textureid = omap_new(sizeof(GLuint), 0);
  odir_foreachentry(odir_convertpath(path), otexturemanager_loadtexture, texturemanager);
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
