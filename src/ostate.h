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
#ifndef OSTATE_H
#define OSTATE_H

#include "oentity.h"
#include "ovector.h"
#include "oscriptmanager.h"
#include "ostring.h"
#include "otypes.h"
#include "oevent.h"
#include "omessagestack.h"
#include <stdio.h>

typedef struct {
  OPtrVector     *entities;
  OPtrVector     *tobeadded;
  OString        *name;
  OString        *state;
  oboolean        updatable;
  oboolean        eventable;
  oboolean        renderable;
  OString        *argument;
  OMessageStack  *messagestack;
  OScriptManager *scriptmanager;
} OState;

OState      *ostate_new                     (const ochar *name, const ochar *argument, OTextManager *textmanager, OScriptManager *scriptmanager);
void         ostate_callluainit             (OState *state);
void         ostate_release                 (OState *state);
oint32       ostate_addentity               (OState *state, const ochar *name, const ochar *initattributes, const onumber x, const onumber y, const onumber z);
OEntity     *ostate_getentityptrwithid      (OState *state, const oint32 id);
OEntity     *ostate_getentityptrwithiderror (OState *state, const oint32 id);
void         ostate_events                  (OState *state, const enum OEvent event);
void         ostate_update                  (OState *state, const onumber tpf);
void         ostate_move                    (OState *state, const onumber tpf);
void         ostate_collide                 (OState *state);
void         ostate_render                  (OState *state);
void         ostate_setupdatable            (OState *state, const oboolean updatable);
void         ostate_seteventable            (OState *state, const oboolean eventable);
void         ostate_setrenderable           (OState *state, const oboolean renderable);
void         ostate_setstatestr             (OState *state, const ochar *characters);
const ochar *ostate_statetocstr             (const OState *state);
void         ostate_messagestackpush        (OState *state, const ochar *message);

#endif
