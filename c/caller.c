#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include "caller.h"

typedef struct {
  char *cookie;
  int status;
} Cookie;

static size_t header_callback(char *buffer, size_t size, size_t nmemb, void *userdata) {
#ifdef VERBOSE
  printf("!!!!!!!!!!!!Headers!!!!!!!!!!!!!! \n%*s:::::::::::::Headers:::::::::::::\n\n", (int)(size * nmemb), buffer);
#endif
  Cookie* data = (Cookie*)userdata;
  // Header: `Set-Cookie: JSESSIONID=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX; Path=/; Secure; HttpOnly`
  if (strncmp(buffer, "Set-Cookie:", 11) == 0) {
    sscanf(buffer, "%*[^=]%*1c%63[^;]", data->cookie);
    if (data->status != -1) { return 0; } // Skip further processing
  } else if (data->status == -1 && strncmp(buffer, "HTTP/1.1", 8) == 0) {
    sscanf(buffer, "%*[^ ]%*[ ]%d", &data->status);
  }
  return size * nmemb;
}

static size_t null_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
#ifdef VERBOSE
  printf("!!!!!!!!!!!!Null!!!!!!!!!!!!!! \n%*s\n:::::::::::::Null:::::::::::::\n\n", (int)(size * nmemb), (char*)ptr);
#endif
  return size * nmemb;
}

static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
#ifdef VERBOSE
  printf("!!!!!!!!!!!!Body!!!!!!!!!!!!!! \n%*s\n:::::::::::::Body:::::::::::::\n\n", (int)(size * nmemb), (char*)ptr);
#endif
  char **data = (char **)userdata;
  // data = (char**)realloc(data, strlen(*data) + size * nmemb + 1);
  data = (char**)malloc(size * nmemb + 1);
  strncpy(*data, (char*)ptr, size * nmemb);
  (*data)[size * nmemb] = '\0';
  return size * nmemb;
}

void *get_auth_curl() {
  CURL *curl = curl_easy_init();
  if (!curl) {
#ifdef DEBUD
    fprintf(stderr, "curl_easy_init() failed \n");
#endif
    return NULL;
  }
  curl_easy_setopt(curl, CURLOPT_URL, "https://webkiosk.thapar.edu/CommonFiles/UserAction.jsp");
  curl_easy_setopt(curl, CURLOPT_POST, 1);

  // DANGER !!
  curl_easy_setopt(curl, CURLOPT_STDERR, NULL);
  // !! DANGER

  curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, null_callback);
  // curl_easy_setopt(curl, CURLOPT_CACHE, 1);
  return curl;
  // curl_easy_cleanup(curl);
}

char const *const authenticate(void *curl_void, char const *const roll_no, char *const password, char const user_type){
  CURL *curl = (CURL*)curl_void;
  CURLcode res;

  Cookie *data = (Cookie*)malloc(sizeof(Cookie)); 
  data->cookie = (char*)malloc(64);
  data->status = -1;

  char post_fields[2048];
  snprintf(post_fields, sizeof(post_fields), "UserType=%c&MemberCode=%s&Password=%s", user_type, roll_no, password);

  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields);
  curl_easy_setopt(curl, CURLOPT_HEADERDATA, data);

  res = curl_easy_perform(curl);

  if (res != CURLE_OK && data->status != 302) {
#ifdef DEBUD
  if (res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed (in authenticate()): %s\n", curl_easy_strerror(res));
  } else if (data->status != 302) {
    fprintf(stderr, "Authentication Failed (status: %d)\n", data->status);
  }
#endif
    return NULL;
  } return data->cookie;
}

void *get_call_curl() {
  CURL *curl = curl_easy_init();
  if (!curl) {
#ifdef DEBUD
    fprintf(stderr, "curl_easy_init() failed \n");
#endif
    return NULL;
  }
  // DANGER !!
  curl_easy_setopt(curl, CURLOPT_STDERR, NULL);
  // !! DANGER

  curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, null_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  // curl_easy_setopt(curl, CURLOPT_CACHE, 1);
  return curl;
  // curl_easy_cleanup(curl);
}
// Cookie:JSESSIONID=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
char const *const make_call(void *curl_void, char const *const cookie, char const *const url) {
  CURL *curl = (CURL*)curl_void;
  CURLcode res;

  char cookie_complete[64];
  snprintf(cookie_complete, sizeof(cookie_complete), "JSESSIONID=%s", cookie);

  char *data = NULL;
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_COOKIE, cookie_complete);
  curl_easy_setopt(curl, CURLOPT_HEADERDATA, &data);

  res = curl_easy_perform(curl);

  if (res != CURLE_OK) {
#ifdef DEBUD
    fprintf(stderr, "curl_easy_perform() failed (in make_call()): %s\n", curl_easy_strerror(res));
#endif
    return NULL;
  }
  return data;
}

