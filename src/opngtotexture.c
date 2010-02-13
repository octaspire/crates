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
#include "opngtotexture.h"
#include "oerror.h"
#include <png.h>

static void opngtotexture_load(OTexture *texture, const ochar *path)
{
  FILE *fp = fopen(path, "rb");
  png_byte sig[8];
  png_structp png_ptr;
  png_infop info_ptr;
  png_bytep *row_pointers = 0;
  png_uint_32 width, height, rowbytes, i;
  int bit_depth, color_type;

  if (!fp)
  {
    fclose(fp);
    opngtotexture_release(texture);
    oerror_fatal("opngtotexture_load: \"%s\" cannot be opened", path);
  }

  if (fread(sig, 1, sizeof(sig), fp) != sizeof(sig))
    oerror_warning("opngtotexture_load: number of items read for the signature was not %d", sizeof(sig));

  if (png_sig_cmp(sig, 0, sizeof(sig)) != 0)
  {
    fclose(fp);
    opngtotexture_release(texture);
    oerror_fatal("opngtotexture_load: \"%s\" is not a png file", path);
  }

  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

  if (!png_ptr)
  {
    fclose(fp);
    opngtotexture_release(texture);
    oerror_fatal("%s", "opngtotexture_load: png read structure cannot be created");
  }

  info_ptr = png_create_info_struct(png_ptr);

  if (!info_ptr)
  {
    fclose(fp);
    opngtotexture_release(texture);
    png_destroy_read_struct(&png_ptr, 0, 0);
    oerror_fatal("%s", "opngtotexture_load: png info structure cannot be created");
  }

  if (setjmp(png_jmpbuf(png_ptr)))
  {
    fclose(fp);
    opngtotexture_release(texture);
    png_destroy_read_struct(&png_ptr, &info_ptr, 0);
    oerror_fatal("%s", "opngtotexture_load: libpng invoked longjmp on error");
  }

  png_init_io(png_ptr, fp);
  png_set_sig_bytes(png_ptr, sizeof(sig));
  png_read_info(png_ptr, info_ptr);

  png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, 0, 0, 0);

  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png_ptr);

  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png_ptr);

  if (bit_depth == 16)
    png_set_strip_16(png_ptr);
  else if (bit_depth < 8)
    png_set_packing(png_ptr);

  png_read_update_info(png_ptr, info_ptr);
  png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, 0, 0, 0);

  texture->width = width;
  texture->height = height;

  switch (color_type)
  {
    case PNG_COLOR_TYPE_GRAY:
      texture->format = GL_LUMINANCE;
      texture->internalformat = 1;
      break;

    case PNG_COLOR_TYPE_GRAY_ALPHA:
      texture->format = GL_LUMINANCE_ALPHA;
      texture->internalformat = 2;
      break;

    case PNG_COLOR_TYPE_RGB:
      texture->format = GL_RGB;
      texture->internalformat = 3;
      break;

    case PNG_COLOR_TYPE_RGB_ALPHA:
      texture->format = GL_RGBA;
      texture->internalformat = 4;
      break;

    default:
      fclose(fp);
      opngtotexture_release(texture);
      png_destroy_read_struct(&png_ptr, 0, 0);
      oerror_fatal("opngtotexture_load: unsupported color type %d", color_type);
      break;
  }

  texture->data = (GLvoid*)oerror_malloc((texture->width * texture->height) * texture->internalformat);
  row_pointers = (png_bytep*)oerror_malloc(sizeof(png_bytep) * texture->height);
  rowbytes = png_get_rowbytes(png_ptr, info_ptr);

  for (i=0; i<texture->height; ++i)
    row_pointers[i] = texture->data + (i * rowbytes);

  png_read_image(png_ptr, row_pointers);
  png_read_end(png_ptr, 0);
  png_destroy_read_struct(&png_ptr, &info_ptr, 0);
  oerror_free(row_pointers);
  fclose(fp);
}

OTexture *opngtotexture_new(const ochar *path)
{
  OTexture *texture = (OTexture*)oerror_malloc(sizeof(OTexture));
  texture->internalformat = 0;
  texture->width = 0;
  texture->height = 0;
  texture->format = 0;
  texture->data = 0;
  opngtotexture_load(texture, path);
  return texture;
}

void opngtotexture_release(OTexture *texture)
{
  if (!texture)
    return;

  if (texture->data)
    oerror_free(texture->data);

  memset(texture, 0, sizeof(OTexture));
  oerror_free(texture);
}
