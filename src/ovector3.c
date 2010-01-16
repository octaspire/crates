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
#include "ovector3.h"
#include "oerror.h"
#include <math.h>

INLINE void ovector3_set(OVector3 *vector3, const onumber x, const onumber y, const onumber z)
{
  vector3->x = x;
  vector3->y = y;
  vector3->z = z;
}

OVector3 *ovector3_new(void)
{
  OVector3 *vector3 = (OVector3*)oerror_malloc(sizeof(OVector3));
  ovector3_set(vector3, 0, 0, 0);
  return vector3;
}

OVector3 *ovector3_newvec(const OVector3 *other)
{
  OVector3 *vector3 = (OVector3*)oerror_malloc(sizeof(OVector3));
  ovector3_set(vector3, other->x, other->y, other->z);
  return vector3;
}

OVector3 *ovector3_new3(const onumber x, const onumber y, const onumber z)
{
  OVector3 *vector3 = (OVector3*)oerror_malloc(sizeof(OVector3));
  ovector3_set(vector3, x, y, z);
  return vector3;
}

void ovector3_release(OVector3 *vector3)
{
  if (!vector3)
    return;

  ovector3_set(vector3, 0, 0, 0);
  memset(vector3, 0, sizeof(OVector3));
  oerror_free(vector3);
}

INLINE void ovector3_add3(OVector3 *vector3, const onumber x, const onumber y, const onumber z)
{
  vector3->x += x;
  vector3->y += y;
  vector3->z += z;
}

void ovector3_add(OVector3 *vector3, const OVector3 *other)
{
  ovector3_add3(vector3, other->x, other->y, other->z);
}

void ovector3_sub(OVector3 *vector3, const OVector3 *other)
{
  ovector3_sub3(vector3, other->x, other->y, other->z);
}

void ovector3_sub3(OVector3 *vector3, const onumber x, const onumber y, const onumber z)
{
  vector3->x -= x;
  vector3->y -= y;
  vector3->z -= z;
}

void ovector3_mul(OVector3 *vector3, const onumber scalar)
{
  vector3->x *= scalar;
  vector3->y *= scalar;
  vector3->z *= scalar;
}

void ovector3_div(OVector3 *vector3, const onumber scalar)
{
  onumber inv = 1 / scalar;
  ovector3_mul(vector3, inv);
}

onumber ovector3_dot(const OVector3 *vector3, const OVector3 *other)
{
  return (vector3->x * other->x) + (vector3->y * other->y) + (vector3->z * other->z);
}

void ovector3_cross(const OVector3 *vector3, const OVector3 *other, OVector3 *result)
{
  result->x = (vector3->y * other->z) - (vector3->z * other->y);
  result->y = (vector3->z * other->x) - (vector3->x * other->z);
  result->z = (vector3->x * other->y) - (vector3->y * other->x);
}

onumber ovector3_length(const OVector3 *vector3)
{
  return sqrtf(ovector3_lengthsquared(vector3));
}

onumber ovector3_lengthsquared(const OVector3 *vector3)
{
  return (vector3->x * vector3->x) + (vector3->y * vector3->y) + (vector3->z * vector3->z);
}

void ovector3_normalize(OVector3 *vector3)
{
  onumber len = ovector3_length(vector3);
  if (len != 0)
    ovector3_div(vector3, len);
}

INLINE onumber ovector3_distancesquared(const OVector3 *vector3, const OVector3 *other)
{
  onumber dx = vector3->x - other->x;
  onumber dy = vector3->y - other->y;
  onumber dz = vector3->z - other->z;
  return (dx*dx) + (dy*dy) + (dz*dz);
}

INLINE onumber ovector3_distance(const OVector3 *vector3, const OVector3 *other)
{
  return sqrtf(ovector3_distancesquared(vector3, other));
}

onumber ovector3_radanglebetween(const OVector3 *vector3, const OVector3 *other)
{
  onumber dot = ovector3_dot(vector3, other);
  return acosf(dot);
}

void ovector3_negate(OVector3 *vector3)
{
  ovector3_set(vector3, -vector3->x, -vector3->y, -vector3->z);
}

void ovector3_zero(OVector3 *vector3)
{
  ovector3_set(vector3, 0, 0, 0);
}

oboolean ovector3_eq(const OVector3 *vector3, const OVector3 *other)
{
  return ovector3_eq3(vector3, other->x, other->y, other->z);
}

oboolean ovector3_eq3(const OVector3 *vector3, const onumber x, const onumber y, const onumber z)
{
  return (vector3->x == x) && (vector3->y == y) && (vector3->z == z);
}

oboolean ovector3_iszero(const OVector3 *vector3)
{
  return ovector3_eq3(vector3, 0, 0, 0);
}
