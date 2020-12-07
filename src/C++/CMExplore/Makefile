PROGRAM = CMExplore
prefix = /usr/local
exec_prefix = ${prefix}
INSTALL = /usr/bin/install -c
EXEEXT =
WINDRES =
REZ =
SETFILE =
NM = nm
BK_DEPS = 
srcdir = .
MYLIBS = 

top_srcdir = ../
LIBS = -lz -ldl -lm -lboost_filesystem -lboost_signals 
LDFLAGS_GUI =
CXXC = g++

CMExplore_CXXFLAGS =  -g -Wall -Wundef -Wno-ctor-dtor-privacy  -fno-strict-aliasing -Wfatal-errors -DCOMMANDVERSION $(CPPFLAGS)

CPPFLAGS = -DWX_PRECOMP -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D_LARGEFILE_SOURCE=1  -I${top_srcdir}/include -DPNG_NO_MMX_CODE -pthread -I/usr/include/gtk-2.0 -I/usr/lib/gtk-2.0/include -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/freetype2 -I/usr/include/libpng12 -I/usr/include/boost -I/usr/local/wxGTK/wxGTK-2.8.7/include/

#CPPFLAGS = -DCOMMANDVERSION -DPOPULATION_H -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D_LARGEFILE_SOURCE=1  -I/usr/include/boost -I${top_srcdir}/include 


CMExplore_OBJECTS =  \
	CMExplore_IOExplore.o \
	CMExplore_bin.o \
	CMExplore_candidate.o \
	CMExplore_classdef.o \
	CMExplore_condition.o \
	CMExplore_conjunction.o \
	CMExplore_constraint.o \
	CMExplore_cutoff.o \
	CMExplore_explore.o \
	CMExplore_feature.o \
	CMExplore_mandatory.o \
	CMExplore_observation.o \
	CMExplore_partition.o \
	CMExplore_performance.o \
	CMExplore_population.o \
	CMExplore_roc.o \
	CMExplore_rocpair.o \
	CMExplore_rule.o \
	CMExplore_set.o \
	CMExplore_timing.o \
	CMExplore_common.o \
	CMExplore_main.o \


CMExplore_IOExplore.o: $(srcdir)/../IOExplore/IOExplore.cpp
	$(CXXC) -c -o $@ $(CMExplore_CXXFLAGS) $(srcdir)/../IOExplore/IOExplore.cpp

CMExplore_bin.o: $(srcdir)/../Explore/bin.cpp
	$(CXXC) -c -o $@ $(CMExplore_CXXFLAGS) $(srcdir)/../Explore/bin.cpp

CMExplore_candidate.o: $(srcdir)/../Explore/candidate.cpp
	$(CXXC) -c -o $@ $(CMExplore_CXXFLAGS) $(srcdir)/../Explore/candidate.cpp

CMExplore_classdef.o: $(srcdir)/../Explore/classdef.cpp
	$(CXXC) -c -o $@ $(CMExplore_CXXFLAGS) $(srcdir)/../Explore/classdef.cpp


CMExplore_condition.o: $(srcdir)/../Explore/condition.cpp
	$(CXXC) -c -o $@ $(CMExplore_CXXFLAGS) $(srcdir)/../Explore/condition.cpp

CMExplore_conjunction.o: $(srcdir)/../Explore/conjunction.cpp
	$(CXXC) -c -o $@ $(CMExplore_CXXFLAGS) $(srcdir)/../Explore/conjunction.cpp

CMExplore_constraint.o: $(srcdir)/../Explore/constraint.cpp
	$(CXXC) -c -o $@ $(CMExplore_CXXFLAGS) $(srcdir)/../Explore/constraint.cpp

CMExplore_cutoff.o: $(srcdir)/../Explore/cutoff.cpp
	$(CXXC) -c -o $@ $(CMExplore_CXXFLAGS) $(srcdir)/../Explore/cutoff.cpp

CMExplore_explore.o: $(srcdir)/../Explore/explore.cpp
	$(CXXC) -c -o $@ $(CMExplore_CXXFLAGS) $(srcdir)/../Explore/explore.cpp

CMExplore_feature.o: $(srcdir)/../Explore/feature.cpp
	$(CXXC) -c -o $@ $(CMExplore_CXXFLAGS) $(srcdir)/../Explore/feature.cpp

CMExplore_mandatory.o: $(srcdir)/../Explore/mandatory.cpp
	$(CXXC) -c -o $@ $(CMExplore_CXXFLAGS) $(srcdir)/../Explore/mandatory.cpp

CMExplore_observation.o: $(srcdir)/../Explore/observation.cpp
	$(CXXC) -c -o $@ $(CMExplore_CXXFLAGS) $(srcdir)/../Explore/observation.cpp

CMExplore_partition.o: $(srcdir)/../Explore/partition.cpp
	$(CXXC) -c -o $@ $(CMExplore_CXXFLAGS) $(srcdir)/../Explore/partition.cpp

CMExplore_performance.o: $(srcdir)/../Explore/performance.cpp
	$(CXXC) -c -o $@ $(CMExplore_CXXFLAGS) $(srcdir)/../Explore/performance.cpp

CMExplore_population.o: $(srcdir)/../Explore/population.cpp
	$(CXXC) -c -o $@ $(CMExplore_CXXFLAGS) $(srcdir)/../Explore/population.cpp

CMExplore_roc.o: $(srcdir)/../Explore/roc.cpp
	$(CXXC) -c -o $@ $(CMExplore_CXXFLAGS) $(srcdir)/../Explore/roc.cpp

CMExplore_rocpair.o: $(srcdir)/../Explore/rocpair.cpp
	$(CXXC) -c -o $@ $(CMExplore_CXXFLAGS) $(srcdir)/../Explore/rocpair.cpp

CMExplore_rule.o: $(srcdir)/../Explore/rule.cpp
	$(CXXC) -c -o $@ $(CMExplore_CXXFLAGS) $(srcdir)/../Explore/rule.cpp

CMExplore_set.o: $(srcdir)/../Explore/set.cpp
	$(CXXC) -c -o $@ $(CMExplore_CXXFLAGS) $(srcdir)/../Explore/set.cpp

CMExplore_timing.o: $(srcdir)/../Explore/timing.cpp
	$(CXXC) -c -o $@ $(CMExplore_CXXFLAGS) $(srcdir)/../Explore/timing.cpp

CMExplore_common.o: $(srcdir)/../common.cpp
	$(CXXC) -c -o $@ $(CMExplore_CXXFLAGS) $(srcdir)/../common.cpp


CMExplore_main.o: $(srcdir)/main.cpp
	$(CXXC) -c -o $@ $(CMExplore_CXXFLAGS) $(srcdir)/main.cpp





CMExplore:      $(CMExplore_OBJECTS)
		$(CXXC)  $(CMExplore_OBJECTS) $(LIBS) $(EXTRALIBS)   -o $(PROGRAM)
		@echo "done"
                	
