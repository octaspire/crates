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
#ifndef OTEXTMANAGER_H
#define OTEXTMANAGER_H

#include "ostring.h"
#include "otexturemanager.h"
#include "otypes.h"
#include <stdint.h>

#ifdef OUSEGLHEADERSDIRECTLY
#include <GL/gl.h>
#else
#include "SDL_opengl.h"
#endif

typedef struct {
  GLuint   list;
  ouint32  numids;
  GLuint   textureid;
} OTextManager;

OTextManager *otextmanager_new              (const ochar *name, OTextureManager *texturemanager);
void          otextmanager_release          (OTextManager *textmanager);
void          otextmanager_printortho       (OTextManager *textmanager, oint32 x, oint32 y, const ouchar *characters, ouint32 set);
void          otextmanager_printperspective (OTextManager *textmanager, const ouchar *characters, ouint32 set);

#endif
