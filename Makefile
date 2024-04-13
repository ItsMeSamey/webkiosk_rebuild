

CC = gcc
JS = bun

all: server

client: src/*
	$(JS) run build
	make server

server: c/*
	$(CC) -s -O2 -o ./dist/server c/server.c c/caller.c -lpthread -lcurl

run: server frontend
	cd dist && ./server


