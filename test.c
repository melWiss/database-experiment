#include "main4.h"
#include <stdio.h>

int main() {
  // Create database
  Database *db = createDatabase("test");
  if (db == NULL) {
    printf("Failed to create database\n");
    return 1;
  }
  printf("Database created successfully\n");

  // Create collection
  Collection *users = createCollection(db, "users");
  if (users == NULL) {
    printf("Failed to create collection\n");
    return 1;
  }

  // Add collection to database's collections list
  db->collections = users;
  db->length++;
  printf("Collection 'users' created successfully\n");

  // Insert a document
  char userData[] = "{\"name\": \"John Doe\", \"age\": 30}";
  printf("Attempting to insert document...\n");
  Document *doc = insertDocument(db, "users", userData);
  if (doc == NULL) {
    printf("Failed to insert document\n");
    return 1;
  }
  printf("Document inserted successfully with ID: %s\n", doc->id);

  // Read the document back
  printf("\nAttempting to read document...\n");
  Document *readDoc = getDocument(db, "users", doc->id);
  if (readDoc == NULL) {
    printf("Failed to read document\n");
    return 1;
  }
  printf("Document read successfully:\n");
  printf("ID: %s\n", readDoc->id);
  printf("Data: %s\n", readDoc->data);
  printf("Collection: %s\n", readDoc->collection);

  // Get the collection and print its documents
  Collection *retrievedCollection = getCollection(db, "users");
  if (retrievedCollection == NULL) {
    printf("Collection not found\n");
    return 1;
  }

  printf("\nAll documents in collection 'users':\n");
  Document *currentDoc = retrievedCollection->docs;
  while (currentDoc != NULL) {
    printf("ID: %s\n", currentDoc->id);
    printf("Data: %s\n", currentDoc->data);
    printf("-------------------\n");
    currentDoc = currentDoc->next;
  }

  printf("Total documents in collection: %d\n", retrievedCollection->count);

  // TODO: Need to implement shinraTensei() to properly free memory
  printf("press anything to quit...\n");
  int exit;
  scanf("%d", &exit);
  return 0;
}
