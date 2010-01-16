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
#include "../odir.h"
#include "../ostring.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

ochar odir_directoryseparator = '\\';
#define ODIR_EXEDIRNAMEMAXLEN 2048
static ochar  odir_exedirname[ODIR_EXEDIRNAMEMAXLEN]; /* more than MAX_PATH + 1 characters */
static ochar* odir_basename = 0;
static oint32 odir_basenamemaxlen = 0;

size_t odir_numentries(const ochar *directoryname)
{
  WIN32_FIND_DATA info;
  size_t count = 0;
  HANDLE h = FindFirstFile(directoryname, &info);

  do
  {
    if (strcmp(info.cFileName, ".") != 0 && strcmp(info.cFileName, "..") != 0)
      ++count;
  }
  while (FindNextFile(h, &info));

  FindClose(h);
  return count;
}

void odir_foreachentry(const ochar *directoryname, ODirEntryCallback callback, void *userdata)
{
  WIN32_FIND_DATA info;
  HANDLE h;
  OString *tmp = ostring_newstr(directoryname);
  ostring_concatstr(tmp, "\\*.*");
  h = FindFirstFile(ostring_tocstr(tmp), &info);

  do
  {
    if (strcmp(info.cFileName, ".") != 0 && strcmp(info.cFileName, "..") != 0)
    {
      ostring_setstr(tmp, "");
      ostring_concatstr(tmp, directoryname);
      ostring_pushback(tmp, odir_directoryseparator);
      ostring_concatstr(tmp, info.cFileName);
      callback(ostring_tocstr(tmp), info.cFileName, userdata);
    }
  }
  while (FindNextFile(h, &info));

  FindClose(h);
  ostring_release(tmp);
}

ochar *odir_convertpath(ochar *path)
{
  oint32 i;
  oint32 len = strlen(path);

  for (i = 0; i < len; ++i)
  {
    if (path[i] == '/')
      path[i] = odir_directoryseparator;
  }

  if (strlen(path) >= odir_basenamemaxlen)
    oerror_fatal("path \"%s\" is too long to be used as basename", path);

  strcpy(odir_basename, path);
  return odir_exedirname;
}

ochar odir_getdirectoryseparator()
{
  return odir_directoryseparator;
}

void odir_buildexedirname(void)
{
  if (GetModuleFileName(0, odir_exedirname, ODIR_EXEDIRNAMEMAXLEN) == 0)
    oerror_fatal("cannot build exedirname; GetModuleFileName failed with error code %d", GetLastError());

  odir_exedirname[ODIR_EXEDIRNAMEMAXLEN - 1] = '\0';
  odir_basename = strrchr(odir_exedirname, odir_getdirectoryseparator());

  if (odir_basename && odir_basename != (odir_exedirname + (ODIR_EXEDIRNAMEMAXLEN - 1)))
  {
    odir_basename++;
    *odir_basename = '\0';
    odir_basenamemaxlen = (odir_exedirname + (ODIR_EXEDIRNAMEMAXLEN - 1)) - odir_basename;
  }
}

ochar *odir_getexedirname(void)
{
  *odir_basename = '\0';
  return odir_exedirname;
}
