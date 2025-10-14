#include "Store.h"
#include <sstream>
#include <stdexcept>
#include <vector>
#include <iomanip>

void printHelp() {
    std::cout << "\nAvailable Commands:\n";
    std::cout << "====================================================================================================\n";
    std::cout << std::left;
    std::cout << std::setw(55) << "Общие команды:" << "Описание:" << "\n";
    std::cout << "----------------------------------------------------------------------------------------------------\n";
    std::cout << std::setw(55) << "  HELP" << "Показать это справочное сообщение." << "\n";
    std::cout << std::setw(55) << "  QUIT" << "Выйти из программы." << "\n";
    std::cout << std::setw(55) << "  <X>CREATE <name>" << "Создать новую структуру данных. X: M, F, L, S, Q, T." << "\n";
    std::cout << std::setw(55) << "  PRINT <name>" << "Напечатать содержимое структуры." << "\n";
    std::cout << std::setw(55) << "  ISMEMBER <name> <value>" << "Проверить, есть ли значение в структуре (не для S, Q)." << "\n";

    std::cout << "\n" << std::setw(55) << "Динамический массив (M - DynamicArray):" << "\n";
    std::cout << "----------------------------------------------------------------------------------------------------\n";
    std::cout << std::setw(55) << "  MPUSH_BACK <name> <value>" << "Добавить элемент в конец." << "\n";
    std::cout << std::setw(55) << "  MINSERT_AT <name> <index> <value>" << "Вставить элемент по индексу." << "\n";
    std::cout << std::setw(55) << "  MSET_AT <name> <index> <value>" << "Заменить элемент по индексу." << "\n";
    std::cout << std::setw(55) << "  MGET <name> <index>" << "Получить элемент по индексу." << "\n";
    std::cout << std::setw(55) << "  MDEL_AT <name> <index>" << "Удалить элемент по индексу." << "\n";
    std::cout << std::setw(55) << "  MLENGTH <name>" << "Получить размер массива." << "\n";

    std::cout << "\n" << std::setw(55) << "Односвязный/Двусвязный список (F/L - FList/LList):" << "\n";
    std::cout << "----------------------------------------------------------------------------------------------------\n";
    std::cout << std::setw(55) << "  <X>PUSH_HEAD <name> <value>" << "Добавить элемент в начало." << "\n";
    std::cout << std::setw(55) << "  <X>PUSH_TAIL <name> <value>" << "Добавить элемент в конец." << "\n";
    std::cout << std::setw(55) << "  <X>INS_BEFORE <name> <target_val> <new_val>" << "Вставить элемент перед указанным значением." << "\n";
    std::cout << std::setw(55) << "  <X>INS_AFTER <name> <target_val> <new_val>" << "Вставить элемент после указанного значения." << "\n";
    std::cout << std::setw(55) << "  <X>GET_HEAD <name>" << "Получить первый элемент." << "\n";
    std::cout << std::setw(55) << "  <X>GET_TAIL <name>" << "Получить последний элемент." << "\n";
    std::cout << std::setw(55) << "  <X>GET_AT <name> <index>" << "Получить элемент по индексу." << "\n";
    std::cout << std::setw(55) << "  <X>DEL_HEAD <name>" << "Удалить первый элемент." << "\n";
    std::cout << std::setw(55) << "  <X>DEL_TAIL <name>" << "Удалить последний элемент." << "\n";
    std::cout << std::setw(55) << "  <X>DEL_BY_VALUE <name> <value>" << "Удалить первое вхождение значения." << "\n";
    std::cout << std::setw(55) << "  <X>DEL_BEFORE <name> <value>" << "Удалить элемент перед указанным значением." << "\n";
    std::cout << std::setw(55) << "  <X>DEL_AFTER <name> <value>" << "Удалить элемент после указанного значения." << "\n";

    std::cout << "\n" << std::setw(55) << "Стек (S - Stack):" << "\n";
    std::cout << "----------------------------------------------------------------------------------------------------\n";
    std::cout << std::setw(55) << "  SPUSH <name> <value>" << "Положить элемент в стек." << "\n";
    std::cout << std::setw(55) << "  SPOP <name>" << "Извлечь элемент из стека." << "\n";
    std::cout << std::setw(55) << "  SPEAK <name>" << "Посмотреть верхний элемент." << "\n";
    std::cout << std::setw(55) << "  SLENGTH <name>" << "Получить размер стека." << "\n";

    std::cout << "\n" << std::setw(55) << "Очередь (Q - Queue):" << "\n";
    std::cout << "----------------------------------------------------------------------------------------------------\n";
    std::cout << std::setw(55) << "  QPUSH <name> <value>" << "Добавить элемент в очередь." << "\n";
    std::cout << std::setw(55) << "  QPOP <name>" << "Извлечь элемент из очереди." << "\n";
    std::cout << std::setw(55) << "  QPEEK <name>" << "Посмотреть первый элемент." << "\n";
    std::cout << std::setw(55) << "  QLENGTH <name>" << "Получить размер очереди." << "\n";

    std::cout << "\n" << std::setw(55) << "АВЛ-Дерево (T - Tree):" << "\n";
    std::cout << "----------------------------------------------------------------------------------------------------\n";
    std::cout << std::setw(55) << "  TINSERT <name> <value>" << "Вставить элемент." << "\n";
    std::cout << std::setw(55) << "  TDEL <name> <value>" << "Удалить элемент." << "\n";
    std::cout << std::setw(55) << "  TGET <name> <value>" << "Найти и показать элемент, если он существует." << "\n";
    std::cout << "====================================================================================================\n";
}

bool processCommand(struct DataStore* store, const std::string& line) {
    std::stringstream commandStream(line);
    std::string command, name, arg1, arg2;
    commandStream >> command;

    if (command == "HELP") {
        printHelp();
        return false;
    }

    try {
        if (command.length() == 7 && command.substr(1) == "CREATE") {
            if (!(commandStream >> name)) throw std::runtime_error("Отсутствует имя для CREATE.");
            StructureType type = NONE_TYPE;
            char typeChar = command[0];
            if (typeChar == 'M') type = ARRAY_TYPE;
            else if (typeChar == 'F') type = FLIST_TYPE;
            else if (typeChar == 'L') type = LLIST_TYPE;
            else if (typeChar == 'S') type = STACK_TYPE;
            else if (typeChar == 'Q') type = QUEUE_TYPE;
            else if (typeChar == 'T') type = TREE_TYPE;
            else throw std::runtime_error("Неизвестный тип структуры для CREATE.");
            createAndAddStructure(store, name, type);
            std::cout << "OK" << std::endl;
            return true;
        }

        if (!(commandStream >> name)) throw std::runtime_error("Отсутствует имя структуры для команды '" + command + "'.");
        struct StoreEntry* entry = findEntry(store, name);
        if (!entry) throw std::runtime_error("Структура '" + name + "' не найдена.");

        // Общие команды
        if (command == "PRINT") {
            switch (entry->type) {
                case ARRAY_TYPE: MPRINT(static_cast<DynamicArray*>(entry->dataPtr)); break;
                case FLIST_TYPE: FPRINT(static_cast<SinglyLinkedList*>(entry->dataPtr)); break;
                case LLIST_TYPE: LPRINT(static_cast<DoublyLinkedList*>(entry->dataPtr)); break;
                case STACK_TYPE: SPRINT(static_cast<Stack*>(entry->dataPtr)); break;
                case QUEUE_TYPE: QPRINT(static_cast<Queue*>(entry->dataPtr)); break;
                case TREE_TYPE: TPRINT(static_cast<AVLTree*>(entry->dataPtr)); break;
                default: throw std::runtime_error("PRINT не поддерживается для этого типа.");
            }
            return false;
        }

        if (command == "ISMEMBER") {
            if (!(commandStream >> arg1)) throw std::runtime_error("Отсутствует значение для ISMEMBER.");
            bool isMember = false;
            switch (entry->type) {
                case ARRAY_TYPE: isMember = MIS_MEMBER(static_cast<DynamicArray*>(entry->dataPtr), arg1); break;
                case FLIST_TYPE: isMember = FIS_MEMBER(static_cast<SinglyLinkedList*>(entry->dataPtr), arg1); break;
                case LLIST_TYPE: isMember = LIS_MEMBER(static_cast<DoublyLinkedList*>(entry->dataPtr), arg1); break;
                case TREE_TYPE: isMember = TIS_MEMBER(static_cast<AVLTree*>(entry->dataPtr), arg1); break;
                default: throw std::runtime_error("ISMEMBER не поддерживается для этого типа.");
            }
            std::cout << (isMember ? "TRUE" : "FALSE") << std::endl;
            return false;
        }

        bool modified = false;

        // Команды для конкретных типов
        switch(entry->type) {
            case ARRAY_TYPE:
                if (command == "MPUSH_BACK") { if (commandStream >> arg1) { MPUSH_BACK(static_cast<DynamicArray*>(entry->dataPtr), arg1); modified = true; } else throw std::runtime_error("Нет значения."); }
                else if (command == "MINSERT_AT") { if (commandStream >> arg1 >> arg2) { MINSERT_AT(static_cast<DynamicArray*>(entry->dataPtr), std::stoi(arg1), arg2); modified = true; } else throw std::runtime_error("Нет индекса/значения."); }
                else if (command == "MSET_AT") { if (commandStream >> arg1 >> arg2) { MSET_AT(static_cast<DynamicArray*>(entry->dataPtr), std::stoi(arg1), arg2); modified = true; } else throw std::runtime_error("Нет индекса/значения."); }
                else if (command == "MDEL_AT") { if (commandStream >> arg1) { std::cout << MDEL_AT(static_cast<DynamicArray*>(entry->dataPtr), std::stoi(arg1)) << std::endl; modified = true; } else throw std::runtime_error("Нет индекса."); }
                else if (command == "MGET") { if (commandStream >> arg1) { std::cout << MGET(static_cast<DynamicArray*>(entry->dataPtr), std::stoi(arg1)) << std::endl; } else throw std::runtime_error("Нет индекса."); }
                else if (command == "MLENGTH") { std::cout << MLENGTH(static_cast<DynamicArray*>(entry->dataPtr)) << std::endl; }
                else throw std::runtime_error("Неизвестная команда для ARRAY.");
                break;

            case FLIST_TYPE: case LLIST_TYPE: {
                char typeChar = (entry->type == FLIST_TYPE) ? 'F' : 'L';
                if (command.length() < 2 || command[0] != typeChar) throw std::runtime_error("Неверный префикс команды для типа списка.");
                std::string op = command.substr(1);
                if (op == "PUSH_HEAD" || op == "PUSH_TAIL") { if (!(commandStream >> arg1)) throw std::runtime_error("Нет значения."); if (typeChar == 'F') { if (op == "PUSH_HEAD") FPUSH_HEAD(static_cast<SinglyLinkedList*>(entry->dataPtr), arg1); else FPUSH_TAIL(static_cast<SinglyLinkedList*>(entry->dataPtr), arg1); } else { if (op == "PUSH_HEAD") LPUSH_HEAD(static_cast<DoublyLinkedList*>(entry->dataPtr), arg1); else LPUSH_TAIL(static_cast<DoublyLinkedList*>(entry->dataPtr), arg1); } modified = true; }
                else if (op == "INS_BEFORE" || op == "INS_AFTER") { if (!(commandStream >> arg1 >> arg2)) throw std::runtime_error("Нет аргументов."); bool res = false; if (typeChar == 'F') res = (op == "INS_BEFORE") ? FINS_BEFORE_VALUE(static_cast<SinglyLinkedList*>(entry->dataPtr), arg1, arg2) : FINS_AFTER_VALUE(static_cast<SinglyLinkedList*>(entry->dataPtr), arg1, arg2); else res = (op == "INS_BEFORE") ? LINS_BEFORE_VALUE(static_cast<DoublyLinkedList*>(entry->dataPtr), arg1, arg2) : LINS_AFTER_VALUE(static_cast<DoublyLinkedList*>(entry->dataPtr), arg1, arg2); std::cout << (res ? "OK" : "Not Found") << std::endl; modified = res; }
                else if (op == "DEL_HEAD" || op == "DEL_TAIL") { if (typeChar == 'F') std::cout << ((op == "DEL_HEAD") ? FDEL_HEAD(static_cast<SinglyLinkedList*>(entry->dataPtr)) : FDEL_TAIL(static_cast<SinglyLinkedList*>(entry->dataPtr))) << std::endl; else std::cout << ((op == "DEL_HEAD") ? LDEL_HEAD(static_cast<DoublyLinkedList*>(entry->dataPtr)) : LDEL_TAIL(static_cast<DoublyLinkedList*>(entry->dataPtr))) << std::endl; modified = true; }
                else if (op == "DEL_BY_VALUE" || op == "DEL_BEFORE" || op == "DEL_AFTER") { if (!(commandStream >> arg1)) throw std::runtime_error("Нет значения."); bool res = false; if (typeChar == 'F') { if (op == "DEL_BY_VALUE") res = FDEL_BY_VALUE(static_cast<SinglyLinkedList*>(entry->dataPtr), arg1); else if (op == "DEL_BEFORE") res = FDEL_BEFORE_VALUE(static_cast<SinglyLinkedList*>(entry->dataPtr), arg1); else res = FDEL_AFTER_VALUE(static_cast<SinglyLinkedList*>(entry->dataPtr), arg1); } else { if (op == "DEL_BY_VALUE") res = LDEL_BY_VALUE(static_cast<DoublyLinkedList*>(entry->dataPtr), arg1); else if (op == "DEL_BEFORE") res = LDEL_BEFORE_VALUE(static_cast<DoublyLinkedList*>(entry->dataPtr), arg1); else res = LDEL_AFTER_VALUE(static_cast<DoublyLinkedList*>(entry->dataPtr), arg1); } std::cout << (res ? "OK" : "Not Found") << std::endl; modified = res; }
                else if (op == "GET_HEAD" || op == "GET_TAIL") { if (typeChar == 'F') std::cout << ((op == "GET_HEAD") ? FGET_HEAD(static_cast<SinglyLinkedList*>(entry->dataPtr)) : FGET_TAIL(static_cast<SinglyLinkedList*>(entry->dataPtr))) << std::endl; else std::cout << ((op == "GET_HEAD") ? LGET_HEAD(static_cast<DoublyLinkedList*>(entry->dataPtr)) : LGET_TAIL(static_cast<DoublyLinkedList*>(entry->dataPtr))) << std::endl; }
                else if (op == "GET_AT") { if (!(commandStream >> arg1)) throw std::runtime_error("Нет индекса."); if (typeChar == 'F') std::cout << FGET_AT(static_cast<SinglyLinkedList*>(entry->dataPtr), std::stoi(arg1)) << std::endl; else std::cout << LGET_AT(static_cast<DoublyLinkedList*>(entry->dataPtr), std::stoi(arg1)) << std::endl; }
                else throw std::runtime_error("Неизвестная команда для списка.");
                break;
            }
            case STACK_TYPE:
                if (command == "SPUSH") { if (commandStream >> arg1) { SPUSH(static_cast<Stack*>(entry->dataPtr), arg1); modified = true; } else throw std::runtime_error("Нет значения."); }
                else if (command == "SPOP") { std::cout << SPOP(static_cast<Stack*>(entry->dataPtr)) << std::endl; modified = true; }
                else if (command == "SPEAK") { std::cout << SPEEK(static_cast<Stack*>(entry->dataPtr)) << std::endl; }
                else if (command == "SLENGTH") { std::cout << SLENGTH(static_cast<Stack*>(entry->dataPtr)) << std::endl; }
                else throw std::runtime_error("Неизвестная команда для STACK.");
                break;
            case QUEUE_TYPE:
                if (command == "QPUSH") { if (commandStream >> arg1) { QPUSH(static_cast<Queue*>(entry->dataPtr), arg1); modified = true; } else throw std::runtime_error("Нет значения."); }
                else if (command == "QPOP") { std::cout << QPOP(static_cast<Queue*>(entry->dataPtr)) << std::endl; modified = true; }
                else if (command == "QPEEK") { std::cout << QPEEK(static_cast<Queue*>(entry->dataPtr)) << std::endl; }
                else if (command == "QLENGTH") { std::cout << QLENGTH(static_cast<Queue*>(entry->dataPtr)) << std::endl; }
                else throw std::runtime_error("Неизвестная команда для QUEUE.");
                break;
            case TREE_TYPE:
                if (command == "TINSERT") { if (commandStream >> arg1) { TINSERT(static_cast<AVLTree*>(entry->dataPtr), arg1); modified = true; } else throw std::runtime_error("Нет значения."); }
                else if (command == "TDEL") { if (commandStream >> arg1) { bool res = TDEL(static_cast<AVLTree*>(entry->dataPtr), arg1); std::cout << (res ? "OK" : "Not Found") << std::endl; modified = res; } else throw std::runtime_error("Нет значения."); }
                else if (command == "TGET") { if (commandStream >> arg1) { bool found = TIS_MEMBER(static_cast<AVLTree*>(entry->dataPtr), arg1); std::cout << (found ? arg1 : "Not Found") << std::endl; } else throw std::runtime_error("Нет значения."); }
                else throw std::runtime_error("Неизвестная команда для TREE.");
                break;
            default:
                throw std::runtime_error("Неподдерживаемый тип структуры.");
        }

        if (modified) {
             if (command.find("DEL") == std::string::npos && command.find("POP") == std::string::npos && command.find("INS") == std::string::npos) {
                std::cout << "OK" << std::endl;
             }
        }
        return modified;

    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return false;
    }
}

int main(int argc, char* argv[]) {
    std::string filePath;
    std::string singleQuery;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--file") {
            if (i + 1 < argc) filePath = argv[++i];
        } else if (arg == "--query") {
            if (i + 1 < argc) singleQuery = argv[++i];
        }
    }

    if (filePath.empty()) {
        std::cerr << "Error: --file argument is required.\n";
        return 1;
    }

    struct DataStore store;
    initializeStore(&store);
    loadFromFile(&store, filePath);

    if (!singleQuery.empty()) {
        if (processCommand(&store, singleQuery)) {
            saveToFile(&store, filePath);
        }
    } else {
        printHelp();
        std::string line;
        while (true) {
            std::cout << "> ";
            if (!std::getline(std::cin, line) || line == "QUIT") break;
            if (line.empty()) continue;
            if (processCommand(&store, line)) {
                saveToFile(&store, filePath);
            }
        }
    }

    destroyStore(&store);
    return 0;
}