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
 * $Id: file.c,v 1.9 2000/10/19 17:35:35 jeh Exp $
 */
#define LIB3DS_EXPORT
#include <lib3ds/file.h>
#include <lib3ds/chunk.h>
#include <lib3ds/readwrite.h>
#include <lib3ds/material.h>
#include <lib3ds/mesh.h>
#include <lib3ds/camera.h>
#include <lib3ds/light.h>
#include <lib3ds/node.h>
#include <stdlib.h>
#include <string.h>
#include <config.h>
#ifdef WITH_DMALLOC
#include <dmalloc.h>
#endif


/*!
 * \defgroup file Files
 *
 * \author J.E. Hoffmann <je-h@gmx.net>
 */


static Lib3dsBool
named_object_read(Lib3dsFile *file, FILE *f)
{
  Lib3dsChunk c;
  char name[64];
  Lib3dsWord chunk;

  if (!lib3ds_chunk_start(&c, LIB3DS_NAMED_OBJECT, f)) {
    return(LIB3DS_FALSE);
  }
  if (!lib3ds_string_read(name, 64, f)) {
    return(LIB3DS_FALSE);
  }
  lib3ds_chunk_tell(&c, f);

  while ((chunk=lib3ds_chunk_next(&c, f))!=0) {
    switch (chunk) {
      case LIB3DS_N_TRI_OBJECT:
        {
          Lib3dsMesh *mesh;

          mesh=lib3ds_mesh_new(name);
          if (!mesh) {
            return(LIB3DS_FALSE);
          }
          lib3ds_chunk_reset(&c, f);
          if (!lib3ds_mesh_read(mesh, f)) {
            return(LIB3DS_FALSE);
          }
          lib3ds_file_insert_mesh(file, mesh);
        }
        break;
      case LIB3DS_N_CAMERA:
        {
          Lib3dsCamera *camera;

          camera=lib3ds_camera_new(name);
          if (!camera) {
            return(LIB3DS_FALSE);
          }
          lib3ds_chunk_reset(&c, f);
          if (!lib3ds_camera_read(camera, f)) {
            return(LIB3DS_FALSE);
          }
          lib3ds_file_insert_camera(file, camera);
        }
        break;
      case LIB3DS_N_DIRECT_LIGHT:
        {
          Lib3dsLight *light;

          light=lib3ds_light_new(name);
          if (!light) {
            return(LIB3DS_FALSE);
          }
          lib3ds_chunk_reset(&c, f);
          if (!lib3ds_light_read(light, f)) {
            return(LIB3DS_FALSE);
          }
          lib3ds_file_insert_light(file, light);
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
ambient_read(Lib3dsFile *file, FILE *f)
{
  Lib3dsChunk c;
  Lib3dsWord chunk;

  if (!lib3ds_chunk_start(&c, LIB3DS_AMBIENT_LIGHT, f)) {
    return(LIB3DS_FALSE);
  }

  while ((chunk=lib3ds_chunk_next(&c, f))!=0) {
    switch (chunk) {
      case LIB3DS_COLOR_F:
        {
          int i;
          for (i=0; i<3; ++i) {
            file->ambient[i]=lib3ds_float_read(f);
          }
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
mdata_read(Lib3dsFile *file, FILE *f)
{
  Lib3dsChunk c;
  Lib3dsWord chunk;

  if (!lib3ds_chunk_start(&c, LIB3DS_MDATA, f)) {
    return(LIB3DS_FALSE);
  }
  
  while ((chunk=lib3ds_chunk_next(&c, f))!=0) {
    switch (chunk) {
      case LIB3DS_MESH_VERSION:
        {
          lib3ds_chunk_unknown(chunk);
        }
        break;
      case LIB3DS_MASTER_SCALE:
        {
          file->master_scale=lib3ds_float_read(f);
        }
        break;
      case LIB3DS_SHADOW_MAP_SIZE:
      case LIB3DS_LO_SHADOW_BIAS:
      case LIB3DS_HI_SHADOW_BIAS:
      case LIB3DS_SHADOW_SAMPLES:
      case LIB3DS_SHADOW_RANGE:
      case LIB3DS_SHADOW_FILTER:
      case LIB3DS_RAY_BIAS:
        {
          lib3ds_chunk_reset(&c, f);
          if (!lib3ds_shadow_read(&file->shadow, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_O_CONSTS:
        {
          int i;
          for (i=0; i<3; ++i) {
            file->construction_plane[i]=lib3ds_float_read(f);
          }
        }
        break;
      case LIB3DS_AMBIENT_LIGHT:
        {
          lib3ds_chunk_reset(&c, f);
          if (!ambient_read(file, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_BIT_MAP:
      case LIB3DS_SOLID_BGND:
      case LIB3DS_V_GRADIENT:
      case LIB3DS_USE_BIT_MAP:
      case LIB3DS_USE_SOLID_BGND:
      case LIB3DS_USE_V_GRADIENT:
        {
          lib3ds_chunk_reset(&c, f);
          if (!lib3ds_background_read(&file->background, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_FOG:
      case LIB3DS_LAYER_FOG:
      case LIB3DS_DISTANCE_CUE:
      case LIB3DS_USE_FOG:
      case LIB3DS_USE_LAYER_FOG:
      case LIB3DS_USE_DISTANCE_CUE:
        {
          lib3ds_chunk_reset(&c, f);
          if (!lib3ds_atmosphere_read(&file->atmosphere, f)) {
            return(LIB3DS_FALSE);
          }
        }
        break;
      case LIB3DS_VIEWPORT_LAYOUT:
      case LIB3DS_DEFAULT_VIEW:
        {
          lib3ds_chunk_unknown(chunk);
        }
        break;
      case LIB3DS_MAT_ENTRY:
        {
          Lib3dsMaterial *material;

          material=lib3ds_material_new();
          if (!material) {
            return(LIB3DS_FALSE);
          }
          lib3ds_chunk_reset(&c, f);
          if (!lib3ds_material_read(material, f)) {
            return(LIB3DS_FALSE);
          }
          lib3ds_file_insert_material(file, material);
        }
        break;
      case LIB3DS_NAMED_OBJECT:
        {
          lib3ds_chunk_reset(&c, f);
          if (!named_object_read(file, f)) {
            return(LIB3DS_FALSE);
          }
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
kfdata_read(Lib3dsFile *file, FILE *f)
{
  Lib3dsChunk c;
  Lib3dsWord chunk;

  if (!lib3ds_chunk_start(&c, LIB3DS_KFDATA, f)) {
    return(LIB3DS_FALSE);
  }
  
  while ((chunk=lib3ds_chunk_next(&c, f))!=0) {
    switch (chunk) {
      case LIB3DS_KFHDR:
        {
          char s[64];
          file->keyf_revision=lib3ds_word_read(f);
          if (!lib3ds_string_read(s, 64, f)) {
            return(LIB3DS_FALSE);
          }
          file->frames=lib3ds_intd_read(f);
        }
        break;
      case LIB3DS_KFSEG:
        {
          file->segment_from=lib3ds_intd_read(f);
          file->segment_to=lib3ds_intd_read(f);
        }
        break;
      case LIB3DS_KFCURTIME:
        {
          file->current_frame=lib3ds_intd_read(f);
        }
        break;
      case LIB3DS_AMBIENT_NODE_TAG:
        {
          Lib3dsNode *node;

          node=lib3ds_node_new_ambient();
          if (!node) {
            return(LIB3DS_FALSE);
          }
          lib3ds_chunk_reset(&c, f);
          if (!lib3ds_node_read(node, f)) {
            return(LIB3DS_FALSE);
          }
          lib3ds_file_insert_node(file, node);
        }
        break;
      case LIB3DS_OBJECT_NODE_TAG:
        {
          Lib3dsNode *node;

          node=lib3ds_node_new_object();
          if (!node) {
            return(LIB3DS_FALSE);
          }
          lib3ds_chunk_reset(&c, f);
          if (!lib3ds_node_read(node, f)) {
            return(LIB3DS_FALSE);
          }
          lib3ds_file_insert_node(file, node);
        }
        break;
      case LIB3DS_CAMERA_NODE_TAG:
        {
          Lib3dsNode *node;

          node=lib3ds_node_new_camera();
          if (!node) {
            return(LIB3DS_FALSE);
          }
          lib3ds_chunk_reset(&c, f);
          if (!lib3ds_node_read(node, f)) {
            return(LIB3DS_FALSE);
          }
          lib3ds_file_insert_node(file, node);
        }
        break;
      case LIB3DS_TARGET_NODE_TAG:
        {
          Lib3dsNode *node;

          node=lib3ds_node_new_target();
          if (!node) {
            return(LIB3DS_FALSE);
          }
          lib3ds_chunk_reset(&c, f);
          if (!lib3ds_node_read(node, f)) {
            return(LIB3DS_FALSE);
          }
          lib3ds_file_insert_node(file, node);
        }
        break;
      case LIB3DS_LIGHT_NODE_TAG:
      case LIB3DS_SPOTLIGHT_NODE_TAG:
        {
          Lib3dsNode *node;

          node=lib3ds_node_new_light();
          if (!node) {
            return(LIB3DS_FALSE);
          }
          lib3ds_chunk_reset(&c, f);
          if (!lib3ds_node_read(node, f)) {
            return(LIB3DS_FALSE);
          }
          lib3ds_file_insert_node(file, node);
        }
        break;
      case LIB3DS_L_TARGET_NODE_TAG:
        {
          Lib3dsNode *node;

          node=lib3ds_node_new_spot();
          if (!node) {
            return(LIB3DS_FALSE);
          }
          lib3ds_chunk_reset(&c, f);
          if (!lib3ds_node_read(node, f)) {
            return(LIB3DS_FALSE);
          }
          lib3ds_file_insert_node(file, node);
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
 * \ingroup file
 */
Lib3dsFile*
lib3ds_open(const char *filename)
{
  FILE *f;
  Lib3dsFile *file;

  f=fopen(filename, "rb");
  if (!f) {
    return(0);
  }
  file=lib3ds_file_new();
  if (!file) {
    fclose(f);
    return(0);
  }
  
  if (!lib3ds_file_read(file, f)) {
    free(file);
    fclose(f);
    return(0);
  }
  fclose(f);
  return(file);
}


/*!
 * \ingroup file
 */
void
lib3ds_close(Lib3dsFile *file)
{
  lib3ds_file_free(file);
}


/*!
 * \ingroup file
 */
Lib3dsFile*
lib3ds_file_new()
{
  Lib3dsFile *file;

  file=(Lib3dsFile*)calloc(sizeof(Lib3dsFile),1);
  if (!file) {
    return(0);
  }
  return(file);
  
}


/*!
 * \ingroup file
 */
void
lib3ds_file_free(Lib3dsFile* file)
{
  ASSERT(file);
  {
    Lib3dsMaterial *p,*q;
    
    for (p=file->materials; p; p=q) {
      q=p->next;
      lib3ds_material_free(p);
    }
    file->materials=0;
  }
  {
    Lib3dsCamera *p,*q;
    
    for (p=file->cameras; p; p=q) {
      q=p->next;
      lib3ds_camera_free(p);
    }
    file->cameras=0;
  }
  {
    Lib3dsLight *p,*q;
    
    for (p=file->lights; p; p=q) {
      q=p->next;
      lib3ds_light_free(p);
    }
    file->lights=0;
  }
  {
    Lib3dsMesh *p,*q;
    
    for (p=file->meshes; p; p=q) {
      q=p->next;
      lib3ds_mesh_free(p);
    }
    file->meshes=0;
  }
  {
    Lib3dsNode *p,*q;
  
    for (p=file->nodes; p; p=q) {
      q=p->next;
      lib3ds_node_free(p);
    }
  }
  free(file);
}


/*!
 * \ingroup file
 */
void
lib3ds_file_eval(Lib3dsFile *file, Lib3dsFloat t)
{
  Lib3dsNode *p;

  for (p=file->nodes; p!=0; p=p->next) {
    lib3ds_node_eval(p, t);
  }
}


/*!
 * \ingroup file
 */
Lib3dsBool
lib3ds_file_read(Lib3dsFile *file, FILE *f)
{
  Lib3dsChunk c;
  Lib3dsWord chunk;

  if (!lib3ds_chunk_start(&c, 0, f)) {
    return(LIB3DS_FALSE);
  }
  lib3ds_chunk_unknown(c.chunk);
  switch (c.chunk) {
    case LIB3DS_MDATA:
      {
        lib3ds_chunk_reset(&c, f);
        if (!mdata_read(file, f)) {
          return(LIB3DS_FALSE);
        }
      }
      break;
    case LIB3DS_M3DMAGIC:
    case LIB3DS_MLIBMAGIC:
    case LIB3DS_CMAGIC:
      {
        while ((chunk=lib3ds_chunk_next(&c, f))!=0) {
          switch (chunk) {
            case LIB3DS_M3D_VERSION:
              {
                file->mesh_version=lib3ds_dword_read(f);
              }
              break;
            case LIB3DS_MDATA:
              {
                lib3ds_chunk_reset(&c, f);
                if (!mdata_read(file, f)) {
                  return(LIB3DS_FALSE);
                }
              }
              break;
            case LIB3DS_KFDATA:
              {
                lib3ds_chunk_reset(&c, f);
                if (!kfdata_read(file, f)) {
                  return(LIB3DS_FALSE);
                }
              }
              break;
            default:
              lib3ds_chunk_unknown(chunk);
          }
        }
      }
      break;
    default:
      return(LIB3DS_FALSE);
  }

  lib3ds_chunk_end(&c, f);
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup file
 */
Lib3dsBool
lib3ds_file_write(Lib3dsFile *file, FILE *f)
{
  /* FIXME: */
  ASSERT(0);
  return(LIB3DS_FALSE);
}


/*!
 * \ingroup file
 */
void
lib3ds_file_insert_material(Lib3dsFile *file, Lib3dsMaterial *material)
{
  Lib3dsMaterial *p,*q;
  
  ASSERT(file);
  ASSERT(material);
  ASSERT(!material->next);

  q=0;
  for (p=file->materials; p!=0; p=p->next) {
    if (strcmp(material->name, p->name)<0) {
      break;
    }
    q=p;
  }
  if (!q) {
    material->next=file->materials;
    file->materials=material;
  }
  else {
    material->next=q->next;
    q->next=material;
  }
}


/*!
 * \ingroup file
 */
void
lib3ds_file_remove_material(Lib3dsFile *file, Lib3dsMaterial *material)
{
  Lib3dsMaterial *p,*q;

  ASSERT(file);
  ASSERT(material);
  ASSERT(file->materials);
  for (p=0,q=file->materials; q; p=q,q=q->next) {
    if (q==material) {
      break;
    }
  }
  if (!q) {
    ASSERT(LIB3DS_FALSE);
    return;
  }
  if (!p) {
    file->materials=material->next;
  }
  else {
    p->next=q->next;
  }
  material->next=0;
}


/*!
 * \ingroup file
 */
Lib3dsMaterial*
lib3ds_file_material_by_name(Lib3dsFile *file, const char *name)
{
  Lib3dsMaterial *p;

  ASSERT(file);
  for (p=file->materials; p!=0; p=p->next) {
    if (strcmp(p->name,name)==0) {
      return(p);
    }
  }
  return(0);
}


/*!
 * \ingroup file
 */
void
lib3ds_file_dump_materials(Lib3dsFile *file)
{
  Lib3dsMaterial *p;

  ASSERT(file);
  for (p=file->materials; p!=0; p=p->next) {
    lib3ds_material_dump(p);
  }
}


/*!
 * \ingroup file
 */
void
lib3ds_file_insert_mesh(Lib3dsFile *file, Lib3dsMesh *mesh)
{
  Lib3dsMesh *p,*q;
  
  ASSERT(file);
  ASSERT(mesh);
  ASSERT(!mesh->next);

  q=0;
  for (p=file->meshes; p!=0; p=p->next) {
    if (strcmp(mesh->name, p->name)<0) {
      break;
    }
    q=p;
  }
  if (!q) {
    mesh->next=file->meshes;
    file->meshes=mesh;
  }
  else {
    mesh->next=q->next;
    q->next=mesh;
  }
}


/*!
 * \ingroup file
 */
void
lib3ds_file_remove_mesh(Lib3dsFile *file, Lib3dsMesh *mesh)
{
  Lib3dsMesh *p,*q;

  ASSERT(file);
  ASSERT(mesh);
  ASSERT(file->meshes);
  for (p=0,q=file->meshes; q; p=q,q=q->next) {
    if (q==mesh) {
      break;
    }
  }
  if (!q) {
    ASSERT(LIB3DS_FALSE);
    return;
  }
  if (!p) {
    file->meshes=mesh->next;
  }
  else {
    p->next=q->next;
  }
  mesh->next=0;
}


/*!
 * \ingroup file
 */
Lib3dsMesh*
lib3ds_file_mesh_by_name(Lib3dsFile *file, const char *name)
{
  Lib3dsMesh *p;

  ASSERT(file);
  for (p=file->meshes; p!=0; p=p->next) {
    if (strcmp(p->name,name)==0) {
      return(p);
    }
  }
  return(0);
}


/*!
 * \ingroup file
 */
void
lib3ds_file_dump_meshes(Lib3dsFile *file)
{
  Lib3dsMesh *p;

  ASSERT(file);
  for (p=file->meshes; p!=0; p=p->next) {
    lib3ds_mesh_dump(p);
  }
}


static void
dump_instances(Lib3dsNode *node, const char* parent)
{
  Lib3dsNode *p;
  char name[255];

  ASSERT(node);
  ASSERT(parent);
  strcpy(name, parent);
  strcat(name, ".");
  strcat(name, node->name);
  if (node->type==LIB3DS_OBJECT_NODE) {
    printf("  %s : %s\n", name, node->data.object.instance);
  }
  for (p=node->childs; p!=0; p=p->next) {
    dump_instances(p, parent);
  }
}


/*!
 * \ingroup file
 */
void
lib3ds_file_dump_instances(Lib3dsFile *file)
{
  Lib3dsNode *p;

  ASSERT(file);
  for (p=file->nodes; p!=0; p=p->next) {
    dump_instances(p,"");
  }
}


/*!
 * \ingroup file
 */
void
lib3ds_file_insert_camera(Lib3dsFile *file, Lib3dsCamera *camera)
{
  Lib3dsCamera *p,*q;
  
  ASSERT(file);
  ASSERT(camera);
  ASSERT(!camera->next);

  q=0;
  for (p=file->cameras; p!=0; p=p->next) {
    if (strcmp(camera->name, p->name)<0) {
      break;
    }
    q=p;
  }
  if (!q) {
    camera->next=file->cameras;
    file->cameras=camera;
  }
  else {
    camera->next=q->next;
    q->next=camera;
  }
}


/*!
 * \ingroup file
 */
void
lib3ds_file_remove_camera(Lib3dsFile *file, Lib3dsCamera *camera)
{
  Lib3dsCamera *p,*q;

  ASSERT(file);
  ASSERT(camera);
  ASSERT(file->cameras);
  for (p=0,q=file->cameras; q; p=q,q=q->next) {
    if (q==camera) {
      break;
    }
  }
  if (!q) {
    ASSERT(LIB3DS_FALSE);
    return;
  }
  if (!p) {
    file->cameras=camera->next;
  }
  else {
    p->next=q->next;
  }
  camera->next=0;
}


/*!
 * \ingroup file
 */
Lib3dsCamera*
lib3ds_file_camera_by_name(Lib3dsFile *file, const char *name)
{
  Lib3dsCamera *p;

  ASSERT(file);
  for (p=file->cameras; p!=0; p=p->next) {
    if (strcmp(p->name,name)==0) {
      return(p);
    }
  }
  return(0);
}


/*!
 * \ingroup file
 */
void
lib3ds_file_dump_cameras(Lib3dsFile *file)
{
  Lib3dsCamera *p;

  ASSERT(file);
  for (p=file->cameras; p!=0; p=p->next) {
    printf("  %s\n", p->name);
  }
}


/*!
 * \ingroup file
 */
void
lib3ds_file_insert_light(Lib3dsFile *file, Lib3dsLight *light)
{
  Lib3dsLight *p,*q;
  
  ASSERT(file);
  ASSERT(light);
  ASSERT(!light->next);

  q=0;
  for (p=file->lights; p!=0; p=p->next) {
    if (strcmp(light->name, p->name)<0) {
      break;
    }
    q=p;
  }
  if (!q) {
    light->next=file->lights;
    file->lights=light;
  }
  else {
    light->next=q->next;
    q->next=light;
  }
}


/*!
 * \ingroup file
 */
void
lib3ds_file_remove_light(Lib3dsFile *file, Lib3dsLight *light)
{
  Lib3dsLight *p,*q;

  ASSERT(file);
  ASSERT(light);
  ASSERT(file->lights);
  for (p=0,q=file->lights; q; p=q,q=q->next) {
    if (q==light) {
      break;
    }
  }
  if (!q) {
    ASSERT(LIB3DS_FALSE);
    return;
  }
  if (!p) {
    file->lights=light->next;
  }
  else {
    p->next=q->next;
  }
  light->next=0;
}


/*!
 * \ingroup file
 */
Lib3dsLight*
lib3ds_file_light_by_name(Lib3dsFile *file, const char *name)
{
  Lib3dsLight *p;

  ASSERT(file);
  for (p=file->lights; p!=0; p=p->next) {
    if (strcmp(p->name,name)==0) {
      return(p);
    }
  }
  return(0);
}


/*!
 * \ingroup file
 */
void
lib3ds_file_dump_lights(Lib3dsFile *file)
{
  Lib3dsLight *p;

  ASSERT(file);
  for (p=file->lights; p!=0; p=p->next) {
    printf("  %s\n", p->name);
  }
}


/*!
 * \ingroup file
 */
Lib3dsNode*
lib3ds_file_node_by_name(Lib3dsFile *file, const char* name, Lib3dsNodeTypes type)
{
  Lib3dsNode *p,*q;

  ASSERT(file);
  for (p=file->nodes; p!=0; p=p->next) {
    if ((p->type==type) && (strcmp(p->name, name)==0)) {
      return(p);
    }
    q=lib3ds_node_by_name(p, name, type);
    if (q) {
      return(q);
    }
  }
  return(0);
}


/*!
 * \ingroup file
 */
Lib3dsNode*
lib3ds_file_node_by_id(Lib3dsFile *file, Lib3dsWord id)
{
  Lib3dsNode *p,*q;

  ASSERT(file);
  for (p=file->nodes; p!=0; p=p->next) {
    if (p->id==id) {
      return(p);
    }
    q=lib3ds_node_by_id(p, id);
    if (q) {
      return(q);
    }
  }
  return(0);
}


/*!
 * \ingroup file
 */
void
lib3ds_file_insert_node(Lib3dsFile *file, Lib3dsNode *node)
{
  Lib3dsNode *parent,*p,*n;
  
  ASSERT(node);
  ASSERT(!node->next);
  ASSERT(!node->parent);

  parent=0;
  if (node->parent_id!=LIB3DS_NO_PARENT) {
    parent=lib3ds_file_node_by_id(file, node->parent_id);
  }
  node->parent=parent;
  
  if (!parent) {
    for (p=0,n=file->nodes; n!=0; p=n,n=n->next) {
      if (strcmp(n->name, node->name)>0) {
        break;
      }
    }
    if (!p) {
      node->next=file->nodes;
      file->nodes=node;
    }
    else {
      node->next=p->next;
      p->next=node;
    }
  }
  else {
    for (p=0,n=parent->childs; n!=0; p=n,n=n->next) {
      if (strcmp(n->name, node->name)>0) {
        break;
      }
    }
    if (!p) {
      node->next=parent->childs;
      parent->childs=node;
    }
    else {
      node->next=p->next;
      p->next=node;
    }
  }

  if (node->id!=LIB3DS_NO_PARENT) {
    for (n=file->nodes; n!=0; n=p) {
      p=n->next;
      if (n->parent_id==node->id) {
        lib3ds_file_remove_node(file, n);
        lib3ds_file_insert_node(file, n);
      }
    }
  }
}


/*!
 * \ingroup file
 */
Lib3dsBool
lib3ds_file_remove_node(Lib3dsFile *file, Lib3dsNode *node)
{
  Lib3dsNode *p,*n;

  if (node->parent) {
    for (p=0,n=node->parent->childs; n; p=n,n=n->next) {
      if (n==node) {
        break;
      }
    }
    if (!n) {
      return(LIB3DS_FALSE);
    }
    
    if (!p) {
      node->parent->childs=n->next;
    }
    else {
      p->next=n->next;
    }
  }
  else {
    for (p=0,n=file->nodes; n; p=n,n=n->next) {
      if (n==node) {
        break;
      }
    }
    if (!n) {
      return(LIB3DS_FALSE);
    }
    
    if (!p) {
      file->nodes=n->next;
    }
    else {
      p->next=n->next;
    }
  }
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup file
 */
void
lib3ds_file_dump_nodes(Lib3dsFile *file)
{
  Lib3dsNode *p;

  ASSERT(file);
  for (p=file->nodes; p!=0; p=p->next) {
    lib3ds_node_dump(p,1);
  }
}


/*!

\typedef Lib3dsFile
  \ingroup file
  \sa _Lib3dsFile

*/
