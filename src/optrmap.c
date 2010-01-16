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
#include "optrmap.h"
#include "oerror.h"
#include "ostring.h"

OPtrMap *optrmap_new(OPtrMapValueCallback valuereleasefunc)
{
  OPtrMap *ptrmap = (OPtrMap*)oerror_malloc(sizeof(OPtrMap));
  ptrmap->keys = optrvector_new(0, (OPtrVectorElementCallback)ostring_release);
  ptrmap->values = optrvector_new(0, valuereleasefunc);
  ptrmap->valuereleasefunc = valuereleasefunc;
  return ptrmap;
}

void optrmap_release(OPtrMap *ptrmap)
{
  if (!ptrmap)
    return;

  optrvector_release(ptrmap->keys);
  optrvector_release(ptrmap->values);
  ptrmap->valuereleasefunc = 0;
  memset(ptrmap, 0, sizeof(OPtrMap));
  oerror_free(ptrmap);
}

void *optrmap_get(OPtrMap *ptrmap, const ochar *key)
{
  oint32 i = optrmap_getindex(ptrmap, key);
  return optrvector_getelementptrat(ptrmap->values, i);
}

void *optrmap_put(OPtrMap *ptrmap, const ochar *key, void *value)
{
  if (optrmap_get(ptrmap, key))
    return 0;

  optrvector_pushback(ptrmap->values, value);
  optrvector_pushback(ptrmap->keys, ostring_newstr(key));
  return optrvector_peekback(ptrmap->values);
}

static void optrmap_removeat(OPtrMap *ptrmap, const ouint32 index)
{
  optrvector_removeelementat(ptrmap->keys, index);

  if (ptrmap->valuereleasefunc)
    ptrmap->valuereleasefunc(optrvector_getelementptrat(ptrmap->values, index));

  optrvector_removeelementat(ptrmap->values, index);
}

void optrmap_remove(OPtrMap *ptrmap, const ochar *key)
{
  oint32 i = optrmap_getindex(ptrmap, key);

  if (i < 0)
    return;

  optrmap_removeat(ptrmap, i);
}

oint32 optrmap_getindex(OPtrMap *ptrmap, const ochar *key)
{
  return optrvector_find(ptrmap->keys, (void*)key, (OPtrVectorElementCompareCallback)ostring_comparestr);
}
