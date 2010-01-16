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
#ifndef OERROR_H
#define OERROR_H

#include "otypes.h"
#include <stdarg.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

void  oerror_fatal   (const char *fmt, ...);
void  oerror_warning (const char *fmt, ...);
void  oerror_info    (const char *fmt, ...);
void *oerror_malloc  (const size_t n);
void *oerror_calloc  (const size_t nmemb, const size_t size);
void *oerror_realloc (void *ptr, const size_t size);
void  oerror_free    (void *ptr);
FILE *oerror_fopen   (const char *path, const char *mode);
void  oerror_status  ();

#endif
