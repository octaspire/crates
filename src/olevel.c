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
#include "olevel.h"
#include "oerror.h"
#include <string.h>

OLevel *olevel_new(void)
{
  OLevel *level = (OLevel*)oerror_malloc(sizeof(OLevel));
  level->lines = optrvector_new(0, (OPtrVectorElementCallback)ostring_release);
  level->arguments = optrvector_new(0, (OPtrVectorElementCallback)ostring_release);
  level->name = 0;
  level->password = 0;
  level->lookat = 0;
  level->postinit = 0;
  return level;
}

void olevel_release(OLevel *level)
{
  if (!level)
    return;

  ostring_release(level->name);
  ostring_release(level->password);
  ostring_release(level->lookat);
  ostring_release(level->postinit);
  optrvector_release(level->lines);
  optrvector_release(level->arguments);
  memset(level, 0, sizeof(OLevel));
  oerror_free(level);
}

const ouint32 olevel_getnumarguments(const OLevel *level)
{
  return optrvector_length(level->arguments);
}

const ochar *olevel_getargumentat(const OLevel *level, const ouint32 index)
{
  OString *s = (OString*)optrvector_getelementptrat(level->arguments, index);

  if (s)
    return ostring_tocstr(s);

  return 0;
}

void olevel_fprintf(OLevel *level, FILE *out)
{
  ouint32 i;
  size_t len = optrvector_length(level->lines);

  fprintf(out, "%s\n", ostring_tocstr(level->name));
  fprintf(out, "%s\n", ostring_tocstr(level->password));

  for (i = 0; i < len; ++i)
    fprintf(out, "line \"%s\"\n", ostring_tocstr((OString*)optrvector_getelementptrat(level->lines, i)));

  len = optrvector_length(level->arguments);
  for (i = 0; i < len; ++i)
    fprintf(out, "argument \"%s\"\n", ostring_tocstr((OString*)optrvector_getelementptrat(level->arguments, i)));
}

void olevel_setname(OLevel *level, const ochar *name)
{
  /* TODO multiple init and problems possible in setname, setpassword, setlookat */
  ostring_release(level->name);
  level->name = ostring_newstr(name);
}

void olevel_setpassword(OLevel *level, const ochar *password)
{
  ostring_release(level->password);
  level->password = ostring_newstr(password);
  ostring_toupper(level->password);
}

void olevel_setlookat(OLevel *level, const ochar *lookat)
{
  ostring_release(level->lookat);
  level->lookat = ostring_newstr(lookat);
}

void olevel_setpostinit(OLevel *level, const ochar *postinit)
{
  ostring_release(level->postinit);
  level->postinit = ostring_newstr(postinit);
}

void olevel_pushbackline(OLevel *level, const ochar *line)
{
  optrvector_pushback(level->lines, ostring_newstr(line));
}

void olevel_pushbackargument (OLevel *level, const ochar *argument)
{
  optrvector_pushback(level->arguments, ostring_newstr(argument));
}
