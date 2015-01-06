find_package(PkgConfig REQUIRED)

pkg_search_module(GLIB REQUIRED glib-2.0)
pkg_search_module(GIO REQUIRED gio-unix-2.0)
pkg_search_module(GOBJECT REQUIRED gobject-2.0)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GLIB_INCLUDE_DIRS}
  ${GIO_INCLUDE_DIRS}
)

add_definitions (-DHAVE_CONFIG_H)

set(libgeebee_SRCS
  xbee.c
  request.c
)

set(libgeebee_HEADERS
  xbee.h
  request.h
)

add_library(GeeBee SHARED ${libgeebee_SRCS})

target_link_libraries(GeeBee
  ${GLIB_LIBRARIES}
  ${GOBJECT_LIBRARIES}
  ${GIO_LIBRARIES}
)

install(TARGETS GeeBee DESTINATION lib${LIB_SUFFIX})

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/geebee.pc.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/geebee-${GEEBEE_VERSION_PLATFORM}.pc
  @ONLY
)

install(
  FILES
    ${libgeebee_HEADERS}
  DESTINATION
    include/geebee-${GEEBEE_VERSION_PLATFORM}/geebee/
)

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/geebee-${GEEBEE_VERSION_PLATFORM}.pc
  DESTINATION
    lib${LIB_SUFFIX}/pkgconfig/
)

add_subdirectory(geebee-tool)
