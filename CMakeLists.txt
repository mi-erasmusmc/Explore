cmake_minimum_required(VERSION 3.17)
project(Explore)

set(CMAKE_CXX_STANDARD 17)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v -stdlib=libc++")

# MAC
set(Boost_INCLUDE_DIR /Users/aniekmarkus/Documents/Documents/Code/C++/boost_1_74_0)
set(Boost_LIBRARY_DIR /Users/aniekmarkus/Documents/Documents/Code/C++/boost_1_74_0/libs)

# LINUX
# set(Boost_INCLUDE_DIR /data/home/amarkus/Documents/boost_1_74_0)
# set(Boost_LIBRARY_DIR /data/home/amarkus/Documents/boost_1_74_0/boost_1_74_0/libs)

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

include_directories(.)
include_directories(CMExplore)
include_directories(CMExplore/Debug)
include_directories(Explore)
include_directories(IOExplore)

# MAC
include_directories(include /usr/local/include/)
link_directories(/usr/local/lib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ltbb")

# LINUX
# set(CMAKE_PREFIX_PATH "/opt/intel/oneapi/tbb/latest/lib/intel64/gcc4.8")
# find_library(TBB_LIB tbb)
# find_path(TBB_PATH
 #       HINTS /opt/intel/oneapi/tbb/latest/include
 #       NAMES tbb/parallel_for.h)
 
 
set(SOURCE_FILES
        Clion/main.cpp
        src/C++/CMExplore/cmdline.h
        src/C++/CMExplore/CMExplore.cbproj
        src/C++/CMExplore/CMExplore.cbproj.local
        src/C++/CMExplore/initialise.h
        src/C++/CMExplore/main.h
        src/C++/CMExplore/Makefile
        src/C++/CMExplore/ProjectGroup1.groupproj
        src/C++/CMExplore/ProjectGroup1.groupproj.local
        src/C++/Explore/bin.cpp
        src/C++/Explore/bin.h
        src/C++/Explore/candidate.cpp
        src/C++/Explore/candidate.h
        src/C++/Explore/classdef.cpp
        src/C++/Explore/classdef.h
        src/C++/Explore/condition.cpp
        src/C++/Explore/condition.h
        src/C++/Explore/conjunction.cpp
        src/C++/Explore/conjunction.h
        src/C++/Explore/constraint.cpp
        src/C++/Explore/constraint.h
        src/C++/Explore/cutoff.cpp
        src/C++/Explore/cutoff.h
        src/C++/Explore/explore.cpp
        src/C++/Explore/explore.h
        src/C++/Explore/feature.cpp
        src/C++/Explore/feature.h
        src/C++/Explore/initialise.h
        src/C++/Explore/mandatory.cpp
        src/C++/Explore/mandatory.h
        src/C++/Explore/observation.cpp
        src/C++/Explore/observation.h
        src/C++/Explore/outbuf.h
        src/C++/Explore/partition.cpp
        src/C++/Explore/partition.h
        src/C++/Explore/performance.cpp
        src/C++/Explore/performance.h
        src/C++/Explore/population.cpp
        src/C++/Explore/population.h
        src/C++/Explore/roc.cpp
        src/C++/Explore/roc.h
        src/C++/Explore/rocpair.cpp
        src/C++/Explore/rocpair.h
        src/C++/Explore/rule.cpp
        src/C++/Explore/rule.h
        src/C++/Explore/set.cpp
        src/C++/Explore/set.h
        src/C++/Explore/timing.cpp
        src/C++/Explore/timing.h
        src/C++/Explore/values.h
        src/C++/IOExplore/initialise.h
        src/C++/IOExplore/IOExplore.cpp
        src/C++/IOExplore/IOExplore.h
        src/C++/Test/iris.arff
        src/C++/Test/iris.cutoff
        src/C++/Test/test.project
        src/C++/common.cpp
        src/C++/common.h
        src/C++/stl.h
        src/C++/stlpmt.lib)

add_executable(Explore ${SOURCE_FILES})

# LINUX
target_link_libraries(Explore PRIVATE "${TBB_LIB}")
target_include_directories(Explore PRIVATE "${TBB_PATH}")

# add_library(ExploreLib SHARED ${SOURCE_FILES})
# add_library(ExploreLib SHARED ${SOURCE_FILES})