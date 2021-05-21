aux_source_directory(. DIR_SRCS)
add_executable(server ${DIR_SRCS})

target_link_libraries(server conf frame msg socket)