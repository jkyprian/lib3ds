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
 * $Id: readwrite.c,v 1.3 2000/10/09 12:33:50 jeh Exp $
 */
#define LIB3DS_EXPORT
#include <lib3ds/readwrite.h>


/*!
 * \defgroup readwrite Portable Binary Input/Ouput
 *
 * \author J.E. Hoffmann <je-h@gmx.net>
 */


/*--WORD & DWORD swapping routines for little/big endian conversations--*/
#ifdef WORDS_BIGENDIAN 
#define SWAPWORD(w) w=\
  ((((w)<<8)&0xFF00)|(((w)>>8)&0xFF));
#define SWAPDWORD(d) d=\
  ((((d)>>24)&0xFF)|(((d)>>8)&0xFF00)|(((d)<<8)&0xFF0000)|(((d)<<24)&0xFF000000));
#else 
#define SWAPWORD(w)   
#define SWAPDWORD(d) 
#endif


/*!
 * \ingroup readwrite
 *
 * Read a byte from a file stream.  
 *
 * \param f  Input file stream. 
 *
 * \return The byte read. 
 */
Lib3dsByte
lib3ds_byte_read(FILE *f)
{
  Lib3dsByte b;

  ASSERT(f);
  fread(&b,1,1,f);
  return(b);
}


/**
 * Read a word from a file stream in little endian format.   
 *
 * \param f  Input file stream. 
 *
 * \return The word read. 
 */
Lib3dsWord
lib3ds_word_read(FILE *f)
{
  Lib3dsWord w;

  ASSERT(f);
  fread(&w,2,1,f);
  SWAPWORD(w);
  return(w);
}


/*!
 * \ingroup readwrite
 *
 * Read a dword from file a stream in little endian format.   
 *
 * \param f  Input file stream. 
 *
 * \return The dword read. 
 */
Lib3dsDword
lib3ds_dword_read(FILE *f)
{
  Lib3dsDword d;        
                         
  ASSERT(f);
  fread(&d,4,1,f);
  SWAPDWORD(d);
  return(d);
}


/*!
 * \ingroup readwrite
 *
 * Read a signed byte from a file stream.   
 *
 * \param f  Input file stream. 
 *
 * \return The signed byte read. 
 */
Lib3dsIntb
lib3ds_intb_read(FILE *f)
{
  Lib3dsIntb b;

  ASSERT(f);
  fread(&b,1,1,f);
  return(b);
}


/*!
 * \ingroup readwrite
 *
 * Read a signed word from a file stream in little endian format.   
 *
 * \param f  Input file stream. 
 *
 * \return The signed word read. 
 */
Lib3dsIntw
lib3ds_intw_read(FILE *f)
{
  Lib3dsWord w;

  ASSERT(f);
  fread(&w,2,1,f);
  SWAPWORD(w);
  return((Lib3dsIntw)w);
}


/*!
 * \ingroup readwrite
 *
 * Read a signed dword a from file stream in little endian format.   
 *
 * \param f  Input file stream. 
 *
 * \return The signed dword read. 
 */
Lib3dsIntd
lib3ds_intd_read(FILE *f)
{
  Lib3dsDword d;        
                         
  ASSERT(f);
  fread(&d,4,1,f);
  SWAPDWORD(d);
  return((Lib3dsIntd)d);
}


/*!
 * \ingroup readwrite
 *
 * Read a float from a file stream in little endian format.   
 *
 * \param f  Input file stream. 
 *
 * \return The float read. 
 */
Lib3dsFloat
lib3ds_float_read(FILE *f)
{
  Lib3dsDword d;

  ASSERT(f);
  fread(&d,4,1,f);
  SWAPDWORD(d);
  return(*((Lib3dsFloat*)&d));
}


/*!
 * \ingroup readwrite
 *
 * Read a zero-terminated string from a file stream.
 *
 * \param s       The buffer to store the read string.
 * \param buflen  Buffer length.
 * \param f       The input file stream.
 *
 * \return        True on success, False otherwise.
 */
Lib3dsBool
lib3ds_string_read(char *s, int buflen, FILE *f)
{
  int k=0;
  ASSERT(f);
  while ((*s++=fgetc(f))!=0) {
    if (++k>buflen) {
      return(LIB3DS_FALSE);
    }
  }
  if (ferror(f)) {
    return(LIB3DS_FALSE);
  }
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup readwrite
 *
 * Writes a byte into a file stream.
 *
 * \param b  The byte to write to the file stream.
 * \param f  The input file stream.
 *
 * \return   True on success, False otherwise.
 */
Lib3dsBool
lib3ds_byte_write(Lib3dsByte b, FILE *f)
{
  ASSERT(f);
  if (fwrite(&b,1,1,f)!=1) {
    return(LIB3DS_FALSE);
  }
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup readwrite
 *
 * Writes a word into a little endian file stream.
 *
 * \param w  The word to write to the file stream.
 * \param f  The input file stream.
 *
 * \return   True on success, False otherwise.
 */
Lib3dsBool
lib3ds_word_write(Lib3dsWord w, FILE *f)
{
  ASSERT(f);
  SWAPWORD(w);
  if (fwrite(&w,2,1,f)!=2) {
    return(LIB3DS_FALSE);
  }
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup readwrite
 *
 * Writes a dword into a little endian file stream.
 *
 * \param d  The dword to write to the file stream.
 * \param f  The input file stream.
 *
 * \return   True on success, False otherwise.
 */
Lib3dsBool
lib3ds_dword_write(Lib3dsDword d, FILE *f)
{
  ASSERT(f);
  SWAPDWORD(d);
  if (fwrite(&d,4,1,f)!=4) {
    return(LIB3DS_FALSE);
  }
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup readwrite
 *
 * Writes a signed byte in a file stream.
 *
 * \param b  The signed byte to write to the file stream.
 * \param f  The input file stream.
 *
 * \return   True on success, False otherwise.
 */
Lib3dsBool
lib3ds_intb_write(Lib3dsIntb b, FILE *f)
{
  ASSERT(f);
  if (fwrite(&b,1,1,f)!=1) {
    return(LIB3DS_FALSE);
  }
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup readwrite
 *
 * Writes a signed word into a little endian file stream.
 *
 * \param w  The signed word to write to the file stream.
 * \param f  The input file stream.
 *
 * \return   True on success, False otherwise.
 */
Lib3dsBool
lib3ds_intw_write(Lib3dsIntw w, FILE *f)
{
  Lib3dsWord ww;

  ASSERT(f);
  ww=(Lib3dsWord)w;
  SWAPWORD(ww);
  if (fwrite(&ww,2,1,f)!=2) {
    return(LIB3DS_FALSE);
  }
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup readwrite
 *
 * Writes a signed dword into a little endian file stream.
 *
 * \param d  The signed dword to write to the file stream.
 * \param f  The input file stream.
 *
 * \return   True on success, False otherwise.
 */
Lib3dsBool
lib3ds_intd_write(Lib3dsIntd d, FILE *f)
{
  Lib3dsDword dd;

  ASSERT(f);
  dd=(Lib3dsDword)d;
  SWAPDWORD(dd);
  if (fwrite(&dd,4,1,f)!=4) {
    return(LIB3DS_FALSE);
  }
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup readwrite
 *
 * Writes a float into a little endian file stream.
 *
 * \param f  The float to write to the file stream.
 * \param f  The input file stream.
 *
 * \return   True on success, False otherwise.
 */
Lib3dsBool
lib3ds_float_write(Lib3dsFloat l, FILE *f)
{
  Lib3dsDword d;

  ASSERT(f);
  d=*((Lib3dsDword*)&l);
  SWAPDWORD(d);
  if (fwrite(&d,4,1,f)!=sizeof(float)) {
    return(LIB3DS_FALSE);
  }
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup readwrite
 *
 * Writes a zero-terminated string into a file stream.
 *
 * \param f  The float to write to the file stream.
 * \param f  The input file stream.
 *
 * \return   True on success, False otherwise.
 */
Lib3dsBool
lib3ds_string_write(const char *s, FILE *f)
{
  ASSERT(s);
  ASSERT(f);
  do fputc(*s,f); while (*s++);
  if (ferror(f)) {
    return(LIB3DS_FALSE);
  }
  return(LIB3DS_TRUE);
}

