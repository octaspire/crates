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
#ifndef OLEVEL_H
#define OLEVEL_H

#include "optrvector.h"
#include "ostring.h"
#include "otypes.h"
#include <stdio.h>

typedef struct {
  OString    *name;
  OString    *password;
  OString    *lookat;
  OString    *postinit;
  OPtrVector *lines;
  OPtrVector *arguments;
} OLevel;

OLevel       *olevel_new              (void);
void          olevel_release          (OLevel *level);
ouint32       olevel_getnumarguments  (const OLevel *level);
const ochar  *olevel_getargumentat    (const OLevel *level, const ouint32 index);
void          olevel_fprintf          (OLevel *level, FILE *out);
void          olevel_setname          (OLevel *level, const ochar *name);
void          olevel_setpassword      (OLevel *level, const ochar *password);
void          olevel_setlookat        (OLevel *level, const ochar *lookat);
void          olevel_setpostinit      (OLevel *level, const ochar *postinit);
void          olevel_pushbackline     (OLevel *level, const ochar *line);
void          olevel_pushbackargument (OLevel *level, const ochar *argument);

#endif
