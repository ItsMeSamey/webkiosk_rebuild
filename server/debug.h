#pragma once
#include <netinet/in.h>

// #define NDEBUG
// #define DEBUG_LEVEL 10

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024*1024 // 1MB

typedef struct {
  struct sockaddr_in address;
  int socket;
} Entity;


#ifndef NDEBUG
#define  derror(x...) printf("\n>> %s:%d\n", __FILE__,__LINE__);perror("INFO(!): " x)
#define  dprint(x...) printf("\n>> %s:%d\n", __FILE__,__LINE__);printf("INFO(!): " x)
#define  D(x...) x
#else // !NDEBUG
#define  derror(x...) ;
#define  dprint(x...) ;
#define  D(x...) ;
#endif

#if defined (DEBUG_LEVEL)  
#define d0error(x...) printf("\n>> %s:%d\n", __FILE__,__LINE__);perror("INFO(0): " x)
#define d0print(x...) printf("\n>> %s:%d\n", __FILE__,__LINE__);printf("INFO(0): " x)
#define D0(x...) x
#else
#define d0error(x...) ;
#define d0print(x...) ;
#define D0(x...) ;
#endif

#if DEBUG_LEVEL > 1
#define d1error(x...) printf("\n>> %s:%d\n", __FILE__,__LINE__);perror("INFO(1): " x)
#define d1print(x...) printf("\n>> %s:%d\n", __FILE__,__LINE__);printf("INFO(1): " x)
#define D1(x...) x
#else
#define d1error(x...) ;
#define d1print(x...) ;
#define D1(x...) ;
#endif

#if DEBUG_LEVEL > 2
#define d2error(x...) printf("\n>> %s:%d\n", __FILE__,__LINE__);perror("INFO(2): " x)
#define d2print(x...) printf("\n>> %s:%d\n", __FILE__,__LINE__);printf("INFO(2): " x)
#define D2(x...) x
#else
#define d2error(x...) ;
#define d2print(x...) ;
#define D2(x...) ;
#endif

#if DEBUG_LEVEL > 3
#define d3error(x...) printf("\n>> %s:%d\n", __FILE__,__LINE__);perror("INFO(3): " x)
#define d3print(x...) printf("\n>> %s:%d\n", __FILE__,__LINE__);printf("INFO(3): " x)
#define D3(x...) x
#else
#define d3error(x...) ;
#define d3print(x...);
#define D3(x...) ;
#endif

#if DEBUG_LEVEL > 4
#define d4error(x...) printf("\n>> %s:%d\n", __FILE__,__LINE__);perror("INFO(4): " x)
#define d4print(x...) printf("\n>> %s:%d\n", __FILE__,__LINE__);printf("INFO(4): " x)
#define D4(x...) x
#else
#define d4error(x...) ;
#define d4print(x...) ;
#define D4(x...) ;
#endif

