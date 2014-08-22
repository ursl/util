ROOTCINT      = $(ROOTSYS)/bin/rootcint
ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)

CXX           = g++
CXXFLAGS      = $(ROOTCFLAGS) -g -Wall -fPIC

LD            = g++
LDFLAGS       = -g
SOFLAGS       = -shared

GLIBS         = $(filter-out -lz, $(ROOTGLIBS))
GLIBS         += -lMinuit


# -- Default rules
$(addprefix obj/,%.o) : %.cc 
	$(CXX) $(CXXFLAGS) -c $< -o $@

%Dict.cc : %.hh %LinkDef.h
	$(ROOTCINT) -f $@ -c $^

%Dict.cc : %.hh
	$(ROOTCINT) -f $@ -c $<

# -- Source code
UTIL = util.o \
       dataset.o \
       selpoint.o \
       hpl.o \
       PidData.o PidTable.o

DICTFILES   = ${UTIL:.o=Dict.o}
DICTHEADERS = ${DICTFILES:.o=.h}

# ================================================================================
util: dir $(addprefix obj/,$(UTIL) $(DICTFILES))
# ------------------------------------------------
	$(CXX) $(SOFLAGS) $(GLIBS) $(addprefix obj/,$(UTIL) $(DICTFILES)) -o lib/libutil.so


# -- create directories if not yet existing
dir:
	mkdir -p obj bin lib

clean:
	rm -f $(addprefix obj/,$(UTIL) $(DICTFILES))
	rm -f $(DICTHEADERS)
	rm -f lib/libutil.so

