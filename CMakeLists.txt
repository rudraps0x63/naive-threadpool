cmake_minimum_required(VERSION 3.25)

project(Threadpool LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(src)

if(BUILD_ALL_UNIT_TESTS)
  add_compile_definitions(BUILD_ALL_UNIT_TESTS)
endif(BUILD_ALL_UNIT_TESTS)

if(BUILD_TESTS OR BUILD_ALL_UNIT_TESTS) # By default only build simple tests
  add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif(BUILD_TESTS)

