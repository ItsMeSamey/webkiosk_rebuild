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
#include "client.h"


SSL_CTX *ctx_client;
struct addrinfo hints, *server_info;
void _init_client(){
  ctx_client = SSL_CTX_new(TLS_client_method());
  if (!ctx_client) {
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
    SSL_CTX_free(ctx_client);
    exit(1);
  }

  return;
  // freeaddrinfo(server_info);
  // SSL_ctx_client_free(ctx);
}

int make_client(Client *c){
  int sockfd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
  if (sockfd == -1) {
    perror("socket");
    return 1;
  } d0print("Socket created successfully");
  SSL *ssl = SSL_new(ctx_client);
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
  c->data = NULL;
  return 0;
  // SSL_free(ssl);
  // close(sockfd);
}

int auth(Client *c, const char *const roll_no, char *const password, const char user_type){
  d4print("!!!!!!!!!!!!!!!!!!!!!!AUTH!!!!!!!!!!!!!!!!!!!!!!");
  d0print("Sending Auth request");
  char header[1024+1];
  int len = snprintf(header, 1024, "POST /CommonFiles/UserAction.jsp HTTP/1.1\nHost:webkiosk.thapar.edu\nContent-Length: %lu\nContent-Type:application/x-www-form-urlencoded\n\n"
                     "UserType=%c&MemberCode=%s&Password=%s", 31+1+strlen(roll_no)+strlen(password), user_type, roll_no, password);
  d4print("%s", header);
  if (SSL_write(c->ssl, header, len) < 0) {
    perror("SSL_write");
    client_unmake((*c));
    return -1;
  } d0print("Sent Auth request");

  int bytes_read = SSL_read(c->ssl, header, 103);
  if (bytes_read < 0) {
    d2error("READ Error");
    return -1;
  }

  d4print("%s", header);
  D3(
    header[bytes_read] = '\0';
  d2print("Header: %s", header);
);
  if (strncmp(header+9, "302", 3) == 0){
    char *ret = c->data;
    strncpy(ret, header+102-32, 32);
    d2print("Returning(cookie): %s\n", ret);
    return 0;
  }
  // seems like ubread data is implicitly ignored
  d4print("::::::::::::::::::::::AUTH::::::::::::::::::::::");
  return 1;
}


int call(Client *c, char const *const cookie, char const *const url){
  char *req = c->data;
  d4print("!!!!!!!!!!!!!!!!!!!!!!CALL!!!!!!!!!!!!!!!!!!!!!!");
  int len = snprintf(req, CALL_BUFFER_SIZE,
                     "GET %s HTTP/1.1\r\n"
                     "Host: webkiosk.thapar.edu\r\n"
                     "User-Agent: curl/8.7.1\r\n"
                     "Accept: */*\r\n"
                     // "Cookie:JSESSIONID=A3256788DA782726FDE969274A39565E\r\n\r\n");
                     "Cookie:JSESSIONID=%s\r\n\r\n", url,cookie);
  // printf("\n:%s:\n%d\n", req, len);
  d4print("%s", req);
  if (SSL_write(c->ssl, req, len) < 0) {
    perror("SSL_write");
    client_unmake((*c));
    return -1;
  } d0print("Sent Call request");

  int bytes_read = SSL_read(c->ssl, req, CALL_BUFFER_SIZE-1);
  if (bytes_read < 0) {
    d2error("READ Error");
    return -1;
  }

  d4print("REQUEST: %s", req);
  D4(
    for (int i = 0; i < bytes_read; i++){
      if (req[i] == '\n') printf("\\n\n");
      else if (req[i] == '\r') printf("\\r");
      else putc(req[i], stdout);
    }
    strstr(req, "\r\n\r\n");
  );
  d4print("::::::::::::::::::::::CALL::::::::::::::::::::::");
  return 0;
}

int main() {
  _init_client();

  Client c;
  if (make_client(&c)) return 1;
  char *cookie = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
  call(&c, cookie, "/StudentFiles/PersonalFiles/StudPersonalInfo.jsp");
  // cookie = auth(&c, "102303535", "", 'S');
  cookie = "A3256788DA782726FDE969274A39565E";
  call(&c, cookie, "/StudentFiles/PersonalFiles/StudPersonalInfo.jsp");
  return 0;
}

