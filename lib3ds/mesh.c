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
 * $Id: mesh.c,v 1.6 2000/10/11 19:53:31 jeh Exp $
 */
#define LIB3DS_EXPORT
#include <lib3ds/mesh.h>
#include <lib3ds/readwrite.h>
#include <lib3ds/chunk.h>
#include <lib3ds/vector.h>
#include <lib3ds/matrix.h>
#include <stdlib.h>
#include <config.h>
#ifdef WITH_DMALLOC
#include <dmalloc.h>
#endif


/*!
 * \defgroup mesh Meshes
 *
 * \author J.E. Hoffmann <je-h@gmx.net>
 */


static Lib3dsBool
face_array_read(Lib3dsMesh *mesh, FILE *f)
{
  Lib3dsChunk c;
  Lib3dsDword chunk;
  int i;
  int faces;

  if (!lib3ds_chunk_start(&c, LIB3DS_FACE_ARRAY, f)) {
    return(LIB3DS_FALSE);
  }
  lib3ds_mesh_free_face_list(mesh);
  
  faces=lib3ds_word_read(f);
  if (faces) {
    if (!lib3ds_mesh_new_face_list(mesh, faces)) {
      LIB3DS_ERROR_LOG;
      return(LIB3DS_FALSE);
    }
    for (i=0; i<faces; ++i) {
      strcpy(mesh->faceL[i].material, "");
      mesh->faceL[i].points[0]=lib3ds_word_read(f);
      mesh->faceL[i].points[1]=lib3ds_word_read(f);
      mesh->faceL[i].points[2]=lib3ds_word_read(f);
      mesh->faceL[i].flags=lib3ds_word_read(f);
    }
    lib3ds_chunk_tell(&c, f);

    while ((chunk=lib3ds_chunk_next(&c, f))!=0) {
      switch (chunk) {
        case LIB3DS_SMOOTH_GROUP:
          {
            int i;
            
            for (i=0; i<mesh->faces; ++i) {
              mesh->faceL[i].smoothing=lib3ds_dword_read(f);
            }
          }
          break;
        case LIB3DS_MSH_MAT_GROUP:
          {
            char name[64];
            int faces;
            int i;
            int index;

            if (!lib3ds_string_read(name, 64, f)) {
              return(LIB3DS_FALSE);
            }
            faces=lib3ds_word_read(f);
            for (i=0; i<faces; ++i) {
              index=lib3ds_word_read(f);
              ASSERT(index<mesh->faces);
              strcpy(mesh->faceL[index].material, name);
            }
          }
          break;
        case LIB3DS_MSH_BOXMAP:
          {
            char name[64];

            if (!lib3ds_string_read(name, 64, f)) {
              return(LIB3DS_FALSE);
            }
            strcpy(mesh->box_map.front, name);
            if (!lib3ds_string_read(name, 64, f)) {
              return(LIB3DS_FALSE);
            }
            strcpy(mesh->box_map.back, name);
            if (!lib3ds_string_read(name, 64, f)) {
              return(LIB3DS_FALSE);
            }
            strcpy(mesh->box_map.left, name);
            if (!lib3ds_string_read(name, 64, f)) {
              return(LIB3DS_FALSE);
            }
            strcpy(mesh->box_map.right, name);
            if (!lib3ds_string_read(name, 64, f)) {
              return(LIB3DS_FALSE);
            }
            strcpy(mesh->box_map.top, name);
            if (!lib3ds_string_read(name, 64, f)) {
              return(LIB3DS_FALSE);
            }
            strcpy(mesh->box_map.bottom, name);
          }
          break;
        default:
          lib3ds_chunk_unknown(chunk);
      }
    }
    
  }
  lib3ds_chunk_end(&c, f);
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup mesh
 */
Lib3dsMesh*
lib3ds_mesh_new(const char *name)
{
  Lib3dsMesh *mesh;

  ASSERT(name);
  ASSERT(strlen(name)<64);
  
  mesh=(Lib3dsMesh*)calloc(sizeof(Lib3dsMesh), 1);
  if (!mesh) {
    return(0);
  }
  strcpy(mesh->name, name);
  return(mesh);
}


/*!
 * \ingroup mesh
 */
void
lib3ds_mesh_free(Lib3dsMesh *mesh)
{
  lib3ds_mesh_free_point_list(mesh);
  lib3ds_mesh_free_flag_list(mesh);
  lib3ds_mesh_free_face_list(mesh);
  memset(mesh, 0, sizeof(Lib3dsMesh));
  free(mesh);
}


/*!
 * \ingroup mesh
 */
Lib3dsBool
lib3ds_mesh_new_point_list(Lib3dsMesh *mesh, Lib3dsDword points)
{
  ASSERT(mesh);
  if (mesh->pointL) {
    ASSERT(mesh->points);
    lib3ds_mesh_free_point_list(mesh);
  }
  ASSERT(!mesh->pointL && !mesh->points);
  mesh->points=0;
  mesh->pointL=calloc(sizeof(Lib3dsPoint)*points,1);
  if (!mesh->pointL) {
    LIB3DS_ERROR_LOG;
    return(LIB3DS_FALSE);
  }
  mesh->points=points;
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup mesh
 */
void
lib3ds_mesh_free_point_list(Lib3dsMesh *mesh)
{
  ASSERT(mesh);
  if (mesh->pointL) {
    ASSERT(mesh->points);
    free(mesh->pointL);
    mesh->pointL=0;
    mesh->points=0;
  }
  else {
    ASSERT(!mesh->points);
  }
}


/*!
 * \ingroup mesh
 */
Lib3dsBool
lib3ds_mesh_new_flag_list(Lib3dsMesh *mesh, Lib3dsDword flags)
{
  ASSERT(mesh);
  if (mesh->flagL) {
    ASSERT(mesh->flags);
    lib3ds_mesh_free_flag_list(mesh);
  }
  ASSERT(!mesh->flagL && !mesh->flags);
  mesh->flags=0;
  mesh->flagL=calloc(sizeof(Lib3dsWord)*flags,1);
  if (!mesh->flagL) {
    LIB3DS_ERROR_LOG;
    return(LIB3DS_FALSE);
  }
  mesh->flags=flags;
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup mesh
 */
void
lib3ds_mesh_free_flag_list(Lib3dsMesh *mesh)
{
  ASSERT(mesh);
  if (mesh->flagL) {
    ASSERT(mesh->flags);
    free(mesh->flagL);
    mesh->flagL=0;
    mesh->flags=0;
  }
  else {
    ASSERT(!mesh->flags);
  }
}


/*!
 * \ingroup mesh
 */
Lib3dsBool
lib3ds_mesh_new_face_list(Lib3dsMesh *mesh, Lib3dsDword faces)
{
  ASSERT(mesh);
  if (mesh->faceL) {
    ASSERT(mesh->faces);
    lib3ds_mesh_free_face_list(mesh);
  }
  ASSERT(!mesh->faceL && !mesh->faces);
  mesh->faces=0;
  mesh->faceL=calloc(sizeof(Lib3dsFace)*faces,1);
  if (!mesh->faceL) {
    LIB3DS_ERROR_LOG;
    return(LIB3DS_FALSE);
  }
  mesh->faces=faces;
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup mesh
 */
void
lib3ds_mesh_free_face_list(Lib3dsMesh *mesh)
{
  ASSERT(mesh);
  if (mesh->faceL) {
    ASSERT(mesh->faces);
    free(mesh->faceL);
    mesh->faceL=0;
    mesh->faces=0;
  }
  else {
    ASSERT(!mesh->faces);
  }
}


/*!
 * \ingroup mesh
 */
void
lib3ds_mesh_bounding_box(Lib3dsMesh *mesh, Lib3dsVector min, Lib3dsVector max)
{
  int i,j;
  Lib3dsFloat v;

  if (!mesh->points) {
    lib3ds_vector_zero(min);
    lib3ds_vector_zero(max);
    return;
  }
 
  lib3ds_vector_copy(min, mesh->pointL[0].pos);
  lib3ds_vector_copy(max, mesh->pointL[0].pos);
  for (i=1; i<mesh->points; ++i) {
    for (j=0; j<3; ++j) {
      v=mesh->pointL[i].pos[j];
      if (v<min[j]) {
        min[j]=v;
      }
      if (v>max[j]) {
        max[j]=v;
      }
    }
  };
}


/*!
 * \ingroup mesh
 */
void
lib3ds_mesh_dump(Lib3dsMesh *mesh)
{
  int i;
  Lib3dsVector p;

  ASSERT(mesh);
  printf("  %s vertices=%ld faces=%ld\n",
    mesh->name,
    mesh->points,
    mesh->faces
  );
  printf("  matrix:\n");
  lib3ds_matrix_dump(mesh->matrix);
  printf("  point list:\n");
  for (i=0; i<mesh->points; ++i) {
    lib3ds_vector_transform(p, mesh->matrix, mesh->pointL[i].pos);
    printf ("    %8f %8f %8f\n", p[0], p[1], p[2]);
  }
}


/*!
 * \ingroup mesh
 */
Lib3dsBool
lib3ds_mesh_read(Lib3dsMesh *mesh, FILE *f)
{
  Lib3dsChunk c;
  Lib3dsDword chunk;

  if (!lib3ds_chunk_start(&c, LIB3DS_N_TRI_OBJECT, f)) {
    return(LIB3DS_FALSE);
  }

  while ((chunk=lib3ds_chunk_next(&c, f))!=0) {
    switch (chunk) {
      case LIB3DS_MESH_MATRIX:
        {
          int i,j;
          
          lib3ds_matrix_identity(mesh->matrix);
          for (i=0; i<4; i++) {
            for (j=0; j<3; j++) {
              mesh->matrix[i][j]=lib3ds_float_read(f);
            }
          }
        }
        break;
      case LIB3DS_MESH_COLOR:
        {
          mesh->color=lib3ds_byte_read(f);
        }
        break;
      case LIB3DS_POINT_ARRAY:
        {
          int i,j;
          int points;
          
          lib3ds_mesh_free_point_list(mesh);
          points=lib3ds_word_read(f);
          if (points) {
            if (!lib3ds_mesh_new_point_list(mesh, points)) {
              LIB3DS_ERROR_LOG;
              return(LIB3DS_FALSE);
            }
            for (i=0; i<mesh->points; ++i) {
              for (j=0; j<3; ++j) {
                mesh->pointL[i].pos[j]=lib3ds_float_read(f);
              }
            }
            ASSERT((!mesh->flags) || (mesh->points==mesh->flags));
          }
        }
        break;
      case LIB3DS_POINT_FLAG_ARRAY:
        {
          int i;
          int flags;
          
          lib3ds_mesh_free_flag_list(mesh);
          flags=lib3ds_word_read(f);
          if (flags) {
            if (!lib3ds_mesh_new_flag_list(mesh, flags)) {
              LIB3DS_ERROR_LOG;
              return(LIB3DS_FALSE);
            }
            for (i=0; i<mesh->flags; ++i) {
              mesh->flagL[i]=lib3ds_word_read(f);
            }
            ASSERT((!mesh->points) || (mesh->points==mesh->flags));
          }
        }
        break;
      case LIB3DS_FACE_ARRAY:
        {
          lib3ds_chunk_reset(&c, f);
          if (!face_array_read(mesh, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_MESH_TEXTURE_INFO:
        {
        }
        break;
      case LIB3DS_TEX_VERTS:
        {
        }
        break;
      default:
        lib3ds_chunk_unknown(chunk);
    }
  }
  {
    Lib3dsMatrix M;
    Lib3dsVector v;
    int j;

    lib3ds_matrix_copy(M, mesh->matrix);
    if (lib3ds_matrix_inv(M)) {
      for (j=0; j<mesh->points; ++j) {
        lib3ds_vector_copy(v, mesh->pointL[j].pos);
        lib3ds_vector_transform(mesh->pointL[j].pos, M, v);
      }
    }
  }
  {
    int j;

    for (j=0; j<mesh->faces; ++j) {
      ASSERT(mesh->faceL[j].points[0]<mesh->points);
      ASSERT(mesh->faceL[j].points[1]<mesh->points);
      ASSERT(mesh->faceL[j].points[2]<mesh->points);
      lib3ds_vector_normal(
        mesh->faceL[j].normal,
        mesh->pointL[mesh->faceL[j].points[0]].pos,
        mesh->pointL[mesh->faceL[j].points[1]].pos,
        mesh->pointL[mesh->faceL[j].points[2]].pos
      );
    }
  }
  
  lib3ds_chunk_end(&c, f);
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup mesh
 */
Lib3dsBool
lib3ds_mesh_write(Lib3dsMesh *mesh, FILE *f)
{
  /* FIXME: */
  ASSERT(0);
  return(LIB3DS_FALSE);
}


/*!

\typedef Lib3dsFace
  \ingroup mesh
  \sa _Lib3dsFace

*/
/*!

\typedef Lib3dsBoxMap
  \ingroup mesh
  \sa _Lib3dsBoxMap

*/
/*!

\typedef Lib3dsMapData
  \ingroup mesh
  \sa _Lib3dsMapData

*/
/*!

\typedef Lib3dsMesh
  \ingroup mesh
  \sa _Lib3dsMesh

*/
