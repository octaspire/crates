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
#include "oglapi.h"
#include "optrvector.h"
#include "osettings.h"

#ifdef OUSEGLHEADERSDIRECTLY
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include "SDL_opengl.h"
#endif

static OGame      *oglapi_privategame = 0;
static OPtrVector *quadrics;

void oglapi_init(OGame *game)
{
  oglapi_privategame = game;
  quadrics = optrvector_new(0, (OPtrVectorElementCallback)gluDeleteQuadric);
}

void oglapi_release()
{
  optrvector_release(quadrics);
  quadrics = 0;
}

INLINE void oglapi_rotate(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
  glRotated(angle, x, y, z);
}

INLINE void oglapi_translate(GLdouble x, GLdouble y, GLdouble z)
{
  glTranslated(x, y, z);
}

INLINE void oglapi_scale(GLdouble x, GLdouble y, GLdouble z)
{
  glScaled(x, y, z);
}

INLINE void oglapi_loadidentity()
{
  glLoadIdentity();
}

INLINE void oglapi_color3(GLdouble red, GLdouble green, GLdouble blue)
{
  glColor3d(red, green, blue);
}

INLINE void oglapi_normal3(GLdouble nx, GLdouble ny, GLdouble nz)
{
  glNormal3d(nx, ny, nz);
}

INLINE void oglapi_texcoord2(GLdouble s, GLdouble t)
{
  glTexCoord2d(s, t);
}

INLINE void oglapi_vertex3(GLdouble x, GLdouble y, GLdouble z)
{
  glVertex3d(x, y, z);
}

INLINE void oglapi_enterortho()
{
  glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, oglapi_privategame->settings->screenwidth, 0, oglapi_privategame->settings->screenheight, -1, 1);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glEnable(GL_BLEND);
}

INLINE void oglapi_leaveortho()
{
  glDisable(GL_BLEND);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glPopAttrib();
}

INLINE void oglapi_bindtexture2D(GLuint texture)
{
  glBindTexture(GL_TEXTURE_2D, texture);
}

INLINE void oglapi_lookat(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, GLdouble centerX, GLdouble centerY, GLdouble centerZ, GLdouble upX, GLdouble upY, GLdouble upZ)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
}

INLINE void oglapi_calllist(GLuint list)
{
  glCallList(list);
}

INLINE void oglapi_enable2dtexturing()
{
  glEnable(GL_TEXTURE_2D);
}

INLINE void oglapi_disable2dtexturing()
{
  glDisable(GL_TEXTURE_2D);
}

INLINE void oglapi_enablelighting()
{
  glEnable(GL_LIGHTING);
}

INLINE void oglapi_disablelighting()
{
  glDisable(GL_LIGHTING);
}

INLINE void oglapi_enableblend()
{
  glEnable(GL_BLEND);
}

INLINE void oglapi_disableblend()
{
  glDisable(GL_BLEND);
}

INLINE void oglapi_pushmatrix()
{
  glPushMatrix();
}

INLINE void oglapi_popmatrix()
{
  glPopMatrix();
}

INLINE void oglapi_pushenablebitattrib()
{
  glPushAttrib(GL_ENABLE_BIT);
}

INLINE void oglapi_popattrib()
{
  glPopAttrib();
}

oint32 oglapi_newquadric()
{
  oint32 index;
  GLUquadric *quad = gluNewQuadric();

  if (!quad)
    return -1;

  index = optrvector_length(quadrics);
  optrvector_pushback(quadrics, quad);
  return index;
}

INLINE oboolean oglapi_disk(oint32 quadid, GLdouble inner, GLdouble outer, GLint slices, GLint loops)
{
  GLUquadric *quad = (GLUquadric*)optrvector_getelementptrat(quadrics, quadid);

  if (!quad)
    return 0;

  gluDisk(quad, inner, outer, slices, loops);
  return 1;
}

INLINE void oglapi_quadi(GLint x1, GLint y1, GLint x2, GLint y2, int textureid)
{
  oglapi_enterortho();
    oglapi_bindtexture2D(textureid);
    glBegin(GL_QUADS);
      glTexCoord2f(0, 1); glVertex2i(x1, y1);
      glTexCoord2f(1, 1); glVertex2i(x2, y1);
      glTexCoord2f(1, 0); glVertex2i(x2, y2);
      glTexCoord2f(0, 0); glVertex2i(x1, y2);
    glEnd();
  oglapi_leaveortho();
}
