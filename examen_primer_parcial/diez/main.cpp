/*
FiFo. Para una cola de enteros, realice:

    a) Agregue a la cola un vector de estadísticas de enteros como atributo privado, tendrá 10 casillas. Lo debe inicializar todo en ceros.
    b) Genere un método que muestre en pantalla los valores de las 10 casillas del vector de estadísticas.
    c) Determine el tamaño de la cola entre 300 y 600 (el que usted quiera).
    d) Genere 3000 enteros que meterá en la cola, deben estar en el rango de 0 a 100.
    e) Si la cola se llena, saque un elemento de la cola para que pueda ingresar uno más.
    f) Si ya se generaron los 3000 enteros, saque de la cola los enteros que restan.
    g) Cada que saque a un entero, debe ver si está entre 0 y 10, si es así, aumentar en 1 la primera celda del vector de estadísticas. Si está entre 11 y 20, en la segunda, y así sucesivamente.
    h) Al terminar de sacar todos los enteros de la cola, ejecute el método que muestra los valores del vector de estadísticas del inciso b.
*/

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

template <class T>
class FiFo {
protected:
    vector<int> stats;
    T* aData;
    int aMax;
    int aCur;
    int aErr;
    int aStep;

public:
    FiFo(int pMax, int pStep = 0);
    ~FiFo(void);

    void push(T pVal);
    T top(void);
    void pop(void);
    bool isEmpty(void);
    bool isFull(void);
    int error(void);
    T get(int pIdx);
    T operator [](int pIdx);
    void repr(void);

    void mostrar_stats() {
      cout << "Stats: [ ";
      for (int i=0; i<10; i++) {
        cout << stats[i] << " ";
      }
      cout << "]" << endl;
    }
private:
    void resize(void);
};

template <class T>
FiFo<T>::FiFo(int pMax, int pStep)
{
    aCur = 0;
    aErr = 0;
    aData = NULL;
    aStep = pStep;
    aMax = 0;

    if (pMax > 0) {
        aMax = pMax;
        aData = new T[aMax];
 
        stats = vector<int>();
        for (int i=0; i<10; i++) {
          stats.push_back(0);
        }
        if (aData == NULL)
            aErr = -2; // No hay memoria
    }
    else
        aErr = -1; // Max fuera de rango
} // Constructor

template <class T>
FiFo<T>::~FiFo(void)
{
    aErr = 0;
    aMax = 0;
    aCur = 0;

    if (aData)
        delete[] aData;
}  // Destructor

template <class T>
void FiFo<T>::push(T pVal)
{
    aErr = 0;

    if (aData) {
        if (!isFull()) {
            aData[aCur] = pVal;
            aCur++;
        }
        else {
            if (aStep != 0) {
                resize();
                if (aErr == 0) {
                    aData[aCur] = pVal;
                    aCur++;
                }
            } else
                aErr = -3; // Intento de push en llena
        }
    }
} // push

template <class T>
T FiFo<T>::top(void)
{
    aErr = 0;

    if (aData) {
        if (!isEmpty()) {
            return aData[0]; // aCur - 1
        }
        else {
            aErr = -4; // Intento de top en vacía
        }
    }
    return (T)0;
} // top

bool in_range(int num, int min, int max) {
  if (num <= max && num >= min) {
    return true;
  } else {
    return false;
  }
}

template <class T>
void FiFo<T>::pop(void)
{
    aErr = 0;

    if (aData) {
        if (!isEmpty()) {
            int valor = aData[0];
           
            for (int i=0; i<10; i++) {
              if (i==0) {
                if (in_range(valor, 0, 10)) {
                  stats[0]++;
                }
              } else {
               if (in_range(valor,i * 10 +1, (i * 10) + 10)) {
                stats[i]++;
                }              
              }
            }
            for (int lIdx = 0; lIdx < aMax - 1; lIdx++) {
                aData[lIdx] = aData[lIdx + 1];
            } // Añadido
            aCur--;
        }
        else {
            aErr = -5; // Intento de top en vacía
        }
    }
} // pop

template <class T>
bool FiFo<T>::isEmpty(void)
{
    return (aCur == 0);
} // isEmpty

template <class T>
bool FiFo<T>::isFull(void)
{
    return (aCur == aMax);
} // isFull

template <class T>
int FiFo<T>::error(void)
{
    return aErr;
} // error

template <class T>
T FiFo<T>::get(int pIdx)
{
    aErr = 0;

    if (aData) {
        if (!isEmpty()) {
            if ((0 <= pIdx) && (pIdx < aCur)) {
                return aData[pIdx];
            }
            else {
                aErr = -7; // Intento de get fuera de rango
            }
        }
        else {
            aErr = -6; // Intento de get en vacía
        }
    }
    return (T)0;
} // get

template <class T>
T FiFo<T>::operator [](int pIdx)
{
    return get(pIdx);
} // operator []

template <class T>
void FiFo<T>::repr(void)
{
    cout << (isEmpty() == true ? "1" : "0");
    cout << "|";
    cout << (isFull() == true ? "1" : "0");
    cout << "|";
    cout << aMax;
    cout << "|";
    cout << aCur;
    cout << "|";
    cout << aErr;
    cout << "||";
    for (int lIdx = 0; lIdx < aCur; lIdx++) {
        cout << aData[lIdx];
        cout << ", ";
    }
    cout << endl;
} // repr

template <class T>
void FiFo<T>::resize(void)
{
    int lMax = 0;
    T* lData = NULL;

    aErr = 0;

    if (aData) {
        if (aStep > 0) {
            lMax = aMax + aStep;
            lData = new T[lMax];
            if (lData) {
                for (int lIdx = 0; lIdx < aMax; lIdx++)
                    lData[lIdx] = aData[lIdx];
                delete[] aData;
                aData = lData;
                aMax = lMax;
            }
            else
                aErr = -9; // No memoria para resize
        }
        else
            aErr = -8; // Intento de resize fuera de rango
    }
} // resize

int random(int min, int max) {
  return rand() % (max - min) + min;
}

int main()
{ 
    srand(time(NULL));
    FiFo<int> lCola = FiFo<int>(random(300, 600), 100);


    for (int i=0; i<3000; i++) {
      if (lCola.isFull()) {
        lCola.pop();
      }
      lCola.push(random(0, 100));
    }

    while (!lCola.isEmpty()) {
      lCola.pop();
    }

    lCola.mostrar_stats();
    cout << "Nota: La suma es 3000 ;)" << endl;
}
