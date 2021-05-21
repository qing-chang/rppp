aux_source_directory(. DIR_SRCS)
add_executable(client ${DIR_SRCS})

target_link_libraries(client conf frame msg socket)