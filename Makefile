

CC = gcc

all: server

./dist/server: c/server.c c/caller.h c/caller.c c/debug.h c/file_loader.h c/server_handler.h
	$(CC) -o ./dist/server c/server.c -lpthread -lcurl

clean:
	rm -rf server dist/*

