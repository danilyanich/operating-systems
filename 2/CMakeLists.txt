cmake_minimum_required (VERSION 3.12.1 FATAL_ERROR)

set(
  CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS}"
)

file(
  GLOB_RECURSE
  SOURCE_FILES
  CONFIGURE_DEPENDS
  "src/*.cpp"
)


project(project)

add_executable(bin ${SOURCE_FILES})
