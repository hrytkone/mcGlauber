PROGRAM = mcGlauber

version = eka
CXX = g++
CXXFLAGS = -O -Wall -g -Wno-deprecated -D$(version)

LD = g++
LDFLAGS = -O
SOFLAGS = -shared
CXXFLAGS += $(shell root-config --cflags)
LIBS = $(shell root-config --glibs)
LDFLAGS = $(shell root-config --ldflags)

HDRSDICT = JHistos.h JNucleon.h
HDRS += $(HDRSDICT) nanoDict.h

SRCS = $(HDRS:.h=.cxx)
OBJS = $(HDRS:.h=.o)


all: $(PROGRAM)

$(PROGRAM): $(OBJS) $(PROGRAM).C
	$(CXX) -std=c++11 -lEG -lPhysics -L$(PWD) $(PROGRAM).C $(CXXFLAGS) $(OBJS) $(LDFLAGS) $(FFTWINC) $(LIBS) $(SHARED) -o $(PROGRAM)

%.cxx:

%: %.cxx
	$(LINK.cc) $^ $(CXXFLAGS) $(LOADLIBES) $(LDLIBS) -o $@

%.o: %.cxx %.h
	$(COMPILE.cc) $(OUTPUT_OPTION) $(FFTWINC) $<

clean:
	rm -f $(PROGRAM) *.o nanoDict* *_h* *_C* *_c*

nanoDict.cc: $(HDRSDICT)
	@echo "Generating dictionary ..."
	@rm -f nanoDict.cc nanoDict.hh nanoDict.h
	@rootcint nanoDict.cc -c -D$(version) $(HDRSDICT) LinkDef.h
