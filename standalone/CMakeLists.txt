cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(escaper_standalone
  LANGUAGES CXX
)

# ---- Dependencies ----

include(../build/cmake/CPM.cmake)

CPMAddPackage(
  NAME escaper
  SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..
)

# ---- Create standalone executable ----
file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_executable(escaper_standalone ${sources})

set_target_properties(escaper_standalone PROPERTIES
  CXX_STANDARD 11
  OUTPUT_NAME "run"
)

target_link_libraries(escaper_standalone escaper)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
