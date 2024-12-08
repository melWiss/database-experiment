#include "main4.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Database *createDatabase(char *path) {
  Database *db = malloc(sizeof(Database));
  db->length = 0;
  db->collections = NULL;
  return db;
}

IndexDefinition *
createIndexDefinition(Database *database,
                      char collectionName[COLLECTION_NAME_SIZE],
                      char field[FIELD_NAME_SIZE], Order order) {
  IndexDefinition *indexDefinition = malloc(sizeof(IndexDefinition));
  indexDefinition->count = 1;
  strcpy(indexDefinition->collection, collectionName);
  strcpy(indexDefinition->field, field);
  indexDefinition->order = asc;
  indexDefinition->indexes = NULL;
  indexDefinition->next = NULL;
  return indexDefinition;
}

Collection *createCollection(Database *database,
                             char collectionName[COLLECTION_NAME_SIZE]) {
  Collection *collection = malloc(sizeof(Collection));
  collection->count = 0;
  strcpy(collection->name, collectionName);
  collection->docs = NULL;
  collection->next = NULL;
  collection->indexes =
      createIndexDefinition(database, collectionName, "id", asc);
  return collection;
}

char *createDocumentID() {
  const char chars[] =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
  const int chars_length = strlen(chars);
  char *autoId = (char *)malloc(21 * sizeof(char));
  static int seeded = 0;
  if (!seeded) {
    srand(time(NULL));
    seeded = 1;
  }
  for (int i = 0; i < 20; i++) {
    int random_index = rand() % chars_length;
    autoId[i] = chars[random_index];
  }
  autoId[20] = '\0';
  assert(strlen(autoId) == 20 && "Invalid auto ID length");
  return autoId;
}
Document *insertDocument(Database *database,
                         char collectionName[COLLECTION_NAME_SIZE],
                         char data[DOCUMENT_DATA_SIZE]) {
  // First check if collection exists
  Collection *collection = getCollection(database, collectionName);
  if (collection == NULL) {
    return NULL;
  }

  Document *document = malloc(sizeof(Document));
  char *newId = createDocumentID();
  strcpy(document->id, newId);
  free(newId); // Free the temporary ID string
  strcpy(document->data, data);
  strcpy(document->collection, collectionName);
  document->next = NULL;

  // Insert document into collection
  if (collection->count == 0) {
    collection->docs = document;
  } else {
    Document *docPtr = collection->docs;
    while (docPtr->next != NULL) {
      docPtr = docPtr->next;
    }
    docPtr->next = document;
  }

  // Add index for the document
  addIndex(collection, "id", document->id, collection->count);
  collection->count++;

  return document;
}

Index *addIndex(Collection *collection, char field[FIELD_NAME_SIZE],
                void *value, int documentReference) {
  Index *index = malloc(sizeof(Index));
  index->value = malloc(DOCUMENT_ID_SIZE); // Allocate memory for the value
  memcpy(index->value, value, DOCUMENT_ID_SIZE); // Use memcpy instead of strcpy
  index->position = documentReference;
  index->next = NULL;

  IndexDefinition *indexDefPtr = collection->indexes;
  while (indexDefPtr != NULL) {
    if (strcmp(indexDefPtr->field, field) == 0) {
      break;
    }
    indexDefPtr = indexDefPtr->next;
  }

  if (indexDefPtr == NULL) {
    free(index->value);
    free(index);
    return NULL;
  }

  if (indexDefPtr->count == 0) {
    indexDefPtr->indexes = index;
  } else {
    Index *indexPtr = indexDefPtr->indexes;
    if (indexPtr == NULL) {
      indexDefPtr->indexes = index;
    } else {
      while (indexPtr->next != NULL) {
        indexPtr = indexPtr->next;
      }
      indexPtr->next = index;
    }
  }
  indexDefPtr->count++;
  return index;
}

Collection *getCollection(Database *database,
                          char collectionName[COLLECTION_NAME_SIZE]) {
  Collection *collPtr = database->collections;
  while (collPtr != NULL) {
    if (strcmp(collPtr->name, collectionName) == 0) {
      break;
    }
    collPtr = collPtr->next;
  }
  return collPtr;
}

Document *getDocument(Database *database,
                      char collectionName[COLLECTION_NAME_SIZE],
                      char id[DOCUMENT_ID_SIZE]) {
  Document *docPtr;
  Collection *collection = getCollection(database, collectionName);
  if (collection == NULL) {
    return NULL;
  }
  Index *index = getIndex(collection, "id", id);
  docPtr = collection->docs;
  // since it's a linked list implementation, we actually don't need to get the
  // index we will traverse the linked list of docs anyway.
  while (docPtr != NULL && strcmp(id, docPtr->id) != 0) {
    docPtr = docPtr->next;
  }
  return docPtr;
}

Index *getIndex(Collection *collection, char field[FIELD_NAME_SIZE],
                void *value) {
  Index *index;
  IndexDefinition *indexDefPtr = collection->indexes;
  while (indexDefPtr != NULL && strcmp(field, indexDefPtr->field) != 0) {
    indexDefPtr = indexDefPtr->next;
  }
  if (indexDefPtr == NULL) {
    return NULL;
  }

  index = indexDefPtr->indexes;
  while (index != NULL && strcmp(index->value, value) != 0) {
    index = index->next;
  }
  if (index == NULL) {
    return NULL;
  }
  return index;
}
