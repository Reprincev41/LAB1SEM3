package main

import (
	"bufio"
	"errors"
	"fmt"
	"io"
	"math"
	"os"
	"strconv"
	"strings"
)

//==========================================================================================
// Структуры данных
//==========================================================================================

// StructureType - перечисление для типов структур данных
type StructureType string

const (
	NoneType   StructureType = "NONE"
	ArrayType  StructureType = "ARRAY"
	FListType  StructureType = "FLIST"
	LListType  StructureType = "LLIST"
	StackType  StructureType = "STACK"
	QueueType  StructureType = "QUEUE"
	TreeType   StructureType = "TREE"
)

// --- Узлы для списков ---
type fNode struct {
	data string
	next *fNode
}

type lNode struct {
	data string
	next *lNode
	prev *lNode
}

// --- DynamicArray (Динамический массив) ---
type DynamicArray struct {
	elements []string
}

func NewDynamicArray() *DynamicArray {
	return &DynamicArray{elements: make([]string, 0)}
}

func (a *DynamicArray) PushBack(value string) {
	a.elements = append(a.elements, value)
}

func (a *DynamicArray) InsertAt(index int, value string) error {
	if index < 0 || index > len(a.elements) {
		return errors.New("invalid index for insert")
	}
	a.elements = append(a.elements[:index], append([]string{value}, a.elements[index:]...)...)
	return nil
}

func (a *DynamicArray) SetAt(index int, value string) error {
	if index < 0 || index >= len(a.elements) {
		return errors.New("invalid index for set")
	}
	a.elements[index] = value
	return nil
}

func (a *DynamicArray) DelAt(index int) (string, error) {
	if index < 0 || index >= len(a.elements) {
		return "", errors.New("invalid index for delete")
	}
	removedValue := a.elements[index]
	a.elements = append(a.elements[:index], a.elements[index+1:]...)
	return removedValue, nil
}

func (a *DynamicArray) Get(index int) (string, error) {
	if index < 0 || index >= len(a.elements) {
		return "", errors.New("invalid index for get")
	}
	return a.elements[index], nil
}

func (a *DynamicArray) IsMember(value string) bool {
	for _, v := range a.elements {
		if v == value {
			return true
		}
	}
	return false
}

func (a *DynamicArray) Length() int {
	return len(a.elements)
}

func (a *DynamicArray) Print() {
	fmt.Println(strings.Join(a.elements, " "))
}

// --- SinglyLinkedList (Односвязный список) ---
type SinglyLinkedList struct {
	head   *fNode
	tail   *fNode
	length int
}

func NewSinglyLinkedList() *SinglyLinkedList {
	return &SinglyLinkedList{}
}

func (l *SinglyLinkedList) PushHead(value string) {
	newNode := &fNode{data: value, next: l.head}
	l.head = newNode
	if l.tail == nil {
		l.tail = newNode
	}
	l.length++
}

func (l *SinglyLinkedList) PushTail(value string) {
	newNode := &fNode{data: value}
	if l.tail == nil {
		l.head = newNode
		l.tail = newNode
	} else {
		l.tail.next = newNode
		l.tail = newNode
	}
	l.length++
}

func (l *SinglyLinkedList) IsMember(value string) bool {
	current := l.head
	for current != nil {
		if current.data == value {
			return true
		}
		current = current.next
	}
	return false
}

// Length() уже реализован через l.length

func (l *SinglyLinkedList) Print() {
	var parts []string
	current := l.head
	for current != nil {
		parts = append(parts, current.data)
		current = current.next
	}
	fmt.Println(strings.Join(parts, " "))
}

// --- DoublyLinkedList (Двусвязный список) ---
type DoublyLinkedList struct {
	head   *lNode
	tail   *lNode
	length int
}

func NewDoublyLinkedList() *DoublyLinkedList {
	return &DoublyLinkedList{}
}

func (l *DoublyLinkedList) PushHead(value string) { // <-- НОВЫЙ МЕТОД
	newNode := &lNode{data: value}
	if l.head == nil {
		l.head = newNode
		l.tail = newNode
	} else {
		newNode.next = l.head
		l.head.prev = newNode
		l.head = newNode
	}
	l.length++
}

func (l *DoublyLinkedList) PushTail(value string) {
	newNode := &lNode{data: value}
	if l.tail == nil {
		l.head = newNode
		l.tail = newNode
	} else {
		l.tail.next = newNode
		newNode.prev = l.tail
		l.tail = newNode
	}
	l.length++
}

func (l *DoublyLinkedList) IsMember(value string) bool {
	current := l.head
	for current != nil {
		if current.data == value {
			return true
		}
		current = current.next
	}
	return false
}

// Length() уже реализован через l.length

func (l *DoublyLinkedList) getNodeAt(index int) (*lNode, error) { // <-- НОВЫЙ МЕТОД
	if index < 0 || index >= l.length {
		return nil, errors.New("invalid index")
	}
	// Оптимизация поиска с головы или хвоста
	current := l.head
	if index > l.length/2 {
		current = l.tail
		for i := l.length - 1; i > index; i-- {
			current = current.prev
		}
	} else {
		for i := 0; i < index; i++ {
			current = current.next
		}
	}
	return current, nil
}

func (l *DoublyLinkedList) Get(index int) (string, error) { // <-- НОВЫЙ МЕТОД
	node, err := l.getNodeAt(index)
	if err != nil {
		return "", err
	}
	return node.data, nil
}

func (l *DoublyLinkedList) SetAt(index int, value string) error { // <-- НОВЫЙ МЕТОД
	node, err := l.getNodeAt(index)
	if err != nil {
		return err
	}
	node.data = value
	return nil
}

func (l *DoublyLinkedList) DelAt(index int) (string, error) { // <-- НОВЫЙ МЕТОД
	if index < 0 || index >= l.length {
		return "", errors.New("invalid index for delete")
	}

	var nodeToRemove *lNode
	if index == 0 {
		nodeToRemove = l.head
		l.head = l.head.next
		if l.head != nil {
			l.head.prev = nil
		} else {
			l.tail = nil
		}
	} else if index == l.length-1 {
		nodeToRemove = l.tail
		l.tail = l.tail.prev
		if l.tail != nil {
			l.tail.next = nil
		} else {
			l.head = nil
		}
	} else {
		var err error
		nodeToRemove, err = l.getNodeAt(index)
		if err != nil {
			return "", err 
		}
		nodeToRemove.prev.next = nodeToRemove.next
		nodeToRemove.next.prev = nodeToRemove.prev
	}

	l.length--
	return nodeToRemove.data, nil
}

func (l *DoublyLinkedList) Print() {
	var parts []string
	current := l.head
	for current != nil {
		parts = append(parts, current.data)
		current = current.next
	}
	fmt.Println(strings.Join(parts, " "))
}


// --- Stack (Стек) ---
type Stack struct {
	top   *fNode
	count int
}

func NewStack() *Stack {
	return &Stack{}
}

func (s *Stack) Push(value string) {
	newNode := &fNode{data: value, next: s.top}
	s.top = newNode
	s.count++
}

func (s *Stack) Pop() (string, error) {
	if s.top == nil {
		return "", errors.New("stack is empty")
	}
	data := s.top.data
	s.top = s.top.next
	s.count--
	return data, nil
}

func (s *Stack) Peek() (string, error) {
    if s.top == nil {
        return "", errors.New("stack is empty")
    }
    return s.top.data, nil
}

func (s *Stack) Length() int {
    return s.count
}

func (s *Stack) Print() {
	var parts []string
	current := s.top
	for current != nil {
		parts = append(parts, current.data)
		current = current.next
	}
	fmt.Println(strings.Join(parts, " "))
}

// --- Queue (Очередь) ---
type Queue struct {
	front *fNode
	rear  *fNode
	count int
}

func NewQueue() *Queue {
	return &Queue{}
}

func (q *Queue) Push(value string) {
	newNode := &fNode{data: value}
	if q.rear == nil {
		q.front = newNode
		q.rear = newNode
	} else {
		q.rear.next = newNode
		q.rear = newNode
	}
	q.count++
}

func (q *Queue) Pop() (string, error) {
	if q.front == nil {
		return "", errors.New("queue is empty")
	}
	data := q.front.data
	q.front = q.front.next
	if q.front == nil {
		q.rear = nil
	}
	q.count--
	return data, nil
}

func (q *Queue) Peek() (string, error) {
    if q.front == nil {
        return "", errors.New("queue is empty")
    }
    return q.front.data, nil
}

func (q *Queue) Length() int {
    return q.count
}

func (q *Queue) Print() {
	var parts []string
	current := q.front
	for current != nil {
		parts = append(parts, current.data)
		current = current.next
	}
	fmt.Println(strings.Join(parts, " "))
}


// --- AVLTree (АВЛ-Дерево) ---
type tNode struct {
	data   int
	height int
	left   *tNode
	right  *tNode
}

type AVLTree struct {
	root *tNode
}

func NewAVLTree() *AVLTree {
	return &AVLTree{}
}

func getHeight(node *tNode) int {
	if node == nil {
		return 0
	}
	return node.height
}

func (n *tNode) updateHeight() {
	if n != nil {
		n.height = 1 + int(math.Max(float64(getHeight(n.left)), float64(getHeight(n.right))))
	}
}

func getBalanceFactor(node *tNode) int {
	if node == nil {
		return 0
	}
	return getHeight(node.left) - getHeight(node.right)
}

func rightRotate(y *tNode) *tNode {
	x := y.left
	T2 := x.right
	x.right = y
	y.left = T2
	y.updateHeight()
	x.updateHeight()
	return x
}

func leftRotate(x *tNode) *tNode {
	y := x.right
	T2 := y.left
	y.left = x
	x.right = T2
	x.updateHeight()
	y.updateHeight()
	return y
}

func balanceNode(node *tNode) *tNode {
	node.updateHeight()
	balance := getBalanceFactor(node)

	if balance > 1 { // Левый случай
		if getBalanceFactor(node.left) < 0 { // Лево-Право
			node.left = leftRotate(node.left)
		}
		return rightRotate(node) // Лево-Лево
	}
	if balance < -1 { // Правый случай
		if getBalanceFactor(node.right) > 0 { // Право-Лево
			node.right = rightRotate(node.right)
		}
		return leftRotate(node) // Право-Право
	}
	return node
}

func (t *AVLTree) Insert(value int) {
	t.root = t.root.insertRecursive(value)
}

func (n *tNode) insertRecursive(value int) *tNode {
	if n == nil {
		return &tNode{data: value, height: 1}
	}
	if value < n.data {
		n.left = n.left.insertRecursive(value)
	} else if value > n.data {
		n.right = n.right.insertRecursive(value)
	} else {
		return n // Дубликаты не вставляем
	}
	return balanceNode(n)
}

func findMinValueNode(node *tNode) *tNode {
	current := node
	for current.left != nil {
		current = current.left
	}
	return current
}

func (t *AVLTree) Delete(value int) bool {
    var deleted bool
	t.root, deleted = t.root.deleteRecursive(value)
    return deleted
}

func (n *tNode) deleteRecursive(value int) (*tNode, bool) {
	if n == nil {
		return n, false
	}
    var deleted bool
	if value < n.data {
		n.left, deleted = n.left.deleteRecursive(value)
	} else if value > n.data {
		n.right, deleted = n.right.deleteRecursive(value)
	} else {
        deleted = true
		if n.left == nil || n.right == nil {
			var temp *tNode
			if n.left != nil {
				temp = n.left
			} else {
				temp = n.right
			}
			if temp == nil {
				n = nil
			} else {
				*n = *temp
			}
		} else {
			temp := findMinValueNode(n.right)
			n.data = temp.data
			n.right, _ = n.right.deleteRecursive(temp.data) // <-- ИСПРАВЛЕНИЕ ЗДЕСЬ
		}
	}
	if n == nil {
		return n, deleted
	}
	return balanceNode(n), deleted
}

func (t *AVLTree) IsMember(value int) bool {
	return t.root.getRecursive(value) != nil
}

func (n *tNode) getRecursive(value int) *tNode {
	if n == nil || n.data == value {
		return n
	}
	if value < n.data {
		return n.left.getRecursive(value)
	}
	return n.right.getRecursive(value)
}


func (t *AVLTree) Print() {
	if t.root == nil {
		fmt.Println("(empty tree)")
	} else {
		printAVLTreeRecursive(t.root, 0)
		fmt.Println("\n======================================================")
		fmt.Println("Дерево показано повернутым на 90 градусов влево.")
		fmt.Println("Корень слева, правое поддерево вверху, левое внизу.")
		fmt.Println("======================================================")
	}
}

func printAVLTreeRecursive(node *tNode, space int) {
	const count = 10
	if node == nil {
		return
	}
	space += count
	printAVLTreeRecursive(node.right, space)
	fmt.Println()
	for i := count; i < space; i++ {
		fmt.Print(" ")
	}
	fmt.Printf("%d (h=%d)\n", node.data, node.height)
	printAVLTreeRecursive(node.left, space)
}

//==========================================================================================
// Хранилище (Store)
//==========================================================================================

const maxStructures = 100

type StoreEntry struct {
	name string
	stype StructureType
	data interface{} // Используем interface{} для хранения любого типа данных
}

type DataStore struct {
	entries map[string]*StoreEntry
}

func NewDataStore() *DataStore {
	return &DataStore{
		entries: make(map[string]*StoreEntry),
	}
}

func (s *DataStore) CreateAndAddStructure(name string, stype StructureType) (interface{}, error) {
	if len(s.entries) >= maxStructures && s.entries[name] == nil {
		return nil, errors.New("maximum number of structures reached")
	}

	var newData interface{}
	switch stype {
	case ArrayType:
		newData = NewDynamicArray()
	case FListType:
		newData = NewSinglyLinkedList()
	case LListType:
		newData = NewDoublyLinkedList()
	case StackType:
		newData = NewStack()
	case QueueType:
		newData = NewQueue()
	case TreeType:
		newData = NewAVLTree()
	default:
		return nil, errors.New("invalid structure type")
	}

	entry := &StoreEntry{name: name, stype: stype, data: newData}
	s.entries[name] = entry
	return newData, nil
}

func (s *DataStore) FindEntry(name string) *StoreEntry {
	return s.entries[name]
}

func SaveToFile(store *DataStore, filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return fmt.Errorf("could not open file '%s' for writing: %w", filename, err)
	}
	defer file.Close()

	writer := bufio.NewWriter(file)

	for name, entry := range store.entries {
		line := ""
		switch v := entry.data.(type) {
		case *DynamicArray:
			line = fmt.Sprintf("ARRAY %s %s", name, strings.Join(v.elements, " "))
		case *SinglyLinkedList:
			var parts []string
			curr := v.head
			for curr != nil {
				parts = append(parts, curr.data)
				curr = curr.next
			}
			line = fmt.Sprintf("FLIST %s %s", name, strings.Join(parts, " "))
		case *DoublyLinkedList:
			var parts []string
			curr := v.head
			for curr != nil {
				parts = append(parts, curr.data)
				curr = curr.next
			}
			line = fmt.Sprintf("LLIST %s %s", name, strings.Join(parts, " "))
		case *Stack:
            var parts []string
            curr := v.top
            for curr != nil {
                parts = append(parts, curr.data)
                curr = curr.next
            }
            // Сохраняем в обратном порядке, чтобы при загрузке порядок был правильным
            for i := len(parts) - 1; i >= 0; i-- {
                line += " " + parts[i]
            }
            line = fmt.Sprintf("STACK %s%s", name, line)
		case *Queue:
			var parts []string
			curr := v.front
			for curr != nil {
				parts = append(parts, curr.data)
				curr = curr.next
			}
			line = fmt.Sprintf("QUEUE %s %s", name, strings.Join(parts, " "))
		case *AVLTree:
			var parts []string
			var inorder func(*tNode)
			inorder = func(n *tNode) {
				if n == nil {
					return
				}
				inorder(n.left)
				parts = append(parts, strconv.Itoa(n.data))
				inorder(n.right)
			}
			inorder(v.root)
			line = fmt.Sprintf("TREE %s %s", name, strings.Join(parts, " "))
		}
		if line != "" {
			fmt.Fprintln(writer, line)
		}
	}

	return writer.Flush()
}

func LoadFromFile(store *DataStore, filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		if os.IsNotExist(err) {
			return nil // Файла нет - это нормально, просто начинаем с пустым хранилищем
		}
		return fmt.Errorf("could not open file '%s' for reading: %w", filename, err)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		parts := strings.Fields(scanner.Text())
		if len(parts) < 2 {
			continue
		}
		typeStr, name := parts[0], parts[1]
		values := parts[2:]

		var stype StructureType
		switch typeStr {
		case "ARRAY": stype = ArrayType
		case "FLIST": stype = FListType
		case "LLIST": stype = LListType
		case "STACK": stype = StackType
		case "QUEUE": stype = QueueType
		case "TREE": stype = TreeType
		default:
			fmt.Fprintf(os.Stderr, "Warning: Unknown structure type '%s' in file. Skipping.\n", typeStr)
			continue
		}
		
		data, err := store.CreateAndAddStructure(name, stype)
		if err != nil {
			fmt.Fprintf(os.Stderr, "Warning: Could not create structure '%s'. Skipping. Error: %v\n", name, err)
			continue
		}
		
		// Заполняем структуру данными
		switch v := data.(type) {
		case *DynamicArray:
			for _, val := range values {
				v.PushBack(val)
			}
		case *SinglyLinkedList:
			for _, val := range values {
				v.PushTail(val)
			}
		case *DoublyLinkedList:
			for _, val := range values {
				v.PushTail(val)
			}
		case *Stack:
			for _, val := range values {
				v.Push(val)
			}
		case *Queue:
			for _, val := range values {
				v.Push(val)
			}
		case *AVLTree:
			for _, val := range values {
				intVal, err := strconv.Atoi(val)
				if err != nil {
					fmt.Fprintf(os.Stderr, "Warning: Could not convert '%s' to integer for tree '%s'. Skipping.\n", val, name)
					continue
				}
				v.Insert(intVal)
			}
		}
	}

	return scanner.Err()
}


func processCommand(store *DataStore, line string) (bool, error) {
	parts := strings.Fields(line)
	if len(parts) == 0 {
		return false, nil // Пустая строка, ничего не делаем
	}

	command := parts[0]
	
	if command == "HELP" {
		// Обычно здесь выводится справка, но в текущем коде она отсутствует
		return false, nil
	}
	
	if strings.HasSuffix(command, "CREATE") && len(command) == 7 {
		if len(parts) < 2 {
			return false, errors.New("missing name for CREATE")
		}
		name := parts[1]
		var stype StructureType
		switch command[0] {
		case 'M': stype = ArrayType
		case 'F': stype = FListType
		case 'L': stype = LListType
		case 'S': stype = StackType
		case 'Q': stype = QueueType
		case 'T': stype = TreeType
		default:
			return false, fmt.Errorf("unknown structure type for CREATE: %c", command[0])
		}
		_, err := store.CreateAndAddStructure(name, stype)
		if err != nil {
			return false, err
		}
		fmt.Println("OK")
		return true, nil
	}

	if len(parts) < 2 {
		return false, fmt.Errorf("missing structure name for command '%s'", command)
	}
	name := parts[1]
	entry := store.FindEntry(name)
	if entry == nil {
		return false, fmt.Errorf("structure '%s' not found", name)
	}

	// Общие команды
	switch command {
	case "PRINT":
		switch v := entry.data.(type) {
		case *DynamicArray: v.Print()
		case *SinglyLinkedList: v.Print()
		case *DoublyLinkedList: v.Print()
		case *Stack: v.Print()
		case *Queue: v.Print()
		case *AVLTree: v.Print()
		}
		return false, nil
	case "ISMEMBER":
		if len(parts) < 3 {
			return false, errors.New("missing value for ISMEMBER")
		}
		value := parts[2]
		isMember := false
		switch v := entry.data.(type) {
		case *DynamicArray: isMember = v.IsMember(value)
		case *SinglyLinkedList: isMember = v.IsMember(value)
		case *DoublyLinkedList: isMember = v.IsMember(value)
		case *AVLTree:
			intVal, err := strconv.Atoi(value)
			if err != nil {
				return false, fmt.Errorf("invalid integer format for tree operation: %s", value)
			}
			isMember = v.IsMember(intVal)
		default:
			return false, errors.New("ISMEMBER is not supported for this type")
		}
		if isMember {
			fmt.Println("TRUE")
		} else {
			fmt.Println("FALSE")
		}
		return false, nil
	}

	// Команды для конкретных типов
	switch v := entry.data.(type) {
	case *DynamicArray:
		switch command {
		case "MPUSH_BACK":
			if len(parts) < 3 { return false, errors.New("missing value") }
			v.PushBack(parts[2])
			fmt.Println("OK")
			return true, nil
		case "MGET":
			if len(parts) < 3 { return false, errors.New("missing index") }
			idx, err := strconv.Atoi(parts[2])
			if err != nil { return false, err }
			val, err := v.Get(idx)
			if err != nil { return false, err }
			fmt.Println(val)
			return false, nil
		case "MLENGTH":
			fmt.Println(v.Length())
			return false, nil
		case "MINSERT_AT": // <-- НОВАЯ КОМАНДА
			if len(parts) < 4 { return false, errors.New("missing index or value") }
			idx, err := strconv.Atoi(parts[2])
			if err != nil { return false, fmt.Errorf("invalid index: %w", err) }
			value := parts[3]
			if err := v.InsertAt(idx, value); err != nil { return false, err }
			fmt.Println("OK")
			return true, nil
		case "MSET_AT": // <-- НОВАЯ КОМАНДА
			if len(parts) < 4 { return false, errors.New("missing index or value") }
			idx, err := strconv.Atoi(parts[2])
			if err != nil { return false, fmt.Errorf("invalid index: %w", err) }
			value := parts[3]
			if err := v.SetAt(idx, value); err != nil { return false, err }
			fmt.Println("OK")
			return true, nil
		case "MDEL_AT": // <-- НОВАЯ КОМАНДА
			if len(parts) < 3 { return false, errors.New("missing index") }
			idx, err := strconv.Atoi(parts[2])
			if err != nil { return false, fmt.Errorf("invalid index: %w", err) }
			val, err := v.DelAt(idx)
			if err != nil { return false, err }
			fmt.Println(val)
			return true, nil
		default:
			return false, fmt.Errorf("unknown command '%s' for DynamicArray", command)
		}
	case *SinglyLinkedList:
		switch command {
		case "FPUSH_HEAD":
			if len(parts) < 3 { return false, errors.New("missing value") }
			v.PushHead(parts[2])
			fmt.Println("OK")
			return true, nil
		case "FPUSH_TAIL":
			if len(parts) < 3 { return false, errors.New("missing value") }
			v.PushTail(parts[2])
			fmt.Println("OK")
			return true, nil
		case "FLENGTH": // <-- НОВАЯ КОМАНДА
			fmt.Println(v.length)
			return false, nil
		default:
			return false, fmt.Errorf("unknown command '%s' for SinglyLinkedList", command)
		}
	case *DoublyLinkedList:
		switch command {
		case "LPUSH_HEAD": // <-- НОВАЯ КОМАНДА
			if len(parts) < 3 { return false, errors.New("missing value") }
			v.PushHead(parts[2])
			fmt.Println("OK")
			return true, nil
		case "LPUSH_TAIL":
			if len(parts) < 3 { return false, errors.New("missing value") }
			v.PushTail(parts[2])
			fmt.Println("OK")
			return true, nil
		case "LGET": // <-- НОВАЯ КОМАНДА
			if len(parts) < 3 { return false, errors.New("missing index") }
			idx, err := strconv.Atoi(parts[2])
			if err != nil { return false, fmt.Errorf("invalid index: %w", err) }
			val, err := v.Get(idx)
			if err != nil { return false, err }
			fmt.Println(val)
			return false, nil
		case "LSET_AT": // <-- НОВАЯ КОМАНДА
			if len(parts) < 4 { return false, errors.New("missing index or value") }
			idx, err := strconv.Atoi(parts[2])
			if err != nil { return false, fmt.Errorf("invalid index: %w", err) }
			value := parts[3]
			if err := v.SetAt(idx, value); err != nil { return false, err }
			fmt.Println("OK")
			return true, nil
		case "LDEL_AT": // <-- НОВАЯ КОМАНДА
			if len(parts) < 3 { return false, errors.New("missing index") }
			idx, err := strconv.Atoi(parts[2])
			if err != nil { return false, fmt.Errorf("invalid index: %w", err) }
			val, err := v.DelAt(idx)
			if err != nil { return false, err }
			fmt.Println(val)
			return true, nil
		case "LLENGTH": // <-- НОВАЯ КОМАНДА
			fmt.Println(v.length)
			return false, nil
		default:
			return false, fmt.Errorf("unknown command '%s' for DoublyLinkedList", command)
		}
	case *Stack:
		switch command {
		case "SPUSH":
			if len(parts) < 3 { return false, errors.New("missing value") }
			v.Push(parts[2])
			fmt.Println("OK")
			return true, nil
		case "SPOP":
			val, err := v.Pop()
			if err != nil { return false, err }
			fmt.Println(val)
			return true, nil
        case "SPEAK":
            val, err := v.Peek()
            if err != nil { return false, err }
            fmt.Println(val)
            return false, nil
        case "SLENGTH":
            fmt.Println(v.Length())
            return false, nil
		default:
			return false, fmt.Errorf("unknown command '%s' for Stack", command)
		}
    case *Queue:
        switch command {
        case "QPUSH":
            if len(parts) < 3 { return false, errors.New("missing value") }
            v.Push(parts[2])
            fmt.Println("OK")
            return true, nil
        case "QPOP":
            val, err := v.Pop()
            if err != nil { return false, err }
            fmt.Println(val)
            return true, nil
        case "QPEEK":
            val, err := v.Peek()
            if err != nil { return false, err }
            fmt.Println(val)
            return false, nil
        case "QLENGTH":
            fmt.Println(v.Length())
            return false, nil
        default:
            return false, fmt.Errorf("unknown command '%s' for Queue", command)
        }
	case *AVLTree:
		switch command {
		case "TINSERT":
			if len(parts) < 3 { return false, errors.New("missing value") }
			val, err := strconv.Atoi(parts[2])
			if err != nil { return false, fmt.Errorf("invalid integer: %s", parts[2]) }
			v.Insert(val)
			fmt.Println("OK")
			return true, nil
		case "TDEL":
			if len(parts) < 3 { return false, errors.New("missing value") }
			val, err := strconv.Atoi(parts[2])
			if err != nil { return false, fmt.Errorf("invalid integer: %s", parts[2]) }
			if v.Delete(val) {
                fmt.Println("OK")
                return true, nil
            } else {
                fmt.Println("Not Found")
                return false, nil
            }
		default:
			return false, fmt.Errorf("unknown command '%s' for AVLTree", command)
		}
	}

	return false, fmt.Errorf("command '%s' not recognized", command)
}

func main() {
	var filePath string
	args := os.Args[1:]
	for i := 0; i < len(args); i++ {
		if args[i] == "--file" && i+1 < len(args) {
			filePath = args[i+1]
			i++
		}
	}

	if filePath == "" {
		fmt.Fprintln(os.Stderr, "Error: --file argument is required.")
		os.Exit(1)
	}

	store := NewDataStore()
	if err := LoadFromFile(store, filePath); err != nil {
		fmt.Fprintf(os.Stderr, "Error loading from file: %v\n", err)
	}

	reader := bufio.NewReader(os.Stdin)
	for {
		fmt.Print("> ")
		line, err := reader.ReadString('\n')
		if err == io.EOF || strings.TrimSpace(line) == "QUIT" {
			break
		}
		line = strings.TrimSpace(line)
		if line == "" {
			continue
		}

		modified, err := processCommand(store, line)
		if err != nil {
			fmt.Fprintf(os.Stderr, "ERROR: %v\n", err)
		} else if modified {
			if err := SaveToFile(store, filePath); err != nil {
				fmt.Fprintf(os.Stderr, "ERROR saving to file: %v\n", err)
			}
		}
	}
}