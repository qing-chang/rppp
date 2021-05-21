cc = g++ -std=c++20 -fcoroutines -pthread

PROGS = ./obj/server ./obj/client

all:$(PROGS)

./obj/server:./obj/conf.o ./obj/io_context.o ./obj/socket_accept_operation.o ./obj/socket_recv_operation.o ./obj/socket_send_operation.o ./obj/socket.o ./obj/server.o ./obj/control.o ./obj/tunnel.o ./obj/msg.o
	$(cc) -o ./obj/server ./obj/conf.o ./obj/io_context.o ./obj/socket_accept_operation.o ./obj/socket_recv_operation.o ./obj/socket_send_operation.o ./obj/socket.o ./obj/server.o ./obj/control.o ./obj/tunnel.o ./obj/msg.o

./obj/client:./obj/conf.o ./obj/io_context.o ./obj/socket_accept_operation.o ./obj/socket_recv_operation.o ./obj/socket_send_operation.o ./obj/socket.o ./obj/client.o ./obj/controller.o ./obj/model.o ./obj/msg.o
	$(cc) -o ./obj/client ./obj/conf.o ./obj/io_context.o ./obj/socket_accept_operation.o ./obj/socket_recv_operation.o ./obj/socket_send_operation.o ./obj/socket.o ./obj/client.o ./obj/controller.o ./obj/model.o ./obj/msg.o

./obj/conf.o:
	$(cc) -o ./obj/conf.o -c ./src/conf/conf.cpp

./obj/io_context.o:
	$(cc) -o ./obj/io_context.o -c ./src/io_context.cpp

./obj/socket_accept_operation.o:
	$(cc) -o ./obj/socket_accept_operation.o -c ./src/socket/socket_accept_operation.cpp

./obj/socket_recv_operation.o:
	$(cc) -o ./obj/socket_recv_operation.o -c ./src/socket/socket_recv_operation.cpp

./obj/socket_send_operation.o:
	$(cc) -o ./obj/socket_send_operation.o -c ./src/socket/socket_send_operation.cpp

./obj/socket.o:
	$(cc) -o ./obj/socket.o -c ./src/socket/socket.cpp

./obj/msg.o:
	$(cc) -o ./obj/msg.o -c ./src//msg/msg.cpp

./obj/server.o:
	$(cc) -o ./obj/server.o -c ./src/server/main.cpp

./obj/control.o:
	$(cc) -o ./obj/control.o -c ./src/server/control.cpp

./obj/tunnel.o:
	$(cc) -o ./obj/tunnel.o -c ./src/server/tunnel.cpp

./obj/client.o:
	$(cc) -o ./obj/client.o -c ./src/client/main.cpp

./obj/controller.o:
	$(cc) -o ./obj/controller.o -c ./src/client/controller.cpp

./obj/model.o:
	$(cc) -o ./obj/model.o -c ./src/client/model.cpp

clean:
	rm -f ./obj/*