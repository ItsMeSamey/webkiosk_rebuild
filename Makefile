

CC = gcc
JS = bun

all: server

frontend: src/*
	$(JS) run build

server: c/*
	$(CC) -s -O2 -o ./dist/server c/server.c -lpthread -lcurl

run: server frontend
	cd dist && ./server

clean:
	rm -rf dist/*

