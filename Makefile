.PHONY: all
all: 
	gcc -pthread dispatcher.c -o dispatcher -Wall
	gcc server.c -o server -Wall
	gcc client.c -o client -Wall

.PHONY: rebuild
rebuild: clean all

.PHONY: clean
clean:
	rm -f dispatcher server client
