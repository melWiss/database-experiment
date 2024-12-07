#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { true = 1, false = 0 } bool;

FILE *getDatabasefile(char *path, char *filemode) {
  FILE *file;
  file = fopen(path, filemode);
  return file;
}

FILE *getReadableDatabasefile(char *path) {
  return getDatabasefile(path, "rb");
}

FILE *getWritableDatabasefile(char *path) {
  return getDatabasefile(path, "wb");
}

int main() {
  FILE *file = getWritableDatabasefile("./db.bin");
  if (file != NULL) {
    printf("file exists!");
  } else {
    printf("file doesnt exist :'(");
  }
  printf("\n");
  char message[3] = "hey";
  fwrite(message, sizeof(message), 1, file);
  fclose(file);
  return 0;
}
