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
    Pila(uint8_t size); // Constructor
    ~Pila(void); // Destructor, se llama al salir del scope o manual
    T get(uint8_t indice);
    void repr(void);
    uint8_t error(void);

  protected: // No usamos el private para permitir a herederos acceder
    uint8_t aMax;
    uint8_t aCur;
    uint8_t aErr;
    T * aData;
};

template<typename T>
void Pila<T>::repr() {
  printf("| c %d | m %d | e %d || ", aCur, aMax, aErr);
  for (int i=0; i<aCur; i++) 
    std::cout << aData[i] << " | ";
  std::cout << std::endl;
}

template<typename T>
Pila<T>::Pila(uint8_t size): aMax(size), aCur(0), aData(NULL), aErr(0) {
  if (size > 0) {
    aData = new T[size];
    if (aData == NULL) /* Si no tiene memoria  */ {
      aErr = 2;
    }
  } else 
      aErr = 1;
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
    if (!is_full()) { // Si no está lleno
      aData[aCur] = element;
      aCur++;
    } else {
      aErr = 3;
    }
  }
}

template<typename T>
T Pila<T>::top() {
  return aData[aCur-1];
}

template<typename T>
void Pila<T>::pop() {
  aData[aCur-1] = 0;
  aCur--;
}

int main() {
  {
    Pila<int> pila = Pila<int>(10);
    pila.push(1);
    pila.push(2);

    int last = pila.top();
    pila.pop();

    pila.repr();
    std::cout << "Last top: " << last << std::endl;

    /*
    Pila<float> pilaf = Pila<float>(10);
    pilaf.push(1.0);
    pilaf.push(2.0);

    float lastf = pilaf.top();
    pilaf.pop();
    pilaf.push(100);

    pilaf.repr();
    std::cout << "Last top: " << lastf << std::endl;
    */
  }
  return 0;
}

