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
#include "ostring.h"
#include "oerror.h"
#include <string.h>
#include <ctype.h>

static ochar ostring_nullchar = '\0';

/* This function MUST be called. Otherwise the null-character must
  be stored in the other function! */
OString *ostring_newstr(const ochar *characters)
{
  size_t len = strlen(characters);
  OString *string = (OString*)oerror_malloc(sizeof(OString));
  string->characters = ovector_new(sizeof(ochar), len + 1, 0);  /* +1 for null */
  ovector_pushback(string->characters, &ostring_nullchar);
  ostring_concatstr(string, characters);
  return string;
}

OString *ostring_newstrn(const ochar *characters, const ouint32 max)
{
  ouint32 i;
  size_t len = strlen(characters);
  OString *string = (OString*)oerror_malloc(sizeof(OString));

  /*TODO: max-function*/
  if (max < len)
    len = max;

  string->characters = ovector_new(sizeof(ochar), 0, 0);  /* +1 for null */
  for (i = 0; i <len; ++i)
    ovector_pushback(string->characters, characters + i);

  ovector_pushback(string->characters, &ostring_nullchar);
  return string;
}

INLINE const ochar *ostring_tocstr(const OString *string)
{
  return (const ochar*)ovector_toptr(string->characters);
}

OString *ostring_new(const OString *other)
{
  return ostring_newstr(ostring_tocstr(other));
}

void ostring_release(OString *string)
{
  if (!string)
    return;

  ovector_release(string->characters);
  memset(string, 0, sizeof(OString));
  oerror_free(string);
}

INLINE void ostring_pushback(OString *string, const ochar character)
{
  ovector_popback(string->characters, 0); /* remove null */
  ovector_pushback(string->characters, &character);
  ovector_pushback(string->characters, &ostring_nullchar);
}

INLINE void ostring_popback(OString *string, ochar *character)
{
  if (ovector_length(string->characters) <= 1) /*do not remove null*/
    return;

  ovector_popback(string->characters, 0); /* remove null */
  ovector_popback(string->characters, character);
  ovector_pushback(string->characters, &ostring_nullchar);
}

void ostring_getcharacterat(const OString *string, const size_t index, ochar *character)
{
  ovector_copyelementat(string->characters, index, character);
}

void ostring_setcharacterat(OString *string, ochar character, const size_t index)
{
  ovector_insertat(string->characters, &character, index);
}

void ostring_removecharacterat(OString *string, const size_t index)
{
  ovector_removeelementat(string->characters, index);
}

INLINE oint32 ostring_comparestr(const OString *string, const ochar *str)
{
  return strcmp(ostring_tocstr(string), str);
}

INLINE oint32 ostring_compare(const OString *string, const OString *other)
{
  return ostring_comparestr(string, ostring_tocstr(other));
}

oint32 ostring_casecompare(const OString *string, const OString *other)
{
  return ostring_casecomparestr(string, ostring_tocstr(other));
}

oint32 ostring_casecomparestr(const OString *string, const ochar *str)
{
  /* TODO better implementation? Check that implementation is correct. */
  int i;
  oint32 a, b;
  int len1 = ostring_length(string);
  int len2 = strlen(str);
  const ochar* str1 = ostring_tocstr(string);

  if (len1 < len2)
    return -1;

  if (len1 > len2)
    return 1;

  for (i = 0; i < len1; ++i)
  {
    a = tolower(str1[i]);
    b = tolower(str[i]);

    if ( a != b)
      return a - b;
  }

  return 0;
}

void ostring_tolower(OString *string)
{
  int i;
  int len = ostring_length(string);
  const ochar* str = ostring_tocstr(string);

  for (i = 0; i < len; ++i)
    ostring_setcharacterat(string, tolower(str[i]), i);
}

void ostring_toupper(OString *string)
{
  int i;
  int len = ostring_length(string);
  const ochar* str = ostring_tocstr(string);

  for (i = 0; i < len; ++i)
    ostring_setcharacterat(string, toupper(str[i]), i);
}

void ostring_concat(OString *string, const OString *other)
{
  ostring_concatstr(string, ostring_tocstr(other));
}

void ostring_concatstr(OString *string, const ochar *characters)
{
  size_t i;
  size_t len = strlen(characters);

  ovector_popback(string->characters, 0); /* remove null */

  /*TODO: write this better*/
  for (i = 0; i < len; ++i)
    ovector_pushback(string->characters, (void*)(characters + i));

  ovector_pushback(string->characters, &ostring_nullchar);
}

void ostring_print(const OString *string)
{
  printf("%s", ostring_tocstr(string));
}

size_t ostring_length(const OString *string)
{
  /* TODO */
  /*return o_vector_length(&string->characters) - 1; *//*-1 for null*/
  return strlen((ochar*)ovector_toptr(string->characters));
}

void ostring_setstr(OString *string, const ochar *characters)
{
  /* TODO: write this better (faster) */
  int i;
  int len = strlen(characters);
  for (i = 0; i < len; ++i)
    ostring_setcharacterat(string, characters[i], i);

  ostring_setcharacterat(string, ostring_nullchar, i);

  /* TODO */
  string->characters->numelements = len + 1;
}
