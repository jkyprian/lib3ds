
SUBDIRS = src example

bin_SCRIPTS = lib3ds-config

AUTOMAKE_OPTIONS = 1.4

EXTRA_DIST = \
	lib3ds.m4 \
	AUTHORS \
	COPYING \
	ChangeLog \
	INSTALL \
	README \
	TODO 

m4datadir = $(datadir)/aclocal
m4data_DATA = lib3ds.m4

.PHONY: release snapshot

release:
	rm -rf .deps */.deps
	$(MAKE) distcheck

snapshot:
	$(MAKE) dist distdir=$(PACKAGE)`date +"%y%m%d"`

