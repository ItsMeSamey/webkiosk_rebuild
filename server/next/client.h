#pragma once

#include <openssl/ssl.h>
#include <stdlib.h>
#include <netdb.h>

#define HOST "webkiosk.thapar.edu"
#define CALL_BUFFER_SIZE 1024

#define client_unmake(x){SSL_free(x.ssl);close(x.sock);free(x.data);}

typedef struct {
  int sock;
  SSL *ssl;
  char* data;
} Client;

void _init_client();
int make_client(Client *c); // malloc data buffer yourself
int auth(Client *c, const char *const roll_no, char *const password, const char user_type);
int call(Client *c, char const *const cookie, char const *const url);


