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
#ifndef OPTRVECTOR_H
#define OPTRVECTOR_H

#include "otypes.h"
#include <stdio.h>

typedef oint32 (*OPtrVectorElementCompareCallback)(const void *a, const void *b);
typedef void   (*OPtrVectorElementAndIndexCallback)(void *element, ouint32 index, void *userdata);
typedef void   (*OPtrVectorElementCallback)(void *element);

typedef struct {
  void                     **elements;
  size_t                    numelements;
  size_t                    numallocated;
  OPtrVectorElementCallback elementreleasefunc;
} OPtrVector;

OPtrVector *optrvector_new                 (const size_t initialsize, OPtrVectorElementCallback elementreleasefunc);
void        optrvector_release             (OPtrVector *ptrvector);
void        optrvector_insertat            (OPtrVector *ptrvector, void* element, const size_t index);
void        optrvector_pushback            (OPtrVector *ptrvector, void* element);
void       *optrvector_popback             (OPtrVector *ptrvector);
void       *optrvector_peekback            (OPtrVector *ptrvector);
void       *optrvector_getelementptrat     (const OPtrVector *ptrvector, const size_t index);
void       *optrvector_getrandomelementptr (const OPtrVector *ptrvector);
void        optrvector_removeelementat     (OPtrVector *ptrvector, const size_t index);
void        optrvector_removeelementatfast (OPtrVector *ptrvector, const size_t index);
size_t      optrvector_length              (const OPtrVector *ptrvector);
void        optrvector_clear               (OPtrVector *ptrvector);
void        optrvector_foreach             (const OPtrVector *ptrvector, OPtrVectorElementCallback callback);
void        optrvector_foreachi            (const OPtrVector *ptrvector, OPtrVectorElementAndIndexCallback callback, void *userdata);
void        optrvector_rforeachi           (const OPtrVector *ptrvector, OPtrVectorElementAndIndexCallback callback, void *userdata);
oint32      optrvector_find                (const OPtrVector *ptrvector, const void *element, OPtrVectorElementCompareCallback callback);
void        optrvector_qsort               (OPtrVector *ptrvector, OPtrVectorElementCompareCallback callback);

#endif
