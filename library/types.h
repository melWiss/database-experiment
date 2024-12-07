
typedef enum { asc, desc } Order;
typedef enum { greater, less, equal } Operation;

typedef struct {
  char collection[12];
  char id[20];
  char data[1024];
} Document;

typedef struct {
  void *value;
  int position;
} Index;

typedef struct {
  char collection[16];
  char field[8];
  Order order;
  Index **indexes;
} IndexDefinition;

typedef struct {
  int length;
  Document *docs;
  IndexDefinition **indexes;
} Database;

typedef struct {
  char field[8];
  Operation operation;
  void *value;
} QueryFilter;

typedef struct {
  int length;
  Document *docs;
} QueryResult;
