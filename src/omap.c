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
#include "omap.h"
#include "oerror.h"
#include "ostring.h"

OMap *omap_new(size_t valuesize, OMapValueCallback valuereleasefunc)
{
  OMap *map = (OMap*)oerror_malloc(sizeof(OMap));
  map->keys = optrvector_new(0, (OPtrVectorElementCallback)ostring_release);
  map->values = ovector_new(valuesize,    0, valuereleasefunc);
  map->valuereleasefunc = valuereleasefunc;
  return map;
}

void omap_release(OMap *map)
{
  if (!map)
    return;

  optrvector_release(map->keys);
  ovector_release(map->values);
  map->valuereleasefunc = 0;
  memset(map, 0, sizeof(OMap));
  oerror_free(map);
}

oint32 omap_getindex(OMap *map, const ochar *key)
{
  return optrvector_find(map->keys, (void*)key, (OPtrVectorElementCompareCallback)ostring_comparestr);
}

void *omap_get(OMap *map, const ochar *key)
{
  oint32 i = omap_getindex(map, key);
  return ovector_getelementptrat(map->values, i);
}

void *omap_put(OMap *map, const ochar *key, const void *value)
{
  if (omap_get(map, key))
    return 0;

  ovector_pushback(map->values, value);
  optrvector_pushback(map->keys, ostring_newstr(key));
  return ovector_peekback(map->values);
}

static void omap_removeat(OMap *map, const ouint32 index)
{
  optrvector_removeelementat(map->keys, index);

  if (map->valuereleasefunc)
    map->valuereleasefunc(ovector_getelementptrat(map->values, index));

  ovector_removeelementat(map->values, index);
}

void omap_remove(OMap *map, const ochar *key)
{
  oint32 i = omap_getindex(map, key);

  if (i < 0)
    return;

  omap_removeat(map, i);
}
