find_package(GtkDoc)

if (GTKDOC_FOUND)
  add_custom_command(
    OUTPUT
      ${CMAKE_CURRENT_BINARY_DIR}/GeeBee-decl.txt
    COMMAND
      ${GTKDOC_SCAN_EXE}
        --module=GeeBee
        --source-dir=${CMAKE_SOURCE_DIR}/src/
        --rebuild-types
    DEPENDS
      ${libgeebee_SRCS}
      ${libgeebee_HEADERS}
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_BINARY_DIR}
  )
  add_custom_command(
    OUTPUT
      ${CMAKE_CURRENT_BINARY_DIR}/GeeBee-docs.xml
    COMMAND
      ${GTKDOC_MKDB_EXE}
        --module=GeeBee
        --output-format=xml
        --xml-mode
        --source-dir=${CMAKE_SOURCE_DIR}/src/
    DEPENDS
      ${CMAKE_CURRENT_BINARY_DIR}/GeeBee-decl.txt
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_BINARY_DIR}
  )
  add_custom_command(
    OUTPUT
      ${CMAKE_CURRENT_BINARY_DIR}/html/index.html
    COMMAND
      ${GTKDOC_MKHTML_EXE}
        GeeBee
        ${CMAKE_CURRENT_BINARY_DIR}/GeeBee-docs.xml
    DEPENDS
      ${CMAKE_CURRENT_BINARY_DIR}/GeeBee-docs.xml
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_BINARY_DIR}/html/
  )
  add_custom_target(
    docs_html
    ALL
    DEPENDS
      ${CMAKE_CURRENT_BINARY_DIR}/html/index.html
  )

  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html/)

  install(
    DIRECTORY
      ${CMAKE_CURRENT_BINARY_DIR}/html/
    DESTINATION
    share/doc/GeeBee-${GEEBEE_VERSION_PLATFORM}/
  )

endif()
