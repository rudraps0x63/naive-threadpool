cmake_minimum_required(VERSION 3.25)

project(Threadpool LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(LIB_NAME threadpool)
set(LIB_SRCS task.h
             threadsafe-queue.h
             threadpool.h
             threadpool.cc
             broker.h
             threadpool-executor.h
             threadpool-executor.cc)

if(BUILD_SHARED_LIBS)
  add_library(${LIB_NAME} SHARED ${LIB_SRCS})
else()
  add_library(${LIB_NAME} STATIC ${LIB_SRCS})
endif(BUILD_SHARED_LIBS)

