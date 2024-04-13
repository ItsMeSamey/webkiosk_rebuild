#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> // fstat
#include <fcntl.h> // open
#include <unistd.h> // close
#include <string.h>
#include "dynamic_array.h"

typedef struct {
  char * content;
  ssize_t len;
} File;

char const* const get_content_type(char const * const filename){
  const char *ext = strrchr(filename, '.');
  if (ext == NULL) {
    return "text/plain";
  }
  if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) {
    return "text/html";
  }
  if (strcmp(ext, ".css") == 0) {
    return "text/css";
  }
  if (strcmp(ext, ".js") == 0) {
    return "application/javascript";
  }
  if (strcmp(ext, ".ico") == 0) {
    return "image/x-icon";
  }
  if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) {
    return "image/jpeg";
  }
  if (strcmp(ext, ".png") == 0) {
    return "image/png";
  }
  if (strcmp(ext, ".gif") == 0) {
    return "image/gif";
  }
  return "text/plain";
}

File get_file(char const * const filename) {
  struct stat file_stat;
  File f = {NULL, 0};
  int fd = open(filename, O_RDONLY);

  if (fd == -1) {
    perror("An error occurred while opening the file");
    return f;
  }
  if (fstat(fd, &file_stat) == -1) {
    perror("An error occurred while getting the file status");
    close(fd);
    return f;
  }
  char buffer[1024+1];
  int occupied = snprintf(buffer, 1024, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %zu\r\n\r\n",
                              get_content_type(filename), file_stat.st_size);
  char *all = (char *)malloc(sizeof(char)*(occupied + file_stat.st_size + 1));
  strncpy(all, buffer, occupied);
  read(fd, all+occupied, file_stat.st_size);
  close(fd);
  f.content = all;
  f.len = file_stat.st_size + occupied;
  return f;
};

File* get_file_list(){
  static File file[4];
  char filename_buff[1024+1];
  FILE *list;

  file[0] = get_file("index.html");

  list = popen("ls assets/*.js", "r");
  fscanf(list, "%[^ ^\n]", filename_buff);
  fclose(list);
  file[1] = get_file(filename_buff);


  list = popen("ls assets/*.css", "r");
  fscanf(list, "%[^ ^\n]", filename_buff);
  fclose(list);
  file[2] = get_file(filename_buff);


  list = popen("ls assets/*.ico", "r");
  fscanf(list, "%[^ ^\n]", filename_buff);
  fclose(list);
  file[3] = get_file(filename_buff);

  return file;
}


