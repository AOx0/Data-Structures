#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>


template<typename T>
class Pila {
  public:
    void push(T element);
    void pop(void);
    T top(void);
    bool is_empty(void);
    bool is_full(void);
    Pila(); // Constructor
    ~Pila(void); // Destructor, se llama al salir del scope o manual
    T get(uint8_t indice);
    void repr(void);
    uint8_t error(void);
    T operator[](uint8_t);

  protected: // No usamos el private para permitir a herederos acceder
    uint8_t aMax;
    uint8_t aCur;
    uint8_t aErr;
    T * aData;
};

template<typename T>
T Pila<T>::get(uint8_t i) {
  if (aCur < i) {
    aErr = 5;
    return (T)NULL;
  } else {
    return aData[i];
  }
}

template<typename T>
T Pila<T>::operator[](uint8_t i) {
  return get(i);
}

template<typename T>
void Pila<T>::repr() {
  printf("| c %d | m %d | e %d || ", aCur, aMax, aErr);
  for (int i=0; i<aCur; i++) 
    std::cout << aData[i] << " | ";
  std::cout << std::endl;
}

template<typename T>
Pila<T>::Pila(): aMax(1), aCur(0), aData(NULL), aErr(0) {
  aData = new T[1];
  if (aData == NULL) /* Si no tiene memoria  */ {
    aErr = 2;
  }
};

template<typename T>
bool Pila<T>::is_full() {
  return aCur == aMax;
}

template<typename T>
bool Pila<T>::is_empty() {
  return aCur == 0;
}

template<typename T>
Pila<T>::~Pila(void) {
  aErr = 0;
  aCur = 0;
  aMax = 0;
  if (aData != NULL) // Solo borramos si hay reservado 
    delete [] aData;
}

template<typename T>
void Pila<T>::push(T element) {
  aErr = 0; // Reiniciamos el estatus de error
  if (aData) {
    if (is_full()) { // Si no está lleno
      aMax++;
      T * aNew = new T[aMax];
      for (int i=0; i<aCur; i++) aNew[i] = aData[i];
      delete [] aData;
      aData = aNew;
    }

    aData[aCur] = element;
    aCur++;
  }
}

template<typename T>
T Pila<T>::top() {
  if (!is_empty()) { // Solo podemos sacar si hay algo
    return aData[aCur-1];
  } else {
    aErr = 4;
    return (T)NULL;
  }
}

template<typename T>
void Pila<T>::pop() {
  if (!is_empty()) {
    aData[aCur-1] = 0;
    aCur--;
  } else {
    aErr = 5;
  } 
}

int main() {
  {
    Pila<int> pila = Pila<int>();
    pila.push(1);
    pila.push(2);

    int last = pila.top();
    pila.pop();

    pila.repr();
    std::cout << "Last top: " << last << std::endl;

    std::cout << pila[0] << std::endl;

    pila.repr();

    pila.push(1);
    pila.push(2);
    pila.repr();
  }
  return 0;
}

