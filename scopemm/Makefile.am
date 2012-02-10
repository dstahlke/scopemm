AM_CPPFLAGS = -Wall -Wextra -I$(top_srcdir)
LDADD =

AM_CPPFLAGS += $(GTKMM_CFLAGS)
LDADD += $(GTKMM_LIBS)

AM_CPPFLAGS += $(BOOST_CPPFLAGS)

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

cppcheck:
	cppcheck $(DEFAULT_INCLUDES) $(INCLUDES) --template gcc --enable=all -q -i attic/ *.h .

lint:
	cpplint.py --filter=-whitespace,-readability/streams,-build/header_guard,-build/include_order,-readability/multiline_string \
	*.cc *.h 2>&1 \
	| grep -v 'Include the directory when naming .h files' \
	| grep -v 'Is this a non-const reference' \
	| grep -v 'Do not use namespace using-directives.' \
	| grep -v 'Use static_cast' \
	| grep -v 'Done processing '
