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
#include "ovector.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "oerror.h"

#define OVECTOR_INDEX(i) ((ochar*)(vector)->elements + ((vector)->elementsize * (i)))

static const oint32  OVECTOR_INITIALSIZE = 8;

OVector *ovector_new(const size_t elementsize, const size_t initialsize, OVectorElementCallback elementreleasefunc)
{
  OVector *vector = (OVector*)oerror_malloc(sizeof(OVector));
  vector->elementsize = elementsize;
  vector->numelements = 0;
  vector->numallocated = initialsize > 0 ? initialsize : OVECTOR_INITIALSIZE;
  vector->elements = oerror_malloc(elementsize * vector->numallocated);
  vector->elementreleasefunc = elementreleasefunc;
  return vector;
}

void ovector_release(OVector *vector)
{
  if (!vector)
    return;

  if (vector->elementreleasefunc)
    ovector_foreach(vector, vector->elementreleasefunc);

  oerror_free(vector->elements);
  vector->elements = 0;
  vector->elementsize = 0;
  vector->numelements = 0;
  vector->numallocated = 0;
  vector->elementreleasefunc = 0;
  memset(vector, 0, sizeof(OVector));
  oerror_free(vector);
}

static void ovector_grow(OVector *vector)
{
  vector->numallocated *= 2;
  vector->elements = oerror_realloc(vector->elements, vector->elementsize * vector->numallocated);
}

INLINE void ovector_insertat(OVector *vector, const void *element, const size_t index)
{
  void *target;
  ouint32 ne = vector->numelements;

  if (index < 0)
    return;

  while (index >= vector->numallocated)
    ovector_grow(vector);

  while (ne < index)
  {
    memset(OVECTOR_INDEX(ne), 0, vector->elementsize);
    vector->numelements++;
    ++ne;
  }

  target = OVECTOR_INDEX(index);
  memcpy(target, element, vector->elementsize);

  if (index < vector->numelements)
    return;

  vector->numelements++;
}

INLINE void ovector_pushback(OVector *vector, const void *element)
{
  ovector_insertat(vector, element, vector->numelements);
}

INLINE void ovector_popback(OVector *vector, void *element)
{
  if (vector->numelements <= 0)
    return;

  if (element != 0)
    memcpy(element, OVECTOR_INDEX(vector->numelements - 1), vector->elementsize);

  vector->numelements--;
}

void *ovector_peekback(OVector *vector)
{
  if (vector->numelements <= 0)
    return 0;

  return OVECTOR_INDEX(vector->numelements - 1);
}

void ovector_copyelementat(const OVector *vector, const size_t index, void *element)
{
  if (index < 0 || index >= vector->numelements)
    return;

  memcpy(element, OVECTOR_INDEX(index), vector->elementsize);
}

INLINE void *ovector_getelementptrat(const OVector *vector, const size_t index)
{
  if (index < 0 || index >= vector->numelements)
    return 0;

  return OVECTOR_INDEX(index);
}

INLINE void *ovector_getrandomelementptr(const OVector *vector)
{
  oint32 index;

  if (vector->numelements == 0)
    return 0;

  index = rand() % vector->numelements;
  return ovector_getelementptrat(vector, index);
}

void ovector_removeelementat(OVector *vector, const size_t index)
{
  void *ptr;

  if (index < 0 || index >= vector->numelements)
    return;

  ptr = OVECTOR_INDEX(index);

  if (vector->elementreleasefunc)
    vector->elementreleasefunc(ptr);

  /* TODO is this correct? Test it. */
  memmove(ptr, OVECTOR_INDEX(index + 1), vector->elementsize * (vector->numelements - index - 1));
  vector->numelements--;
}

/* Put last element on the place of the removed one. Doesn't preserve order but should be faster than
 o_vector_remove_element_at.*/
void ovector_removeelementatfast(OVector* vector, const size_t index)
{
  void *ptr;

  if (index < 0 || index >= vector->numelements)
    return;

  ptr = OVECTOR_INDEX(index);

  if (vector->elementreleasefunc)
    vector->elementreleasefunc(ptr);

  /* TODO is this correct? Test it. */
  if (index != (vector->numelements - 1))
    memcpy(ptr, OVECTOR_INDEX(vector->numelements - 1), vector->elementsize);

  vector->numelements--;
}

INLINE size_t ovector_length(const OVector *vector)
{
  return vector->numelements;
}

INLINE void ovector_clear(OVector *vector)
{
  vector->numelements = 0;
}

INLINE const void *ovector_toptr(const OVector *vector)
{
  return vector->elements;
}

void ovector_foreach(const OVector *vector, OVectorElementCallback callback)
{
  ouint32 i;
  ouint32 len = ovector_length(vector);

  for (i = 0; i < len; ++i)
    callback(ovector_getelementptrat(vector, i));
}

void ovector_foreachi(const OVector *vector, OVectorElementAndIndexCallback callback, void *userdata)
{
  ouint32 i;
  ouint32 len = ovector_length(vector);

  for (i = 0; i < len; ++i)
    callback(ovector_getelementptrat(vector, i), i, userdata);
}

void ovector_rforeachi(const OVector *vector, OVectorElementAndIndexCallback callback, void *userdata)
{
  int i;
  ouint32 len = ovector_length(vector);

  for (i = len - 1; i >= 0; --i)
    callback(ovector_getelementptrat(vector, i), i, userdata);
}

oint32 ovector_find(const OVector *vector, const void *element, OVectorElementCompareCallback callback)
{
  ouint32 i;
  ouint32 len = ovector_length(vector);

  for (i = 0; i < len; ++i)
    if (callback(ovector_getelementptrat(vector, i), element) == 0)
      return i;

  return -1;
}

void ovector_qsort(OVector *vector, OVectorElementCompareCallback callback)
{
  qsort(vector->elements, ovector_length(vector), vector->elementsize, callback);
}
