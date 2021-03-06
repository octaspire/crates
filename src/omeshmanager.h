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
#ifndef OMESHMANAGER_H
#define OMESHMANAGER_H

#include "oscriptmanager.h"
#include "ostring.h"
#include "otypes.h"
#include "omap.h"

#ifdef OUSEGLHEADERSDIRECTLY
#include <GL/gl.h>
#else
#include "SDL_opengl.h"
#endif

typedef struct {
  OMap           *name2listid;
  OScriptManager *scriptmanager;
} OMeshManager;

OMeshManager *omeshmanager_new          (OScriptManager *scriptmanager);
void          omeshmanager_release      (OMeshManager *meshmanager);
GLuint        omeshmanager_getidforname (OMeshManager *meshmanager, const ochar *name);

#endif
