#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { true = 1, false = 0 } bool;

void nl() { printf("\n"); }

FILE *getDatabasefile(char *path, char *filemode) {
  return fopen(path, filemode);
}

FILE *getReadableDatabasefile(char *path) {
  return getDatabasefile(path, "rb");
}

FILE *getWritableDatabasefile(char *path) {
  return getDatabasefile(path, "wb");
}

bool fileExists(char *path) {
  FILE *file = getReadableDatabasefile(path);
  if (file != NULL) {
    fclose(file);
    return true;
  }
  fclose(file);
  return false;
}

typedef struct {
  char id[20];
  char data[1024];
} Document;

typedef struct {
  char value[20];
  int ref;
} Index;

typedef struct {
  int documentsCount;
  Document *headDocument;
  Index *headIndex;
  char filePath[256];
} Database;

void printDocument(Document *docs) {
  printf("_______________________________________\n");
  printf("ID: %s\n", docs->id);
  printf("DATA: %s\n", docs->data);
  printf("_______________________________________\n");
}

Database getDatabase(char *path) {
  FILE *file = getReadableDatabasefile(path);
  Database db;
  strcpy(db.filePath, path);
  if (file == NULL) {
    db.documentsCount = 0;
    db.headDocument = NULL;
    db.headIndex = NULL;
    return db;
  }
  fread(&db.documentsCount, sizeof(int), 1, file);
  fread(&db.headDocument, sizeof(Document), 1, file);
  fseek(file, sizeof(int) + sizeof(Document) * db.documentsCount, SEEK_SET);
  fread(&db.headIndex, sizeof(Index), 1, file);
  return db;
}

void writeDocument(Document *document, Database *database) {
  printf("\nWRITING_DOC\n");
  FILE *file = getWritableDatabasefile(database->filePath);
  database->documentsCount++;
  fwrite(&database->documentsCount, sizeof(int), 1, file);
  fseek(file, sizeof(int) + sizeof(Document) * (database->documentsCount - 1),
        SEEK_SET);
  fwrite(document, sizeof(Document), 1, file);
}

void writeDocuments(Document *docs, int length, FILE *file) {
  // printf("\nWRITING_DOCS!\n");
  fwrite(docs, sizeof(Document), length, file);
  // for (Document *ptr = docs; length--; ptr++) {
  //   printDocument(ptr);
  // }
}
int writeExample();
int readExample();
int main() { return readExample(); }

int writeExample() {
  FILE *file = getWritableDatabasefile("./db.bin");

  int length = 4;
  Document *docs = malloc(sizeof(Document) * length);
  Document *ptr = docs;
  Database *database = malloc(sizeof(Database));
  strcpy(ptr->data, "lorem ipsum boy");
  strcpy(ptr->id, "jkblkejnms;dvf");
  ptr++;
  strcpy(ptr->data, "lorem ipsum boy");
  strcpy(ptr->id, "jkblkejnms;dvf");
  ptr++;
  strcpy(ptr->data, "lorem ipsum boy");
  strcpy(ptr->id, "jkblkejnms;dvf");
  ptr++;
  strcpy(ptr->data, "lorem ipsum boy");
  strcpy(ptr->id, "jkblkejnms;dvf");
  ptr++;
  writeDocuments(docs, length, file);
  return 0;
}

int readExample() {
  FILE *file = getReadableDatabasefile("./db.bin");
  if (file == NULL) {
    printf("FILE NOT EXIST\n");
    return 1;
  }
  int length = 4;
  Document *docs = malloc(sizeof(Document) * length);
  fread(docs, sizeof(Document), length, file);
  for (Document *ptr = docs; length--; ptr++) {
    printDocument(ptr);
  }
  return 0;
}
