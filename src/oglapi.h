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
#ifndef OGLAPI_H
#define OGLAPI_H

#include "ogame.h"

#ifdef OUSEGLHEADERSDIRECTLY
#include <GL/gl.h>
#else
#include "SDL_opengl.h"
#endif

void     oglapi_init                (OGame *game);
void     oglapi_release             ();
void     oglapi_rotate              (GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
void     oglapi_translate           (GLdouble x, GLdouble y, GLdouble z);
void     oglapi_scale               (GLdouble x, GLdouble y, GLdouble z);
void     oglapi_loadidentity        (void);
void     oglapi_color3              (GLdouble red, GLdouble green, GLdouble blue);
void     oglapi_normal3             (GLdouble nx, GLdouble ny, GLdouble nz);
void     oglapi_texcoord2           (GLdouble s, GLdouble t);
void     oglapi_vertex3             (GLdouble x, GLdouble y, GLdouble z);
void     oglapi_enterortho          (void);
void     oglapi_leaveortho          (void);
void     oglapi_bindtexture2D       (GLuint texture);
void     oglapi_lookat              (GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, GLdouble centerX, GLdouble centerY, GLdouble centerZ, GLdouble upX, GLdouble upY, GLdouble upZ);
void     oglapi_calllist            (GLuint list);
void     oglapi_enable2dtexturing   ();
void     oglapi_disable2dtexturing  ();
void     oglapi_enablelighting      ();
void     oglapi_disablelighting     ();
void     oglapi_enableblend         ();
void     oglapi_disableblend        ();
void     oglapi_pushmatrix          ();
void     oglapi_popmatrix           ();
void     oglapi_pushenablebitattrib ();
void     oglapi_popattrib           ();
oint32   oglapi_newquadric          ();
oboolean oglapi_disk                (oint32 quadid, GLdouble inner, GLdouble outer, GLint slices, GLint loops);
void     oglapi_quadi               (GLint x1, GLint y1, GLint x2, GLint y2, int textureid);

#endif
