# ======================================================================
# NOTE: make all
# -----
#
# ======================================================================
.PHONY: prep clean vars

ROOTCINT      = rootcling
ROOTCFLAGS    = $(shell root-config --cflags)
ROOTGLIBS     = $(shell root-config --glibs)
ROOTLDFLAGS   = $(shell root-config --ldflags)

ifdef SCRAM_ARCH
  # CXX         := $(shell scramv1 tool info gcc-cxxcompiler | grep CXX= | sed s/CXX=//)
  # this likely is decrepit
  # CXX         := /cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/gcc/5.3.0/bin/gcc
else
  ifdef CXXCOMPILER
    CXX         := $(CXXCOMPILER)
  else
    CXX         := c++
  endif
endif

CXXFLAGS      = -g -O0 -Wall -fPIC -pipe
CXXFLAGS     += $(ROOTCFLAGS)

LD            = $(CXX)
LDFLAGS       = $(CXXFLAGS) $(ROOTLDFLAGS) -dynamiclib -shared
SOFLAGS       = $(CXXFLAGS) $(ROOTLDFLAGS) -dynamiclib -shared

GLIBS         = $(filter-out -lz, $(ROOTGLIBS))
GLIBS         += -lMinuit #-lRooFitCore -lRooFit

# -- Source code
UTIL = util.o \
       initFunc.o \
       json.o \
       AnalysisCuts.o \
       AnalysisDistribution.o fitPsYield.o \
       HistCutEfficiency.o \
       dataset.o \
       selpoint.o \
       plotClass.o \
       PidData.o PidTable.o \
       PixelNoiseMaskFile.o

DICT = ${UTIL:.o=Dict.o}
DICTHEADERS = ${UTIL:.o=Dict.h}
DICTSOURCES = ${UTIL:.o=Dict.cc}

# -- Default rules
$(addprefix obj/,%.o) : %.cc %.hh
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(addprefix obj/,%.o) : %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%Dict.cc : %.hh %LinkDef.h
	$(ROOTCINT) $@ $^
	mv $(subst Dict.cc,Dict_rdict.pcm,$@) lib

%Dict.cc : %.hh
	$(ROOTCINT) $@  $^
	mv $(subst Dict.cc,Dict_rdict.pcm,$@) lib

$(addprefix lib/,%.pcm) :
	cd lib && ln -s $< && cd -



# -- Targets
all: prep lib bin


lib: $(addprefix obj/,$(UTIL)  $(DICT))
	$(CXX) $(SOFLAGS) $(addprefix obj/,$(UTIL) $(DICT)) -o lib/libAnaUtil.so $(GLIBS) -lMinuit


# -- create directories if not yet existing
prep:
	mkdir -p obj bin lib

clean:
	rm -f $(addprefix obj/,$(UTIL) $(DICT))
	rm -f $(DICTHEADERS)
	rm -f lib/*.pcm
	rm -f lib/libAnaUtil.so
