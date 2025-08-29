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
    ifdef CXX
    CXX         := $(CXX)
    else
      CXX         := c++
    endif
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
       numpy.o \
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

LHOST := $(shell hostname)

# -- Targets
all: prep lib bin


lib: $(addprefix obj/,$(UTIL)  $(DICT))
	$(CXX) $(SOFLAGS) $(addprefix obj/,$(UTIL) $(DICT)) -o lib/libAnaUtil.so $(GLIBS) -lMinuit

bin: jpegAna serializeRootFile

LBINARIES = jpegAna serializeRootFile
# Define hostnames where jpegAna should NOT be built
EXCLUDE_JPEG_HOSTS = merlin7 merlin6 mu3ebe

ifeq ($(filter-out $(EXCLUDE_JPEG_HOSTS),$(LHOST)),)
  $(info ************  non-MOOR HOST ************)
  BINARIES = $(filter-out jpegAna, $(LBINARIES))
endif

bin: $(BINARIES)


jpegAna: jpegAna.cc
	$(CXX) $(CXXFLAGS) -o bin/jpegAna jpegAna.cc  $(GLIBS) /opt/homebrew/lib/libjpeg.dylib

serializeRootFile: serializeRootFile.cc
	$(CXX) $(CXXFLAGS) -o bin/serializeRootFile serializeRootFile.cc $(GLIBS)

testMu3eCDB: testMu3eCDB.cc
	$(CXX) $(CXXFLAGS) -o testMu3eCDB testMu3eCDB.cc -I /Users/ursl/mu3e/software/mu3e-cdb/install/include/ \
  -I /Users/ursl/fsx/geant4/geant4-11.1.3/include/Geant4/ \
  -I /opt/homebrew/include/eigen3/ \
  -L/Users/ursl/mu3e/software/mu3e-cdb/install/lib -l mu3e_conddb -l mu3e_rec -l mu3e_util $(GLIBS)


# -- create directories if not yet existing
prep:
	mkdir -p obj bin lib

clean:
	rm -f $(addprefix obj/,$(UTIL) $(DICT))
	rm -f $(DICTHEADERS)
	rm -rf bin/*
	rm -f lib/*.pcm
	rm -f lib/libAnaUtil.so
	rm -f jpegAna
