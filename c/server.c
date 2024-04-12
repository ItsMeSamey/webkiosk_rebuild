#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "file_loader.h"
#include "debug.h"
#include "server_handler.h"


void create_server(Entity *const server){
  server->socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server->socket == -1) {
    perror("socket");
    exit(1);
  }

  // Set up server address
  server->address.sin_family = AF_INET;
  server->address.sin_addr.s_addr = INADDR_ANY;
  server->address.sin_port = htons(SERVER_PORT);

  if (bind(server->socket, (struct sockaddr*)&server->address, sizeof(server->address)) == -1) {
    perror("bind");
    close(server->socket);
    exit(1);
  }
  if (listen(server->socket, 5) == -1) {
    perror("listen");
    close(server->socket);
    exit(1);
  }
}

int main() {
  file_list = get_file_list();
  socklen_t client_address_size;
  char buffer[BUFFER_SIZE];

  Entity server;
  create_server(&server);
  printf("Server listening on port %d\n", SERVER_PORT);
  Entity client;

  while (1) {
    client_address_size = sizeof(client.address);
    client.socket = accept(server.socket, (struct sockaddr*)&client.address, &client_address_size);
    if (client.socket == -1) {
#ifdef DEBUG
      perror("accept");
#endif
      continue;
    }
    if (recv(client.socket, buffer, BUFFER_SIZE, 0) == -1) {
#ifdef DEBUG
      perror("recv");
#endif
      close(client.socket);
      continue;
    }
#ifdef VERBOSE
    printf("Buffer:\n %s\n--------------------------\n", buffer);
#endif

    handle_request(buffer, &client);
    close(client.socket);
  }

  close(server.socket);
  return 0;
}

