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
#ifndef OVECTOR_H
#define OVECTOR_H

#include "otypes.h"
#include <stdio.h>

typedef oint32 (*OVectorElementCompareCallback)(const void *a, const void *b);
typedef void   (*OVectorElementAndIndexCallback)(void *element, ouint32 index, void *userdata);
typedef void   (*OVectorElementCallback)(void *element);

typedef struct {
  void                  *elements;
  size_t                 elementsize;
  size_t                 numelements;
  size_t                 numallocated;
  OVectorElementCallback elementreleasefunc;
} OVector;

OVector    *ovector_new                 (const size_t elementsize, const size_t initialsize, OVectorElementCallback elementreleasefunc);
void        ovector_release             (OVector *vector);
void        ovector_insertat            (OVector *vector, const void* element, const size_t index);
void        ovector_pushback            (OVector *vector, const void* element);
void        ovector_popback             (OVector *vector, void* element);
void       *ovector_peekback            (OVector *vector);
void        ovector_copyelementat       (const OVector *vector, const size_t index, void *element);
void       *ovector_getelementptrat     (const OVector *vector, const size_t index);
void       *ovector_getrandomelementptr (const OVector *vector);
void        ovector_removeelementat     (OVector *vector, const size_t index);
void        ovector_removeelementatfast (OVector *vector, const size_t index);
size_t      ovector_length              (const OVector *vector);
void        ovector_clear               (OVector *vector);
const void *ovector_toptr               (const OVector *vector);
void        ovector_foreach             (const OVector *vector, OVectorElementCallback callback);
void        ovector_foreachi            (const OVector *vector, OVectorElementAndIndexCallback callback, void *userdata);
void        ovector_rforeachi           (const OVector *vector, OVectorElementAndIndexCallback callback, void *userdata);
oint32      ovector_find                (const OVector *vector, const void *element, OVectorElementCompareCallback callback);
void        ovector_qsort               (OVector *vector, OVectorElementCompareCallback callback);

#endif
