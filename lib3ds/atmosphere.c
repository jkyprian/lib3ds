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
 * $Id: atmosphere.c,v 1.2 2000/10/09 12:33:50 jeh Exp $
 */
#define LIB3DS_EXPORT
#include <lib3ds/atmosphere.h>
#include <lib3ds/chunk.h>
#include <lib3ds/readwrite.h>


/*!
 * \defgroup atmosphere Atmosphere Settings
 *
 * \author J.E. Hoffmann <je-h@gmx.net>
 */


static Lib3dsBool
fog_read(Lib3dsFog *fog, FILE *f)
{
  Lib3dsChunk c;
  Lib3dsDword chunk;

  if (!lib3ds_chunk_start(&c, LIB3DS_FOG, f)) {
    return(LIB3DS_FALSE);
  }
  fog->near_plane=lib3ds_float_read(f);
  fog->near_density=lib3ds_float_read(f);
  fog->far_plane=lib3ds_float_read(f);
  fog->far_density=lib3ds_float_read(f);
  lib3ds_chunk_tell(&c, f);
  
  while ((chunk=lib3ds_chunk_next(&c, f))!=0) {
    switch (chunk) {
      case LIB3DS_LIN_COLOR_F:
        {
          int i;
          for (i=0; i<3; ++i) {
            fog->col[i]=lib3ds_float_read(f);
          }
        }
        break;
      case LIB3DS_COLOR_F:
        break;
      case LIB3DS_FOG_BGND:
        {
          fog->fog_background=LIB3DS_TRUE;
        }
        break;
      default:
        lib3ds_chunk_unknown(chunk);
    }
  }
  
  lib3ds_chunk_end(&c, f);
  return(LIB3DS_TRUE);
}


static Lib3dsBool
layer_fog_read(Lib3dsLayerFog *fog, FILE *f)
{
  Lib3dsChunk c;
  Lib3dsDword chunk;

  if (!lib3ds_chunk_start(&c, LIB3DS_LAYER_FOG, f)) {
    return(LIB3DS_FALSE);
  }
  fog->near_y=lib3ds_float_read(f);
  fog->far_y=lib3ds_float_read(f);
  fog->density=lib3ds_float_read(f);
  fog->flags=lib3ds_dword_read(f);
  lib3ds_chunk_tell(&c, f);
  
  while ((chunk=lib3ds_chunk_next(&c, f))!=0) {
    switch (chunk) {
      case LIB3DS_LIN_COLOR_F:
        {
          int i;
          for (i=0; i<3; ++i) {
            fog->col[i]=lib3ds_float_read(f);
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
distance_cue_read(Lib3dsDistanceCue *cue, FILE *f)
{
  Lib3dsChunk c;
  Lib3dsDword chunk;

  if (!lib3ds_chunk_start(&c, LIB3DS_DISTANCE_CUE, f)) {
    return(LIB3DS_FALSE);
  }
  cue->near_plane=lib3ds_float_read(f);
  cue->near_dimming=lib3ds_float_read(f);
  cue->far_plane=lib3ds_float_read(f);
  cue->far_dimming=lib3ds_float_read(f);
  lib3ds_chunk_tell(&c, f);
  
  while ((chunk=lib3ds_chunk_next(&c, f))!=0) {
    switch (chunk) {
      case LIB3DS_DCUE_BGND:
        {
          cue->cue_background=LIB3DS_TRUE;
        }
        break;
      default:
        lib3ds_chunk_unknown(chunk);
    }
  }
  
  lib3ds_chunk_end(&c, f);
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup atmosphere
 */
Lib3dsBool
lib3ds_atmosphere_read(Lib3dsAtmosphere *atmosphere, FILE *f)
{
  Lib3dsChunk c;

  if (!lib3ds_chunk_read(&c, f)) {
    return(LIB3DS_FALSE);
  }
  
  switch (c.chunk) {
      case LIB3DS_FOG:
        {
          lib3ds_chunk_reset(&c, f);
          if (!fog_read(&atmosphere->fog, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_LAYER_FOG:
        {
          lib3ds_chunk_reset(&c, f);
          if (!layer_fog_read(&atmosphere->layer_fog, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_DISTANCE_CUE:
        {
          lib3ds_chunk_reset(&c, f);
          if (!distance_cue_read(&atmosphere->dist_cue, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_USE_FOG:
        {
          atmosphere->fog.use=LIB3DS_TRUE;
        }
        break;
      case LIB3DS_USE_LAYER_FOG:
        {
          atmosphere->fog.use=LIB3DS_TRUE;
        }
        break;
      case LIB3DS_USE_DISTANCE_CUE:
        {
          atmosphere->dist_cue.use=LIB3DS_TRUE;
        }
        break;
  }

  return(LIB3DS_TRUE);
}


/*!
 * \ingroup atmosphere
 */
Lib3dsBool
lib3ds_atmosphere_write(Lib3dsAtmosphere *atmosphere, FILE *f)
{
  /* FIXME: */
  ASSERT(0);
  return(LIB3DS_FALSE);
}


/*!

\typedef Lib3dsAtmosphere
  \ingroup atmosphere
  \sa _Lib3dsAtmosphere

*/
