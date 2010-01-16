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
#include "optrvector.h"
#include "oerror.h"
#include <stdlib.h>

static const oint32  OPTRVECTOR_INITIALSIZE = 8;

OPtrVector *optrvector_new(const size_t initialsize, OPtrVectorElementCallback elementreleasefunc)
{
  OPtrVector *ptrvector = (OPtrVector*)oerror_malloc(sizeof(OPtrVector));
  ptrvector->numelements = 0;
  ptrvector->numallocated = initialsize > 0 ? initialsize : OPTRVECTOR_INITIALSIZE;
  ptrvector->elements = (void**)oerror_calloc(ptrvector->numallocated, sizeof(void*));
  ptrvector->elementreleasefunc = elementreleasefunc;
  return ptrvector;
}

void optrvector_release(OPtrVector *ptrvector)
{
  if (!ptrvector)
    return;

  if (ptrvector->elementreleasefunc)
    optrvector_foreach(ptrvector, ptrvector->elementreleasefunc);

  oerror_free(ptrvector->elements);
  ptrvector->elements = 0;
  ptrvector->numelements = 0;
  ptrvector->numallocated = 0;
  ptrvector->elementreleasefunc = 0;
  memset(ptrvector, 0, sizeof(OPtrVector));
  oerror_free(ptrvector);
}

static void optrvector_grow(OPtrVector *ptrvector)
{
  ptrvector->numallocated *= 2;
  ptrvector->elements = (void**)oerror_realloc(ptrvector->elements, sizeof(void*) * ptrvector->numallocated);
}

void optrvector_insertat(OPtrVector *ptrvector, void* element, const size_t index)
{
  ouint32 ne = ptrvector->numelements;

  if (index < 0)
    return;

  while (index >= ptrvector->numallocated)
    optrvector_grow(ptrvector);

  while (ne < index)
  {
    ptrvector->elements[ne] = 0;
    ptrvector->numelements++;
    ++ne;
  }
  ptrvector->elements[index] = element;

  if (index < ptrvector->numelements)
    return;

  ptrvector->numelements++;
}

INLINE void optrvector_pushback(OPtrVector *ptrvector, void* element)
{
  optrvector_insertat(ptrvector, element, ptrvector->numelements);
}

INLINE void *optrvector_popback(OPtrVector *ptrvector)
{
  void* element = 0;

  if (ptrvector->numelements <= 0)
    return element;

  element = ptrvector->elements[ptrvector->numelements - 1];
  ptrvector->numelements--;
  return element;
}

INLINE void *optrvector_peekback(OPtrVector *ptrvector)
{
  if (ptrvector->numelements <= 0)
    return 0;

  return ptrvector->elements[ptrvector->numelements - 1];
}

INLINE void *optrvector_getelementptrat(const OPtrVector *ptrvector, const size_t index)
{
  if (index < 0 || index >= ptrvector->numelements)
    return 0;

  return ptrvector->elements[index];
}

INLINE void *optrvector_getrandomelementptr (const OPtrVector *ptrvector)
{
  oint32 index;

  if (ptrvector->numelements == 0)
    return 0;

  index = rand() % ptrvector->numelements;
  return optrvector_getelementptrat(ptrvector, index);
}

void optrvector_removeelementat(OPtrVector *ptrvector, const size_t index)
{
  void *ptr;

  if (index < 0 || index >= ptrvector->numelements)
    return;

  ptr = ptrvector->elements[index];

  if (ptrvector->elementreleasefunc)
    ptrvector->elementreleasefunc(ptr);

  /* TODO is this correct? Test it. */
  memmove(ptrvector->elements + index, ptrvector->elements + (index + 1), (ptrvector->numelements - index - 1)*sizeof(void*));
  ptrvector->numelements--;
}

void optrvector_removeelementatfast(OPtrVector *ptrvector, const size_t index)
{
  void *ptr;

  if (index < 0 || index >= ptrvector->numelements)
    return;

  ptr = ptrvector->elements[index];

  if (ptrvector->elementreleasefunc)
    ptrvector->elementreleasefunc(ptr);

  /* TODO is this correct? Test it. */
  if (index != (ptrvector->numelements - 1))
     ptrvector->elements[index] = ptrvector->elements[(ptrvector->numelements - 1)];

  ptrvector->numelements--;
}

INLINE size_t optrvector_length(const OPtrVector *ptrvector)
{
  return ptrvector->numelements;
}

INLINE void optrvector_clear(OPtrVector *ptrvector)
{
  ptrvector->numelements = 0;
}

void optrvector_foreach(const OPtrVector *ptrvector, OPtrVectorElementCallback callback)
{
  ouint32 i;
  ouint32 len = optrvector_length(ptrvector);

  for (i = 0; i < len; ++i)
    callback(ptrvector->elements[i]);
}

void optrvector_foreachi(const OPtrVector *ptrvector, OPtrVectorElementAndIndexCallback callback, void *userdata)
{
  ouint32 i;
  ouint32 len = optrvector_length(ptrvector);

  for (i = 0; i < len; ++i)
    callback(ptrvector->elements[i], i, userdata);
}

void optrvector_rforeachi(const OPtrVector *ptrvector, OPtrVectorElementAndIndexCallback callback, void *userdata)
{
  int i;
  ouint32 len = optrvector_length(ptrvector);

  for (i = len - 1; i >= 0; --i)
    callback(ptrvector->elements[i], i, userdata);
}

oint32 optrvector_find(const OPtrVector *ptrvector, const void *element, OPtrVectorElementCompareCallback callback)
{
  ouint32 i;
  ouint32 len = optrvector_length(ptrvector);

  for (i = 0; i < len; ++i)
    if (callback(ptrvector->elements[i], element) == 0)
      return i;

  return -1;
}

void optrvector_qsort(OPtrVector *ptrvector, OPtrVectorElementCompareCallback callback)
{
  qsort(ptrvector->elements, optrvector_length(ptrvector), sizeof(void*), callback);
}
