#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = RecoTool_ClusterRecoUtilDict
SHLIB = libRecoTool_ClusterRecoUtil.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += $(shell larlite-config --includes)
INCFLAGS += $(shell recotool-config --includes)
#INCFLAGS += -I../Alg

# Add -p flag to CINT compilation
CINTFLAGS += -p

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

# linker flags
LDFLAGS += $(shell larlite-config --libs)

#SUBDIRS := Alg
#
#
## .phony: all clean
#
#all: subdir
#	@for i in $(SUBDIRS); do ( echo "" && echo "Compiling $$i..." && cd $$i && $(MAKE) ) || exit $$?; done
#
## clean:
## 	@for i in $(SUBDIRS); do ( echo "" && echo "Cleaning $$i..." && cd $$i && $(MAKE) clean && rm -f $(LARLIGHT_LIBDIR)/$$i.* ) || exit $$?; done
#
#subdir: dummy_subdir
#	cd Alg; $(MAKE)


include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# call the common GNUmakefile
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE


#dummy_subdir:
#	true

