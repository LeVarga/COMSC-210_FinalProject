// Programmer: Levente Varga
// Programmer's ID: 1758518
#ifndef QUEUE_H
#define QUEUE_H

template <typename V>
class Queue {
private:
    struct Node {
        V value;
        Node* next;
    };
    int siz;
    Node* firstNode;
    Node* lastNode;
    V dummy = V();

public:
    Queue(); // may have a defaulted parameter
    Queue(const Queue<V>&); // copy constructor
    ~Queue();
    Queue<V>& operator=(const Queue<V>&);
    void push(const V&);
    const V& front() const { return (firstNode == 0) ? dummy : firstNode->value;}
    const V& back() const { return (lastNode == 0) ? dummy : lastNode->value; }
    void pop(); // remove the oldest node
    int size() const { return siz; };
    bool empty() const { return siz == 0; };
    void clear();
};

template <typename V>
Queue<V>::Queue() {
  firstNode = 0;
  lastNode = 0;
  siz = 0;
}

template <typename V>
Queue<V>::Queue(const Queue<V>& original) {
  firstNode = 0;
  lastNode = 0; // temporary tail
  siz = original.siz;
  for (Node* p = original.firstNode; p; p = p->next) {
    Node* temp = new Node;
    temp->value = p->value;
    temp->next = 0;
    if (lastNode) lastNode->next = temp;
    else firstNode = temp;
    lastNode = temp;
  }
}

template <typename V>
Queue<V>& Queue<V>::operator=(const Queue<V>& original) {
  if (this != &original) {
    // deallocate existing list
    while (firstNode) {
      Node* p = firstNode;
      firstNode = firstNode->next;
      delete p;
    }
    // build new queue
    lastNode = 0; // temporary tail
    for (Node* p = original.firstNode; p; p = p->next) {
      Node* temp = new Node;
      temp->value = p->value;
      temp->next = 0;
      if (lastNode) lastNode->next = temp;
      else firstNode = temp;
      lastNode = temp;
    }
    siz = original.siz;
  }
  return *this;
}

template <typename V>
Queue<V>::~Queue( ) {
  while (firstNode) {
    Node* p = firstNode;
    firstNode = firstNode->next;
    delete p;
  }
}

template <typename V>
void Queue<V>::clear() {
  lastNode = 0;
  while (firstNode) {
    Node* p = firstNode;
    firstNode = firstNode->next;
    delete p;
    --siz;
  }
}

template <typename V>
void Queue<V>::pop() {
  if (firstNode) {
    Node* p = firstNode;
    firstNode = firstNode->next;
    delete p;
    --siz;
  }
  if (siz == 0) lastNode = 0;
}

template <typename V>
void Queue<V>::push(const V& value) {
  Node* temp = new Node{value};
  if (lastNode) lastNode->next = temp;
  else firstNode = temp;
  lastNode = temp;
  ++siz;
}
#endif //QUEUE_H
