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
 * $Id: chunk.c,v 1.5 2000/10/11 19:53:31 jeh Exp $
 */
#define LIB3DS_EXPORT
#include <lib3ds/chunk.h>
#include <lib3ds/readwrite.h>
#include <lib3ds/chunktable.h>
#include <string.h>


/*!
 * \defgroup chunk Chunk Handling
 *
 * \author J.E. Hoffmann <je-h@gmx.net>
 */


#ifdef _DEBUG
static char lib3ds_chunk_level[128]="";


static void
lib3ds_chunk_debug_enter(Lib3dsChunk *c)
{
  strcat(lib3ds_chunk_level, "  ");
}


static void
lib3ds_chunk_debug_leave(Lib3dsChunk *c)
{
  lib3ds_chunk_level[strlen(lib3ds_chunk_level)-2]=0;
}


static void
lib3ds_chunk_debug_dump(Lib3dsChunk *c)
{
  /*
  fprintf(stderr, "%s%s (0x%X) size=%lu\n",
    lib3ds_chunk_level,
    lib3ds_chunk_name(c->chunk),
    c->chunk,
    c->size
    );*/
}
#endif


/*!
 * \ingroup chunk
 *
 * Reads a 3d-Studio chunk header from a little endian file stream.
 *
 * \param c  The chunk to store the data.
 * \param f  The file stream.
 *
 * \return   True on success, False otherwise.
 */
Lib3dsBool
lib3ds_chunk_read(Lib3dsChunk *c, FILE *f)
{
  ASSERT(c);
  ASSERT(f);
  c->cur=ftell(f);
  c->chunk=lib3ds_word_read(f);
  c->size=lib3ds_dword_read(f);
  c->end=c->cur+c->size;
  c->cur+=6;
  if (ferror(f)) {
    return(LIB3DS_FALSE);
  }
  return(LIB3DS_TRUE);
  
}


/*!
 * \ingroup chunk
 */
Lib3dsBool
lib3ds_chunk_start(Lib3dsChunk *c, Lib3dsWord chunk, FILE *f)
{
  ASSERT(c);
  ASSERT(f);
  if (!lib3ds_chunk_read(c, f)) {
    return(LIB3DS_FALSE);
  }
  #ifdef _DEBUG
  lib3ds_chunk_debug_enter(c);
  #endif
  return((chunk==0) || (c->chunk==chunk));
}


/*!
 * \ingroup chunk
 */
void
lib3ds_chunk_tell(Lib3dsChunk *c, FILE *f)
{
  c->cur=ftell(f);
}


/*!
 * \ingroup chunk
 */
Lib3dsDword
lib3ds_chunk_next(Lib3dsChunk *c, FILE *f)
{
  Lib3dsChunk d;

  if (c->cur>=c->end) {
    ASSERT(c->cur==c->end);
    return(0);
  }

  fseek(f, c->cur, SEEK_SET);
  d.chunk=lib3ds_word_read(f);
  d.size=lib3ds_dword_read(f);
  #ifdef _DEBUG
  lib3ds_chunk_debug_dump(&d);
  #endif
  c->cur+=d.size;
  return(d.chunk);
}


/*!
 * \ingroup chunk
 */
void
lib3ds_chunk_reset(Lib3dsChunk *c, FILE *f)
{
  fseek(f, -6, SEEK_CUR);
}


/*!
 * \ingroup chunk
 */
void
lib3ds_chunk_end(Lib3dsChunk *c, FILE *f)
{
  #ifdef _DEBUG
  lib3ds_chunk_debug_leave(c);
  #endif
  fseek(f, c->end, SEEK_SET);
}


/*!
 * \ingroup chunk
 *
 * Writes a 3d-Studio chunk header into a little endian file stream.
 *
 * \param c  The chunk to be written.
 * \param f  The file stream.
 *
 * \return   True on success, False otherwise.
 */
Lib3dsBool
lib3ds_chunk_write(Lib3dsChunk *c, FILE *f)
{
  ASSERT(c);
  if (!lib3ds_word_write(c->chunk, f)) {
    return(LIB3DS_FALSE);
  }
  if (!lib3ds_dword_write(c->size, f)) {
    return(LIB3DS_FALSE);
  }
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup chunk
 */
const char*
lib3ds_chunk_name(Lib3dsWord chunk)
{
  Lib3dsChunkTable *p;

  for (p=lib3ds_chunk_table; p->name!=0; ++p) {
    if (p->chunk==chunk) {
      return(p->name);
    }
  }
  return("***UNKNOWN***");
}


/*!
 * \ingroup chunk
 */
void
lib3ds_chunk_unknown(Lib3dsWord chunk)
{
  #ifdef _DEBUG
  /*
  fprintf(stderr, "%s***WARNING*** Unknown Chunk: %s (0x%X)\n",
    lib3ds_chunk_level,
    lib3ds_chunk_name(chunk),
    chunk
    );*/
  #endif
}







