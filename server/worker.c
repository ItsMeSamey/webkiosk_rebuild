#include <curl/curl.h>
#include <pthread.h>
#include <bits/pthreadtypes.h>

typedef struct {
  CURL *curl;
  pthread_mutex_t lock;
} Worker;



void async_handler(int socket){
  return;
}


