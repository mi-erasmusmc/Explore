cmake_minimum_required(VERSION 3.17)
project(Explore)

set(CMAKE_CXX_STANDARD 14)

set(Boost_INCLUDE_DIR /Users/aniekmarkus/Documents/Documents/Code/C++/boost_1_74_0)
set(Boost_LIBRARY_DIR /Users/aniekmarkus/Documents/Documents/Code/C++/boost_1_74_0/libs)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

include_directories(.)
include_directories(CMExplore)
include_directories(CMExplore/Debug)
include_directories(Explore)
include_directories(IOExplore)

add_executable(Explore
        CMExplore/Debug/CMExplore.pch
        CMExplore/cmdline.h
        CMExplore/initialise.h
        CMExplore/main.cpp
        CMExplore/main.h
        Explore/bin.cpp
        Explore/bin.h
        Explore/candidate.cpp
        Explore/candidate.h
        Explore/classdef.cpp
        Explore/classdef.h
        Explore/condition.cpp
        Explore/condition.h
        Explore/conjunction.cpp
        Explore/conjunction.h
        Explore/constraint.cpp
        Explore/constraint.h
        Explore/cutoff.cpp
        Explore/cutoff.h
        Explore/explore.cpp
        Explore/explore.h
        Explore/feature.cpp
        Explore/feature.h
        Explore/initialise.h
        Explore/mandatory.cpp
        Explore/mandatory.h
        Explore/observation.cpp
        Explore/observation.h
        Explore/outbuf.h
        Explore/partition.cpp
        Explore/partition.h
        Explore/performance.cpp
        Explore/performance.h
        Explore/population.cpp
        Explore/population.h
        Explore/roc.cpp
        Explore/roc.h
        Explore/rocpair.cpp
        Explore/rocpair.h
        Explore/rule.cpp
        Explore/rule.h
        Explore/set.cpp
        Explore/set.h
        Explore/timing.cpp
        Explore/timing.h
        Explore/values.h
        IOExplore/initialise.h
        IOExplore/IOExplore.cpp
        IOExplore/IOExplore.h
        common.cpp
        common.h
        stl.h)
