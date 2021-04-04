.PHONY: all
all: primary_server secondary_server client

.PHONY: rebuild
rebuild: clean all

.PHONY: clean
clean:
	rm -f primary_server secondary_server client
