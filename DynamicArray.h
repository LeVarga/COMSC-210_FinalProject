// Programmer: Levente Varga
// Programmer's ID: 1758518
#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

template <typename T>
class DynamicArray {
    T* value;
    int cap;
    T dummy = T();

public:
    DynamicArray(int=2);
    DynamicArray(const DynamicArray<T>&); // copy constructor
    ~DynamicArray() { delete[] value; } // destructor
    DynamicArray<T>& operator=(const DynamicArray<T>&); // assignment operator
    int capacity() const;
    void capacity(int);
    T operator[](int) const;
    T& operator[](int);
};

template <typename T>
DynamicArray<T>::DynamicArray(int cap) {
  this->cap = cap;
  value = new T[cap];
  for (int i = 0; i < cap; i++)
    value[i] = T();
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& original) {
  cap = original.cap;
  value = new T[cap];
  for (int i = 0; i < cap; i++)
    value[i] = original.value[i];
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& original) {
  if (this != &original) { // of it's not a self copy...{
    // do what the destructor does
    delete[] value;
    // do what the copy constructor does
    cap = original.cap; // still just a copy
    value = new T[cap]; // not a copy -- new object gets its own array
    for (int i = 0; i < cap; i++) // copy the contents of the array from the original...
      value[i] = original.value[i]; // ...to the copy
  }
  return *this; // return a self reference
}

template <typename T>
int DynamicArray<T>::capacity() const {
  return cap;
}

template <typename T>
void DynamicArray<T>::capacity(int cap) {
  // allocate a new array with the new capacity
  T* temp = new T[cap];
  // get the lesser of the new and old capacities
  int limit = (cap < this->cap ? cap : this->cap) ;
  // copy the contents
  for (int i = 0; i < limit; i++)
    temp[i] = value[i];
  // set added values to their defaults
  for (int i = limit; i < cap; i++)
    temp[i] = T();
  // deallocate original array
  delete[] value;
  // switch newly allocated array into the object
  value = temp;
  //update the capacity
  this->cap = cap;
}

template <typename T>
T DynamicArray<T>::operator[](int i) const {
  if (i >= cap || i < 0) return T();
  return value[i];
}

template <typename T>
T& DynamicArray<T>::operator[](int i) {
  if (i < 0) return dummy;
  if (i >= cap) capacity(2 * i);
  return value[i];
}

#endif //DYNAMICARRAY_H
