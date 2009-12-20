DEMOS = demo-mouse demo-raster demo-simple demo-stl
DEMOS_BLITZ = demo-blitz demo-blitz-raster

CPPFLAGS    = -I. -Wall -Wextra -fPIC
EXE_LDFLAGS = -L. -lscopemm

# optimization/debug flags:
#CPPFLAGS += -g -DBZ_DEBUG -O0
CPPFLAGS += -g -O2

# flags for gtkmm
CPPFLAGS += `pkg-config gtkmm-2.4 --cflags`
LDFLAGS += `pkg-config gtkmm-2.4 --libs`

all: libscopemm.so demos

demos: $(DEMOS)

# not built be default because some people don't have blitz
blitz-demos: $(DEMOS_BLITZ)

clean:
	rm -f *.o libscopemm.so $(DEMOS) $(DEMOS_BLITZ)

libscopemm.so: scopemm-rastercanvas.o scopemm-lineplot.o scopemm-mouseadapter.o scopemm-plotcanvas.o scopemm-grid.o scopemm-affine.o scopemm-axeslayer.o
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

lint:
	cpplint.py --filter=-whitespace,-readability/streams,-build/include_order,-legal/copyright,-build/header_guard \
		*.[ch]* scopemm/*.h 2>&1 \
		|grep -v 'Include the directory when naming .h files' \
		|grep -v 'Using deprecated casting style.  Use static_cast<\(double\|int\)>'
