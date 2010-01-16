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
#include "oerror.h"
#include <assert.h>
#include <errno.h>
#include <stdlib.h>

#undef OERROR_VERBOSE

static oint32  oerror_nmalloc    = 0;
static oint32  oerror_nfree      = 0;
#ifdef OERROR_VERBOSE
  static ouint32 oerror_nrealloc = 0;
  static ouint32 oerror_nbytes   = 0;
  static ouint32 oerror_rbytes   = 0;
#endif

/* Idea from K&R2 p. 109 */
void oerror_fatal(const char *fmt, ...)
{
  va_list args;

  fflush(stdout);
  fprintf(stderr, "fatal: ");
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  if (fmt[0] != '\0' && fmt[strlen(fmt)-1] == ':')
    fprintf(stderr, " %s", strerror(errno));

  fprintf(stderr, "\n");
  exit(EXIT_FAILURE);
}

void oerror_warning(const char *fmt, ...)
{
  va_list args;

  fflush(stdout);
  fprintf(stderr, "warning: ");
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  if (fmt[0] != '\0' && fmt[strlen(fmt)-1] == ':')
    fprintf(stderr, " %s", strerror(errno));

  fprintf(stderr, "\n");
}

void oerror_info(const char *fmt, ...)
{
  va_list args;

  fflush(stdout);
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  if (fmt[0] != '\0' && fmt[strlen(fmt)-1] == ':')
    fprintf(stderr, " %s", strerror(errno));

  fprintf(stderr, "\n");
}

/* Idea from K&R2 p. 110 */
void *oerror_malloc(const size_t n)
{
  void *p = malloc(n);

  if (p == 0)
    oerror_fatal("malloc of %u bytes failed:", n);

  memset(p, 0, n);
  oerror_nmalloc++;
  #ifdef OERROR_VERBOSE
    printf("malloc %d\n", oerror_nmalloc);
    oerror_nbytes += n;
  #endif
  return p;
}

void *oerror_calloc(const size_t nmemb, const size_t size)
{
  void *p = calloc(nmemb, size);

  if (p == 0)
    oerror_fatal("calloc of %u elements each of %u bytes failed:", nmemb, size);

  oerror_nmalloc++;
  #ifdef OERROR_VERBOSE
    printf("calloc %d\n", oerror_nmalloc);
    oerror_nbytes += (nmemb * size);
  #endif
  return p;
}

void *oerror_realloc(void *ptr, const size_t size)
{
  void *p = realloc(ptr, size);

  if (p == 0)
    oerror_fatal("realloc to size %u bytes failed:", size);

  #ifdef OERROR_VERBOSE
    oerror_nrealloc++;
    printf("realloc %d\n", oerror_nrealloc);
    oerror_rbytes += size;
  #endif
  return p;
}

void oerror_free(void *ptr)
{
  assert(ptr);
  free(ptr);
  oerror_nfree++;
  #ifdef OERROR_VERBOSE
    printf("free %d\n", oerror_nfree);
  #endif
}

FILE *oerror_fopen(const char *path, const char *mode)
{
  FILE* p = fopen(path, mode);

  if (p == 0)
    oerror_fatal("fopen of \"%s\" in mode \"%s\" failed:", path, mode);

  return p;
}

void oerror_status()
{
  #ifdef OERROR_VERBOSE
    oerror_info("num_mallocs=%d num_frees=%d num_reallocs=%d num_bytes=%d num_rbytes=%d", oerror_nmalloc, oerror_nfree, oerror_nrealloc, oerror_nbytes, oerror_rbytes);
  #else
    oerror_info("num_mallocs=%d num_frees=%d", oerror_nmalloc, oerror_nfree);
  #endif
}
