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
#include "oscriptmanager.h"
#include "oerror.h"
#include "ostring.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

OScriptManager *oscriptmanager_new(void)
{
  OScriptManager *scriptmanager = (OScriptManager*)oerror_malloc(sizeof(OScriptManager));
  scriptmanager->loadednames = optrvector_new(0, (OPtrVectorElementCallback)ostring_release);
  scriptmanager->L = luaL_newstate();
  luaL_openlibs(scriptmanager->L);
  return scriptmanager;
}

void oscriptmanager_release(OScriptManager *scriptmanager)
{
  if (!scriptmanager)
    return;

  lua_close(scriptmanager->L);
  scriptmanager->L = 0;
  optrvector_release(scriptmanager->loadednames);
  oerror_free(scriptmanager);
}

void oscriptmanager_loadscript(OScriptManager *scriptmanager, const ochar *scriptname)
{
  if (optrvector_find(scriptmanager->loadednames, scriptname, (OPtrVectorElementCompareCallback)ostring_comparestr) > -1)
    return;

  if (luaL_loadfile(scriptmanager->L, scriptname) || lua_pcall(scriptmanager->L, 0, 0, 0))
  {
    oerror_fatal("cannot load or run Lua-script \"%s\" (%s)", scriptname, lua_tostring(scriptmanager->L, -1));
    return;
  }

  optrvector_pushback(scriptmanager->loadednames, ostring_newstr(scriptname));
}

/* This is inspired from PIL and http://lua-users.org/lists/lua-l/2007-01/msg00418.html */
void oscriptmanager_calltablefunctionva(OScriptManager *scriptmanager, const ochar *namespacetable, const ochar *func, const ochar *sig, ...)
{
  va_list vl;
  int narg, nres;
  int tableindex = 0;
  lua_State *L = scriptmanager->L;

  va_start(vl, sig);
  lua_getglobal(L, namespacetable); /* Get the table that represents the name space */
  tableindex = lua_gettop(L);
  lua_getfield(L, tableindex, func);

  /* Push arguments */

  for (narg = 0; *sig; narg++)
  {
    luaL_checkstack(L, 1, "too many arguments");

    switch (*sig++)
    {
      case 'd':
        lua_pushnumber(L, va_arg(vl, double));
        break;

      case 'i':
        lua_pushinteger(L, va_arg(vl, int));
        break;

      case 's':
        lua_pushstring(L, va_arg(vl, ochar*));
        break;

      case '>':
        goto endargs;

      default:
        oerror_fatal("invalid option (%c)", *(sig - 1));
    }
  }

endargs:

  nres = strlen(sig);

  if (lua_pcall(L, narg, nres, 0) != 0)
    oerror_fatal("error calling '%s': '%s'", func, lua_tostring(L, -1));

  /* Retrieve results */

  nres = -nres;

  while (*sig)
  {
    switch(*sig++)
    {
      case 'd':
        if (!lua_isnumber(L, nres))
          oerror_fatal("wrong result type");
        *va_arg(vl, double*) = lua_tonumber(L, nres);
        break;

      case 'i':
        if (!lua_isnumber(L, nres))
          oerror_fatal("wrong result type");
        *va_arg(vl, int*) = lua_tointeger(L, nres);
        break;

      case 's':
        if (!lua_isstring(L, nres))
          oerror_fatal("wrong result type");
        *va_arg(vl, const ochar**) = lua_tostring(L, nres);
        break;

      default:
        oerror_fatal("invalid option (%c)", *(sig - 1));
    }
  }

  va_end(vl);
  lua_settop(L, (tableindex - 1));
}


/* This is inspired from PIL and http://lua-users.org/lists/lua-l/2007-01/msg00418.html */
void oscriptmanager_calltabletablefunctionva(OScriptManager *scriptmanager, const ochar *namespacetable, const ochar *state, const ochar *func, const ochar *sig, ...)
{
  va_list vl;
  int narg, nres;
  int tableindex1 = 0;
  int tableindex2 = 0;
  lua_State *L = scriptmanager->L;

  va_start(vl, sig);
  lua_getglobal(L, namespacetable); /* Get the table that represents the name space */
  tableindex1 = lua_gettop(L);
  lua_getfield(L, tableindex1, state); /* state table */
  tableindex2 = lua_gettop(L);
  lua_getfield(L, tableindex2, func);

  /* Push arguments */

  for (narg = 0; *sig; narg++)
  {
    luaL_checkstack(L, 1, "too many arguments");

    switch (*sig++)
    {
      case 'd':
        lua_pushnumber(L, va_arg(vl, double));
        break;

      case 'i':
        lua_pushinteger(L, va_arg(vl, int));
        break;

      case 's':
        lua_pushstring(L, va_arg(vl, ochar*));
        break;

      case '>':
        goto endargs;

      default:
        oerror_fatal("invalid option (%c)", *(sig - 1));
    }
  }

endargs:

  nres = strlen(sig);

  if (lua_pcall(L, narg, nres, 0) != 0)
    oerror_fatal("error calling '%s': '%s'", func, lua_tostring(L, -1));

  /* Retrieve results */

  nres = -nres;

  while (*sig)
  {
    switch(*sig++)
    {
      case 'd':
        if (!lua_isnumber(L, nres))
          oerror_fatal("wrong result type");
        *va_arg(vl, double*) = lua_tonumber(L, nres);
        break;

      case 'i':
        if (!lua_isnumber(L, nres))
          oerror_fatal("wrong result type");
        *va_arg(vl, int*) = lua_tointeger(L, nres);
        break;

      case 's':
        if (!lua_isstring(L, nres))
          oerror_fatal("wrong result type");
        *va_arg(vl, const ochar**) = lua_tostring(L, nres);
        break;

      default:
        oerror_fatal("invalid option (%c)", *(sig - 1));
    }
  }

  va_end(vl);
  /* TODO: is this correct after adding the second inner table? */
  lua_settop(L, (tableindex1 - 1));
}

void oscriptmanager_gettablefieldva(OScriptManager *scriptmanager, const ochar *namespacetable, const ochar *fieldname, const ochar sig, void *value, oboolean warnings)
{
  int tableindex = 0;
  lua_State *L = scriptmanager->L;
  lua_getglobal(L, namespacetable); /* Get the table that represents the name space */

  if (!lua_istable(L, -1))
    oerror_fatal("%s is not a table", namespacetable);

  tableindex = lua_gettop(L);
  lua_getfield(L, tableindex, fieldname);

  switch(sig)
  {
    case 'd':
      if (lua_isnumber(L, -1))
        *((double*)value) = lua_tonumber(L, -1);
      else if (warnings)
        oerror_warning("Field \"%s\" in table \"%s\" is not number", fieldname, namespacetable);
      break;

    case 'i':
      if (lua_isnumber(L, -1))
        *((int*)value) = lua_tointeger(L, -1);
      else if (warnings)
        oerror_warning("Field \"%s\" in table \"%s\" is not integer", fieldname, namespacetable);
      break;

    case 's':
      if (lua_isstring(L, -1))
        *((const ochar**)value) = lua_tostring(L, -1);
      else if (warnings)
        oerror_warning("Field \"%s\" in table \"%s\" is not string", fieldname, namespacetable);
      break;

    case 'b':
      if (lua_isboolean(L, -1))
        *((oboolean*)value) = lua_toboolean(L, -1);
      else if (warnings)
        oerror_warning("Field \"%s\" in table \"%s\" is not boolean", fieldname, namespacetable);
      break;

    default:
      oerror_fatal("invalid option (%c)", sig);
  }

  lua_settop(L, (tableindex - 1));
}
