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
#ifndef OENTITY_H
#define OENTITY_H

#include "ovector3.h"
#include "oscriptmanager.h"
#include "ostring.h"
#include "oevent.h"
#include "otypes.h"
#include <stdio.h>

#ifdef OUSEGLHEADERSDIRECTLY
#include <GL/gl.h>
#else
#include "SDL_opengl.h"
#endif

typedef struct {
  OVector3       *location;
  OVector3       *velocity;
  OVector3       *rotation;
  OString        *name;
  OString        *initattributes;
  OString        *state;
  oint32          id;
  oint32          textureid;
  oint32          listid;
  oboolean        alive;
  oboolean        eventable;
  oboolean        updatable;
  oboolean        renderable;
  oboolean        postrenderable;
  oboolean        collidable;
  oboolean        movable;
  OScriptManager *scriptmanager;
} OEntity;

OEntity     *oentity_new                (const ochar *name, const ochar *initattributes, const oint32 id, OScriptManager *scriptmanager);
void         oentity_callluainit        (OEntity *entity);
void         oentity_release            (OEntity *entity);
void         oentity_setid              (OEntity *entity, const oint32 id);
oint32       oentity_getid              (const OEntity *entity);
oboolean     oentity_isalive            (const OEntity *entity);
oboolean     oentity_iseventable        (const OEntity *entity);
oboolean     oentity_isupdatable        (const OEntity *entity);
oboolean     oentity_isrenderable       (const OEntity *entity);
oboolean     oentity_ispostrenderable   (const OEntity *entity);
oboolean     oentity_iscollidable       (const OEntity *entity);
oboolean     oentity_ismovable          (const OEntity *entity);
void         oentity_setalive           (OEntity *entity, const oboolean alive);
void         oentity_seteventable       (OEntity *entity, const oboolean eventable);
void         oentity_setupdatable       (OEntity *entity, const oboolean updatable);
void         oentity_setrenderable      (OEntity *entity, const oboolean renderable);
void         oentity_setpostrenderable  (OEntity *entity, const oboolean postrenderable);
void         oentity_setcollidable      (OEntity *entity, const oboolean collidable);
void         oentity_setmovable         (OEntity *entity, const oboolean movable);
oboolean     oentity_hascollision       (const OEntity *entity, const OEntity *other);
const ochar *oentity_onwhichsideisother (const OEntity *entity, const OEntity *other);
void         oentity_onevent            (OEntity *entity, const enum OEvent event);
void         oentity_onupdate           (OEntity *entity, const onumber tpf);
void         oentity_onrender           (OEntity *entity);
void         oentity_oncollision        (OEntity *entity, OEntity *other);
void         oentity_setstatestr        (OEntity *entity, const ochar *characters);
const ochar *oentity_statetocstr        (const OEntity *entity);
void         oentity_fprint             (const OEntity *entity, FILE *to);

#endif
