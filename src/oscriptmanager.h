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
#ifndef OSCRIPTMANAGER_H
#define OSCRIPTMANAGER_H

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "otypes.h"
#include "optrvector.h"
#include <stdio.h>

typedef struct {
  lua_State  *L;
  OPtrVector *loadednames;
} OScriptManager;

OScriptManager *oscriptmanager_new                      (void);
void            oscriptmanager_release                  (OScriptManager *scriptmanager);
void            oscriptmanager_loadscript               (OScriptManager *scriptmanager, const ochar *scriptname);
void            oscriptmanager_calltablefunctionva      (OScriptManager *scriptmanager, const ochar *namespacetable, const ochar *func, const ochar *sig, ...);
void            oscriptmanager_calltabletablefunctionva (OScriptManager *scriptmanager, const ochar *namespacetable, const ochar *state, const ochar *func, const ochar *sig, ...);
void            oscriptmanager_gettablefieldva          (OScriptManager *scriptmanager, const ochar *namespacetable, const ochar *fieldname, const ochar sig, void *value, oboolean warnings);

#endif
