.PHONY: all
all: server client

.PHONY: rebuild
rebuild: clean all

.PHONY: clean
clean:
	rm -f server client
