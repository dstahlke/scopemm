AM_CPPFLAGS = -Wall -Wextra -I$(top_srcdir)
LDADD =

AM_CPPFLAGS += $(GTKMM_CFLAGS)
LDADD += $(GTKMM_LIBS)

# libtoolize told me to add this
ACLOCAL_AMFLAGS = -I m4

h_sources = affine.h axeslayer.h bbox.h grid.h layerimpl.h lineplot.h mouseadapter.h plotcanvas.h rastercanvas.h scopemm.h
cc_sources = affine.cc axeslayer.cc grid.cc lineplot.cc mouseadapter.cc plotcanvas.cc rastercanvas.cc

# Tutorial at:
# http://www.openismus.com/documents/linux/building_libraries/building_libraries.shtml
# FIXME - make sure version numbers are OK

library_includedir=$(includedir)/scopemm-$(PACKAGE_RELEASE)/scopemm
library_include_HEADERS = $(h_sources)

lib_LTLIBRARIES = libscopemm-1.0.la
libscopemm_1_0_la_SOURCES= $(h_sources) $(cc_sources)
libscopemm_1_0_la_LDFLAGS= -version-info $(LIBRARY_VERSION) -release $(PACKAGE_RELEASE)
