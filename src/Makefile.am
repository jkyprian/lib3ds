#
# Free Animation and Modeling Project - 3ds file library
# Copyright (C) 1996-2000 by J.E. Hoffmann
# All rights reserved.
#
# This program is  free  software;  you can redistribute it and/or modify it
# under the terms of the GNU  General  Public  License  as  published by the
# Free  Software  Foundation;  either version 2 of the License,  or (at your
# option) any later version.
#
# This  program  is  distributed in  the  hope that it will  be useful,  but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE. See the GNU  General  Public  License
# for more details.
#
# You should have received a copy of the GNU  General  Public  License along
# with this program;  if not, write to the  Free Software Foundation,  Inc.,
# 675 Mass Ave, Cambridge, MA 02139, USA.
#
# $Id: Makefile.am,v 1.3 2000/05/27 17:57:50 jeh Exp $
#/

INCLUDES= @GLIB_CFLAGS@

lib_LTLIBRARIES = lib3ds.la

lib3ds_la_SOURCES = \
  lib3ds.cpp

include_HEADERS = \
  lib3ds.h

lib3ds_la_LDFLAGS= \
 -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
 -release $(LT_RELEASE)

lib3ds_la_LIBADD = \
  @MATH3D_LIBS@



