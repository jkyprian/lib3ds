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
 * $Id: light.c,v 1.3 2000/10/09 12:33:50 jeh Exp $
 */
#define LIB3DS_EXPORT
#include <lib3ds/light.h>
#include <lib3ds/chunk.h>
#include <lib3ds/readwrite.h>
#include <stdlib.h>
#include <math.h>
#include <config.h>
#ifdef WITH_DMALLOC
#include <dmalloc.h>
#endif


/*!
 * \defgroup light Lights
 *
 * \author J.E. Hoffmann <je-h@gmx.net>
 */


/*!
 * \ingroup light
 */
Lib3dsLight*
lib3ds_light_new(const char *name)
{
  Lib3dsLight *light;

  ASSERT(name);
  ASSERT(strlen(name)<64);
  
  light=(Lib3dsLight*)calloc(sizeof(Lib3dsLight), 1);
  if (!light) {
    return(0);
  }
  strcpy(light->name, name);
  return(light);
}


/*!
 * \ingroup light
 */
void
lib3ds_light_free(Lib3dsLight *light)
{
  memset(light, 0, sizeof(Lib3dsLight));
  free(light);
}


/*!
 * \ingroup light
 */
Lib3dsBool
lib3ds_light_read(Lib3dsLight *light, FILE *f)
{
  Lib3dsChunk c;
  Lib3dsDword chunk;

  if (!lib3ds_chunk_start(&c, LIB3DS_N_DIRECT_LIGHT, f)) {
    return(LIB3DS_FALSE);
  }
  {
    int i;
    for (i=0; i<3; ++i) {
      light->position[i]=lib3ds_float_read(f);
    }
  }
  lib3ds_chunk_tell(&c, f);
  
  while ((chunk=lib3ds_chunk_next(&c, f))!=0) {
    switch (chunk) {
      case LIB3DS_COLOR_F:
        {
          int i;
          for (i=0; i<3; ++i) {
            light->color[i]=lib3ds_float_read(f);
          }
        }
        break;
      case LIB3DS_DL_OFF:
        {
          light->off=LIB3DS_TRUE;
        }
        break;
      case LIB3DS_DL_OUTER_RANGE:
        {
          light->outer_range=lib3ds_float_read(f);
        }
        break;
      case LIB3DS_DL_INNER_RANGE:
        {
          light->inner_range=lib3ds_float_read(f);
        }
        break;
      case LIB3DS_DL_MULTIPLIER:
        {
          light->multiplier=lib3ds_float_read(f);
        }
        break;
      case LIB3DS_DL_EXCLUDE:
        {
          /* FIXME: */
          lib3ds_chunk_unknown(chunk);
        }
      case LIB3DS_DL_ATTENUATE:
        {
          light->attenuation=lib3ds_float_read(f);
        }
        break;
      case LIB3DS_DL_SPOTLIGHT:
        {
          int i;
          light->spot_light=LIB3DS_TRUE;
          for (i=0; i<3; ++i) {
            light->spot[i]=lib3ds_float_read(f);
          }
          light->hot_spot=lib3ds_float_read(f);
          light->fall_off=lib3ds_float_read(f);
        }
        break;
      case LIB3DS_DL_SPOT_ROLL:
        {
          light->roll=lib3ds_float_read(f);
        }
        break;
      case LIB3DS_DL_SHADOWED:
        {
          light->shadowed=LIB3DS_TRUE;
        }
        break;
      case LIB3DS_DL_LOCAL_SHADOW2:
        {
          light->shadow_bias=lib3ds_float_read(f);
          light->shadow_filter=lib3ds_float_read(f);
          light->shadow_size=lib3ds_word_read(f);
        }
        break;
      case LIB3DS_DL_SEE_CONE:
        {
          light->see_cone=LIB3DS_TRUE;
        }
        break;
      case LIB3DS_DL_SPOT_RECTANGULAR:
        {
          light->rectangular_spot=LIB3DS_TRUE;
        }
        break;
      case LIB3DS_DL_SPOT_ASPECT:
        {
          light->spot_aspect=lib3ds_float_read(f);
        }
        break;
      case LIB3DS_DL_SPOT_PROJECTOR:
        {
          light->use_projector=LIB3DS_TRUE;
          if (!lib3ds_string_read(light->projector, 64, f)) {
            return(LIB3DS_FALSE);
          }
        }
      case LIB3DS_DL_SPOT_OVERSHOOT:
        {
          light->spot_overshoot=LIB3DS_TRUE;
        }
        break;
      case LIB3DS_DL_RAY_BIAS:
        {
          light->ray_bias=lib3ds_float_read(f);
        }
        break;
      case LIB3DS_DL_RAYSHAD:
        {
          light->ray_shadows=LIB3DS_TRUE;
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
 * \ingroup light
 */
Lib3dsBool
lib3ds_light_write(Lib3dsLight *light, FILE *f)
{
  /* FIXME: */
  ASSERT(0);
  return(LIB3DS_FALSE);
}


/*!

\typedef Lib3dsLight
  \ingroup light
  \sa _Lib3dsLight

*/

