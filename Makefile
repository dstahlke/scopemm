PROGS = libscopemm.so demo-mouse demo-raster demo-simple demo-stl
# comment this out if you don't have blitz
PROGS += demo-blitz demo-blitz-raster

CPPFLAGS    = -I. -Wall -Wextra -fPIC
EXE_LDFLAGS = -L. -lscopemm

# optimization/debug flags:
#CPPFLAGS += -g -DBZ_DEBUG -O0
CPPFLAGS += -g -O2

# flags for gtkmm
CPPFLAGS += `pkg-config gtkmm-2.4 --cflags`
LDFLAGS += `pkg-config gtkmm-2.4 --libs`

all: $(PROGS)

clean:
	rm -f *.o $(PROGS)

libscopemm.so: scopemm-rastercanvas.o scopemm-lineplot.o scopemm-mouseadapter.o
	gcc -shared -o $@ $^ $(LDFLAGS)

demo-simple: demo-simple.o
	g++ -o $@ $^ $(LDFLAGS) $(EXE_LDFLAGS)

demo-stl: demo-stl.o
	g++ -o $@ $^ $(LDFLAGS) $(EXE_LDFLAGS)

demo-raster: demo-raster.o
	g++ -o $@ $^ $(LDFLAGS) $(EXE_LDFLAGS)

demo-mouse: demo-mouse.o
	g++ -o $@ $^ $(LDFLAGS) $(EXE_LDFLAGS)

demo-blitz.o: demo-blitz.cc
	g++ -c -o $@ $^ $(CPPFLAGS) `pkg-config blitz --cflags`

demo-blitz: demo-blitz.o
	g++ -o $@ $^ $(LDFLAGS) $(EXE_LDFLAGS) `pkg-config blitz --libs`

demo-blitz-raster.o: demo-blitz-raster.cc
	g++ -c -o $@ $^ $(CPPFLAGS) `pkg-config blitz --cflags`

demo-blitz-raster: demo-blitz-raster.o
	g++ -o $@ $^ $(LDFLAGS) $(EXE_LDFLAGS) `pkg-config blitz --libs`
