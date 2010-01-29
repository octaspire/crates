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
#include <sys/sysctl.h>

extern ochar  odir_exedirname[ODIR_EXEDIRNAMEMAXLEN];
extern ochar* odir_basename;
extern oint32 odir_basenamemaxlen;

/* The idea behind this function is based on public domain code written by Nicolai Haehnle.
   The original code was found from http://www.flipcode.org/archives/getexename.c
   This is not the original version, but modified one. */
void odir_buildexedirname(void)
{
  int mib[4];
  int ret = 0;
  size_t buflen = ODIR_EXEDIRNAMEMAXLEN;
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_PATHNAME;
  mib[3] = -1;
  
  ret = sysctl(mib, 4, odir_exedirname, &buflen, 0, 0);
  
  if (ret != 0)
    oerror_fatal("cannot build exedirname; sysctl failed:");

  odir_basename = strrchr(odir_exedirname, odir_getdirectoryseparator());
  
  if (odir_basename && odir_basename != (odir_exedirname + (ODIR_EXEDIRNAMEMAXLEN - 1)))
  {
    odir_basename++;
    *odir_basename = '\0';
    odir_basenamemaxlen = (odir_exedirname + (ODIR_EXEDIRNAMEMAXLEN - 1)) - odir_basename;
  }
}
