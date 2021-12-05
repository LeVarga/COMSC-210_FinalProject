// Programmer: Levente Varga
// Programmer's ID: 1758518
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <list>
#include "Queue.h"

using namespace std;

template <typename K, typename V, int CAP>
class HashTable {
private:
    struct Node {
        K key;
        V value;
    };

    list<Node> data[CAP];
    int(*hashCode)(const K&);
    int siz = 0;
public:
    HashTable(int(*)(const K&) = 0);
    double loadFactor() const { return 1.0 * siz / CAP; }
    V operator[] (const K&) const;
    V& operator[] (const K&);
    bool containsKey(const K&) const;
    void deleteKey(const K&);
    Queue<K> keys() const;
    int size() const { return siz; }
    void clear();
};

template <typename K, typename V, int CAP>
HashTable<K, V, CAP>::HashTable(int (*hashCode)(const K &)) {
  this->hashCode = hashCode;
}

template <typename K, typename V, int CAP>
V HashTable<K, V, CAP>::operator[](const K& key) const {
  int index = hashCode(key) % CAP;
  if (index < 0) index += CAP;
  for (const auto& node : data[index]) if (node.key == key) return node.value;
  return V();
}

template <typename K, typename V, int CAP>
V& HashTable<K, V, CAP>::operator[](const K& key) {
  int index = hashCode(key) % CAP;
  if (index < 0) index += CAP;
  for (auto& node : data[index]) if (node.key == key) return node.value;
  data[index].push_back(Node{key, V()});
  siz++;
  return data[index].back().value;
}

template <typename K, typename V, int CAP>
bool HashTable<K, V, CAP>::containsKey(const K& key) const {
  int index = hashCode(key) % CAP;
  if (index < 0) index += CAP;
  for (const auto& node : data[index]) if (node.key == key) return true;
  return false;
}

template <typename K, typename V, int CAP>
void HashTable<K, V, CAP>::deleteKey(const K& key) {
  int index = hashCode(key) % CAP;
  if (index < 0) index += CAP;
  typename list<Node>::const_iterator it;
  for (it = data[index].begin(); it != data[index].end(); it++) {
    if (it->key == key) {
      data[index].erase(it);
      siz--;
      break;
    }
  }
}

template <typename K, typename V, int CAP>
Queue<K> HashTable<K, V, CAP>::keys() const {
  Queue<K> keys;
  for (const auto& i : data) {
    for (const auto& node : i) {
      keys.push(node.key);
    }
  }
  return keys;
}

template <typename K, typename V, int CAP>
void HashTable<K, V, CAP>::clear() {
  siz = 0;
  for (auto& i : data) {
    i.clear();
  }
}
#endif //HASHTABLE_H