#ifndef STORE_H
#define STORE_H

#include "DataStructures.h"

struct StoreEntry {
    char name[MAX_NAME_LENGTH];
    enum StructureType type;
    void* dataPtr;
    bool isUsed;
};

struct DataStore {
    struct StoreEntry entries[MAX_STRUCTURES];
    int count;
};

void initializeStore(struct DataStore* store);
struct StoreEntry* findEntry(struct DataStore* store, const std::string& name);
void* createAndAddStructure(struct DataStore* store, const std::string& name, enum StructureType type);
void destroyStore(struct DataStore* store);
void saveToFile(const struct DataStore* store, const std::string& filename);
void loadFromFile(struct DataStore* store, const std::string& filename);

#endif