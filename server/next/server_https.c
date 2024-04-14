#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>


#define SERVER_CONNECTION 64
// void serve_file(int client_socket, const char* filename) {
//   File *ref;
//   if (filename == NULL) {
//     send_response(client_socket, 404, "Not Found");
//     return;
//   } else if (*filename == '\0' || strncmp(filename, "index.html", 10) == 0) {
//     ref = file_list+0;
//   } else if (strncmp(filename, "assets/", 7) == 0) {
//     while (*filename != '.' && *filename != '\0') filename++;
//     if (*filename == '\0') {
//       send_response(client_socket, 404, "Not Found");
//       return;
//     }
//     filename++;
//     if (strncmp(filename, "js", 2) == 0) ref = file_list + 1;
//     else if (strncmp(filename, "css", 3) == 0) ref = file_list + 2;
//     else if (strncmp(filename, "ico", 3) == 0) ref = file_list + 3;
//     else {
//       send_response(client_socket, 404, "Not Found");
//       return;
//     }
//   }
//
//   D3(
//   int count = 0;
//   D4(count = d4print("%s", ref->content);)
//   d3print("File length: %li = %d\n", ref->len, count);
//   );
//   send(client_socket, ref->content, ref->len, 0);
// }

SSL_CTX *ctx_server;
int sock_server;
void _init_server(){
  ctx_server = SSL_CTX_new(TLS_server_method());
  if (ctx_server == NULL) {
    perror("SSL_CTX_new");
    exit(1);
  }

  // openssl req -x509 -newkey rsa:2048 -keyout server.key -out server.crt -days 365 -subj "/C=IN/ST=Punjab/L=Patiala/O=Thapar/CN=127.0.0.1"
  if (SSL_CTX_use_certificate_file(ctx_server, "server.crt", SSL_FILETYPE_PEM) <= 0) {
    ERR_print_errors_fp(stderr);
    exit(1);
  }
  if (SSL_CTX_use_PrivateKey_file(ctx_server, "server.key", SSL_FILETYPE_PEM) <= 0) {
    ERR_print_errors_fp(stderr);
    exit(1);
  }

  sock_server = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_server < 0) {
    perror("socket");
    exit(1);
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(8080);
  addr.sin_addr.s_addr = INADDR_ANY;
  if (bind(sock_server, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind");
    exit(1);
  }

  if (listen(sock_server, SERVER_CONNECTION) < 0) {
    perror("listen");
    exit(1);
  }
}

int main() {
  int clientfd;
  struct sockaddr_in clientaddr;
  socklen_t clientaddrlen = sizeof(clientaddr);
  clientfd = accept(sock_server, (struct sockaddr *)&clientaddr, &clientaddrlen);
  // perror("Ok");
  if (clientfd < 0) {
    perror("accept");
    exit(1);
  }

  SSL *ssl = SSL_new(ctx_server);
  if (ssl == NULL) {
    perror("SSL_new");
    exit(1);
  }

  SSL_set_fd(ssl, clientfd);
  if (SSL_accept(ssl) != 1) {
    perror("SSL_accept");
    exit(1);
  }

  char buf[1024];
  int n = SSL_read(ssl, buf, sizeof(buf));
  if (n < 0) {
    perror("SSL_read");
    exit(1);
  }

  printf("%*s", n, buf);

  const char* not_found_response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
  n = SSL_write(ssl, not_found_response, strlen(not_found_response));
  if (n < 0) {
    perror("SSL_write");
    exit(1);
  }

  n = SSL_write(ssl, buf, strlen(buf));
  if (n < 0) {
    perror("SSL_write");
    exit(1);
  }

  // Close the connection.
  SSL_shutdown(ssl);
  SSL_free(ssl);
  close(clientfd);
  close(sock_server);

  return 0;
}

