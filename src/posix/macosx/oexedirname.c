/*
  Crates - 3D puzzle game.
  Copyright (C) 2008-2010  Octaspire games

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
#include "../../odir.h"
#include "../../oerror.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <mach-o/dyld.h>

extern ochar  odir_exedirname[ODIR_EXEDIRNAMEMAXLEN];
extern ochar* odir_basename;
extern oint32 odir_basenamemaxlen;

void odir_buildexedirname(void)
{
  oint32 retval;
  ouint32 size = sizeof(odir_exedirname);

  if ((retval = _NSGetExecutablePath(odir_exedirname, &size)) == 0)
  {
    odir_basename = strrchr(odir_exedirname, odir_getdirectoryseparator());
    odir_basename = strrchr(odir_exedirname, odir_getdirectoryseparator());

    if (odir_basename && odir_basename != (odir_exedirname + (ODIR_EXEDIRNAMEMAXLEN - 1)))
    {
      odir_basename++;
      *odir_basename = '\0';
      odir_basenamemaxlen = (odir_exedirname + (ODIR_EXEDIRNAMEMAXLEN - 1)) - odir_basename;
    }
  }
  else
    oerror_fatal("odir_buildexedirname: _NSGetExecutablePath failed with return value %d", retval);
}
