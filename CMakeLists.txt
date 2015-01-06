cmake_minimum_required(VERSION 2.8)
project(GeeBee)

set(GEEBEE_VERSION_MAJOR 0)
set(GEEBEE_VERSION_MINOR 0)
set(GEEBEE_VERSION_PATCH 1)
set(GEEBEE_VERSION_PLATFORM "1.0")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(src)
add_subdirectory(docs)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/GeeBeeConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/GeeBeeConfig.cmake
  @ONLY
)

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/GeeBeeConfig.cmake
  DESTINATION
    lib${LIB_SUFFIX}/cmake/GeeBee/
)

