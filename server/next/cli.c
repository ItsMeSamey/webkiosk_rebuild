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
#include "client.h"

char is_exit_char(char c){
  if (c == 'q' || c == 'Q' || c == '\0') return 1;
  return 0;
}


int main() {
  system("stty raw");
  char ch;
  printf("Enter a character: ");
  while (is_exit_char(ch = getchar()) == 0) {
    printf("\b\b\b\bYou entered: %c\r\n", ch);
  }
  system("stty cooked");

  return 0;
}

