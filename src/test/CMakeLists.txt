aux_source_directory(. DIR_SRCS)
add_executable(tcpServer ${DIR_SRCS})

target_link_libraries(tcpServer frame socket)