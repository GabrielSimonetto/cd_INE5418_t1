.PHONY: all
all:
	gcc -pthread -o server server.c
	gcc -o client client.c

.PHONY: rebuild
rebuild: clean all

.PHONY: clean
clean:
	rm -f server client
