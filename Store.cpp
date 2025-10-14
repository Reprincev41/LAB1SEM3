#include "Store.h"
#include <fstream>
#include <sstream>
#include <functional>
#include <cstring>
#include <stdexcept>

void initializeStore(struct DataStore* store) {
    for (int i = 0; i < MAX_STRUCTURES; ++i) {
        store->entries[i].isUsed = false;
        store->entries[i].dataPtr = nullptr;
        store->entries[i].type = NONE_TYPE;
        store->entries[i].name[0] = '\0';
    }
    store->count = 0;
}

struct StoreEntry* findEntry(struct DataStore* store, const std::string& name) {
    for (int i = 0; i < MAX_STRUCTURES; ++i) {
        if (store->entries[i].isUsed && name == store->entries[i].name) {
            return &store->entries[i];
        }
    }
    return nullptr;
}

struct StoreEntry* findFreeEntry(struct DataStore* store) {
    if (store->count >= MAX_STRUCTURES) return nullptr;
    for (int i = 0; i < MAX_STRUCTURES; ++i) {
        if (!store->entries[i].isUsed) {
            return &store->entries[i];
        }
    }
    return nullptr;
}

void destroyEntryData(struct StoreEntry* entry) {
    if (!entry || !entry->isUsed) return;
    switch (entry->type) {
        case ARRAY_TYPE:
            MDESTROY(static_cast<DynamicArray*>(entry->dataPtr));
            delete static_cast<DynamicArray*>(entry->dataPtr);
            break;
        case FLIST_TYPE:
            FDESTROY(static_cast<SinglyLinkedList*>(entry->dataPtr));
            delete static_cast<SinglyLinkedList*>(entry->dataPtr);
            break;
        case LLIST_TYPE:
            LDESTROY(static_cast<DoublyLinkedList*>(entry->dataPtr));
            delete static_cast<DoublyLinkedList*>(entry->dataPtr);
            break;
        case STACK_TYPE:
            SDESTROY(static_cast<Stack*>(entry->dataPtr));
            delete static_cast<Stack*>(entry->dataPtr);
            break;
        case QUEUE_TYPE:
            QDESTROY(static_cast<Queue*>(entry->dataPtr));
            delete static_cast<Queue*>(entry->dataPtr);
            break;
        case TREE_TYPE:
            TDESTROY(static_cast<AVLTree*>(entry->dataPtr));
            delete static_cast<AVLTree*>(entry->dataPtr);
            break;
        default:
            break;
    }
    entry->dataPtr = nullptr;
    entry->isUsed = false;
    entry->type = NONE_TYPE;
    entry->name[0] = '\0';
}

void* createAndAddStructure(struct DataStore* store, const std::string& name, enum StructureType type) {
    struct StoreEntry* entry = findEntry(store, name);
    if(entry) {
        destroyEntryData(entry);
        store->count--;
    }

    entry = findFreeEntry(store);
    if (!entry) throw std::runtime_error("Maximum number of structures reached.");
    void* newData = nullptr;
    if (type == ARRAY_TYPE) {
        DynamicArray* arr = new DynamicArray;
        MCREATE(arr);
        newData = arr;
    } else if (type == FLIST_TYPE) {
        SinglyLinkedList* list = new SinglyLinkedList;
        FCREATE(list);
        newData = list;
    } else if (type == LLIST_TYPE) {
        DoublyLinkedList* list = new DoublyLinkedList;
        LCREATE(list);
        newData = list;
    } else if (type == STACK_TYPE) {
        Stack* stack = new Stack;
        SCREATE(stack);
        newData = stack;
    } else if (type == QUEUE_TYPE) {
        Queue* queue = new Queue;
        QCREATE(queue);
        newData = queue;
    } else if (type == TREE_TYPE) {
        AVLTree* tree = new AVLTree;
        TCREATE(tree);
        newData = tree;
    } else {
        throw std::runtime_error("Invalid structure type.");
    }
    strncpy(entry->name, name.c_str(), MAX_NAME_LENGTH - 1);
    entry->name[MAX_NAME_LENGTH - 1] = '\0';
    entry->type = type;
    entry->dataPtr = newData;
    entry->isUsed = true;
    store->count++;
    return newData;
}

void destroyStore(struct DataStore* store) {
    for (int i = 0; i < MAX_STRUCTURES; ++i) {
        if (store->entries[i].isUsed) {
            destroyEntryData(&store->entries[i]);
        }
    }
    store->count = 0;
}

void saveToFile(const struct DataStore* store, const std::string& filename) {
    if (filename.empty()) return;
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open file '" << filename << "' for writing." << std::endl;
        return;
    }
    for (int i = 0; i < MAX_STRUCTURES; ++i) {
        const struct StoreEntry* entry = &store->entries[i];
        if (!entry->isUsed) continue;
        switch (entry->type) {
            case ARRAY_TYPE: file << "ARRAY "; break;
            case FLIST_TYPE: file << "FLIST "; break;
            case LLIST_TYPE: file << "LLIST "; break;
            case STACK_TYPE: file << "STACK "; break;
            case QUEUE_TYPE: file << "QUEUE "; break;
            case TREE_TYPE: file << "TREE "; break;
            default: continue;
        }
        file << entry->name;
        if (entry->type == ARRAY_TYPE) {
            DynamicArray* arr = static_cast<DynamicArray*>(entry->dataPtr);
            for (int j = 0; j < arr->size; ++j) file << " " << MGET(arr, j);
        } else if (entry->type == FLIST_TYPE) {
            SinglyLinkedList* list = static_cast<SinglyLinkedList*>(entry->dataPtr);
            struct FNode* current = list->head;
            while(current) { file << " " << current->data; current = current->next; }
        } else if (entry->type == LLIST_TYPE) {
            DoublyLinkedList* list = static_cast<DoublyLinkedList*>(entry->dataPtr);
            struct LNode* current = list->head;
            while(current) { file << " " << current->data; current = current->next; }
        } else if (entry->type == STACK_TYPE) {
            Stack* stack = static_cast<Stack*>(entry->dataPtr);
            std::function<void(FNode*)> saveRecursive =
                [&](struct FNode* node) {
                if (!node) return;
                saveRecursive(node->next);
                file << " " << node->data;
            };
            saveRecursive(stack->top);
        } else if (entry->type == QUEUE_TYPE) {
            Queue* queue = static_cast<Queue*>(entry->dataPtr);
            struct FNode* current = queue->front;
            while(current) { file << " " << current->data; current = current->next; }
        } else if (entry->type == TREE_TYPE) {
            AVLTree* tree = static_cast<AVLTree*>(entry->dataPtr);
            std::function<void(TNode*)> saveInorder =
                [&](struct TNode* node) {
                if (!node) return;
                saveInorder(node->left);
                file << " " << node->data;
                saveInorder(node->right);
            };
            saveInorder(tree->root);
        }
        file << std::endl;
    }
}

void loadFromFile(struct DataStore* store, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;
    destroyStore(store);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream lineStream(line);
        std::string typeStr, name, value;
        lineStream >> typeStr >> name;
        if (typeStr == "ARRAY") {
            DynamicArray* arr = static_cast<DynamicArray*>(createAndAddStructure(store, name, ARRAY_TYPE));
            while (lineStream >> value) MPUSH_BACK(arr, value);
        } else if (typeStr == "FLIST") {
            SinglyLinkedList* list = static_cast<SinglyLinkedList*>(createAndAddStructure(store, name, FLIST_TYPE));
            while (lineStream >> value) FPUSH_TAIL(list, value);
        } else if (typeStr == "LLIST") {
            DoublyLinkedList* list = static_cast<DoublyLinkedList*>(createAndAddStructure(store, name, LLIST_TYPE));
            while (lineStream >> value) LPUSH_TAIL(list, value);
        } else if (typeStr == "STACK") {
            Stack* stack = static_cast<Stack*>(createAndAddStructure(store, name, STACK_TYPE));
            while (lineStream >> value) SPUSH(stack, value);
        } else if (typeStr == "QUEUE") {
            Queue* queue = static_cast<Queue*>(createAndAddStructure(store, name, QUEUE_TYPE));
            while (lineStream >> value) QPUSH(queue, value);
        } else if (typeStr == "TREE") {
            AVLTree* tree = static_cast<AVLTree*>(createAndAddStructure(store, name, TREE_TYPE));
            while (lineStream >> value) TINSERT(tree, value);
        }
    }
}