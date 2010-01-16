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
#ifndef OSTRING_H
#define OSTRING_H

#include "otypes.h"
#include "ovector.h"
#include <stdio.h>

typedef struct {
  OVector  *characters;
} OString;

OString     *ostring_newstr            (const ochar *characters);
OString     *ostring_newstrn           (const ochar *characters, const ouint32 max);
OString     *ostring_new               (const OString *other);
void         ostring_release           (OString *string);
void         ostring_pushback          (OString *string, const ochar character);
void         ostring_popback           (OString *string, ochar *character);
void         ostring_getcharacterat    (const OString *string, const size_t index, ochar *character);
void         ostring_setcharacterat    (OString *string, ochar character, const size_t index);
void         ostring_removecharacterat (OString *string, const size_t index);
const ochar *ostring_tocstr            (const OString *string);
oint32       ostring_compare           (const OString *string, const OString *other);
oint32       ostring_comparestr        (const OString *string, const ochar *str);
oint32       ostring_casecompare       (const OString *string, const OString *other);
oint32       ostring_casecomparestr    (const OString *string, const ochar *str);
void         ostring_tolower           (OString *string);
void         ostring_toupper           (OString *string);
void         ostring_concat            (OString *string, const OString *other);
void         ostring_concatstr         (OString *string, const ochar *characters);
void         ostring_print             (const OString *string);
size_t       ostring_length            (const OString *string);
void         ostring_setstr            (OString *string, const ochar *characters);

#endif
