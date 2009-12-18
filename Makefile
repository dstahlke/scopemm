#CPPFLAGS += -g -DBZ_DEBUG -O0
CPPFLAGS += -g -O2

CPPFLAGS += -Wall -I. `pkg-config blitz gtkmm-2.4 --cflags`

# blitz lib only needed if -DBZ_DEBUG used
LDFLAGS += `pkg-config blitz gtkmm-2.4 --libs`

PROGS = demo-blitz demo-simple demo-stl demo-mouse demo-blitz-raster

COMMON_OBJS = scopemm-gridcanvas.o scopemm-lineplot.o scopemm-mouseadapter.o

all: $(PROGS)

demo-simple: demo-simple.o $(COMMON_OBJS)
	g++ -o $@ $^ $(LDFLAGS)

demo-stl: demo-stl.o $(COMMON_OBJS)
	g++ -o $@ $^ $(LDFLAGS)

demo-mouse: demo-mouse.o $(COMMON_OBJS)
	g++ -o $@ $^ $(LDFLAGS)

demo-blitz: demo-blitz.o $(COMMON_OBJS)
	g++ -o $@ $^ $(LDFLAGS)

demo-blitz-raster: demo-blitz-raster.o $(COMMON_OBJS)
	g++ -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o $(PROGS)
