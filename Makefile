MYMAKE
CXX = g++ -std=c++20
CXXFLAGS = -Wall -g -MMD -I/usr/X11/include  # Specify the path to X11 headers
LDFLAGS = -L/usr/X11/lib  # Specify the path to X11 library
LDLIBS = -lX11  # Link against the X11 library

SOURCES = $(wildcard *.cc)
OBJECTS = ${SOURCES:.cc=.o}
DEPENDS = ${OBJECTS:.o=.d}
EXEC = chess

# First target in the makefile is the default target.
$(EXEC): $(OBJECTS)
    $(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $(OBJECTS) -o $(EXEC)

%.o: %.cc 
    $(CXX) -c -o $@ $< $(CXXFLAGS)

-include ${DEPENDS}

.PHONY: clean 
clean:
    rm -f $(OBJECTS) $(DEPENDS) $(EXEC)

