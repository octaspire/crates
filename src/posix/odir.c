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
#include "../oerror.h"
#include "../ostring.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

ochar odir_directoryseparator = '/';
#define ODIR_EXEDIRNAMEMAXLEN 2048
static ochar  odir_exedirname[ODIR_EXEDIRNAMEMAXLEN];
static ochar* odir_basename = 0;
static oint32 odir_basenamemaxlen = 0;

size_t odir_numentries(const ochar *directoryname)
{
  struct dirent *entry;
  size_t count = 0;
  DIR *dir = opendir(directoryname);

  if (dir)
  {
    while ((entry=readdir(dir)))
    {
      if (strncmp(entry->d_name, ".", NAME_MAX) != 0 && strncmp(entry->d_name, "..", NAME_MAX) != 0)
        ++count;
    }

    closedir(dir);
  }

  return count;
}

void odir_foreachentry(const ochar *directoryname, ODirEntryCallback callback, void *userdata)
{
  struct dirent *entry;
  DIR *dir = opendir(directoryname);
  OString *tmp = ostring_newstr("");

  if (dir)
  {
    while ((entry=readdir(dir)))
    {
      if (strncmp(entry->d_name, ".", NAME_MAX) != 0 && strncmp(entry->d_name, "..", NAME_MAX) != 0)
      {
        ostring_setstr(tmp, "");
        ostring_concatstr(tmp, directoryname);
        ostring_pushback(tmp, odir_directoryseparator);
        ostring_concatstr(tmp, entry->d_name);
        callback(ostring_tocstr(tmp), entry->d_name, userdata);
      }
    }

    closedir(dir);
  }

  ostring_release(tmp);
}

ochar* odir_convertpath(ochar *path)
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

/* The idea behind this function is based on public domain code written by Nicolai Haehnle.
   The original code was found from http://www.flipcode.org/archives/getexename.c
   This is not the original version, but modified one. */
void odir_buildexedirname(void)
{
  ochar linkname[64];
  oint32 ret;
  pid_t pid = getpid();

  if (snprintf(linkname, sizeof(linkname), "/proc/%i/exe", pid) < 0)
    oerror_fatal("cannot build exedirname; building linkname for pid %d failed", pid);

  ret = readlink(linkname, odir_exedirname, ODIR_EXEDIRNAMEMAXLEN);

  if (ret == -1)
    oerror_fatal("cannot build exedirname; readlink failed:");

  if (ret >= ODIR_EXEDIRNAMEMAXLEN)
    oerror_fatal("cannot build exedirname; readlink failed because buffer is too short");

  odir_exedirname[ret] = '\0';
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
