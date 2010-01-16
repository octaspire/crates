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
#ifndef OVECTOR3_H
#define OVECTOR3_H

#include "otypes.h"
#include <stdio.h>

typedef struct {
  onumber x, y, z;
} OVector3;

OVector3 *ovector3_new             (void);
OVector3 *ovector3_newvec          (const OVector3 *other);
OVector3 *ovector3_new3            (onumber x, const onumber y, const onumber z);
void      ovector3_release         (OVector3 *vector3);
void      ovector3_set             (OVector3 *vector3, const onumber x, const onumber y, const onumber z);
void      ovector3_add             (OVector3 *vector3, const OVector3 *other);
void      ovector3_add3            (OVector3 *vector3, const onumber x, const onumber y, const onumber z);
void      ovector3_sub             (OVector3 *vector3, const OVector3 *other);
void      ovector3_sub3            (OVector3 *vector3, const onumber x, const onumber y, const onumber z);
void      ovector3_mul             (OVector3 *vector3, const onumber scalar);
void      ovector3_div             (OVector3 *vector3, const onumber scalar);
onumber   ovector3_dot             (const OVector3 *vector3, const OVector3 *other);
void      ovector3_cross           (const OVector3 *vector3, const OVector3 *other, OVector3 *result);
onumber   ovector3_length          (const OVector3 *vector3);
onumber   ovector3_lengthsquared   (const OVector3 *vector3);
void      ovector3_normalize       (OVector3 *vector3);
onumber   ovector3_distance        (const OVector3 *vector3, const OVector3 *other);
onumber   ovector3_distancesquared (const OVector3 *vector3, const OVector3 *other);
onumber   ovector3_radanglebetween (const OVector3 *vector3, const OVector3 *other);
void      ovector3_negate          (OVector3 *vector3);
void      ovector3_zero            (OVector3 *vector3);
oboolean  ovector3_eq              (const OVector3 *vector3, const OVector3 *other);
oboolean  ovector3_eq3             (const OVector3 *vector3, const onumber x, const onumber y, const onumber z);
oboolean  ovector3_iszero          (const OVector3 *vector3);

#endif
