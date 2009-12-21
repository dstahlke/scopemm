INCLUDES = -I$(top_srcdir)
LDADD = $(top_srcdir)/scopemm/.libs/libscopemm-1.0.a

INCLUDES += $(GTKMM_CFLAGS)
LDADD += $(GTKMM_LIBS)

noinst_PROGRAMS = demo-mouse demo-raster demo-simple demo-stl
# FIXME - detect blitz
#bin_PROGRAMS += demo-blitz-raster demo-blitz

demo_mouse_SOURCES = demo-mouse.cc

demo_raster_SOURCES = demo-raster.cc

demo_simple_SOURCES = demo-simple.cc

demo_stl_SOURCES = demo-stl.cc

#demo_blitz_raster_SOURCES = demo-blitz-raster.cc

#demo_blitz_SOURCES = demo-blitz.cc
