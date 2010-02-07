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
#include "otypes.h"
#include "oerror.h"
#include <stdlib.h>

static const ochar *oversion_releasestr = "crates 0.3.3";

void oversion_printinfo()
{
  oerror_info("%s", oversion_releasestr);
}

void oversion_checkargs(oint32 *argcptr, ochar *argv[])
{
  if (*argcptr > 1 && ((strcmp("-v", argv[1]) == 0) || (strcmp("--version", argv[1]) == 0)))
  {
    oversion_printinfo();
    (*argcptr)--;
    argv++;
    exit(0);
  }
}

const ochar *oversion_getreleasestr(void)
{
  return oversion_releasestr;
}

