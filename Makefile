.PHONY: all
all: 
	gcc dispatcher.c -o dispatcher -Wall
	gcc server.c -o server -Wall
	gcc client.c -o client -Wall

.PHONY: rebuild
rebuild: clean all

.PHONY: clean
clean:
	rm -f dispatcher server client server_socket
