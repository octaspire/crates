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
#include "otextmanager.h"
#include "oglapi.h"
#include "otexturemanager.h"
#include <string.h>

OTextManager *otextmanager_new(const ochar *name, OTextureManager *texturemanager)
{
  oint32 loop;
  onumber cx, cy;
  OTextManager *textmanager = (OTextManager*)oerror_malloc(sizeof(OTextManager));
  textmanager->list = 0;
  textmanager->numids = 256;
  textmanager->list = glGenLists(textmanager->numids);
  textmanager->textureid = otexturemanager_getidforname(texturemanager, name);

  glBindTexture(GL_TEXTURE_2D, textmanager->textureid);
  for ( loop = 0; loop < 256; loop++ )
  {
    cx = 1 - (loop % 16) / 16.0;
    cy = 1 - (loop / 16) / 16.0;

    glNewList(textmanager->list + (255-loop), GL_COMPILE);
      glBegin(GL_QUADS);
        glTexCoord2f(cx-0.0625, cy);
        glVertex2i(0, 0);
        glTexCoord2f(cx, cy);
        glVertex2i(16, 0 );
        glTexCoord2f(cx, cy-0.0625f);
        glVertex2i(16, 16);
        glTexCoord2f(cx-0.0625f, cy-0.0625f);
        glVertex2i(0, 16);
      glEnd( );
      /* One character forward */
      glTranslated(8, 0, 0);
    glEndList();
  }

  return textmanager;
}

void otextmanager_release(OTextManager *textmanager)
{
  if (!textmanager)
    return;

  glDeleteLists(textmanager->list, textmanager->numids);
  textmanager->list = 0;
  textmanager->numids = 0;
  oerror_free(textmanager);
}

INLINE void otextmanager_printortho(OTextManager *textmanager, oint32 x, oint32 y, const ouchar *characters, ouint32 set)
{
  if (set > 1)
    set = 1;

  glBindTexture(GL_TEXTURE_2D, textmanager->textureid);

  oglapi_enterortho();
    glTranslated(x, y, 0);
    glListBase(textmanager->list - 32 + ((128 * set) - (16*set)));
    glCallLists(strlen((const char*)characters), GL_UNSIGNED_BYTE, characters);
  oglapi_leaveortho();
}

void otextmanager_printperspective(OTextManager *textmanager, const ouchar *characters, ouint32 set)
{
  if (set > 1)
    set = 1;

  glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT);
  glEnable(GL_BLEND);
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glBindTexture(GL_TEXTURE_2D, textmanager->textureid);
  glListBase(textmanager->list - 32 + ((128 * set) - (16*set)));
  glCallLists(strlen((const char*)characters), GL_UNSIGNED_BYTE, characters);
  glPopAttrib();
}
