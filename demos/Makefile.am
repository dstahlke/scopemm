AM_CPPFLAGS = -Wall -Wextra -I$(top_srcdir)
LDADD = $(top_srcdir)/scopemm/.libs/libscopemm-1.0.a

AM_CPPFLAGS += $(GTKMM_CFLAGS)
LDADD += $(GTKMM_LIBS)

AM_CPPFLAGS += $(BOOST_CPPFLAGS)

noinst_PROGRAMS = demo-mouse demo-raster demo-simple demo-stl

demo_mouse_SOURCES = demo-mouse.cc
demo_raster_SOURCES = demo-raster.cc
demo_simple_SOURCES = demo-simple.cc
demo_stl_SOURCES = demo-stl.cc

if HAVE_BLITZ
noinst_PROGRAMS += demo-blitz-raster demo-blitz

demo_blitz_raster_SOURCES = demo-blitz-raster.cc
demo_blitz_raster_LDADD = $(LDADD) $(BLITZ_LIBS)
demo_blitz_raster_CPPFLAGS = $(AM_CPPFLAGS) $(BLITZ_CFLAGS)

demo_blitz_SOURCES = demo-blitz.cc
demo_blitz_LDADD = $(LDADD) $(BLITZ_LIBS)
demo_blitz_CPPFLAGS = $(AM_CPPFLAGS) $(BLITZ_CFLAGS)
endif
