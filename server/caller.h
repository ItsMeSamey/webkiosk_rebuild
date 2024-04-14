#pragma once
 
int auth(char const *const roll_no, char *const password, char const user_type, char *to);
int call(char const *const cookie, char const *const url, char **out);

