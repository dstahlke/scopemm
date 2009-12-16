#CPPFLAGS += -g -DBZ_DEBUG -O0
CPPFLAGS += -g -O2

CPPFLAGS += -Wall -I. `pkg-config blitz gtkmm-2.4 --cflags`

# blitz lib only needed if -DBZ_DEBUG used
LDFLAGS += `pkg-config blitz gtkmm-2.4 --libs`

PROGS = demo-blitz demo-stl

COMMON_OBJS = scopemm-gridcanvas.o scopemm-lineplot.o

all: $(PROGS)

demo-blitz: demo-blitz.o $(COMMON_OBJS)
	g++ -o $@ $^ $(LDFLAGS)

demo-stl: demo-stl.o $(COMMON_OBJS)
	g++ -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o $(PROGS)
