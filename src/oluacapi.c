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
#include "oluacapi.h"
#include "odir.h"
#include "oglapi.h"
#include "optrvector.h"
#include "osoundmanager.h"
#include "omusicmanager.h"
#include "oversion.h"
#include "lauxlib.h"
#include <assert.h>
#include <stdio.h>
#include <math.h>

static OGame      *oluacapi_game;

/**
  i>nnn
  id:integer -> x:number, y:number, z:number
*/
static int entity_getlocation(lua_State *L)
{
  OEntity *e;
  int id = luaL_checkint(L, 1);
  e = ostate_getentityptrwithiderror(oluacapi_game->state, id);
  lua_pushnumber(L, e->location->x);
  lua_pushnumber(L, e->location->y);
  lua_pushnumber(L, e->location->z);
  return 3;
}

/**
  innn>
  id:integer, x:number, y:number, z:number ->
*/
static int entity_setlocation(lua_State *L)
{
  OEntity *e;
  int id = luaL_checkint(L, 1);
  onumber x = luaL_checknumber(L, 2);
  onumber y = luaL_checknumber(L, 3);
  onumber z = luaL_checknumber(L, 4);
  e = ostate_getentityptrwithiderror(oluacapi_game->state, id);
  e->location->x = x;
  e->location->y = y;
  e->location->z = z;
  return 0;
}

/**
  i>nnn
  id:integer -> x:number, y:number, z:number
*/
static int entity_getvelocity(lua_State *L)
{
  OEntity *e;
  int id = luaL_checkint(L, 1);
  e = ostate_getentityptrwithiderror(oluacapi_game->state, id);
  lua_pushnumber(L, e->velocity->x);
  lua_pushnumber(L, e->velocity->y);
  lua_pushnumber(L, e->velocity->z);
  return 3;
}

/**
  innn>
  id:integer, x:number, y:number, z:number ->
*/
static int entity_setvelocity(lua_State *L)
{
  OEntity *e;
  int id = luaL_checkint(L, 1);
  onumber x = luaL_checknumber(L, 2);
  onumber y = luaL_checknumber(L, 3);
  onumber z = luaL_checknumber(L, 4);
  e = ostate_getentityptrwithiderror(oluacapi_game->state, id);
  e->velocity->x = x;
  e->velocity->y = y;
  e->velocity->z = z;
  return 0;
}

/**
  i>s
  id:integer -> name:string
*/
static int entity_getname(lua_State *L)
{
  OEntity *e;
  int id = luaL_checkint(L, 1);
  e = ostate_getentityptrwithiderror(oluacapi_game->state, id);
  lua_pushstring(L, ostring_tocstr(e->name));
  return 1;
}

INLINE static onumber oluacapi_trunc(onumber x)
{
  if (x < 0)
    return ceil(x);

  return floor(x);
}

/**
  i>
  id:integer ->
*/
static int entity_trunclocation(lua_State *L)
{
  OEntity *e;
  int id = luaL_checkint(L, 1);
  e = ostate_getentityptrwithiderror(oluacapi_game->state, id);
  e->location->x = oluacapi_trunc(e->location->x);
  e->location->y = oluacapi_trunc(e->location->y);
  e->location->z = oluacapi_trunc(e->location->z);
  return 0;
}

/**
  i>b
  id:integer -> stopped:boolean
*/
static int entity_isstopped(lua_State *L)
{
  OEntity *e;
  int id = luaL_checkint(L, 1);
  e = ostate_getentityptrwithiderror(oluacapi_game->state, id);
  /* TODO error handling */
  lua_pushboolean(L, ovector3_iszero(e->velocity));
  return 1;
}

/**
  is>b
  id:integer, name:string -> isname:boolean
*/
static int entity_isname(lua_State *L)
{
  OEntity *e;
  int id = luaL_checkint(L, 1);
  const char *name = luaL_checkstring(L, 2);
  e = ostate_getentityptrwithiderror(oluacapi_game->state, id);
  lua_pushboolean(L, (ostring_comparestr(e->name, name) == 0));
  return 1;
}

/**
  ib>
  id:integer, alive:boolean ->
*/
static int entity_setalive(lua_State *L)
{
  int id = luaL_checkint(L, 1);
  int alive = lua_toboolean(L, 2);
  OEntity *entity = ostate_getentityptrwithiderror(oluacapi_game->state, id);
  oentity_setalive(entity, alive);
  return 0;
}

/**
  ib>
  id:integer, eventable:boolean ->
*/
static int entity_seteventable(lua_State *L)
{
  int id = luaL_checkint(L, 1);
  int eventable = lua_toboolean(L, 2);
  OEntity *entity = ostate_getentityptrwithiderror(oluacapi_game->state, id);
  oentity_seteventable(entity, eventable);
  return 0;
}

/**
  ib>
  id:integer, updatable:boolean ->
*/
static int entity_setupdatable(lua_State *L)
{
  int id = luaL_checkint(L, 1);
  int updatable = lua_toboolean(L, 2);
  OEntity *entity = ostate_getentityptrwithiderror(oluacapi_game->state, id);
  oentity_setupdatable(entity, updatable);
  return 0;
}

/**
  ib>
  id:integer, renderable:boolean ->
*/
static int entity_setrenderable(lua_State *L)
{
  int id = luaL_checkint(L, 1);
  int renderable = lua_toboolean(L, 2);
  OEntity *entity = ostate_getentityptrwithiderror(oluacapi_game->state, id);
  oentity_setrenderable(entity, renderable);
  return 0;
}

/**
  ib>
  id:integer, postrenderable:boolean ->
*/
static int entity_setpostrenderable(lua_State *L)
{
  int id = luaL_checkint(L, 1);
  int postrenderable = lua_toboolean(L, 2);
  OEntity *entity = ostate_getentityptrwithiderror(oluacapi_game->state, id);
  oentity_setpostrenderable(entity, postrenderable);
  return 0;
}

/**
  ib>
  id:integer, collidable:boolean ->
*/
static int entity_setcollidable(lua_State *L)
{
  int id = luaL_checkint(L, 1);
  int collidable = lua_toboolean(L, 2);
  OEntity *entity = ostate_getentityptrwithiderror(oluacapi_game->state, id);
  oentity_setcollidable(entity, collidable);
  return 0;
}

/**
  ib>
  id:integer, movable:boolean ->
*/
static int entity_setmovable(lua_State *L)
{
  int id = luaL_checkint(L, 1);
  int movable = lua_toboolean(L, 2);
  OEntity *entity = ostate_getentityptrwithiderror(oluacapi_game->state, id);
  oentity_setmovable(entity, movable);
  return 0;
}

/**
  i>b
  id:integer -> movable:boolean
*/
static int entity_ismovable(lua_State *L)
{
  int id = luaL_checkint(L, 1);
  OEntity *entity = ostate_getentityptrwithiderror(oluacapi_game->state, id);

  if (entity)
    lua_pushboolean(L, oentity_ismovable(entity));
  else
    lua_pushboolean(L, 0);

  return 1;
}

/**
  is>
  id:integer, texturename:string ->
*/
static int entity_settexture(lua_State *L)
{
  OEntity *e;
  int textureid;
  int id = luaL_checkint(L, 1);
  const char *texturename = luaL_checkstring(L, 2);
  e = ostate_getentityptrwithiderror(oluacapi_game->state, id);
  textureid = otexturemanager_getidforname(oluacapi_game->texturemanager, texturename);
  e->textureid = textureid;
  return 0;
}

/**
  is>
  id:integer, meshname:string ->
*/
static int entity_setmesh(lua_State *L)
{
  OEntity *e;
  int meshid;
  int id = luaL_checkint(L, 1);
  const char *meshname = luaL_checkstring(L, 2);
  e = ostate_getentityptrwithiderror(oluacapi_game->state, id);
  meshid = omeshmanager_getidforname(oluacapi_game->meshmanager, meshname);
  e->listid = meshid;
  return 0;
}

/**
  i>nnn
  id:integer -> x:number, y:number, z:number
*/
static int entity_getrotation(lua_State *L)
{
  OEntity *e;
  int id = luaL_checkint(L, 1);
  e = ostate_getentityptrwithiderror(oluacapi_game->state, id);
  lua_pushnumber(L, e->rotation->x);
  lua_pushnumber(L, e->rotation->y);
  lua_pushnumber(L, e->rotation->z);
  return 3;
}

/**
  innn>
  id:integer, x:number, y:number, z:number ->
*/
static int entity_setrotation(lua_State *L)
{
  OEntity *e;
  int id = luaL_checkint(L, 1);
  onumber x = luaL_checknumber(L, 2);
  onumber y = luaL_checknumber(L, 3);
  onumber z = luaL_checknumber(L, 4);
  e = ostate_getentityptrwithiderror(oluacapi_game->state, id);
  e->rotation->x = x;
  e->rotation->y = y;
  e->rotation->z = z;
  return 0;
}

/**
  ii>b
  id:integer, oid:integer -> collision:boolean
*/
static int entity_hascollision(lua_State *L)
{
  OEntity *e1;
  OEntity *e2;
  int id1  = luaL_checkint(L, 1);
  int id2 = luaL_checkint(L, 2);
  e1 = ostate_getentityptrwithiderror(oluacapi_game->state, id1);
  e2 = ostate_getentityptrwithiderror(oluacapi_game->state, id2);
  lua_pushboolean(L, oentity_hascollision(e1, e2));
  return 1;
}

/**
  ii>s
  id:integer, oid:integer -> side:string[north|south|east|west|top|bottom|unknown]
*/
static int entity_onwhichsideisother(lua_State *L)
{
  OEntity *e1;
  OEntity *e2;
  int id1  = luaL_checkint(L, 1);
  int id2 = luaL_checkint(L, 2);
  e1 = ostate_getentityptrwithiderror(oluacapi_game->state, id1);
  e2 = ostate_getentityptrwithiderror(oluacapi_game->state, id2);
  lua_pushstring(L, oentity_onwhichsideisother(e1, e2));
  return 1;
}

/**
  ii>n
  id:integer, oid:integer -> distance:number
*/
static int entity_getdistancesquared(lua_State *L)
{
  OEntity *e1;
  OEntity *e2;
  onumber distance;
  int id1 = luaL_checkint(L, 1);
  int id2 = luaL_checkint(L, 2);
  e1 = ostate_getentityptrwithiderror(oluacapi_game->state, id1);
  e2 = ostate_getentityptrwithiderror(oluacapi_game->state, id2);
  distance = ovector3_distancesquared(e1->location, e2->location);
  lua_pushnumber(L, distance);
  return 1;
}

/**
  i>nnn
  id:integer -> w:number, h:number, d:number
*/
static int entity_getextentsquared(lua_State *L)
{
  /* The extents could be changed to be dynamic and calculated from the mesh. */
  OEntity *e1;
  int id1  = luaL_checkint(L, 1);
  e1 = ostate_getentityptrwithiderror(oluacapi_game->state, id1);
  lua_pushnumber(L, 4);
  lua_pushnumber(L, 4);
  lua_pushnumber(L, 4);
  return 3;
}

/**
  is>
  id:integer, statename:string ->
*/
static int entity_setstate(lua_State *L)
{
  OEntity *e;
  int id = luaL_checkint(L, 1);
  const char *statename = luaL_checkstring(L, 2);
  e = ostate_getentityptrwithiderror(oluacapi_game->state, id);
  oentity_setstatestr(e, statename);
  return 0;
}

/**
  i>s
  id:integer -> statename:string
*/
static int entity_getstate(lua_State *L)
{
  OEntity *e;
  const char *statename;
  int id = luaL_checkint(L, 1);
  e = ostate_getentityptrwithiderror(oluacapi_game->state, id);
  statename = ostring_tocstr(e->state);
  lua_pushstring(L, statename);
  return 1;
}

/*---------------------------------------------------------------------------*/

/**
  ssnnn>i
  name:string, attributes:string, x:number, y:number, z:number -> id:integer
*/
static int state_addentity(lua_State *L)
{
  oint32 nid = -1;
  const ochar *name = luaL_checkstring(L, 1);
  const ochar *attributes = luaL_checkstring(L, 2);
  onumber x = luaL_checknumber(L, 3);
  onumber y = luaL_checknumber(L, 4);
  onumber z = luaL_checknumber(L, 5);
  nid = ostate_addentity(oluacapi_game->state, name, attributes, x, y, z);
  lua_pushinteger(L, nid);
  return 1;
}

/**
  s>
  statename:string ->
*/
static int state_setstate(lua_State *L)
{
  const char *statename = luaL_checkstring(L, 1);
  ostate_setstatestr(oluacapi_game->state, statename);
  return 0;
}

/**
  b>
  updatable:boolean ->
*/
static int state_setupdatable(lua_State *L)
{
  int updatable = lua_toboolean(L, 1);
  ostate_setupdatable(oluacapi_game->state, updatable);
  return 0;
}

/**
  b>
  eventable:boolean ->
*/
static int state_seteventable(lua_State *L)
{
  int eventable = lua_toboolean(L, 1);
  ostate_seteventable(oluacapi_game->state, eventable);
  return 0;
}

/**
  b>
  renderable:boolean ->
*/
static int state_setrenderable(lua_State *L)
{
  int renderable = lua_toboolean(L, 1);
  ostate_setrenderable(oluacapi_game->state, renderable);
  return 0;
}

/**
  s>
  message:string ->
*/
static int state_messagestackpush(lua_State *L)
{
  const char *message = luaL_checkstring(L, 1);
  ostate_messagestackpush(oluacapi_game->state, message);
  return 0;
}

/*---------------------------------------------------------------------------*/

/**
  nnnn>
  angleindegrees:number, x:number, y:number, z:number ->
*/
static int opengl_rotate(lua_State *L)
{
  onumber angle = luaL_checknumber(L, 1);
  onumber x = luaL_checknumber(L, 2);
  onumber y = luaL_checknumber(L, 3);
  onumber z = luaL_checknumber(L, 4);
  oglapi_rotate(angle, x, y, z);
  return 0;
}

/**
  nnn>
  x:number, y:number, z:number ->
*/
static int opengl_translate(lua_State *L)
{
  onumber x = luaL_checknumber(L, 1);
  onumber y = luaL_checknumber(L, 2);
  onumber z = luaL_checknumber(L, 3);
  oglapi_translate(x, y, z);
  return 0;
}

/**
  nnn>
  x:number, y:number, z:number ->
*/
static int opengl_scale(lua_State *L)
{
  onumber x = luaL_checknumber(L, 1);
  onumber y = luaL_checknumber(L, 2);
  onumber z = luaL_checknumber(L, 3);
  oglapi_scale(x, y, z);
  return 0;
}

/**
  >
 ->
*/
static int opengl_loadidentity(lua_State *L)
{
  oglapi_loadidentity();
  return 0;
}

/**
  nnn>
  r:number, g:number, b:number ->
*/
static int opengl_color3(lua_State *L)
{
  onumber r = luaL_checknumber(L, 1);
  onumber g = luaL_checknumber(L, 2);
  onumber b = luaL_checknumber(L, 3);
  oglapi_color3(r, g, b);
  return 0;
}

/**
  nnn>
  nx:number, ny:number, nz:number ->
*/
static int opengl_normal3(lua_State *L)
{
  onumber nx = luaL_checknumber(L, 1);
  onumber ny = luaL_checknumber(L, 2);
  onumber nz = luaL_checknumber(L, 3);
  oglapi_normal3(nx, ny, nz);
  return 0;
}

/**
  nn>
  s:number, t:number ->
*/
static int opengl_texcoord2(lua_State *L)
{
  onumber s = luaL_checknumber(L, 1);
  onumber t = luaL_checknumber(L, 2);
  oglapi_texcoord2(s, t);
  return 0;
}

/**
  nnn>
  x:number, y:number, z:number ->
*/
static int opengl_vertex3(lua_State *L)
{
  onumber x = luaL_checknumber(L, 1);
  onumber y = luaL_checknumber(L, 2);
  onumber z = luaL_checknumber(L, 3);
  oglapi_vertex3(x, y, z);
  return 0;
}

/**
  nnnnnnnnn>
  eyex:number, eyey:number, eyez:number, centerx:number, centery:number, centerz:number, upx:number, upy:number, upz:number ->
*/
static int opengl_lookat(lua_State *L)
{
  onumber eyeX = luaL_checknumber(L, 1);
  onumber eyeY = luaL_checknumber(L, 2);
  onumber eyeZ = luaL_checknumber(L, 3);
  onumber centerX = luaL_checknumber(L, 4);
  onumber centerY = luaL_checknumber(L, 5);
  onumber centerZ = luaL_checknumber(L, 6);
  onumber upX = luaL_checknumber(L, 7);
  onumber upY = luaL_checknumber(L, 8);
  onumber upZ = luaL_checknumber(L, 9);
  oglapi_lookat(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
  return 0;
}

/**
  i>
  id:integer ->
*/
static int opengl_render(lua_State *L)
{
  int id = luaL_checkint(L, 1);
  oglapi_calllist(id);
  return 0;
}

/**
  >
  ->
*/
static int opengl_enable2dtexturing(lua_State *L)
{
  oglapi_enable2dtexturing();
  return 0;
}

/**
  >
  ->
*/
static int opengl_disable2dtexturing(lua_State *L)
{
  oglapi_disable2dtexturing();
  return 0;
}

/**
  i>
  textureid:integer ->
*/
static int opengl_bindtexture2d(lua_State *L)
{
  int textureid = luaL_checkint(L, 1);
  oglapi_bindtexture2D(textureid);
  return 0;
}

/**
  >
  ->
*/
static int opengl_enablelighting(lua_State *L)
{
  oglapi_enablelighting();
  return 0;
}

/**
  >
  ->
*/
static int opengl_disablelighting(lua_State *L)
{
  oglapi_disablelighting();
  return 0;
}

/**
  >
  ->
*/
static int opengl_enableblend(lua_State *L)
{
  oglapi_enableblend();
  return 0;
}

/**
  >
  ->
*/
static int opengl_disableblend(lua_State *L)
{
  oglapi_disableblend();
  return 0;
}

/**
  >
  ->
*/
static int opengl_pushmatrix(lua_State *L)
{
  oglapi_pushmatrix();
  return 0;
}

/**
  >
  ->
*/
static int opengl_popmatrix(lua_State *L)
{
  oglapi_popmatrix();
  return 0;
}

/**
  >
  ->
*/
static int opengl_pushenablebitattrib(lua_State *L)
{
  oglapi_pushenablebitattrib();
  return 0;
}

/**
  >
  ->
*/
static int opengl_popattrib(lua_State *L)
{
  oglapi_popattrib();
  return 0;
}

/**
  >i
  ->id:integer
*/
static int opengl_newquadric(lua_State *L)
{
  oint32 index = oglapi_newquadric();

  if (index < 0)
  {
    lua_pushnil(L);
    lua_pushstring(L, "no enough memory for new quadric");
    return 2;
  }

  lua_pushinteger(L, index);
  return 1;
}

/**
  innii>
  quadid:integer, number:inner, number:outer, slices:integer, loops:integer ->
*/
static int opengl_disk(lua_State *L)
{
  int quadid = luaL_checkint(L, 1);
  onumber inner = luaL_checknumber(L, 2);
  onumber outer = luaL_checknumber(L, 3);
  int slices = luaL_checkint(L, 4);
  int loops = luaL_checkint(L, 5);

  if (oglapi_disk(quadid, inner, outer, slices, loops) == 0)
  {
    lua_pushnil(L);
    lua_pushstring(L, "no such quadric");
    return 2;
  }

  return 0;
}

/*---------------------------------------------------------------------------*/

/**
  iisi>
  x:integer, y:integer, text:string, set:integer[0|1] ->
*/
static int text_printortho(lua_State *L)
{
  int x = luaL_checkint(L, 1);
  int y = luaL_checkint(L, 2);
  const ouchar *characters = (const ouchar*)luaL_checkstring(L, 3);
  int set = luaL_checkint(L, 4);
  otextmanager_printortho(oluacapi_game->textmanager, x, y, characters, set);
  return 0;
}

/**
  si>
  text:string, set:integer[0|1] ->
*/
static int text_printperspective(lua_State *L)
{
  const ouchar *characters = (const ouchar*)luaL_checkstring(L, 1);
  int set = luaL_checkint(L, 2);
  otextmanager_printperspective(oluacapi_game->textmanager, characters, set);
  return 0;
}

/*---------------------------------------------------------------------------*/

/**
  i>
  textureid:integer ->
*/
static int background_render(lua_State *L)
{
  int textureid = luaL_checkint(L, 1);
  int w = oluacapi_game->settings->screenwidth;
  int h = oluacapi_game->settings->screenheight;
  oglapi_quadi(0, 0, w, h, textureid);
  return 0;
}

/**
  iii>
  textureid:integer, width:integer, height:integer ->
*/
static int background_renderwithsizehints(lua_State *L)
{
  int textureid = luaL_checkint(L, 1);
  int width  = luaL_checkint(L, 2);
  int height = luaL_checkint(L, 3);
  /* TODO max()*/
  if (width > oluacapi_game->settings->screenwidth)
    width = oluacapi_game->settings->screenwidth;

  if (height > oluacapi_game->settings->screenheight)
    height = oluacapi_game->settings->screenheight;

  oglapi_quadi(0, 0, width, height, textureid);
  return 0;
}

/**
  iiiii>
  textureid:integer, x:integer, y:integer, width:integer, height:integer ->
*/
static int background_renderwithsizes(lua_State *L)
{
  int textureid = luaL_checkint(L, 1);
  int x  = luaL_checkint(L, 2);
  int y = luaL_checkint(L, 3);
  int x2 = x + luaL_checkint(L, 4);
  int y2 = y + luaL_checkint(L, 5);
  oglapi_quadi(x, y, x2, y2, textureid);
  return 0;
}

/*---------------------------------------------------------------------------*/

/**
  >
 ->
*/
static int game_quit(lua_State *L)
{
  ogame_quit(oluacapi_game);
  return 0;
}

/**
  >t{s*}
 ->names:table{name:string*}
*/
static int game_getmissionnames(lua_State *L)
{
  ouint32 i;
  ouint32 len = omissionmanager_getnummissions(oluacapi_game->missionmanager);

  lua_newtable(L);

  for(i = 0; i < len; ++i)
  {
    lua_pushstring(L,omissionmanager_getmissionnameat(oluacapi_game->missionmanager, i));
    lua_rawseti(L,-2,i + 1);
  }

  return 1;
}

/**
  ss>
  statename:string, argument:string ->
*/
static int game_changestate(lua_State *L)
{
  const char *statename = luaL_checkstring(L, 1);
  const char *argument  = luaL_checkstring(L, 2);
  /*ostate_release(oluacapi_game->state);
  oluacapi_game->state = ostate_new(statename, argument, oluacapi_game->textmanager, oluacapi_game->scriptmanager);
  ostate_callluainit(oluacapi_game->state);
  return 0;*/
  ostring_setstr(oluacapi_game->nextstatename,     statename);
  ostring_setstr(oluacapi_game->nextstateargument, argument);
  return 0;
}

/**
  s>
  missionname:string ->
*/
static int game_parsemission(lua_State *L)
{
  const char *missionname = luaL_checkstring(L, 1);
  OMission *mission = omissionmanager_getmissionptrnamed(oluacapi_game->missionmanager, missionname);
  omission_parse(mission);
  oluacapi_game->missionmanager->current = mission;
  return 0;
}

/**
  >
 ->
*/
static int game_printmission(lua_State *L)
{
  OMission *mission = oluacapi_game->missionmanager->current;
  /* TODO error checking: null */
  omission_printf(mission);
  return 0;
}

static int push_level(lua_State *L, OLevel *level)
{
  ouint32 i;
  ouint32 len;

  if (level == 0)
  {
    lua_pushnil(L);
    lua_pushstring(L, "nil level");
    return 2;
  }

  lua_newtable(L);
  lua_pushstring(L, ostring_tocstr(level->name));
  lua_setfield(L, -2, "name");
  lua_pushstring(L, ostring_tocstr(level->password));
  lua_setfield(L, -2, "password");
  lua_pushstring(L, ostring_tocstr(level->lookat));
  lua_setfield(L, -2, "lookAt");

  if (level->postinit)
  {
    lua_pushstring(L, ostring_tocstr(level->postinit));
    lua_setfield(L, -2, "postinit");
  }

  lua_pushstring(L, "lines");
  lua_newtable(L);
  len = optrvector_length(level->lines);

  for (i = 0; i < len; ++i)
  {
    lua_pushstring(L, ostring_tocstr((OString*)optrvector_getelementptrat(level->lines, i)));
    lua_rawseti(L,-2,i + 1);
  }

  lua_settable(L, -3);
  lua_pushstring(L, "arguments");
  lua_newtable(L);
  len = olevel_getnumarguments(level);

  for (i = 0; i < len; ++i)
  {
    lua_pushstring(L, olevel_getargumentat(level, i));
    lua_rawseti(L,-2,i + 1);
  }

  lua_settable(L, -3);
  return 1;
}

/**
  s>t{ssst{s*}t{s*}}
  password:string -> level:table{name:string, password:string, lookAt:string, lines:table{line:string*}arguments:table{argument:string*}}
*/
static int game_getlevelwithpassword(lua_State *L)
{
  OLevel *level;
  const ochar *password;
  OMission *mission = oluacapi_game->missionmanager->current;

  if (mission == 0)
  {
    lua_pushnil(L);
    lua_pushstring(L, "nil mission");
    return 2;
  }

  password = luaL_checkstring(L, 1);
  level = omission_getlevelwithpassword(mission, password);
  return push_level(L, level);
}

/**
  >t{sss(s)t{s*}t{s*}}
  password:string -> level:table{name:string, password:string, lookAt:string, (postinit:string), lines:table{line:string*}arguments:table{argument:string*}}
*/
static int game_getcurrentlevel(lua_State *L)
{
  OLevel *level;
  OMission *mission = oluacapi_game->missionmanager->current;

  if (mission == 0)
  {
    lua_pushnil(L);
    lua_pushstring(L, "nil mission");
    return 2;
  }

  level = omission_getcurrentlevelptr(mission);
  return push_level(L, level);
}

/**
  s>b
  password:string -> haslevel:boolean
*/
static int game_haslevelwithpassword(lua_State *L)
{
  const ochar *password;
  OMission *mission = oluacapi_game->missionmanager->current;

  if (mission == 0)
  {
    lua_pushnil(L);
    lua_pushstring(L, "nil mission");
    return 2;
  }

  password = luaL_checkstring(L, 1);
  lua_pushboolean(L, omission_haslevelwithpassword(mission, password));
  return 1;
}

/**
  s>
  password:string ->
*/
static int game_setcurrentlevelfrompassword(lua_State *L)
{
  const ochar *password;
  OMission *mission = oluacapi_game->missionmanager->current;

  if (mission == 0)
  {
    lua_pushnil(L);
    lua_pushstring(L, "nil mission");
    return 2;
  }

  password = luaL_checkstring(L, 1);
  omission_setcurrentlevelfrompassword(mission, password);
  return 0;
}

/**
  s>s
  symbol:string -> name:string
*/
static int game_entitytranslatesymboltoname(lua_State *L)
{
  const ochar *symbol = luaL_checkstring(L, 1);
  OString *name = oentitymanager_getnameforsymbol(oluacapi_game->entitymanager, symbol);

  if (!name)
  {
    lua_pushnil(L);
    lua_pushstring(L, "unknown symbol");
    return 2;
  }

  lua_pushstring(L, ostring_tocstr(name));
  return 1;
}

/**
  ss>b
  symbol:string, name:string -> success:boolean
*/
static int game_entityregistersymbolforname(lua_State *L)
{
  oboolean success;
  const ochar *symbol  = luaL_checkstring(L, 1);
  const ochar *name    = luaL_checkstring(L, 2);
  success = oentitymanager_registersymbolforname(oluacapi_game->entitymanager, symbol, name);
  lua_pushboolean(L, success);
  return 1;
}

/**
  >s
 -> release:string
*/
static int game_getreleasestr(lua_State *L)
{
  lua_pushstring(L, oversion_getreleasestr());
  return 1;
}

/**
  >b
 ->hasnextlevel:boolean
*/
static int mission_hasnextlevel(lua_State *L)
{
  OMission *mission = oluacapi_game->missionmanager->current;

  if (mission == 0)
  {
    lua_pushnil(L);
    lua_pushstring(L, "nil mission");
    return 2;
  }

  lua_pushboolean(L, omission_hasnextlevel(mission));
  return 1;
}

/**
  >
 ->
*/
static int mission_nextlevel(lua_State *L)
{
  OMission *mission = oluacapi_game->missionmanager->current;

  if (mission == 0)
  {
    lua_pushnil(L);
    lua_pushstring(L, "nil mission");
    return 2;
  }

  omission_nextlevel(mission);
  return 0;
}

/**
  >
 ->
*/
static int mission_reset(lua_State *L)
{
  OMission *mission = oluacapi_game->missionmanager->current;

  if (mission == 0)
  {
    lua_pushnil(L);
    lua_pushstring(L, "nil mission");
    return 2;
  }

  omission_reset(mission);
  return 0;
}

/**
  >
 ->
*/
static int mission_resettimer(lua_State *L)
{
  OMission *mission = oluacapi_game->missionmanager->current;

  if (mission == 0)
  {
    lua_pushnil(L);
    lua_pushstring(L, "nil mission");
    return 2;
  }

  omission_resettimer(mission);
  return 0;
}

/**
  >
 ->
*/
static int mission_stoptimer(lua_State *L)
{
  OMission *mission = oluacapi_game->missionmanager->current;

  if (mission == 0)
  {
    lua_pushnil(L);
    lua_pushstring(L, "nil mission");
    return 2;
  }

  omission_stoptimer(mission);
  return 0;
}

/**
  >i
 ->numlevels:integer
*/
static int mission_getnumlevels(lua_State *L)
{
  OMission *mission = oluacapi_game->missionmanager->current;

  if (mission == 0)
  {
    lua_pushnil(L);
    lua_pushstring(L, "nil mission");
    return 2;
  }

  lua_pushinteger(L, omission_getnumlevels(mission));
  return 1;
}

/**
  >i
 ->numpassed:integer
*/
static int mission_getnumpassed(lua_State *L)
{
  OMission *mission = oluacapi_game->missionmanager->current;

  if (mission == 0)
  {
    lua_pushnil(L);
    lua_pushstring(L, "nil mission");
    return 2;
  }

  lua_pushinteger(L, omission_getnumpassed(mission));
  return 1;
}

/**
  >i
 ->currentlevelnum:integer
*/
static int mission_getcurrentlevelnum(lua_State *L)
{
  OMission *mission = oluacapi_game->missionmanager->current;

  if (mission == 0)
  {
    lua_pushnil(L);
    lua_pushstring(L, "nil mission");
    return 2;
  }

  lua_pushinteger(L, omission_getcurrentlevelnum(mission));
  return 1;
}

/*---------------------------------------------------------------------------*/

/**
  >iii
 ->width:integer, height:integer, bpp:integer
*/
static int settings_getscreendimensions(lua_State *L)
{
  lua_pushinteger(L, oluacapi_game->settings->screenwidth);
  lua_pushinteger(L, oluacapi_game->settings->screenheight);
  lua_pushinteger(L, oluacapi_game->settings->screenbpp);
  return 3;
}

/*---------------------------------------------------------------------------*/

/**
  nnn>i
  x:number, y:number, z:number -> id:integer
*/
static int level_getentityidat(lua_State *L)
{
  int i, index = -1;
  int len = optrvector_length(oluacapi_game->state->entities);
  OEntity *entity;
  onumber x = luaL_checknumber(L, 1);
  onumber y = luaL_checknumber(L, 2);
  onumber z = luaL_checknumber(L, 3);
  for (i=0; i<len; ++i)
  {
    entity = (OEntity*)optrvector_getelementptrat(oluacapi_game->state->entities, i);
    if (ovector3_eq3(entity->location, x, y, z))
    {
      index = i;
      break;
    }
  }

  if (index > -1 && !oentity_isalive((OEntity*)optrvector_getelementptrat(oluacapi_game->state->entities, index)))
    lua_pushnumber(L, -1);
  else
    lua_pushnumber(L, index);

  return 1;
}

/*---------------------------------------------------------------------------*/

/**
  s>i
  name:string -> id:integer
*/
static int sound_nametoid(lua_State *L)
{
  const ochar *name = luaL_checkstring(L, 1);
  oint32 id = -1;

  if (osettings_issounds(oluacapi_game->settings))
     id = osoundmanager_getidforname(oluacapi_game->audiomanager->soundmanager, name);

  /* TODO error handling */
  lua_pushinteger(L, id);
  return 1;
}

/**
  i>
  id:integer ->
*/
static int sound_play(lua_State *L)
{
  oint32 id = luaL_checkinteger(L, 1);

  if (osettings_issounds(oluacapi_game->settings))
    osoundmanager_play(oluacapi_game->audiomanager->soundmanager, id);

  return 0;
}

/**
  s>i
  name:string -> id:integer
*/
static int music_nametoid(lua_State *L)
{
  const ochar *name = luaL_checkstring(L, 1);
  oint32 id = -1;

  if (osettings_ismusic(oluacapi_game->settings))
    id = omusicmanager_getidforname(oluacapi_game->audiomanager->musicmanager, name);

  /* TODO error handling */
  lua_pushinteger(L, id);
  return 1;
}

/**
  i(i)>
  id:integer, (optloops:integer) ->
*/
static int music_play(lua_State *L)
{
  oint32 id = luaL_checkinteger(L, 1);
  oint32 loops = luaL_optint(L, 2, -1);

  if (osettings_ismusic(oluacapi_game->settings))
    omusicmanager_play(oluacapi_game->audiomanager->musicmanager, id, loops);

  return 0;
}

/**
  (i)>
  (optid:integer) ->
*/
static int music_playrandom(lua_State *L)
{
  oint32 loops = luaL_optint(L, 1, -1);

  if (osettings_ismusic(oluacapi_game->settings))
    omusicmanager_playrandom(oluacapi_game->audiomanager->musicmanager, loops);

  return 0;
}

/**
  >
 ->
*/
static int music_stop(lua_State *L)
{
  if (osettings_ismusic(oluacapi_game->settings))
    omusicmanager_stop(oluacapi_game->audiomanager->musicmanager);

  return 0;
}

/*---------------------------------------------------------------------------*/

/**
  s>i
  name:string -> id:integer
*/
static int texture_nametoid(lua_State *L)
{
  const ochar *name = luaL_checkstring(L, 1);
  oint32 id = otexturemanager_getidforname(oluacapi_game->texturemanager, name);
  /* TODO error handling */
  lua_pushinteger(L, id);
  return 1;
}

/*---------------------------------------------------------------------------*/

/**
  s>i
  name:string -> id:integer
*/
static int mesh_nametoid(lua_State *L)
{
  const ochar *name = luaL_checkstring(L, 1);
  oint32 id = omeshmanager_getidforname(oluacapi_game->meshmanager, name);
  /* TODO error handling */
  lua_pushinteger(L, id);
  return 1;
}

/*------------------------------   Aux --------------------------------------*/
OMission* parsingMission = 0;

/**
  t{sss(s)t{s*}t{s*}}>
  level:table{name:string, password:string, lookAt:string, (postinit:string), lines:table{line:string*}arguments:table{argument:string*}}
*/
static int level(lua_State *L)
{
  /* TODO error handling */
  OLevel* level;
  const ochar* str;
  int i, len = 0;

  luaL_checktype(L, 1, LUA_TTABLE);
  level = omission_pushbacknewlevel(parsingMission);
  lua_getfield(L, 1, "name");
  str = luaL_checkstring(L, -1);
  olevel_setname(level, str);
  lua_getfield(L, 1, "password");
  str = luaL_checkstring(L, -1);
  olevel_setpassword(level, str);
  lua_getfield(L, 1, "lookAt");
  str = luaL_checkstring(L, -1);
  olevel_setlookat(level, str);

  lua_getfield(L, 1, "postinit");
  if (lua_isstring(L, -1))
  {
    str = luaL_checkstring(L, -1);
    olevel_setpostinit(level, str);
  }

  lua_getfield(L, 1, "lines");
  luaL_checktype(L, -1, LUA_TTABLE);
  len = lua_objlen(L, -1);

  for (i=0; i<len; ++i)
  {
    lua_rawgeti(L, -1, i+1);
    str = luaL_checkstring(L, -1);
    olevel_pushbackline(level, str);
    lua_pop(L, 1);
  }

  lua_getfield(L, 1, "attributes");

  if (!lua_istable(L, -1))
    return 0;

  len = lua_objlen(L, -1);

  for (i=0; i<len; ++i)
  {
    lua_rawgeti(L, -1, i+1);
    str = luaL_checkstring(L, -1);
    olevel_pushbackargument(level, str);
    lua_pop(L, 1);
  }

  return 0;
}

/**
  s>
  string:string ->
*/
static int game_dostring(lua_State *L)
{
  const ochar *string = luaL_checkstring(L, 1);
  if (luaL_dostring(L, string) == 1)
  {
    lua_pushnil(L);
    lua_pushstring(L, "cannot run the string");
    return 2;
  }

  return 0;
}

/**
  s>
  name:string ->
*/
static int halloffame_add(lua_State *L)
{
  FILE *fp = 0;
  ochar dirname[] = "resources/halloffame/";
  const ochar *name = luaL_checkstring(L, 1);
  OMission *mission = oluacapi_game->missionmanager->current;
  OString *path = ostring_newstr(odir_convertpath(dirname));
  ostring_concat(path, oluacapi_game->missionmanager->current->name);
  fp = fopen(ostring_tocstr(path), "a"); /*TODO*/

  if (fp == 0)
  {
    lua_pushnil(L);
    lua_pushstring(L, "cannot write hall of fame");
  }

  fprintf(fp, "%s %u\n", name, omission_getdurationinseconds(mission));
  fclose(fp);
  ostring_release(path);
  path = 0;
  return 0;
}

/**
  >t{t{name:s, seconds:i}*}
 ->statistics:table{table{name:string, seconds:integer}*}
*/
static int halloffame_getstatistics(lua_State *L)
{
  FILE *fp = 0;
  ssize_t read;
  char heroname[256]; /* TODO buffer overflow possible with too long inputline */
  ouint32 seconds;
  int i = 0;
  ochar dirname[] = "resources/halloffame/";
  OString *path = ostring_newstr(odir_convertpath(dirname));
  ostring_concat(path, oluacapi_game->missionmanager->current->name);
  memset(heroname, 0, 256);
  fp = fopen(ostring_tocstr(path), "a+"); /*TODO*/

  if (fp == 0)
  {
    lua_pushnil(L);
    lua_pushstring(L, "cannot read hall of fame");
  }

  lua_newtable(L);

  while ((read = fscanf(fp, "%s %u\n", heroname, &seconds)) == 2) {
    lua_newtable(L);
    lua_pushstring(L, "name");
    lua_pushstring(L, heroname);
    lua_settable(L, -3);
    lua_pushstring(L, "seconds");
    lua_pushinteger(L, seconds);
    lua_settable(L, -3);
    lua_rawseti(L,-2,i + 1);
    ++i;
  }

  fclose(fp);
  ostring_release(path);
  path = 0;
  return 1;
}

/**
  >s
 ->directoryseparator:string
*/
static int dir_getdirectoryseparator(lua_State *L)
{
  ochar separator[] = {'\0', '\0'};
  separator[0] = odir_getdirectoryseparator();
  lua_pushlstring(L, separator, 1);
  return 1;
}

void oluacapi_registerfunction(const char *name, lua_CFunction func)
{
  assert(oluacapi_game);
  assert(oluacapi_game->scriptmanager);
  assert(oluacapi_game->scriptmanager->L);
  lua_pushcfunction(oluacapi_game->scriptmanager->L, *func);
  lua_setglobal(oluacapi_game->scriptmanager->L, name);
}

void oluacapi_init(OGame *game)
{
  oluacapi_game = game;
  oluacapi_registerfunction("entity_getlocation",               entity_getlocation);
  oluacapi_registerfunction("entity_setlocation",               entity_setlocation);
  oluacapi_registerfunction("entity_getvelocity",               entity_getvelocity);
  oluacapi_registerfunction("entity_setvelocity",               entity_setvelocity);
  oluacapi_registerfunction("entity_getname",                   entity_getname);
  oluacapi_registerfunction("entity_trunclocation",             entity_trunclocation);
  oluacapi_registerfunction("entity_isstopped",                 entity_isstopped);
  oluacapi_registerfunction("entity_isname",                    entity_isname);
  oluacapi_registerfunction("entity_setalive",                  entity_setalive);
  oluacapi_registerfunction("entity_seteventable",              entity_seteventable);
  oluacapi_registerfunction("entity_setupdatable",              entity_setupdatable);
  oluacapi_registerfunction("entity_setrenderable",             entity_setrenderable);
  oluacapi_registerfunction("entity_setpostrenderable",         entity_setpostrenderable);
  oluacapi_registerfunction("entity_setcollidable",             entity_setcollidable);
  oluacapi_registerfunction("entity_setmovable",                entity_setmovable);
  oluacapi_registerfunction("entity_ismovable",                 entity_ismovable);
  oluacapi_registerfunction("entity_settexture",                entity_settexture);
  oluacapi_registerfunction("entity_setmesh",                   entity_setmesh);
  oluacapi_registerfunction("entity_getrotation",               entity_getrotation);
  oluacapi_registerfunction("entity_setrotation",               entity_setrotation);
  oluacapi_registerfunction("entity_hascollision",              entity_hascollision);
  oluacapi_registerfunction("entity_onwhichsideisother",        entity_onwhichsideisother);
  oluacapi_registerfunction("entity_setstate",                  entity_setstate);
  oluacapi_registerfunction("entity_getstate",                  entity_getstate);
  oluacapi_registerfunction("entity_getdistancesquared",        entity_getdistancesquared);
  oluacapi_registerfunction("entity_getextentsquared",          entity_getextentsquared );

  oluacapi_registerfunction("state_addentity",                  state_addentity);
  oluacapi_registerfunction("state_setstate",                   state_setstate);
  oluacapi_registerfunction("state_setupdatable",               state_setupdatable);
  oluacapi_registerfunction("state_seteventable",               state_seteventable);
  oluacapi_registerfunction("state_setrenderable",              state_setrenderable);
  oluacapi_registerfunction("state_messagestackpush",           state_messagestackpush);

  oluacapi_registerfunction("opengl_rotate",                    opengl_rotate);
  oluacapi_registerfunction("opengl_translate",                 opengl_translate);
  oluacapi_registerfunction("opengl_scale",                     opengl_scale);
  oluacapi_registerfunction("opengl_loadidentity",              opengl_loadidentity);
  oluacapi_registerfunction("opengl_color3",                    opengl_color3);
  oluacapi_registerfunction("opengl_normal3",                   opengl_normal3);
  oluacapi_registerfunction("opengl_texcoord2",                 opengl_texcoord2);
  oluacapi_registerfunction("opengl_vertex3",                   opengl_vertex3);
  oluacapi_registerfunction("opengl_lookat",                    opengl_lookat);
  oluacapi_registerfunction("opengl_render",                    opengl_render);
  oluacapi_registerfunction("opengl_enable2dtexturing",         opengl_enable2dtexturing);
  oluacapi_registerfunction("opengl_disable2dtexturing",        opengl_disable2dtexturing);
  oluacapi_registerfunction("opengl_bindtexture2d",             opengl_bindtexture2d);
  oluacapi_registerfunction("opengl_enablelighting",            opengl_enablelighting);
  oluacapi_registerfunction("opengl_disablelighting",           opengl_disablelighting);
  oluacapi_registerfunction("opengl_enableblend",               opengl_enableblend);
  oluacapi_registerfunction("opengl_disableblend",              opengl_disableblend);
  oluacapi_registerfunction("opengl_pushmatrix",                opengl_pushmatrix);
  oluacapi_registerfunction("opengl_popmatrix",                 opengl_popmatrix);
  oluacapi_registerfunction("opengl_pushenablebitattrib",       opengl_pushenablebitattrib);
  oluacapi_registerfunction("opengl_popattrib",                 opengl_popattrib);
  oluacapi_registerfunction("opengl_newquadric",                opengl_newquadric);
  oluacapi_registerfunction("opengl_disk",                      opengl_disk);

  oluacapi_registerfunction("text_printortho",                  text_printortho);
  oluacapi_registerfunction("text_printperspective",            text_printperspective);

  oluacapi_registerfunction("background_render",                background_render);
  oluacapi_registerfunction("background_renderwithsizehints",   background_renderwithsizehints);
  oluacapi_registerfunction("background_renderwithsizes",       background_renderwithsizes);

  oluacapi_registerfunction("game_quit",                        game_quit);
  oluacapi_registerfunction("game_getmissionnames",             game_getmissionnames);
  oluacapi_registerfunction("game_changestate",                 game_changestate);
  oluacapi_registerfunction("game_parsemission",                game_parsemission);
  oluacapi_registerfunction("game_printmission",                game_printmission);
  oluacapi_registerfunction("game_getlevelwithpassword",        game_getlevelwithpassword);
  oluacapi_registerfunction("game_getcurrentlevel",             game_getcurrentlevel);
  oluacapi_registerfunction("game_haslevelwithpassword",        game_haslevelwithpassword);
  oluacapi_registerfunction("game_setcurrentlevelfrompassword", game_setcurrentlevelfrompassword);
  oluacapi_registerfunction("game_entitytranslatesymboltoname", game_entitytranslatesymboltoname);
  oluacapi_registerfunction("game_entityregistersymbolforname", game_entityregistersymbolforname);
  oluacapi_registerfunction("game_getreleasestr",               game_getreleasestr);
  oluacapi_registerfunction("mission_hasnextlevel",             mission_hasnextlevel);
  oluacapi_registerfunction("mission_nextlevel",                mission_nextlevel);
  oluacapi_registerfunction("mission_reset",                    mission_reset);
  oluacapi_registerfunction("mission_resettimer",               mission_resettimer);
  oluacapi_registerfunction("mission_stoptimer",                mission_stoptimer);
  oluacapi_registerfunction("mission_getnumlevels",             mission_getnumlevels);
  oluacapi_registerfunction("mission_getnumpassed",             mission_getnumpassed);
  oluacapi_registerfunction("mission_getcurrentlevelnum",       mission_getcurrentlevelnum);

  oluacapi_registerfunction("settings_getscreendimensions",     settings_getscreendimensions);

  oluacapi_registerfunction("level_getentityidat",              level_getentityidat);

  oluacapi_registerfunction("sound_nametoid",                   sound_nametoid);
  oluacapi_registerfunction("sound_play",                       sound_play);
  oluacapi_registerfunction("music_nametoid",                   music_nametoid);
  oluacapi_registerfunction("music_play",                       music_play);
  oluacapi_registerfunction("music_playrandom",                 music_playrandom);
  oluacapi_registerfunction("music_stop",                       music_stop);

  oluacapi_registerfunction("texture_nametoid",                 texture_nametoid);

  oluacapi_registerfunction("mesh_nametoid",                    mesh_nametoid);

  oluacapi_registerfunction("level",                            level);
  oluacapi_registerfunction("game_dostring",                    game_dostring);

  oluacapi_registerfunction("halloffame_add",                   halloffame_add);
  oluacapi_registerfunction("halloffame_getstatistics",         halloffame_getstatistics);

  oluacapi_registerfunction("dir_getdirectoryseparator",        dir_getdirectoryseparator);
}
