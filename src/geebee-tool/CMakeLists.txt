set(geebee_tool_SRCS
  main.c
)

add_executable(geebee-tool ${geebee_tool_SRCS})

target_link_libraries(geebee-tool GeeBee)

install(
  TARGETS
    geebee-tool
  DESTINATION
    bin/
)

