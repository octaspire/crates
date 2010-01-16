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
#ifndef OPTRMAP_H
#define OPTRMAP_H

#include "otypes.h"
#include "optrvector.h"

typedef void (*OPtrMapValueCallback)(void *element);

typedef struct {
  OPtrVector           *keys;
  OPtrVector           *values;
  OPtrMapValueCallback  valuereleasefunc;
} OPtrMap;

OPtrMap *optrmap_new      (OPtrMapValueCallback valuereleasefunc);
void     optrmap_release  (OPtrMap *ptrmap);
void    *optrmap_get      (OPtrMap *ptrmap, const ochar *key);
void    *optrmap_put      (OPtrMap *ptrmap, const ochar *key, void *value);
void     optrmap_remove   (OPtrMap *ptrmap, const ochar *key);
oint32   optrmap_getindex (OPtrMap *ptrmap, const ochar *key);

#endif
