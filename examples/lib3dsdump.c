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
 * $Id: lib3dsdump.c,v 1.3 2000/10/19 20:35:29 jeh Exp $
 */
#include <lib3ds/file.h>
#include <stdlib.h>
#include <string.h>
#include <config.h>
#ifdef WITH_DMALLOC
#include <dmalloc.h>
#endif


/*!
\example lib3dsdump.c

Displays imformation about the content of a <i>3DS</i> file.

\code
Syntax: lib3dsdump [options] filename [options]

Options:
  -h/--help          This help
  -m/--materials     Dump materials
  -t/--trimeshes     Dump meshes
  -i/--instance      Dump mesh instances
  -c/--cameras       Dump cameras
  -l/--lights        Dump lights
  -n/--nodes         Dump node hierarchy
\endcode

\author J.E. Hoffmann <je-h@gmx.net>
*/


static void
help()
{
  fprintf(stderr,
"The 3D Studio File Format Library - lib3dsdump Version " VERSION "\n"
"Copyright (C) 1996-2000 by J.E. Hoffmann <je-h@gmx.net>\n"
"All rights reserved.\n"
"\n"
"Syntax: lib3dsdump [options] filename [options]\n"
"\n"
"Options:\n"
"  -h/--help          This help\n"
"  -m/--materials     Dump materials\n"
"  -t/--trimeshes     Dump meshes\n"
"  -i/--instance      Dump mesh instances\n"
"  -c/--cameras       Dump cameras\n"
"  -l/--lights        Dump lights\n"
"  -n/--nodes         Dump node hierarchy\n"
"\n"
);
  exit(1);
}

typedef enum _Flags {
  LIB3DS2M_MATERIALS  =0x0001,
  LIB3DS2M_TRIMESHES  =0x0002,
  LIB3DS2M_INSTANCES  =0x0004,
  LIB3DS2M_CAMERAS    =0x0008,
  LIB3DS2M_LIGHTS     =0x0010,
  LIB3DS2M_NODES      =0x0020
} Flags;

static const char* filename=0;
static Lib3dsDword flags=0;


static void
parse_args(int argc, char **argv)
{
  int i;
  
  for (i=1; i<argc; ++i) {
    if (argv[i][0]=='-') {
      if ((strcmp(argv[i],"-h")==0) || (strcmp(argv[i],"--help")==0)) {
        help();
      }
      else
      if ((strcmp(argv[i],"-m")==0) || (strcmp(argv[i],"--material")==0)) {
        flags|=LIB3DS2M_MATERIALS;
      }
      else
      if ((strcmp(argv[i],"-t")==0) || (strcmp(argv[i],"--trimesh")==0)) {
        flags|=LIB3DS2M_TRIMESHES;
      }
      else
      if ((strcmp(argv[i],"-i")==0) || (strcmp(argv[i],"--instance")==0)) {
        flags|=LIB3DS2M_INSTANCES;
      }
      else
      if ((strcmp(argv[i],"-c")==0) || (strcmp(argv[i],"--camera")==0)) {
        flags|=LIB3DS2M_CAMERAS;
      }
      else
      if ((strcmp(argv[i],"-l")==0) || (strcmp(argv[i],"--light")==0)) {
        flags|=LIB3DS2M_LIGHTS;
      }
      else
      if ((strcmp(argv[i],"-n")==0) || (strcmp(argv[i],"--nodes")==0)) {
        flags|=LIB3DS2M_NODES;
      }
      else {
        help();
      }
    }
    else {
      if (filename) {
        help();
      }
      filename=argv[i];
    }
  }
  if (!filename) {
    help();
  }
  if (!flags) {
    flags=0xFFFFFFFF;
  }
}


int
main(int argc, char **argv)
{
  Lib3dsFile *f=0;

  parse_args(argc, argv);
  f=lib3ds_open(filename);
  if (!f) {
    fprintf(stderr, "***ERROR***\nLoading file %s failed\n", filename);
    exit(1);
  }

  if (flags&LIB3DS2M_MATERIALS) {
    printf("Dumping materials:\n");
    lib3ds_file_dump_materials(f);
    printf("\n");
  }
  if (flags&LIB3DS2M_TRIMESHES) {
    printf("Dumping meshes:\n");
    lib3ds_file_dump_meshes(f);
    printf("\n");
  }
  if (flags&LIB3DS2M_INSTANCES) {
    printf("Dumping instances:\n");
    lib3ds_file_dump_instances(f);
    printf("\n");
  }
  if (flags&LIB3DS2M_CAMERAS) {
    printf("Dumping cameras:\n");
    lib3ds_file_dump_cameras(f);
    printf("\n");
  }
  if (flags&LIB3DS2M_LIGHTS) {  
    printf("Dumping lights:\n");
    lib3ds_file_dump_lights(f);
    printf("\n");
  }
  if (flags&LIB3DS2M_NODES) {
    printf("Dumping node hierarchy:\n");
    lib3ds_file_dump_nodes(f);
    printf("\n");
  }
  
  lib3ds_close(f);
  return(0);
}
