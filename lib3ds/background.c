/*
 * The 3D Studio File Format Library
 * Copyright (C) 1996-2000 by J.E. Hoffmann <je-h@gmx.net>
 * All rights reserved.
 *
 * This program is  free  software;  you can redistribute it and/or modify it
 * under the terms of the  GNU Lesser General Public License  as published by 
 * the  Free Software Foundation;  either version 2.1 of the License,  or (at 
 * your option) any later version.
 *
 * This  program  is  distributed in  the  hope that it will  be useful,  but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or  FITNESS FOR A  PARTICULAR PURPOSE.  See the  GNU Lesser General Public  
 * License for more details.
 *
 * You should  have received  a copy of the GNU Lesser General Public License
 * along with  this program;  if not, write to the  Free Software Foundation,
 * Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Id: background.c,v 1.2 2000/10/09 12:33:50 jeh Exp $
 */
#define LIB3DS_EXPORT
#include <lib3ds/background.h>
#include <lib3ds/chunk.h>
#include <lib3ds/readwrite.h>


/*!
 * \defgroup background Background Settings
 *
 * \author J.E. Hoffmann <je-h@gmx.net>
 */


static Lib3dsBool
solid_bgnd_read(Lib3dsBackground *background, FILE *f)
{
  Lib3dsChunk c;
  Lib3dsDword chunk;

  if (!lib3ds_chunk_start(&c, LIB3DS_SOLID_BGND, f)) {
    return(LIB3DS_FALSE);
  }

  while ((chunk=lib3ds_chunk_next(&c, f))!=0) {
    switch (chunk) {
      case LIB3DS_LIN_COLOR_F:
        {
          int i;
          for (i=0; i<3; ++i) {
            background->solid.col[i]=lib3ds_float_read(f);
          }
        }
        break;
      case LIB3DS_COLOR_F:
        break;
      default:
        lib3ds_chunk_unknown(chunk);
    }
  }
  
  lib3ds_chunk_end(&c, f);
  return(LIB3DS_TRUE);
}


static Lib3dsBool
v_gradient_read(Lib3dsBackground *background, FILE *f)
{
  Lib3dsChunk c;
  Lib3dsDword chunk;
  int col;

  if (!lib3ds_chunk_start(&c, LIB3DS_V_GRADIENT, f)) {
    return(LIB3DS_FALSE);
  }
  background->gradient.percent=lib3ds_float_read(f);
  lib3ds_chunk_tell(&c, f);

  col=0;
  while ((chunk=lib3ds_chunk_next(&c, f))!=0) {
    switch (chunk) {
      case LIB3DS_LIN_COLOR_F:
        {
          int i;

          switch (col) {
            case 0:
              {
                for (i=0; i<3; ++i) {
                  background->gradient.top[i]=lib3ds_float_read(f);
                }
              }
              break;
            case 1:
              {
                for (i=0; i<3; ++i) {
                  background->gradient.middle[i]=lib3ds_float_read(f);
                }
              }
              break;
            case 2:
              {
                for (i=0; i<3; ++i) {
                  background->gradient.bottom[i]=lib3ds_float_read(f);
                }
              }
              break;
            default:
              ASSERT(0);
          }
          col++;
        }
        break;
      case LIB3DS_COLOR_F:
        break;
      default:
        lib3ds_chunk_unknown(chunk);
    }
  }
  
  lib3ds_chunk_end(&c, f);
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup background
 */
Lib3dsBool
lib3ds_background_read(Lib3dsBackground *background, FILE *f)
{
  Lib3dsChunk c;

  if (!lib3ds_chunk_read(&c, f)) {
    return(LIB3DS_FALSE);
  }
  
  switch (c.chunk) {
    case LIB3DS_BIT_MAP:
      {
        if (!lib3ds_string_read(background->bitmap.name, 64, f)) {
            return(LIB3DS_FALSE);
        }
      }
        break;
      case LIB3DS_SOLID_BGND:
        {
          lib3ds_chunk_reset(&c, f);
          if (!solid_bgnd_read(background, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_V_GRADIENT:
        {
          lib3ds_chunk_reset(&c, f);
          if (!v_gradient_read(background, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_USE_BIT_MAP:
        {
          background->bitmap.use=LIB3DS_TRUE;
        }
        break;
      case LIB3DS_USE_SOLID_BGND:
        {
          background->solid.use=LIB3DS_TRUE;
        }
        break;
      case LIB3DS_USE_V_GRADIENT:
        {
          background->gradient.use=LIB3DS_TRUE;
        }
        break;
  }
  
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup background
 */
Lib3dsBool
lib3ds_background_write(Lib3dsBackground *background, FILE *f)
{
  /* FIXME: */
  ASSERT(0);
  return(LIB3DS_FALSE);
}


/*!

\typedef Lib3dsBackground
  \ingroup background
  \sa _Lib3dsBackground

*/
