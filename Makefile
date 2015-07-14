# Build libslhatools.so
# Created 12-Jul-2015 Harrison B. Prosper
# ----------------------------------------------------------------------------
ifndef ROOTSYS
	$(error Please set up Root)
endif

ifndef EXTERNAL
$(error Please set EXTERNAL=to location of your bin, lib, etc. directories)
endif
# ----------------------------------------------------------------------------
NAME	:= slhatools
incdir	:= include
srcdir	:= src
libdir	:= lib
bindir	:= bin
$(shell mkdir -p lib)

# get lists of sources
SRCS	:=  	$(srcdir)/slhatools.cc

CINTSRCS:= $(wildcard $(srcdir)/*_dict.cc)

OTHERSRCS:= $(filter-out $(CINTSRCS) $(SRCS),$(wildcard $(srcdir)/*.cc))

# list of dictionaries to be created
DICTIONARIES	:= $(SRCS:.cc=_dict.cc)

# get list of objects
OBJECTS		:= $(SRCS:.cc=.o) $(OTHERSRCS:.cc=.o) $(DICTIONARIES:.cc=.o)

#say := $(shell echo "DICTIONARIES:     $(DICTIONARIES)" >& 2)
#say := $(shell echo "" >& 2)
#say := $(shell echo "SRCS: $(SRCS)" >& 2)
#say := $(shell echo "OBJECTS: $(OBJECTS)" >& 2)
#$(error bye)
# ----------------------------------------------------------------------------
ROOTCINT	:= rootcint

# check for clang++, otherwise use g++
COMPILER	:= $(shell which clang++ >& $(HOME)/.cxx; tail $(HOME)/.cxx)
COMPILER	:= $(shell basename "$(COMPILER)")
ifeq ($(COMPILER),clang++)
CXX		:= clang++
LD		:= clang++
else
CXX		:= g++
LD		:= g++
endif
CPPFLAGS	:= -I. -I$(incdir)
CXXFLAGS	:= -O -Wall -fPIC -g -ansi -Wshadow -Wextra \
$(shell root-config --cflags)
LDFLAGS		:= -g
# ----------------------------------------------------------------------------
# which operating system?
OS := $(shell uname -s)
ifeq ($(OS),Darwin)
	LDFLAGS += -dynamiclib
	LDEXT	:= .dylib
else
	LDFLAGS	+= -shared
	LDEXT	:= .so
endif	
LDFLAGS += $(shell root-config --ldflags)
LIBS 	:= -lPyROOT $(shell root-config --libs --nonew)
LIBRARY	:= $(libdir)/lib$(NAME)$(LDEXT)
# ----------------------------------------------------------------------------
all: $(LIBRARY)

install:
	cp $(bindir)/*.py $(EXTERNAL)/bin
	cp $(libdir)/lib$(NAME)$(LDEXT) $(EXTERNAL)/lib
	find $(libdir) -name "*.pcm" -exec cp {} $(EXTERNAL)/lib \;

$(LIBRARY)	: $(OBJECTS)
	@echo ""
	@echo "=> Linking shared library $@"
	$(LD) $(LDFLAGS) $^ $(LIBS)  -o $@

$(OBJECTS)	: %.o	: 	%.cc
	@echo ""
	@echo "=> Compiling $<"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(DICTIONARIES)	: $(srcdir)/%_dict.cc	: $(incdir)/%.h $(srcdir)/%_linkdef.h
	@echo ""
	@echo "=> Building dictionary $@"
	$(ROOTCINT)	-f $@ -c $(CPPFLAGS) $^
	find $(srcdir) -name "*$(NAME)*.pcm" -exec mv {} $(libdir) \;

tidy:
	rm -rf $(srcdir)/*_dict*.* $(srcdir)/*.o 

clean:
	rm -rf $(libdir)/* $(srcdir)/*_dict*.* $(srcdir)/*.o
	rm -rf $(EXTERNAL)/lib/*$(NAME)*
