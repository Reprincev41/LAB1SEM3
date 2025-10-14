#include "DataStructures.h"
#include <stdexcept>

void resizeArray(struct DynamicArray* array, int newCapacity) {
    if (newCapacity < array->size) newCapacity = array->size;
    if (newCapacity < 4) newCapacity = 4;
    std::string* newElements = new std::string[newCapacity];
    for (int i = 0; i < array->size; ++i) {
        newElements[i] = array->elements[i];
    }
    delete[] array->elements;
    array->elements = newElements;
    array->capacity = newCapacity;
}

void MCREATE(struct DynamicArray* array) {
    array->size = 0;
    array->capacity = 4;
    array->elements = new std::string[array->capacity];
}

void MDESTROY(struct DynamicArray* array) {
    if (array->elements != nullptr) {
        delete[] array->elements;
        array->elements = nullptr;
    }
    array->size = 0;
    array->capacity = 0;
}

void MPUSH_BACK(struct DynamicArray* array, const std::string& value) {
    if (array->size == array->capacity) {
        resizeArray(array, array->capacity * 2);
    }
    array->elements[array->size++] = value;
}

void MINSERT_AT(struct DynamicArray* array, int index, const std::string& value) {
    if (index < 0 || index > array->size) throw std::out_of_range("Invalid index for insert.");
    if (array->size == array->capacity) {
        resizeArray(array, array->capacity * 2);
    }
    for (int i = array->size; i > index; --i) {
        array->elements[i] = array->elements[i - 1];
    }
    array->elements[index] = value;
    array->size++;
}

void MSET_AT(struct DynamicArray* array, int index, const std::string& value) {
    if (index < 0 || index >= array->size) throw std::out_of_range("Invalid index for set.");
    array->elements[index] = value;
}

std::string MDEL_AT(struct DynamicArray* array, int index) {
    if (index < 0 || index >= array->size) throw std::out_of_range("Invalid index.");
    std::string removedValue = array->elements[index];
    for (int i = index; i < array->size - 1; ++i) {
        array->elements[i] = array->elements[i + 1];
    }
    array->size--;
    if (array->size > 0 && array->size <= array->capacity / 4) {
        resizeArray(array, array->capacity / 2);
    }
    return removedValue;
}

std::string MGET(const struct DynamicArray* array, int index) {
    if (index < 0 || index >= array->size) throw std::out_of_range("Invalid index.");
    return array->elements[index];
}

bool MIS_MEMBER(const struct DynamicArray* array, const std::string& value) {
    for (int i = 0; i < array->size; ++i) {
        if (array->elements[i] == value) return true;
    }
    return false;
}

int MLENGTH(const struct DynamicArray* array) {
    return array->size;
}

void MPRINT(const struct DynamicArray* array) {
    for (int i = 0; i < array->size; ++i) {
        std::cout << array->elements[i] << (i < array->size - 1 ? " " : "");
    }
    std::cout << std::endl;
}

struct FNode* createFNode(const std::string& value) {
    struct FNode* node = new struct FNode;
    node->data = value;
    node->next = nullptr;
    return node;
}

void FCREATE(struct SinglyLinkedList* list) {
    list->head = list->tail = nullptr;
    list->length = 0;
}

void FDESTROY(struct SinglyLinkedList* list) {
    struct FNode* current = list->head;
    while (current != nullptr) {
        struct FNode* next = current->next;
        delete current;
        current = next;
    }
    FCREATE(list);
}

void FPUSH_HEAD(struct SinglyLinkedList* list, const std::string& value) {
    struct FNode* newNode = createFNode(value);
    newNode->next = list->head;
    list->head = newNode;
    if (list->tail == nullptr) {
        list->tail = newNode;
    }
    list->length++;
}

void FPUSH_TAIL(struct SinglyLinkedList* list, const std::string& value) {
    struct FNode* newNode = createFNode(value);
    if (list->tail == nullptr) {
        list->head = list->tail = newNode;
    } else {
        list->tail->next = newNode;
        list->tail = newNode;
    }
    list->length++;
}

bool FINS_BEFORE_VALUE(struct SinglyLinkedList* list, const std::string& beforeValue, const std::string& newValue) {
    struct FNode* current = list->head;
    struct FNode* prev = nullptr;
    while (current != nullptr && current->data != beforeValue) {
        prev = current;
        current = current->next;
    }
    if (current == nullptr) return false;

    struct FNode* newNode = createFNode(newValue);
    if (prev == nullptr) {
        newNode->next = list->head;
        list->head = newNode;
    } else {
        prev->next = newNode;
        newNode->next = current;
    }
    list->length++;
    return true;
}

bool FINS_AFTER_VALUE(struct SinglyLinkedList* list, const std::string& afterValue, const std::string& newValue) {
    struct FNode* current = list->head;
    while (current != nullptr && current->data != afterValue) {
        current = current->next;
    }
    if (current == nullptr) return false;

    struct FNode* newNode = createFNode(newValue);
    newNode->next = current->next;
    current->next = newNode;

    if (current == list->tail) {
        list->tail = newNode;
    }
    list->length++;
    return true;
}

std::string FDEL_HEAD(struct SinglyLinkedList* list) {
    if (list->head == nullptr) throw std::runtime_error("Singly Linked List is empty.");
    std::string data = list->head->data;
    struct FNode* temp = list->head;
    list->head = list->head->next;
    if (list->head == nullptr) list->tail = nullptr;
    delete temp;
    list->length--;
    return data;
}

std::string FDEL_TAIL(struct SinglyLinkedList* list) {
    if (list->tail == nullptr) throw std::runtime_error("Singly Linked List is empty.");
    std::string data = list->tail->data;
    if (list->head == list->tail) {
        delete list->head;
        list->head = list->tail = nullptr;
    } else {
        struct FNode* current = list->head;
        while (current->next != list->tail) {
            current = current->next;
        }
        delete list->tail;
        list->tail = current;
        current->next = nullptr;
    }
    list->length--;
    return data;
}

bool FDEL_BY_VALUE(struct SinglyLinkedList* list, const std::string& value) {
    struct FNode* current = list->head;
    struct FNode* prev = nullptr;
    while (current != nullptr && current->data != value) {
        prev = current;
        current = current->next;
    }
    if (current == nullptr) return false;
    if (prev == nullptr) {
        list->head = current->next;
        if (list->head == nullptr) list->tail = nullptr;
    } else {
        prev->next = current->next;
        if (current == list->tail) list->tail = prev;
    }
    delete current;
    list->length--;
    return true;
}

bool FDEL_BEFORE_VALUE(struct SinglyLinkedList* list, const std::string& value) {
    if (list->head == nullptr || list->head->data == value) {
        return false;
    }

    if (list->head->next != nullptr && list->head->next->data == value) {
        FDEL_HEAD(list);
        return true;
    }

    struct FNode* current = list->head;

    while (current->next != nullptr && current->next->next != nullptr && current->next->next->data != value) {
        current = current->next;
    }

    if (current->next != nullptr && current->next->next != nullptr && current->next->next->data == value) {
        struct FNode* toDelete = current->next;
        current->next = toDelete->next;
        if (toDelete == list->tail) list->tail = current;
        delete toDelete;
        list->length--;
        return true;
    }

    return false;
}

bool FDEL_AFTER_VALUE(struct SinglyLinkedList* list, const std::string& value) {
    struct FNode* current = list->head;
    while (current != nullptr && current->data != value) {
        current = current->next;
    }

    if (current == nullptr || current->next == nullptr) {
        return false;
    }

    struct FNode* toDelete = current->next;
    current->next = toDelete->next;

    if (toDelete == list->tail) {
        list->tail = current;
    }

    delete toDelete;
    list->length--;
    return true;
}

std::string FGET_HEAD(const struct SinglyLinkedList* list) {
    if (list->head == nullptr) throw std::runtime_error("Singly Linked List is empty.");
    return list->head->data;
}

std::string FGET_TAIL(const struct SinglyLinkedList* list) {
    if (list->tail == nullptr) throw std::runtime_error("Singly Linked List is empty.");
    return list->tail->data;
}

std::string FGET_AT(const struct SinglyLinkedList* list, int index) {
    if (index < 0 || index >= list->length) throw std::out_of_range("Invalid index.");
    struct FNode* current = list->head;
    for(int i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

bool FIS_MEMBER(const struct SinglyLinkedList* list, const std::string& value) {
    struct FNode* current = list->head;
    while (current != nullptr) {
        if (current->data == value) return true;
        current = current->next;
    }
    return false;
}

void FPRINT(const struct SinglyLinkedList* list) {
    struct FNode* current = list->head;
    while (current != nullptr) {
        std::cout << current->data << (current->next != nullptr ? " " : "");
        current = current->next;
    }
    std::cout << std::endl;
}

void LCREATE(struct DoublyLinkedList* list) {
    list->head = list->tail = nullptr;
    list->length = 0;
}

void LDESTROY(struct DoublyLinkedList* list) {
    struct LNode* current = list->head;
    while (current != nullptr) {
        struct LNode* next = current->next;
        delete current;
        current = next;
    }
    LCREATE(list);
}

void LPUSH_HEAD(struct DoublyLinkedList* list, const std::string& value) {
    struct LNode* newNode = new struct LNode;
    newNode->data = value;
    newNode->prev = nullptr;
    newNode->next = list->head;
    if (list->head == nullptr) {
        list->tail = newNode;
    } else {
        list->head->prev = newNode;
    }
    list->head = newNode;
    list->length++;
}

void LPUSH_TAIL(struct DoublyLinkedList* list, const std::string& value) {
    struct LNode* newNode = new struct LNode;
    newNode->data = value;
    newNode->next = nullptr;
    newNode->prev = list->tail;
    if (list->tail == nullptr) {
        list->head = newNode;
    } else {
        list->tail->next = newNode;
    }
    list->tail = newNode;
    list->length++;
}

bool LINS_BEFORE_VALUE(struct DoublyLinkedList* list, const std::string& beforeValue, const std::string& newValue) {
    struct LNode* current = list->head;
    while (current != nullptr && current->data != beforeValue) {
        current = current->next;
    }
    if (current == nullptr) return false;

    struct LNode* newNode = new struct LNode;
    newNode->data = newValue;
    newNode->next = current;
    newNode->prev = current->prev;

    if (current->prev == nullptr) {
        list->head = newNode;
    } else {
        current->prev->next = newNode;
    }
    current->prev = newNode;
    list->length++;
    return true;
}

bool LINS_AFTER_VALUE(struct DoublyLinkedList* list, const std::string& afterValue, const std::string& newValue) {
    struct LNode* current = list->head;
    while (current != nullptr && current->data != afterValue) {
        current = current->next;
    }
    if (current == nullptr) return false;

    struct LNode* newNode = new struct LNode;
    newNode->data = newValue;
    newNode->prev = current;
    newNode->next = current->next;

    if (current->next == nullptr) {
        list->tail = newNode;
    } else {
        current->next->prev = newNode;
    }
    current->next = newNode;
    list->length++;
    return true;
}

std::string LDEL_HEAD(struct DoublyLinkedList* list) {
    if (list->head == nullptr) throw std::runtime_error("Doubly Linked List is empty.");
    std::string data = list->head->data;
    struct LNode* temp = list->head;
    list->head = list->head->next;
    if (list->head != nullptr) {
        list->head->prev = nullptr;
    } else {
        list->tail = nullptr;
    }
    delete temp;
    list->length--;
    return data;
}

std::string LDEL_TAIL(struct DoublyLinkedList* list) {
    if (list->tail == nullptr) throw std::runtime_error("Doubly Linked List is empty.");
    std::string data = list->tail->data;
    struct LNode* temp = list->tail;
    list->tail = list->tail->prev;
    if (list->tail != nullptr) {
        list->tail->next = nullptr;
    } else {
        list->head = nullptr;
    }
    delete temp;
    list->length--;
    return data;
}

bool LDEL_BY_VALUE(struct DoublyLinkedList* list, const std::string& value) {
    struct LNode* current = list->head;
    while (current != nullptr && current->data != value) {
        current = current->next;
    }
    if (current == nullptr) return false;
    if (current->prev != nullptr) {
        current->prev->next = current->next;
    } else {
        list->head = current->next;
    }
    if (current->next != nullptr) {
        current->next->prev = current->prev;
    } else {
        list->tail = current->prev;
    }
    delete current;
    list->length--;
    return true;
}

bool LDEL_BEFORE_VALUE(struct DoublyLinkedList* list, const std::string& value) {
    struct LNode* target = list->head;
    while (target != nullptr && target->data != value) {
        target = target->next;
    }

    if (target == nullptr || target->prev == nullptr) {
        return false;
    }

    struct LNode* toDelete = target->prev;

    if (toDelete->prev != nullptr) {
        toDelete->prev->next = target;
    } else {
        list->head = target;
    }
    target->prev = toDelete->prev;

    delete toDelete;
    list->length--;
    return true;
}

bool LDEL_AFTER_VALUE(struct DoublyLinkedList* list, const std::string& value) {
    struct LNode* target = list->head;
    while (target != nullptr && target->data != value) {
        target = target->next;
    }

    if (target == nullptr || target->next == nullptr) {
        return false;
    }

    struct LNode* toDelete = target->next;

    if (toDelete->next != nullptr) {
        toDelete->next->prev = target;
    } else {
        list->tail = target;
    }
    target->next = toDelete->next;

    delete toDelete;
    list->length--;
    return true;
}

std::string LGET_HEAD(const struct DoublyLinkedList* list) {
    if (list->head == nullptr) throw std::runtime_error("Doubly Linked List is empty.");
    return list->head->data;
}

std::string LGET_TAIL(const struct DoublyLinkedList* list) {
    if (list->tail == nullptr) throw std::runtime_error("Doubly Linked List is empty.");
    return list->tail->data;
}

std::string LGET_AT(const struct DoublyLinkedList* list, int index) {
    if (index < 0 || index >= list->length) throw std::out_of_range("Invalid index.");
    struct LNode* current = list->head;
    for(int i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

bool LIS_MEMBER(const struct DoublyLinkedList* list, const std::string& value) {
    struct LNode* current = list->head;
    while (current != nullptr) {
        if (current->data == value) return true;
        current = current->next;
    }
    return false;
}

void LPRINT(const struct DoublyLinkedList* list) {
    struct LNode* current = list->head;
    while (current != nullptr) {
        std::cout << current->data << (current->next != nullptr ? " " : "");
        current = current->next;
    }
    std::cout << std::endl;
}

void SCREATE(struct Stack* stack) {
    stack->top = nullptr;
    stack->count = 0;
}

void SDESTROY(struct Stack* stack) {
    struct FNode* current = stack->top;
    while (current != nullptr) {
        struct FNode* next = current->next;
        delete current;
        current = next;
    }
    SCREATE(stack);
}

void SPUSH(struct Stack* stack, const std::string& value) {
    struct FNode* newNode = createFNode(value);
    newNode->next = stack->top;
    stack->top = newNode;
    stack->count++;
}

std::string SPOP(struct Stack* stack) {
    if (stack->top == nullptr) throw std::runtime_error("Stack is empty.");
    std::string data = stack->top->data;
    struct FNode* temp = stack->top;
    stack->top = stack->top->next;
    delete temp;
    stack->count--;
    return data;
}

std::string SPEEK(const struct Stack* stack) {
    if (stack->top == nullptr) throw std::runtime_error("Stack is empty.");
    return stack->top->data;
}

int SLENGTH(const struct Stack* stack) {
    return stack->count;
}

void SPRINT(const struct Stack* stack) {
    struct FNode* current = stack->top;
    while (current != nullptr) {
        std::cout << current->data << (current->next != nullptr ? " " : "");
        current = current->next;
    }
    std::cout << std::endl;
}

void QCREATE(struct Queue* queue) {
    queue->front = queue->rear = nullptr;
    queue->count = 0;
}

void QDESTROY(struct Queue* queue) {
    struct FNode* current = queue->front;
    while (current != nullptr) {
        struct FNode* next = current->next;
        delete current;
        current = next;
    }
    QCREATE(queue);
}

void QPUSH(struct Queue* queue, const std::string& value) {
    struct FNode* newNode = createFNode(value);
    if (queue->rear == nullptr) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
    queue->count++;
}

std::string QPOP(struct Queue* queue) {
    if (queue->front == nullptr) throw std::runtime_error("Queue is empty.");
    std::string data = queue->front->data;
    struct FNode* temp = queue->front;
    queue->front = queue->front->next;
    if (queue->front == nullptr) {
        queue->rear = nullptr;
    }
    delete temp;
    queue->count--;
    return data;
}

std::string QPEEK(const struct Queue* queue) {
    if (queue->front == nullptr) throw std::runtime_error("Queue is empty.");
    return queue->front->data;
}

int QLENGTH(const struct Queue* queue) {
    return queue->count;
}

void QPRINT(const struct Queue* queue) {
    struct FNode* current = queue->front;
    while (current != nullptr) {
        std::cout << current->data << (current->next != nullptr ? " " : "");
        current = current->next;
    }
    std::cout << std::endl;
}

void TCREATE(struct AVLTree* tree) {
    tree->root = nullptr;
}

int getHeight(struct TNode* node) {
    return (node == nullptr) ? 0 : node->height;
}

void updateHeight(struct TNode* node) {
    if (node != nullptr) {
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    }
}

int getBalanceFactor(struct TNode* node) {
    return (node == nullptr) ? 0 : getHeight(node->left) - getHeight(node->right);
}

struct TNode* rightRotate(struct TNode* y) {
    struct TNode* x = y->left;
    struct TNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    updateHeight(y);
    updateHeight(x);
    return x;
}

struct TNode* leftRotate(struct TNode* x) {
    struct TNode* y = x->right;
    struct TNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    updateHeight(x);
    updateHeight(y);
    return y;
}

struct TNode* balanceNode(struct TNode* node) {
    updateHeight(node);
    int balance = getBalanceFactor(node);
    if (balance > 1 && getBalanceFactor(node->left) >= 0) return rightRotate(node);
    if (balance > 1 && getBalanceFactor(node->left) < 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && getBalanceFactor(node->right) <= 0) return leftRotate(node);
    if (balance < -1 && getBalanceFactor(node->right) > 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

struct TNode* TINSERT_recursive(struct TNode* node, const std::string& value) {
    if (node == nullptr) {
        struct TNode* newNode = new struct TNode;
        newNode->data = value;
        newNode->height = 1;
        newNode->left = newNode->right = nullptr;
        return newNode;
    }
    if (value < node->data) node->left = TINSERT_recursive(node->left, value);
    else if (value > node->data) node->right = TINSERT_recursive(node->right, value);
    else return node;
    return balanceNode(node);
}

void TINSERT(struct AVLTree* tree, const std::string& value) {
    tree->root = TINSERT_recursive(tree->root, value);
}

struct TNode* findMinValueNode(struct TNode* node) {
    struct TNode* current = node;
    while (current->left != nullptr) current = current->left;
    return current;
}

struct TNode* TDEL_recursive(struct TNode* root, const std::string& value, bool& deleted) {
    if (root == nullptr) return root;
    if (value < root->data) root->left = TDEL_recursive(root->left, value, deleted);
    else if (value > root->data) root->right = TDEL_recursive(root->right, value, deleted);
    else {
        deleted = true;
        if ((root->left == nullptr) || (root->right == nullptr)) {
            struct TNode* temp = root->left ? root->left : root->right;
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else *root = *temp;
            delete temp;
        } else {
            struct TNode* temp = findMinValueNode(root->right);
            root->data = temp->data;
            bool tempDeleted = false;
            root->right = TDEL_recursive(root->right, temp->data, tempDeleted);
        }
    }
    if (root == nullptr) return root;
    return balanceNode(root);
}

bool TDEL(struct AVLTree* tree, const std::string& value) {
    bool deleted = false;
    tree->root = TDEL_recursive(tree->root, value, deleted);
    return deleted;
}

struct TNode* TGET_recursive(struct TNode* node, const std::string& value) {
    if (node == nullptr || node->data == value) return node;
    if (value < node->data) return TGET_recursive(node->left, value);
    return TGET_recursive(node->right, value);
}

bool TIS_MEMBER(const struct AVLTree* tree, const std::string& value) {
    return TGET_recursive(tree->root, value) != nullptr;
}

void TPRINT_inorder(struct TNode* root) {
    if (root != nullptr) {
        TPRINT_inorder(root->left);
        std::cout << root->data << " ";
        TPRINT_inorder(root->right);
    }
}

void TPRINT(const struct AVLTree* tree) {
    TPRINT_inorder(tree->root);
    std::cout << std::endl;
}

void TDESTROY_recursive(struct TNode* node) {
    if (node != nullptr) {
        TDESTROY_recursive(node->left);
        TDESTROY_recursive(node->right);
        delete node;
    }
}

void TDESTROY(struct AVLTree* tree) {
    TDESTROY_recursive(tree->root);
    TCREATE(tree);
}