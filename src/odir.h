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
#ifndef ODIR_H
#define ODIR_H

#include "otypes.h"
#include <stdio.h>

#ifndef NAME_MAX
#define NAME_MAX 1024 /* TODO */
#endif

#define ODIR_EXEDIRNAMEMAXLEN 2048

typedef void (*ODirEntryCallback)(const ochar *path, const ochar *filename, void *userdata);

size_t  odir_numentries            (const ochar *directoryname);
void    odir_foreachentry          (const ochar *directoryname, ODirEntryCallback callback, void *userdata);
ochar  *odir_convertpath           (ochar *path);
ochar   odir_getdirectoryseparator (void);
void    odir_buildexedirname       (void);
ochar  *odir_getexedirname         (void);

#endif
