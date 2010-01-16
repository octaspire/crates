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
#include "ostate.h"
#include "oerror.h"
#include "odir.h"
#include "oscriptmanager.h"
#include "oentity.h"
#include <string.h>
#include <assert.h>

OState *ostate_new(const ochar *name, const ochar *argument, OTextManager *textmanager, OScriptManager *scriptmanager)
{
  ochar path[64] = "resources/states/";
  const ouint32 maxnlen = 40;
  OState *state = 0;

  if (strlen(name) > maxnlen)
    oerror_fatal("state name \"%s\" is too long (longer than %d)", name, maxnlen);

  state = (OState*)oerror_malloc(sizeof(OState));
  state->scriptmanager = scriptmanager;
  state->messagestack = omessagestack_new(textmanager);
  state->entities = optrvector_new(0, (OPtrVectorElementCallback)oentity_release);
  state->tobeadded = optrvector_new(0, 0);
  state->name = ostring_newstr(name);
  state->state = ostring_newstr("initial");
  state->argument = ostring_newstr(argument);
  state->updatable = 0;
  state->eventable = 0;
  state->renderable = 0;
  strncat(path, name, maxnlen);
  strcat(path, ".lua");
  oscriptmanager_loadscript(scriptmanager, odir_convertpath(path));
  return state;
}

void ostate_callluainit(OState *state)
{
  oscriptmanager_calltabletablefunctionva(state->scriptmanager, ostring_tocstr(state->name), ostring_tocstr(state->state), "init", "s>", ostring_tocstr(state->argument));
}

void ostate_release(OState *state)
{
  if (!state)
    return;

  optrvector_release(state->entities);
  optrvector_release(state->tobeadded);
  ostring_release(state->name);
  ostring_release(state->state);
  ostring_release(state->argument);
  omessagestack_release(state->messagestack);
  state->updatable = 0;
  state->eventable = 0;
  state->renderable = 0;
  memset(state, 0, sizeof(OState));
  oerror_free(state);
}

oint32 ostate_addentity(OState *state, const ochar *name, const ochar *initattributes, const onumber x, const onumber y, const onumber z)
{
  /* TODO adding throung tobeadded */
  OEntity *entity = 0;
  oint32 index = optrvector_length(state->entities);
  entity = oentity_new(name, initattributes, index, state->scriptmanager);
  ovector3_set(entity->location, x, y, z);
  optrvector_pushback(state->entities, entity);
  oentity_callluainit(entity);
  return index;
}

INLINE OEntity *ostate_getentityptrwithid(OState *state, const oint32 id)
{
  return (OEntity*)optrvector_getelementptrat(state->entities, id);
}

INLINE OEntity *ostate_getentityptrwithiderror(OState *state, const oint32 id)
{
  OEntity *ptr = (OEntity*)optrvector_getelementptrat(state->entities, id);

  if (ptr == 0)
    oerror_fatal("state has no entity with id %d", id);

  return ptr;
}

void ostate_events(OState *state, const enum OEvent event)
{
  size_t i;
  OEntity *entityptr;
  OPtrVector *vec = state->entities;
  size_t len = optrvector_length(vec);

  for (i = 0; i < len; ++i)
  {
    entityptr = (OEntity*)optrvector_getelementptrat(vec, i);
    if (!oentity_isalive(entityptr) || !oentity_iseventable(entityptr))
      continue;

    oentity_onevent(entityptr, event);
  }

  if (state->eventable)
    oscriptmanager_calltabletablefunctionva(state->scriptmanager, ostring_tocstr(state->name), ostring_tocstr(state->state), "event", "s>", oevent_itoa[event]);
}

void ostate_update(OState *state, const onumber tpf)
{
  size_t i;
  OEntity *entityptr;
  OPtrVector *vec = state->entities;
  size_t len = optrvector_length(vec);

  for (i = 0; i < len; ++i)
  {
    entityptr = (OEntity*)optrvector_getelementptrat(vec, i);
    if (!oentity_isalive(entityptr) || !oentity_isupdatable(entityptr))
      continue;

    oentity_onupdate(entityptr, tpf);
  }

  if (state->updatable)
    oscriptmanager_calltabletablefunctionva(state->scriptmanager, ostring_tocstr(state->name), ostring_tocstr(state->state), "update", "d>", tpf);

  omessagestack_update(state->messagestack, tpf);
}

void ostate_move(OState *state, const onumber tpf)
{
  size_t i;
  OEntity *entityptr;
  onumber dx, dy, dz;
  OPtrVector *vec = state->entities;
  size_t len = optrvector_length(vec);

  for (i = 0; i < len; ++i)
  {
    entityptr = (OEntity*)optrvector_getelementptrat(vec, i);
    if (!oentity_isalive(entityptr) || !oentity_ismovable(entityptr))
      continue;

    dx = tpf * entityptr->velocity->x;
    dy = tpf * entityptr->velocity->y;
    dz = tpf * entityptr->velocity->z;
    ovector3_add3(entityptr->location, dx, dy, dz);
  }
}

void ostate_collide(OState *state)
{
  size_t i, j;
  OEntity *entityColptr;
  OEntity *entityMovptr;
  OPtrVector *vec = state->entities;
  size_t len = optrvector_length(vec);

  for (i = 0; i < len; ++i)
  {
    entityMovptr = (OEntity*)optrvector_getelementptrat(vec, i);
    assert(entityMovptr);

    if (!oentity_isalive(entityMovptr) || !oentity_ismovable(entityMovptr))
      continue;

    for (j = 0; j < len; ++j)
    {
      if (i == j)
        continue;

      entityColptr = (OEntity*)optrvector_getelementptrat(vec, j);
      assert(entityColptr);

      if (!oentity_isalive(entityColptr) || !oentity_iscollidable(entityColptr))
        continue;

      if (oentity_hascollision(entityMovptr, entityColptr))
      {
        oentity_oncollision(entityMovptr, entityColptr);
        oentity_oncollision(entityColptr, entityMovptr);
      }
    }
  }
}

void ostate_render(OState *state)
{
  size_t i;
  OEntity *entityptr;
  OPtrVector *vec = state->entities;
  size_t len = optrvector_length(vec);

  for (i = 0; i < len; ++i)
  {
    entityptr = (OEntity*)optrvector_getelementptrat(vec, i);
    if (!oentity_isalive(entityptr) || !oentity_isrenderable(entityptr))
      continue;

    oentity_onrender(entityptr);
  }

  if (state->renderable)
    oscriptmanager_calltabletablefunctionva(state->scriptmanager, ostring_tocstr(state->name), ostring_tocstr(state->state), "render", ">");

  omessagestack_render(state->messagestack);
}

void ostate_setupdatable(OState *state, const oboolean updatable)
{
  state->updatable = updatable;
}

void ostate_seteventable(OState *state, const oboolean eventable)
{
  state->eventable = eventable;
}

void ostate_setrenderable(OState *state, const oboolean renderable)
{
  state->renderable = renderable;
}

void ostate_setstatestr(OState *state, const ochar *characters)
{
  ostring_setstr(state->state, characters);
}

const ochar *ostate_statetocstr(const OState *state)
{
  return ostring_tocstr(state->state);
}

void ostate_messagestackpush(OState *state, const ochar *message)
{
  omessagestack_push(state->messagestack, message);
}
