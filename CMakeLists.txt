PROJECT(themas)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

SET(CMAKE_VERBOSE_MAKEFILE ON)

#add definitions, compiler switches, etc.
ADD_DEFINITIONS(-Wall -O2 -DNDEBUG `getconf LFS_CFLAGS` -fPIC)

ADD_EXECUTABLE(set_intersection_test
               src/set_intersection/test
              )

ADD_EXECUTABLE(set_intersection_profile
               src/set_intersection/profile
              )