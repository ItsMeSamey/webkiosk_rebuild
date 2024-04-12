#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "debug.h"
#include "file_loader.h"

File* file_list;

typedef struct {
  struct sockaddr_in address;
  int socket;
} Entity;


// Function to send a basic HTTP response
void send_response(int client_socket, int status_code, const char* message) {
  char response[BUFFER_SIZE];
  snprintf(response, BUFFER_SIZE, "HTTP/1.1 %d %s\r\nContent-Length: %zu\r\n\r\n%s", status_code, message, strlen(message), message);
  send(client_socket, response, strlen(response), 0);
}

void serve_file(int client_socket, const char* filename) {
  File *ref;
  if (filename == NULL) {
    send_response(client_socket, 404, "Not Found");
    return;
  } else if (*filename == '\0' || strncmp(filename, "index.html", 10) == 0) {
    ref = file_list+0;
  } else if (strncmp(filename, "assets/", 7) == 0) {
    while (*filename != '.' && *filename != '\0') filename++;
    if (*filename == '\0') {
      send_response(client_socket, 404, "Not Found");
      return;
    }
    filename++;
    if (strncmp(filename, "js", 2) == 0) ref = file_list + 1;
    else if (strncmp(filename, "css", 3) == 0) ref = file_list + 2;
    else if (strncmp(filename, "ico", 3) == 0) ref = file_list + 3;
    else {
      send_response(client_socket, 404, "Not Found");
      return;
    }
  }

#ifdef DEBUG
  int count = 0;
#ifdef VERBOSE
  count = printf("%s", ref->content);
#endif
  printf("File length: %li = %d\n", ref->len, count);
#endif

  send(client_socket, ref->content, ref->len, 0);
}

void handle_request(char* buffer, Entity *client) {
  if(strncmp(buffer, "GET", 3) == 0){
    // Extract requested filename from the request (assuming basic GET request)
    char* filename = strchr(buffer, '/')+1;  // Get first word (assumed to be filename)
    if (filename == NULL) {
      send_response(client->socket, 400, "Bad Request");
      close(client->socket);
      return;
    }
    if (*filename == ' ' || strncmp(filename, "index.html", 10) == 0) {
      serve_file(client->socket, "index.html");
    } else if (strncmp(filename, "assets/", 7) == 0) {
      char* copy = filename;
      while(*copy != ' ')copy++;
      *copy = '\0';
#ifdef VERBOSE
    printf("Requested file:\n %s\n--------------------------\n", filename);
#endif
      serve_file(client->socket, filename);
    } else {
      send_response(client->socket, 404, "Not Found");
    }
  }
}


