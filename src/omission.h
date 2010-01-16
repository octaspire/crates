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
#ifndef OMISSION_H
#define OMISSION_H

#include "olevel.h"
#include "ovector.h"
#include "ostring.h"
#include "otypes.h"
#include <stdio.h>

typedef struct {
  OString    *name;
  OString    *path;
  OPtrVector *levels;
  ouint32    current;
  ouint32    numpassed;
  ouint32    starttime;
  ouint32    endtime;
} OMission;

OMission *omission_new                         (const ochar *name, const ochar *path);
void      omission_release                     (OMission *mission);
void      omission_reset                       (OMission *mission);
void      omission_resettimer                  (OMission *mission);
void      omission_stoptimer                   (OMission *mission);
ouint32   omission_getnumlevels                (const OMission *mission);
ouint32   omission_getnumpassed                (const OMission *mission);
ouint32   omission_getcurrentlevelnum          (const OMission *mission);
void      omission_setcurrentlevelnum          (OMission *mission, const ouint32 num);
OLevel   *omission_getlevelptrat               (OMission *mission, const ouint32 index);
OLevel   *omission_getlevelwithpassword        (OMission *mission, const ochar *password);
OLevel   *omission_getcurrentlevelptr          (OMission *mission);
void      omission_nextlevel                   (OMission *mission);
oboolean  omission_haslevelwithpassword        (OMission *mission, const ochar *password);
void      omission_setcurrentlevelfrompassword (OMission *mission, const ochar *password);
OLevel   *omission_pushbacknewlevel            (OMission *mission);
void      omission_parse                       (OMission *mission);
void      omission_pushbacklinetolastlevel     (OMission *mission, const ochar *line);
void      omission_fprintf                     (OMission *mission, FILE *out);
void      omission_printf                      (OMission *mission);
oboolean  omission_hasnextlevel                (OMission *mission);
ouint32   omission_getdurationinseconds        (OMission *mission);

#endif
