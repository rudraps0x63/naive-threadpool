cmake_minimum_required(VERSION 3.25)

project(Threadpool-Tests LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(SRCS_DIR ${CMAKE_SOURCE_DIR}/src)
file(GLOB_RECURSE TEST_SRCS
     LIST_DIRECTORIES TRUE
     ${PROJECT_SOURCE_DIR}/*.cc)

foreach(TEST_FILE IN ITEMS ${TEST_SRCS})
  get_filename_component(FILENAME ${TEST_FILE} NAME_WLE)
  add_executable(${FILENAME} ${TEST_FILE})
  target_include_directories(${FILENAME} PRIVATE ${SRCS_DIR})
  target_link_libraries(${FILENAME} PRIVATE threadpool)
endforeach(TEST_FILE)
