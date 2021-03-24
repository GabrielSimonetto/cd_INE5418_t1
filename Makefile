.PHONY: all
all: server client

.PHONY: rebuild
rebuild: clean all

.PHONY: clean
clean:
	rm -f server client

# all: clean server client
# build: server client

# server: 
# 	gcc server.c -o server

# client:
# 	gcc client.c -o client

# # clean:
# # 	rm server && rm client


# .PHONY: all
# all: server client

# .PHONY: rebuild
# rebuild: clean all

# .PHONY: clean
# clean:
#   rm -f server client