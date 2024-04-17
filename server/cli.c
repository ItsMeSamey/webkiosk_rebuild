#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "debug.h"
#include "server.c"

char is_exit_char(char c){
  if (c == 'q' || c == 'Q' || c == '\0') return 1;
  return 0;
}

int main() {

  // system("stty raw");
  // char ch;
  // printf("Enter a character: ");
  // while (is_exit_char(ch = getchar()) == 0) {
  //   printf("\b\b\b\bYou entered: %c\r\n", ch);
  // }
  // system("stty cooked");
  // return 0;

  get_file_list();
  socklen_t client_address_size;
  char buffer[BUFFER_SIZE];

  Entity server;
  create_server(&server);
  dprint("Server listening on port %d\n", SERVER_PORT);
  Entity client;
  

  while (1) {
    client_address_size = sizeof(client.address);
    client.socket = accept(server.socket, (struct sockaddr*)&client.address, &client_address_size);
    if (client.socket == -1) {
      d0error("accept");
      continue;
    }
    if (recv(client.socket, buffer, BUFFER_SIZE, 0) == -1) {
      d0error("recv");
      close(client.socket);
      continue;
    }
    d4print("Buffer:\n %s\n--------------------------\n", buffer);

    handle_request(buffer, &client);
    close(client.socket);
  }

  close(server.socket);
  return 0;
}

