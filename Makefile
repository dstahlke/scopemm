#CFLAGS_O = -g -DBZ_DEBUG -O0
CFLAGS_O = -g -O3
CXXFLAGS = -Wall -I. `pkg-config blitz gtkmm-2.4 --cflags` $(CFLAGS_O)
# blitz lib only needed if -DBZ_DEBUG used
LIBRARIES = `pkg-config blitz gtkmm-2.4 --libs`
EXEC = demo

OBJECTS = gtk-blitz.o demo.o

all: $(EXEC)

$(EXEC): $(OBJECTS)
	g++ -o $@ $(OBJECTS) $(LIBRARIES)

clean:
	rm -f $(OBJECTS) $(EXEC)
