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
 * $Id: material.c,v 1.5 2000/10/09 12:33:50 jeh Exp $
 */
#define LIB3DS_EXPORT
#include <lib3ds/material.h>
#include <lib3ds/chunk.h>
#include <lib3ds/readwrite.h>
#include <stdlib.h>
#include <math.h>
#include <config.h>
#ifdef WITH_DMALLOC
#include <dmalloc.h>
#endif


/*!
 * \defgroup material Materials
 *
 * \author J.E. Hoffmann <je-h@gmx.net>
 */


/*!
 * \ingroup material
 */
Lib3dsMaterial*
lib3ds_material_new()
{
  Lib3dsMaterial *material;

  material=(Lib3dsMaterial*)calloc(sizeof(Lib3dsMaterial), 1);
  if (!material) {
    return(0);
  }
  return(material);
}


/*!
 * \ingroup material
 */
void
lib3ds_material_free(Lib3dsMaterial *material)
{
  memset(material, 0, sizeof(Lib3dsMaterial));
  free(material);
}


static Lib3dsBool
color_read(Lib3dsRgba rgb, FILE *f)
{
  Lib3dsChunk c;
  Lib3dsDword chunk;

  if (!lib3ds_chunk_start(&c, 0, f)) {
    return(LIB3DS_FALSE);
  }

  while ((chunk=lib3ds_chunk_next(&c, f))!=0) {
    switch (chunk) {
      case LIB3DS_LIN_COLOR_24:
        {
          int i;
          for (i=0; i<3; ++i) {
            rgb[i]=1.0*lib3ds_byte_read(f)/255.0;
          }
          rgb[3]=1.0f;
        }
        break;
      case LIB3DS_COLOR_24:
        break;
      default:
        lib3ds_chunk_unknown(chunk);
    }
  }
  
  lib3ds_chunk_end(&c, f);
  return(LIB3DS_TRUE);
}


static Lib3dsBool
int_percentage_read(Lib3dsIntw *p, FILE *f)
{
  Lib3dsChunk c;
  Lib3dsDword chunk;

  if (!lib3ds_chunk_start(&c, 0, f)) {
    return(LIB3DS_FALSE);
  }

  while ((chunk=lib3ds_chunk_next(&c, f))!=0) {
    switch (chunk) {
      case LIB3DS_INT_PERCENTAGE:
        {
          *p=lib3ds_intw_read(f);
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
texture_map_read(Lib3dsTextureMap *map, FILE *f)
{
  Lib3dsChunk c;
  Lib3dsDword chunk;

  if (!lib3ds_chunk_start(&c, 0, f)) {
    return(LIB3DS_FALSE);
  }

  while ((chunk=lib3ds_chunk_next(&c, f))!=0) {
    switch (chunk) {
      case LIB3DS_MAT_MAPNAME:
        {
          if (!lib3ds_string_read(map->name, 64, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_MAP_TILING:
        {
          map->flags=lib3ds_intw_read(f);
        }
        break;
      case LIB3DS_MAT_MAP_TEXBLUR:
        {
          map->blur=lib3ds_float_read(f);
        }
        break;
      case LIB3DS_MAT_MAP_USCALE:
        {
          map->scale[0]=lib3ds_float_read(f);
        }
        break;
      case LIB3DS_MAT_MAP_VSCALE:
        {
          map->scale[1]=lib3ds_float_read(f);
        }
        break;
      case LIB3DS_MAT_MAP_UOFFSET:
        {
          map->offset[0]=lib3ds_float_read(f);
        }
        break;
      case LIB3DS_MAT_MAP_VOFFSET:
        {
          map->offset[1]=lib3ds_float_read(f);
        }
        break;
      case LIB3DS_MAT_MAP_ANG:
        {
          map->rotation=lib3ds_float_read(f);
        }
        break;
      case LIB3DS_MAT_MAP_COL1:
        {
          map->tint_1[0]=1.0*lib3ds_byte_read(f)/255.0;
          map->tint_1[1]=1.0*lib3ds_byte_read(f)/255.0;
          map->tint_1[2]=1.0*lib3ds_byte_read(f)/255.0;
        }
        break;
      case LIB3DS_MAT_MAP_COL2:
        {
          map->tint_2[0]=1.0*lib3ds_byte_read(f)/255.0;
          map->tint_2[1]=1.0*lib3ds_byte_read(f)/255.0;
          map->tint_2[2]=1.0*lib3ds_byte_read(f)/255.0;
        }
        break;
      case LIB3DS_MAT_MAP_RCOL:
        {
          map->tint_r[0]=1.0*lib3ds_byte_read(f)/255.0;
          map->tint_r[1]=1.0*lib3ds_byte_read(f)/255.0;
          map->tint_r[2]=1.0*lib3ds_byte_read(f)/255.0;
        }
        break;
      case LIB3DS_MAT_MAP_GCOL:
        {
          map->tint_g[0]=1.0*lib3ds_byte_read(f)/255.0;
          map->tint_g[1]=1.0*lib3ds_byte_read(f)/255.0;
          map->tint_g[2]=1.0*lib3ds_byte_read(f)/255.0;
        }
        break;
      case LIB3DS_MAT_MAP_BCOL:
        {
          map->tint_b[0]=1.0*lib3ds_byte_read(f)/255.0;
          map->tint_b[1]=1.0*lib3ds_byte_read(f)/255.0;
          map->tint_b[2]=1.0*lib3ds_byte_read(f)/255.0;
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
 * \ingroup material
 */
void
lib3ds_material_dump(Lib3dsMaterial *material)
{
  ASSERT(material);
  printf("  %s\n", material->name);
}


/*!
 * \ingroup material
 */
Lib3dsBool
lib3ds_material_read(Lib3dsMaterial *material, FILE *f)
{
  Lib3dsChunk c;
  Lib3dsDword chunk;

  ASSERT(material);
  if (!lib3ds_chunk_start(&c, LIB3DS_MAT_ENTRY, f)) {
    return(LIB3DS_FALSE);
  }

  while ((chunk=lib3ds_chunk_next(&c, f))!=0) {
    switch (chunk) {
      case LIB3DS_MAT_NAME:
        {
          if (!lib3ds_string_read(material->name, 64, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_AMBIENT:
        {
          lib3ds_chunk_reset(&c, f);
          if (!color_read(material->ambient, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_DIFFUSE:
        {
          lib3ds_chunk_reset(&c, f);
          if (!color_read(material->diffuse, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_SPECULAR:
        {
          lib3ds_chunk_reset(&c, f);
          if (!color_read(material->specular, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_SHININESS:
        {
          material->shininess=lib3ds_float_read(f);
        }
        break;
      case LIB3DS_MAT_SHIN2PCT:
        {
          lib3ds_chunk_reset(&c, f);
          if (!int_percentage_read(&material->shin_strength, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_TRANSPARENCY:
        {
          lib3ds_chunk_reset(&c, f);
          if (!int_percentage_read(&material->transparency, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_XPFALL:
        {
          lib3ds_chunk_reset(&c, f);
          if (!int_percentage_read(&material->falloff, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_USE_XPFALL:
        {
          material->use_falloff=LIB3DS_TRUE;
        }
        break;
      case LIB3DS_MAT_REFBLUR:
        {
          lib3ds_chunk_reset(&c, f);
          if (!int_percentage_read(&material->blur, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_USE_REFBLUR:
        {
          material->use_blur=LIB3DS_TRUE;
        }
        break;
      case LIB3DS_MAT_SHADING:
        {
          material->shading=lib3ds_intw_read(f);
        }
        break;
      case LIB3DS_MAT_SELF_ILLUM:
        {
          material->self_illum=LIB3DS_TRUE;
        }
        break;
      case LIB3DS_MAT_TWO_SIDE:
        {
          material->two_sided=LIB3DS_TRUE;
        }
        break;
      case LIB3DS_MAT_DECAL:
        {
          material->map_decale=LIB3DS_TRUE;
        }
        break;
      case LIB3DS_MAT_ADDITIVE:
        {
          material->additive=LIB3DS_TRUE;
        }
        break;
      case LIB3DS_MAT_FACEMAP:
        {
          material->face_map=LIB3DS_TRUE;
        }
        break;
      case LIB3DS_MAT_PHONGSOFT:
        {
          material->soften=LIB3DS_TRUE;
        }
        break;
      case LIB3DS_MAT_WIRE:
        {
          material->use_wire=LIB3DS_TRUE;
        }
        break;
      case LIB3DS_MAT_WIREABS:
        {
          material->use_wire_abs=LIB3DS_TRUE;
        }
        break;
      case LIB3DS_MAT_WIRE_SIZE:
        {
          material->wire_size=lib3ds_float_read(f);
        }
        break;
      case LIB3DS_MAT_TEXMAP:
        {
          lib3ds_chunk_reset(&c, f);
          if (!texture_map_read(&material->texture1_map, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_TEXMASK:
        {
          lib3ds_chunk_reset(&c, f);
          if (!texture_map_read(&material->texture1_mask, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_TEX2MAP:
        {
          lib3ds_chunk_reset(&c, f);
          if (!texture_map_read(&material->texture2_map, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_TEX2MASK:
        {
          lib3ds_chunk_reset(&c, f);
          if (!texture_map_read(&material->texture2_mask, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_OPACMAP:
        {
          lib3ds_chunk_reset(&c, f);
          if (!texture_map_read(&material->opacity_map, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_OPACMASK:
        {
          lib3ds_chunk_reset(&c, f);
          if (!texture_map_read(&material->opacity_mask, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_BUMPMAP:
        {
          lib3ds_chunk_reset(&c, f);
          if (!texture_map_read(&material->bump_map, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_BUMPMASK:
        {
          lib3ds_chunk_reset(&c, f);
          if (!texture_map_read(&material->bump_mask, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_SPECMAP:
        {
          lib3ds_chunk_reset(&c, f);
          if (!texture_map_read(&material->specular_map, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_SPECMASK:
        {
          lib3ds_chunk_reset(&c, f);
          if (!texture_map_read(&material->specular_mask, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_SHINMAP:
        {
          lib3ds_chunk_reset(&c, f);
          if (!texture_map_read(&material->shininess_map, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_SHINMASK:
        {
          lib3ds_chunk_reset(&c, f);
          if (!texture_map_read(&material->shininess_mask, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_SELFIMAP:
        {
          lib3ds_chunk_reset(&c, f);
          if (!texture_map_read(&material->self_illum_map, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_SELFIMASK:
        {
          lib3ds_chunk_reset(&c, f);
          if (!texture_map_read(&material->self_illum_mask, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_REFLMAP:
        {
          lib3ds_chunk_reset(&c, f);
          if (!texture_map_read(&material->reflection_map, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_REFLMASK:
        {
          lib3ds_chunk_reset(&c, f);
          if (!texture_map_read(&material->reflection_mask, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MAT_ACUBIC:
        {
          lib3ds_intb_read(f);
          material->autorefl_map.level=lib3ds_intb_read(f);
          material->autorefl_map.flags=lib3ds_intw_read(f);
          material->autorefl_map.size=lib3ds_intd_read(f);
          material->autorefl_map.frame_step=lib3ds_intd_read(f);
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
 * \ingroup material
 */
Lib3dsBool
lib3ds_material_write(Lib3dsMaterial *material, FILE *f)
{
  /* FIXME: */
  ASSERT(0);
  return(LIB3DS_FALSE);
}


/*!

\typedef Lib3dsMaterial
  \ingroup material
  \sa _Lib3dsMaterial

*/

