#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "debug.h"

#define STR(x)#x
#define HOST "webkiosk.thapar.edu"
// #define REQUEST "GET / HTTP/1.1\r\nHost: webkiosk.thapar.edu\r\nConnection: close\r\n\r\n"
#define REQUEST STR(\
POST /CommonFiles/UserAction.jsp HTTP/1.1\r\n\
Host: webkiosk.thapar.edu\r\n\
Content-Length: 52\r\n\
Content-Type: application/x-www-form-urlencoded\r\n\r\n\
UserType=S&MemberCode=102303535&&Password=1234567890\
)

#define client_unmake(x){SSL_free(x.ssl);close(x.sock);}
#define client_send(ssl, data, len) SSL_write(ssl, data, len)

typedef struct {
  int sock;
  SSL *ssl;
} Client;

SSL_CTX *ctx;
struct addrinfo hints, *server_info;
void _init(){
  ctx = SSL_CTX_new(TLS_client_method());
  if (!ctx) {
    ERR_print_errors_fp(stderr);
    perror("\nCannot initiaize ssl context ;");
    exit(1);
  }

  d0print("SSL context created successfull");

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int ret = getaddrinfo(HOST, "https", &hints, &server_info);
  if (ret != 0) {
    d0print("getaddrinfo: %s", gai_strerror(ret));
    perror("Cannot get adress of " HOST);
    SSL_CTX_free(ctx);
    exit(1);
  }

  return;
  // freeaddrinfo(server_info);
  // SSL_CTX_free(ctx);
}

int make_client(Client *c){
  int sockfd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
  if (sockfd == -1) {
    perror("socket");
    return 1;
  } d0print("Socket created successfully");
  SSL *ssl = SSL_new(ctx);
  if (!ssl) {
    ERR_print_errors_fp(stderr);
    close(sockfd);
    return 3;
  } d0print("Created new SSL object");

  if (connect(sockfd, server_info->ai_addr, server_info->ai_addrlen) < 0) {
    perror("connect");
    close(sockfd);
    return 2;
  } d0print("Connected to server");

  SSL_set_fd(ssl, sockfd);
  if (SSL_connect(ssl) != 1) {
    ERR_print_errors_fp(stderr);
    SSL_free(ssl);close(sockfd);
    return 4;
  } d0print("Established SSL connection");

  c->sock = sockfd;
  c->ssl = ssl;
  return 0;
  // SSL_free(ssl);
  // close(sockfd);
}


int main() {
  d1print("%s\n", REQUEST);

  _init();

  Client c;
  if (make_client(&c)) return 1;

  d1print("Sending GET request");
  if (client_send(c.ssl, REQUEST, strlen(REQUEST))) {
    perror("SSL_write");
    client_unmake(c);
    return 1;
  } d0print("Sent GET request");

  char buffer[1024*1024];
  int bytes_read;
  while ((bytes_read = SSL_read(c.ssl, buffer, sizeof(buffer) - 1)) > 0) {
    buffer[bytes_read] = '\0';
    printf("%s", buffer);
  }

  return 0;
}

