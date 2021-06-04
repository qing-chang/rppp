add_executable(tcpServer tcpServer.cpp)
target_link_libraries(tcpServer frame socket)

add_executable(tcpClient tcpClient.cpp)
target_link_libraries(tcpClient frame socket)