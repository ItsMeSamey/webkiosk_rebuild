#pragma once
 
void *get_auth_curl();
char *authenticate(void *curl_void, char const *const roll_no, char *const password, char const user_type);
void *get_call_curl();
char *make_call(void *curl_void, char const *const cookie, char const *const url);


