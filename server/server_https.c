#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

int main() {
  SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());
  if (ctx == NULL) {
    perror("SSL_CTX_new");
    exit(1);
  }

  // openssl req -x509 -newkey rsa:2048 -keyout server.key -out server.crt -days 365 -subj "/C=IN/ST=Punjab/L=Patiala/O=Thapar/CN=127.0.0.1"
  if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0) {
    ERR_print_errors_fp(stderr);
    exit(1);
  }
  if (SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0) {
    ERR_print_errors_fp(stderr);
    exit(1);
  }

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("socket");
    exit(1);
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(8080);
  addr.sin_addr.s_addr = INADDR_ANY;
  if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind");
    exit(1);
  }

  if (listen(sockfd, 10) < 0) {
    perror("listen");
    exit(1);
  }

  int clientfd;
  struct sockaddr_in clientaddr;
  socklen_t clientaddrlen = sizeof(clientaddr);
  clientfd = accept(sockfd, (struct sockaddr *)&clientaddr, &clientaddrlen);
  // perror("Ok");
  if (clientfd < 0) {
    perror("accept");
    exit(1);
  }

  SSL *ssl = SSL_new(ctx);
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
  close(sockfd);

  return 0;
}

