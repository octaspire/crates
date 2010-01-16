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
#include "obits.h"
#include "oentity.h"
#include "oerror.h"
#include "odir.h"
#include "ostate.h"
#include "oscriptmanager.h"
#include "ovector3.h"
#include <string.h>
#include <stdlib.h>

OEntity *oentity_new(const ochar *name, const ochar *initattributes, const oint32 id, OScriptManager *scriptmanager)
{
  ochar path[64] = "resources/entities/";
  const ouint32 maxnlen = 40;
  OEntity *entity = 0;

  if (strlen(name) > maxnlen)
    oerror_fatal("entity name \"%s\" is too long (longer than %d)", name, maxnlen);

  entity = (OEntity*)oerror_malloc(sizeof(OEntity));
  entity->scriptmanager = scriptmanager;
  entity->location = ovector3_new();
  entity->velocity = ovector3_new();
  entity->rotation = ovector3_new();
  entity->name = ostring_newstr(name);
  entity->initattributes = ostring_newstr(initattributes);
  entity->state = ostring_newstr("initial");
  entity->id = id;
  entity->alive = 1;
  entity->postrenderable = 0;
  entity->textureid = -1;
  entity->listid    = -1;
  strncat(path, name, maxnlen);
  strcat(path, ".lua");
  oscriptmanager_loadscript(entity->scriptmanager, odir_convertpath(path));
  return entity;
}

void oentity_callluainit(OEntity *entity)
{
  oscriptmanager_calltabletablefunctionva(entity->scriptmanager, ostring_tocstr(entity->name), ostring_tocstr(entity->state), "init", "is>", entity->id, ostring_tocstr(entity->initattributes));
}

void oentity_release(OEntity *entity)
{
  if (!entity)
    return;

  ovector3_release(entity->location);
  ovector3_release(entity->velocity);
  ovector3_release(entity->rotation);
  ostring_release(entity->name);
  ostring_release(entity->initattributes);
  ostring_release(entity->state);
  entity->id = -1;
  entity->alive = 0;
  memset(entity, 0, sizeof(OEntity));
  oerror_free(entity);
}

INLINE void oentity_setid(OEntity *entity, const oint32 id)
{
  entity->id = id;
}

INLINE oint32 oentity_getid(const OEntity *entity)
{
  return entity->id;
}

INLINE oboolean oentity_isalive(const OEntity *entity)
{
  return entity->alive;
}

INLINE oboolean oentity_iseventable(const OEntity *entity)
{
  return entity->eventable;
}

INLINE oboolean oentity_isupdatable(const OEntity *entity)
{
  return entity->updatable;
}

INLINE oboolean oentity_isrenderable(const OEntity *entity)
{
  return entity->renderable;
}

INLINE oboolean oentity_ispostrenderable(const OEntity *entity)
{
  return entity->postrenderable;
}

INLINE oboolean oentity_iscollidable(const OEntity *entity)
{
  return entity->collidable;
}

INLINE oboolean oentity_ismovable(const OEntity *entity)
{
  return entity->movable;
}

INLINE void oentity_setalive(OEntity *entity, const oboolean alive)
{
  entity->alive = alive;
}

INLINE void oentity_seteventable(OEntity *entity, const oboolean eventable)
{
  entity->eventable = eventable;
}

INLINE void oentity_setupdatable(OEntity *entity, const oboolean updatable)
{
  entity->updatable = updatable;
}

INLINE void oentity_setrenderable(OEntity *entity, const oboolean renderable)
{
  entity->renderable = renderable;
}

INLINE void oentity_setpostrenderable(OEntity *entity, const oboolean postrenderable)
{
  entity->postrenderable = postrenderable;
}

INLINE void oentity_setcollidable(OEntity *entity, const oboolean collidable)
{
  entity->collidable = collidable;
}

INLINE void oentity_setmovable(OEntity *entity, const oboolean movable)
{
  entity->movable = movable;
}

INLINE oboolean oentity_hascollision(const OEntity *entity, const OEntity *other)
{
  /* TODO mesh manager could calculate extends and those could be used. But it
  is both a bit more faster and simpler not to calculate those. */
  return ovector3_distancesquared(entity->location, other->location) <= 4;
}

const ochar *oentity_onwhichsideisother(const OEntity *entity, const OEntity *other)
{
  static const ochar *north   = "north";
  static const ochar *south   = "south";
  static const ochar *east    = "east";
  static const ochar *west    = "west";
  static const ochar *top     = "top";
  static const ochar *bottom  = "bottom";
  static const ochar *unknown = "unknown";
  const onumber epsilon = 0.3;

  onumber diff = entity->location->x - other->location->x;
  if (abs(diff) > epsilon)
    return (diff < 0) ? east : west;

  diff = entity->location->z - other->location->z;
  if (abs(diff) > epsilon)
    return (diff < 0) ? south : north;

  /* TODO check are these correct: bottom <-> top ? */
  diff = entity->location->y - other->location->y;
  if (abs(diff) > epsilon)
    return (diff < 0) ? bottom : top;

  return unknown;
}

void oentity_onevent(OEntity *entity, const enum OEvent event)
{
  oscriptmanager_calltabletablefunctionva(entity->scriptmanager, ostring_tocstr(entity->name), ostring_tocstr(entity->state), "event", "is>", entity->id, oevent_itoa[event]);
}

void oentity_onupdate(OEntity *entity, const onumber tpf)
{
  oscriptmanager_calltabletablefunctionva(entity->scriptmanager, ostring_tocstr(entity->name), ostring_tocstr(entity->state), "update", "id>", entity->id, tpf);
}

void oentity_oncollision(OEntity *entity, OEntity *other)
{
  oscriptmanager_calltabletablefunctionva(entity->scriptmanager, ostring_tocstr(entity->name), ostring_tocstr(entity->state), "collision", "ii>", entity->id, other->id);
}

void oentity_onrender(OEntity *entity)
{
  glPushMatrix();
  glTranslatef(entity->location->x, entity->location->y, entity->location->z);

  if (entity->rotation->x)
    glRotatef(entity->rotation->x, 1, 0, 0);

  if (entity->rotation->y)
    glRotatef(entity->rotation->y, 0, 1, 0);

  if (entity->rotation->z)
    glRotatef(entity->rotation->z, 0, 0, 1);

  if (entity->textureid > -1)
    glBindTexture(GL_TEXTURE_2D, entity->textureid);

  if (entity->listid > -1)
    glCallList(entity->listid);

  if (entity->postrenderable)
    oscriptmanager_calltabletablefunctionva(entity->scriptmanager, ostring_tocstr(entity->name), ostring_tocstr(entity->state), "render", "i>", entity->id);

  glPopMatrix();
}

void oentity_setstatestr(OEntity *entity, const ochar *characters)
{
  ostring_setstr(entity->state, characters);
}

const ochar *oentity_statetocstr(const OEntity *entity)
{
  return ostring_tocstr(entity->state);
}

void oentity_fprint(const OEntity *entity, FILE *to)
{
  fprintf(to, "Entity id=%d name=%s, state=%s location=%f,%f,%f, velocity=%f,%f,%f\n", entity->id, ostring_tocstr(entity->name), ostring_tocstr(entity->state), entity->location->x, entity->location->y, entity->location->z, entity->velocity->x, entity->velocity->y, entity->velocity->z);
}
