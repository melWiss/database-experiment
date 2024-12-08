#define COLLECTION_NAME_SIZE 12
#define DOCUMENT_ID_SIZE 20
#define DOCUMENT_DATA_SIZE 1024
#define FIELD_NAME_SIZE 8

typedef enum { asc, desc } Order;
typedef enum { greater, less, equal } Operation;
typedef enum { false, true } Boolean;

typedef struct Document {
  char collection[COLLECTION_NAME_SIZE];
  char id[DOCUMENT_ID_SIZE];
  char data[DOCUMENT_DATA_SIZE];
  struct Document *next;
} Document;

typedef struct Index {
  void *value;
  int position;
  struct Index *next;
} Index;

typedef struct IndexDefinition {
  char collection[COLLECTION_NAME_SIZE];
  char field[FIELD_NAME_SIZE];
  Order order;
  int count;
  Index *indexes;
  struct IndexDefinition *next;
} IndexDefinition;

typedef struct {
  char field[FIELD_NAME_SIZE];
  Operation operation;
  void *value;
} QueryFilter;

typedef struct {
  int length;
  Document *docs;
} QueryResult;

typedef struct Collection {
  char name[COLLECTION_NAME_SIZE];
  int count;
  Document *docs;
  IndexDefinition *indexes;
  struct Collection *next;
} Collection;

typedef struct {
  int length;
  Collection *collections;
} Database;

char *createDocumentID();

Database *createDatabase(char *path);
Collection *createCollection(Database *database,
                             char collectionName[COLLECTION_NAME_SIZE]);

QueryResult *query(Database *database,
                   char collectionName[COLLECTION_NAME_SIZE],
                   QueryFilter *filters, int limit);
Document *getDocument(Database *database,
                      char collectionName[COLLECTION_NAME_SIZE],
                      char id[DOCUMENT_ID_SIZE]);
Collection *getCollection(Database *database,
                          char collectionName[COLLECTION_NAME_SIZE]);
Document *insertDocument(Database *database,
                         char collectionName[COLLECTION_NAME_SIZE],
                         char data[DOCUMENT_DATA_SIZE]);
Boolean updateDocument(Database *database, Document *newDoc);
Document deleteDocument(Database *database,
                        char collectionName[COLLECTION_NAME_SIZE],
                        char id[DOCUMENT_ID_SIZE]);
IndexDefinition *
createIndexDefinition(Database *database,
                      char collectionName[COLLECTION_NAME_SIZE],
                      char field[FIELD_NAME_SIZE], Order order);

Index *addIndex(Collection *collection, char field[FIELD_NAME_SIZE],
                void *value, int documentReference);

Index *updateIndex(Database *database,
                   char collectionName[COLLECTION_NAME_SIZE],
                   char field[FIELD_NAME_SIZE], Order order, void *value,
                   int documentReference);

Boolean removeIndex(Database *database,
                    char collectionName[COLLECTION_NAME_SIZE],
                    char field[FIELD_NAME_SIZE], Order order,
                    int documentReference);
Index *getIndex(Collection *collection, char field[FIELD_NAME_SIZE],
                void *value);

void shinraTensei(Database *db);
