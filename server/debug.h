#pragma once
#include <netinet/in.h>

// #define VERBOSE
// #define DEBUG

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024*1024 // 1MB

typedef struct {
  struct sockaddr_in address;
  int socket;
} Entity;

