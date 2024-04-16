#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> // fstat
#include <fcntl.h> // open
#include <unistd.h> // close
#include <string.h>

typedef struct {
  char * content;
  ssize_t len;
} File;

char const* const get_content_type(char const * const filename){
  const char *ext = strchr(filename, '.');
  if (ext==NULL || *(ext+1) == '\0') goto T;
  else ext++;
  if (*ext == 'h' ) {
    if (*(ext+4) =='.') return "text/html\r\nContent-Encoding: gzip";
    return "text/html";
  }
  if (*ext == 'c') {
    if (*(ext+3) =='.') return "text/css\r\nContent-Encoding:gzip";
    return "text/css";
  }
  if (*ext == 'j') {
    if (*(ext+2) =='.') return "application/javascript\r\nContent-Encoding:gzip";
    return "application/javascript";
  }
  if (*ext == 'i') {
    if (*(ext+3) =='.') return "image/x-icon\r\nContent-Encoding:gzip";
    return "image/x-icon";
  }
T:return "text/plain";
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


#define lsopen(format, index) list = popen("ls " format, "r");fscanf(list, "%128[^ ^\n]", filename_buff);fclose(list);file[index] = get_file(filename_buff);
File* get_file_list(){
  static File file[2];
  char filename_buff[128+1];
  FILE *list;

  lsopen("index.html*", 0)
  // lsopen("assets/*.js*", 1)
  // lsopen("assets/*.css*", 2)
  lsopen("*.ico*", 1)
  
  return file;
}
#undef lsopen


