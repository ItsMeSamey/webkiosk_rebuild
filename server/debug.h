#pragma once
#include <netinet/in.h>

// #define VERBOSE
// #define DEBUG

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024*1024 // 1MB

typedef struct {
  struct sockaddr_in address;
  int socket;
} Entity;


#if defined (DEBUG_LEVEL)
#define d0error(x...) perror("\nINFO(0): " x)
#define d0print(x...) printf("\nINFO(0): " x)
#define a0rt(x...)    assert(x...);
#define D0(x...) x
#else
#define d0error(x...)
#define d0print(x...)
#define a0rt(x...)
#define D0(x...)
#endif

#if DEBUG_LEVEL > 1
#define d1error(x...) perror("\nINFO(1): " x)
#define d1print(x...) printf("\nINFO(1): " x)
#define a1rt(x...)    assert(x...);
#define D1(x...) x
#else
#define d1error(x...)
#define d1print(x...)
#define a1rt(x...)
#define D1(x...)
#endif

#if DEBUG_LEVEL > 2
#define d2error(x...) perror("\nINFO(2): " x)
#define d2print(x...) printf("\nINFO(2): " x)
#define a2rt(x...)    assert(x...);
#define D2(x...) x
#else
#define d2error(x...)
#define d2print(x...)
#define a2rt(x...)
#define D2(x...)
#endif

#if DEBUG_LEVEL > 3
#define d3error(x...) perror("\nINFO(3): " x)
#define d3print(x...) printf("\nINFO(3): " x)
#define a3rt(x...)    assert(x...);
#define D3(x...) x
#else
#define d3error(x...)
#define d3print(x...)
#define a2rt(x...)
#define D3(x...)
#endif

#if DEBUG_LEVEL > 4
#define d3error(x...) perror("\nINFO(4): " x)
#define d3print(x...) printf("\nINFO(4): " x)
#define a4rt(x...)    assert(x...);
#define D4(x...) x
#else
#define d4error(x...)
#define d4print(x...)
#define a4rt(x...)
#define D4(x...)
#endif

