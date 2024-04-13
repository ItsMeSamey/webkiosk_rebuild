#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "debug.h"
#include "file_loader.h"
#include "caller.h"

File* file_list;
#define R404

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

  D3(
  int count = 0;
  D4(count = d4print("%s", ref->content);)
  d3print("File length: %li = %d\n", ref->len, count);
  );
  send(client_socket, ref->content, ref->len, 0);
}

void handle_request(char* buffer, Entity *client) {
  if(strncmp(buffer, "GET", 3) == 0){
    // Extract requested filename from the request (assuming basic GET request)
    char* filename = strchr(buffer, '/');
    if (filename == NULL) {
    CLOSE:
      send_response(client->socket, 400, "Bad Request");
      close(client->socket);
      return;
    }
    filename++;
    if (*filename == ' ' || strncmp(filename, "index.html", 10) == 0) {
      serve_file(client->socket, "index.html");
    } else if (strncmp(filename, "assets/", 7) == 0) {
      char* copy = filename;
      while(*copy != ' ')copy++;
      *copy = '\0';
      d2print("Requested file:\n %s\n--------------------------\n", filename);
      serve_file(client->socket, filename);
    } else if (strncmp(filename, "api/", 4) == 0) {
      d1print("at `API`");
      filename = strchr(filename, '/');
      filename++;
      char * cookie = filename;
      filename = strchr(filename, '/');
      if (filename ==NULL) {
        d1print(stderr, "apt error (cookie): %s", cookie);
        goto CLOSE;
      }
      *filename = '\0';
      filename++;
      char* path = filename;
      filename = strchr(filename, ' ');
      if (filename ==NULL) {
        d1print(stderr, "apt error (cookie; path): %s;%s", cookie, path);
        goto CLOSE;
      }
      *filename = '\0';
      d1print(stderr, "INFO (cookie; path): %s;%s", cookie, path);
      void* curl =get_call_curl();
      char url[1024];
      snprintf(url, 1024, "https://webkiosk.thapar.edu/%s", path);
      char* data = make_call(curl, cookie, url);
      if (data) {
        send_response(client->socket, 200, data);
        free(data);
      }
      else send_response(client->socket, 500, "Internal error");
      close(client->socket);
      return;
    } else if (strncmp(filename, "auth/", 4) == 0) {
      d1print("at `AUTH`");
      filename = strchr(filename, '/');
      filename++;
      char * username = filename;
      filename = strchr(filename, '/');
      if (filename ==NULL) {
#ifdef DEBUG
        fprintf(stderr, "auth error (username): %s", username);
#endif
        goto CLOSE;
      }
      *filename = '\0';
      filename++;
      char* password = filename;
      filename = strchr(filename, '/');
      if (filename ==NULL) {
#ifdef DEBUG
        fprintf(stderr, "auth error (username; password): %s;%s", username, password);
#endif
        goto CLOSE;
      }
      *filename = '\0';
      filename++;

      void* curl = get_auth_curl();
      char* data = authenticate(curl, username, password, *filename);
      if (data) {
        send_response(client->socket, 200, data);
        free(data);
      }
      else send_response(client->socket, 500, "Internal error");
      close(client->socket);
      return;
    } else {
#ifdef DEBUG
      printf("THE UNKNOWN !!");
#endif
      goto CLOSE;
    }
  }
  close(client->socket);
  return;
}


