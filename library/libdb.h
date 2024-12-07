#include "types.h"

Database *createDatabase(char *path);
Database *readDatabase(char *path);
bool existDatabase(Database *database);

void setIndex(Index *index, Database *database, char *path);
bool createDocument(Document *document, Database *database, char *path);
Document *readDocument(char *collection, char *id, Database *database,
                       char *path);
bool setDocument(Document *document, Database *database, char *path);
bool updateDocument(Document *document, Database *database, char *path);

QueryResult query(char collection[12], QueryFilter *filters, int limit);
QueryResult getDocs(char collection[12]);
QueryResult getDoc(char collection[12], char id[20]);
