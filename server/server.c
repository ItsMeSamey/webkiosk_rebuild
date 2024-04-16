#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "dynamic_array.h"
#include "file_loader.c"
#include "caller.c"
#include "debug.h"


#define R400 "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n"
#define R404 "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n"
#define R500 "HTTP/1.1 500 Internal error\r\nContent-Length: 0\r\n\r\n"
#define ssend(client, string) send(client, string, strlen(string), 0)

File* file_list;

void serve_file(int client_socket, const char* filename) {
  d2print("Serving: %s\n", filename);
  File *ref;
  if (filename == NULL) {
    ssend(client_socket, R404);
    return;
  } else if (*filename == '\0' || strncmp(filename, "index.html", 10) == 0) {
    ref = file_list+0;
  } else {
    while (*filename != '.' && *filename != '\0') filename++;
    if (*filename == '\0') {
      ssend(client_socket, R404);
      return;
    }
    filename++;
    if (strncmp(filename, "ico", 2) == 0) ref = file_list + 1;
    else {
      d1print("Unknown Request");
      ssend(client_socket, R404);
      return;
    }
  }

  D3(int count = 0;D4(count = d4print("%s", ref->content);)d3print("File length: %li = %d\n", ref->len, count););

  send(client_socket, ref->content, ref->len, 0);
}

#define NLF(delim) while (*filename != delim && *filename != '\0') filename++;if (*filename == '\0') goto ERR;else *filename = '\0';filename++;
void handle_request(char* buffer, Entity *client) {
  char* filename;
  if(strncmp(buffer, "GET /auth/", 10) == 0){
    d1print("at `AUTH`");
    filename = buffer+10;
    if (*filename == '\0') goto ERR;

    char *roll_no = filename;NLF('/');
    char *password = filename;NLF('/');
    if (auth(roll_no, password, *filename, buffer) == 302){
      ssend(client->socket, buffer);
    }
  } else if(strncmp(buffer, "GET /api/", 9) == 0){
    d1print("at `API`");
    filename = buffer+9;
    if (*filename == '\0') goto ERR;

    char *cookie = filename;NLF('/');
    char *url = filename;NLF(' ');
    char *out = NULL;
    if(call(cookie, url, &out) == 0 && out != NULL){
      d4print("Response : %*s", (int)len, out);
      send(client->socket, out, DARRAY_SIZE(out)[1], 0);
      DARRAY_FREE(out);
      goto END;
    }
    ssend(client->socket, R500);
    goto END;
  } else if(strncmp(buffer, "GET /", 5) == 0){
    d1print("at `/` (serving files)");

    filename = buffer+5;NLF(' ');
    d2print("Requested file:\n %s\n--------------------------\n", buffer+5);
    serve_file(client->socket, buffer+5);
  }

ERR:
  ssend(client->socket, R400);
END:
  close(client->socket);
}

void create_server(Entity *const server){
  server->socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server->socket == -1) {
    derror("socket");
    exit(1);
  }

  // Set up server address
  server->address.sin_family = AF_INET;
  server->address.sin_addr.s_addr = INADDR_ANY;
  server->address.sin_port = htons(SERVER_PORT);

  if (bind(server->socket, (struct sockaddr*)&server->address, sizeof(server->address)) == -1) {
    derror("bind");
    close(server->socket);
    exit(1);
  }
  if (listen(server->socket, 5) == -1) {
    derror("listen");
    close(server->socket);
    exit(1);
  }
}

