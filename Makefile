PROGS = demo-mouse demo-raster demo-simple demo-stl

CPPFLAGS = -Wall -Wextra -I.

# optimization/debug flags:
#CPPFLAGS += -g -DBZ_DEBUG -O0
CPPFLAGS += -g -O2

CPPFLAGS += `pkg-config gtkmm-2.4 --cflags`
LDFLAGS += `pkg-config gtkmm-2.4 --libs`

# comment this out if you don't have blitz
PROGS += demo-blitz demo-blitz-raster
CPPFLAGS += `pkg-config blitz --cflags`
LDFLAGS += `pkg-config blitz --libs`

COMMON_OBJS = scopemm-rastercanvas.o scopemm-lineplot.o scopemm-mouseadapter.o

all: $(PROGS)

demo-simple: demo-simple.o $(COMMON_OBJS)
	g++ -o $@ $^ $(LDFLAGS)

demo-stl: demo-stl.o $(COMMON_OBJS)
	g++ -o $@ $^ $(LDFLAGS)

demo-raster: demo-raster.o $(COMMON_OBJS)
	g++ -o $@ $^ $(LDFLAGS)

demo-mouse: demo-mouse.o $(COMMON_OBJS)
	g++ -o $@ $^ $(LDFLAGS)

demo-blitz: demo-blitz.o $(COMMON_OBJS)
	g++ -o $@ $^ $(LDFLAGS) `pkg-config blitz --libs`

demo-blitz-raster: demo-blitz-raster.o $(COMMON_OBJS)
	g++ -o $@ $^ $(LDFLAGS) `pkg-config blitz --libs`

clean:
	rm -f *.o $(PROGS)
