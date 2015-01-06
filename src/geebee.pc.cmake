prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=@CMAKE_INSTALL_PREFIX@
libdir=@CMAKE_INSTALL_PREFIX@/lib@LIB_SUFFIX@
includedir=@CMAKE_INSTALL_PREFIX@/include/geebee-@GEEBEE_VERSION_PLATFORM@

Name: GeeBee
Description: GeeBee library
Version: @GEEBEE_VERSION@
Requires: glib-2.0 gobject-2.0
Libs: -L${libdir} -lGeeBee
Cflags: -I${includedir}
