typedef struct hashTable *HashTable;

HashTable HashTableCreate(void);

void HashTableDestroy(HashTable);

void HashTableInsert(HashTable, const char *key);

bool HashTableSearch(HashTable, const char *key);
