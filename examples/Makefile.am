 #
 # The 3D Studio File Format Library
 # Copyright (C) 1996-2000 by J.E. Hoffmann <je-h@gmx.net>
 # All rights reserved.
 #
 # This program is  free  software;  you can redistribute it and/or modify it
 # under the terms of the  GNU Lesser General Public License  as published by 
 # the  Free Software Foundation;  either version 2.1 of the License,  or (at 
 # your option) any later version.
 #
 # This  program  is  distributed in  the  hope that it will  be useful,  but
 # WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 # or  FITNESS FOR A  PARTICULAR PURPOSE.  See the  GNU Lesser General Public  
 # License for more details.
 #
 # You should  have received  a copy of the GNU Lesser General Public License
 # along with  this program;  if not, write to the  Free Software Foundation,
 # Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 #
 # $Id: Makefile.am,v 1.4 2000/10/25 17:22:01 jeh Exp $
 #

INCLUDES = \
  -I$(top_srcdir) \
  @DMALLOC_CFLAGS@

bin_PROGRAMS = \
  lib3dsdump \
  lib3ds2m \
  lib3ds2rib

lib3dsdump_SOURCES = lib3dsdump.c
lib3ds2m_SOURCES = lib3ds2m.c
lib3ds2rib_SOURCES = lib3ds2rib.c

LDADD = \
  $(top_builddir)/lib3ds/lib3ds.la \
  @DMALLOC_LIBS@

MANPAGES = \
  lib3dsdump.1 \
  lib3ds2m.1 \
  lib3ds2rib.1

man_MANS = $(MANPAGES)
EXTRA_DIST = $(MANPAGES)




