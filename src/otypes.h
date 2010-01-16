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
#ifndef OTYPES_H
#define OTYPES_H

typedef unsigned int   ouint32;
typedef signed int     oint32;
typedef unsigned short ouint16;
typedef signed short   oint16;
typedef unsigned char  ouint8;
typedef signed char    oint8;
typedef unsigned char  ouchar;
typedef char           ochar;
typedef float          ofloat;
typedef double         odouble;
typedef double         onumber; /* rational numbers used with Lua. */
typedef oint32         oboolean;

#define INLINE __inline__

#endif
