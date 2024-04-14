#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include "caller.h"
#include "dynamic_array.h"

typedef struct {
  char *cookie;
  int status;
} Cookie;

static size_t header_callback(char *buffer, size_t size, size_t nmemb, void *userdata) {
  d4print("!!!!!!!!!!!!Headers!!!!!!!!!!!!!! \n%*s:::::::::::::Headers:::::::::::::\n\n", (int)(size * nmemb), buffer);
  Cookie* data = (Cookie*)userdata;
  // Header: `Set-Cookie: JSESSIONID=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX; Path=/; Secure; HttpOnly`
  if (strncmp(buffer, "Set-Cookie:", 11) == 0) {
    sscanf(buffer+11, "%*[^=]%*1c%33[^;]", data->cookie);
    if (data->status != -1) { return 0; } // Skip further processing
  } else if (data->status == -1 && strncmp(buffer, "HTTP/1.1 ", 9) == 0) {
    sscanf(buffer+9, "%d", &data->status);
  }
  return size * nmemb;
}

static size_t null_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
  d4print("!!!!!!!!!!!!Null!!!!!!!!!!!!!! \n%*s\n:::::::::::::Null:::::::::::::\n\n", (int)(size * nmemb), (char*)ptr);
  return size * nmemb;
}

static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
  d4print("!!!!!!!!!!!!Body!!!!!!!!!!!!!! \n%*s\n:::::::::::::Body:::::::::::::\n\n", (int)(size * nmemb), (char*)ptr);
  char **data = (char **)userdata;
  size_t cap = (size*nmemb);
  if (*data == NULL){
    DARRAY_MAKE(char, *data);
    DARRAY_RESIZE(char, (*data), cap)
    strncpy(*data, (char*)ptr, size * nmemb);
    (*data)[size * nmemb] = '\0';
  } else {
    int pre = strlen(*data);
    DARRAY_RESIZE(char, *data, pre+cap+1)
    strncpy((*data) + pre, (char*)ptr, size * nmemb);
    (*data)[pre + size*nmemb] = '\0';
  }
  return size * nmemb;
}

CURL *get_auth_curl() {
  CURL *curl = curl_easy_init();
  if (!curl) {
    d0print("curl_easy_init() failed \n");
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

int auth(char const *const roll_no, char *const password, char const user_type, char* to){
  CURL *curl = get_auth_curl();
  CURLcode res;

  Cookie data;
  data.cookie = to;
  data.status = -1;

  char post_fields[1024];
  snprintf(post_fields, sizeof(post_fields), "UserType=%c&MemberCode=%s&Password=%s", user_type, roll_no, password);

  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields);
  curl_easy_setopt(curl, CURLOPT_HEADERDATA, &data);

  res = curl_easy_perform(curl);

  D0(
    if (res != CURLE_OK && data.status != 302) {
      if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed (in authenticate()): %s\n", curl_easy_strerror(res));
      } else if (data.status != 302) {
        fprintf(stderr, "Authentication Failed (status: %d)\n", data.status);
      }
    }
  )
  return data.status;
}

CURL *get_call_curl() {
  CURL *curl = curl_easy_init();
  if (!curl) {
    d0print("curl_easy_init() failed \n");
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
int call(char const *const cookie, char const *const _url, __DARRAY(char **out)) {
  CURL *curl = get_call_curl();
  CURLcode res;

  char cookie_complete[64];
  struct curl_slist *chunk = NULL;
  snprintf(cookie_complete, sizeof(cookie_complete), "Cookie:JSESSIONID=%s", cookie);
  chunk = curl_slist_append(chunk, cookie_complete);

  char url[256];
  snprintf(url, 256, "https://webkiosk.thapar.edu/%s", _url);
  char *data = NULL;
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

  res = curl_easy_perform(curl);

  if (res != CURLE_OK) {
    d0print("curl_easy_perform() failed (in make_call()): %s\n", curl_easy_strerror(res));
    return -1;
  }
  *out = data;
  return 0;
}

