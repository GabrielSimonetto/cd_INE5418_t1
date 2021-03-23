all: clean server client
build: server client

server: 
	gcc server.c -o server

client:
	gcc client.c -o client

clean:
	rm server && rm client

