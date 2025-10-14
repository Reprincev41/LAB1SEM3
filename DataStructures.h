#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>

#define MAX_NAME_LENGTH 32
#define MAX_STRUCTURES 100

enum StructureType {
    NONE_TYPE, ARRAY_TYPE, FLIST_TYPE, LLIST_TYPE, STACK_TYPE, QUEUE_TYPE, TREE_TYPE
};

struct FNode {
    std::string data;
    struct FNode* next;
};

struct LNode {
    std::string data;
    struct LNode* next;
    struct LNode* prev;
};

struct TNode {
    std::string data;
    int height;
    struct TNode* left;
    struct TNode* right;
};

struct DynamicArray {
    std::string* elements;
    int size;
    int capacity;
};

struct SinglyLinkedList {
    struct FNode* head;
    struct FNode* tail;
    int length;
};

struct DoublyLinkedList {
    struct LNode* head;
    struct LNode* tail;
    int length;
};

struct Stack {
    struct FNode* top;
    int count;
};

struct Queue {
    struct FNode* front;
    struct FNode* rear;
    int count;
};

struct AVLTree {
    struct TNode* root;
};

void MCREATE(struct DynamicArray* array);
void MDESTROY(struct DynamicArray* array);
void MPUSH_BACK(struct DynamicArray* array, const std::string& value);
void MINSERT_AT(struct DynamicArray* array, int index, const std::string& value);
void MSET_AT(struct DynamicArray* array, int index, const std::string& value);
std::string MDEL_AT(struct DynamicArray* array, int index);
std::string MGET(const struct DynamicArray* array, int index);
bool MIS_MEMBER(const struct DynamicArray* array, const std::string& value);
int MLENGTH(const struct DynamicArray* array);
void MPRINT(const struct DynamicArray* array);

void FCREATE(struct SinglyLinkedList* list);
void FDESTROY(struct SinglyLinkedList* list);
void FPUSH_HEAD(struct SinglyLinkedList* list, const std::string& value);
void FPUSH_TAIL(struct SinglyLinkedList* list, const std::string& value);
bool FINS_BEFORE_VALUE(struct SinglyLinkedList* list, const std::string& beforeValue, const std::string& newValue);
bool FINS_AFTER_VALUE(struct SinglyLinkedList* list, const std::string& afterValue, const std::string& newValue);
std::string FDEL_HEAD(struct SinglyLinkedList* list);
std::string FDEL_TAIL(struct SinglyLinkedList* list);
bool FDEL_BY_VALUE(struct SinglyLinkedList* list, const std::string& value);
bool FDEL_BEFORE_VALUE(struct SinglyLinkedList* list, const std::string& value);
bool FDEL_AFTER_VALUE(struct SinglyLinkedList* list, const std::string& value);
std::string FGET_HEAD(const struct SinglyLinkedList* list);
std::string FGET_TAIL(const struct SinglyLinkedList* list);
std::string FGET_AT(const struct SinglyLinkedList* list, int index);
bool FIS_MEMBER(const struct SinglyLinkedList* list, const std::string& value);
void FPRINT(const struct SinglyLinkedList* list);

void LCREATE(struct DoublyLinkedList* list);
void LDESTROY(struct DoublyLinkedList* list);
void LPUSH_HEAD(struct DoublyLinkedList* list, const std::string& value);
void LPUSH_TAIL(struct DoublyLinkedList* list, const std::string& value);
bool LINS_BEFORE_VALUE(struct DoublyLinkedList* list, const std::string& beforeValue, const std::string& newValue);
bool LINS_AFTER_VALUE(struct DoublyLinkedList* list, const std::string& afterValue, const std::string& newValue);
std::string LDEL_HEAD(struct DoublyLinkedList* list);
std::string LDEL_TAIL(struct DoublyLinkedList* list);
bool LDEL_BY_VALUE(struct DoublyLinkedList* list, const std::string& value);
bool LDEL_BEFORE_VALUE(struct DoublyLinkedList* list, const std::string& value);
bool LDEL_AFTER_VALUE(struct DoublyLinkedList* list, const std::string& value);
std::string LGET_HEAD(const struct DoublyLinkedList* list);
std::string LGET_TAIL(const struct DoublyLinkedList* list);
std::string LGET_AT(const struct DoublyLinkedList* list, int index);
bool LIS_MEMBER(const struct DoublyLinkedList* list, const std::string& value);
void LPRINT(const struct DoublyLinkedList* list);

void SCREATE(struct Stack* stack);
void SDESTROY(struct Stack* stack);
void SPUSH(struct Stack* stack, const std::string& value);
std::string SPOP(struct Stack* stack);
std::string SPEEK(const struct Stack* stack);
int SLENGTH(const struct Stack* stack);
void SPRINT(const struct Stack* stack);

void QCREATE(struct Queue* queue);
void QDESTROY(struct Queue* queue);
void QPUSH(struct Queue* queue, const std::string& value);
std::string QPOP(struct Queue* queue);
std::string QPEEK(const struct Queue* queue);
int QLENGTH(const struct Queue* queue);
void QPRINT(const struct Queue* queue);

void TCREATE(struct AVLTree* tree);
void TDESTROY(struct AVLTree* tree);
void TINSERT(struct AVLTree* tree, const std::string& value);
bool TDEL(struct AVLTree* tree, const std::string& value);
bool TIS_MEMBER(const struct AVLTree* tree, const std::string& value);
void TPRINT(const struct AVLTree* tree);

#endif