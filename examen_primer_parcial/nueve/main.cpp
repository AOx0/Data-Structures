/*
LiFo. Para una pila de reales, realice:

    a) Un método que entregue el porcentaje de utilización de la memoria asignada a la pila.
    b) Un método que busque dentro de la pila y entregue el índice del elemento más pequeño de todos.
    c) Con un ejemplar de esa clase de pila, del tamaño entre 250 y 750 (el que usted quiera), llénela con valores aleatorios entre 0.5 y 0.75, sin pasar del 75% de ocupación de la pila.
    d) Muestre el índice del elemento menor dentro de la pila.
    e) Saque de la pila los elementos necesarios para que tenga una utilización cercana al 25%
*/

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ostream>

using namespace std;

class LiFo {
protected:
    double* aData;
    int aMax;
    int aCur;
    int aErr;
    int aStep;

public:
    LiFo(int pMax, int pStep = 0);
    ~LiFo(void);
    void push(double pVal);
    double top(void);
    void pop(void);
    bool isEmpty(void);
    bool isFull(void);

    int error(void);
    double get(int pIdx);
    double operator [](int pIdx);
    void repr(void);
    
    // Metodos nuevos
    double porcentaje();
    int menor();
private:
    void resize(void);
};

LiFo::LiFo(int pMax, int pStep)
{
    aCur = 0;
    aErr = 0;
    aData = NULL;
    aStep = pStep;
    aMax = 0;

    if (pMax > 0) {
        aMax = pMax;
        aData = new double[aMax];
        if (aData == NULL)
            aErr = -2; // No hay memoria
    }
    else
        aErr = -1; // Max fuera de rango
} // Constructor

LiFo::~LiFo(void)
{
    aErr = 0;
    aMax = 0;
    aCur = 0;

    if (aData)
        delete[] aData;
}  // Destructor

void LiFo::push(double pVal)
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

double LiFo::top(void)
{
    aErr = 0;

    if (aData) {
        if (!isEmpty()) {
            return aData[aCur - 1];
        }
        else {
            aErr = -4; // Intento de top en vacía
        }
    }
    return (double)0;
} // top

void LiFo::pop(void)
{
    aErr = 0;

    if (aData) {
        if (!isEmpty()) {
            aCur--;
        }
        else {
            aErr = -5; // Intento de top en vacía
        }
    }
} // pop

bool LiFo::isEmpty(void)
{
    return (aCur == 0);
} // isEmpty

bool LiFo::isFull(void)
{
    return (aCur == aMax);
} // isFull

int LiFo::error(void)
{
    return aErr;
} // error

double LiFo::get(int pIdx)
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
    return (double)0;
} // get

double LiFo::operator [](int pIdx)
{
    return get(pIdx);
} // operator []

void LiFo::repr(void)
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

void LiFo::resize(void)
{
    int lMax = 0;
    double* lData = NULL;

    aErr = 0;

    if (aData) {
        if (aStep > 0) {
            lMax = aMax + aStep;
            lData = new double[lMax];
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

double LiFo::porcentaje() {
  return (aCur)/(double)aMax * 100.0;
}

int LiFo::menor() {
  int menor = -1;
  double mas_pequeño = 24000000000000000;
  for (int i=0; i<aCur; i++) {
    if (aData[i] < mas_pequeño) {
      mas_pequeño = aData[i];
      menor = i;
    }
  }

  return menor;
}

int random(int min, int max) {
  return rand() % (max - min) + min;
}

int main() {
    srand(time(NULL));

    int numero_elementos = random(250, 750);
    int elementos_real = (double)numero_elementos * 0.75;


    LiFo lPila = LiFo(numero_elementos, 2);

    cout << "Numero de elementos por generar: " << elementos_real;
    cout << "(" << (elementos_real/(double)numero_elementos * 100) << "%) " << endl;

    cout << endl << "Generando elementos para llegar aprox al 75%..." << endl;
    for (int i=0; i<elementos_real-1; i++) {
      lPila.push(random(50, 75)/100.0);
    }

    cout << "El elemento mas pequeño está en el indice " << lPila.menor() << " (" << lPila.get(lPila.menor()) << ")" << endl;
    cout << "La pila está llena al " << lPila.porcentaje() << "%" << endl;

    int elementos_eliminados = 0;
    while (lPila.porcentaje() > 25.0) {
      elementos_eliminados ++;
      lPila.pop();
    }
   
    cout << endl << "Eliminando elementos para llegar al 75%..." << endl;

    cout << "El elemento mas pequeño está en el indice " << lPila.menor() << " (" << lPila.get(lPila.menor()) << ")" << endl;
    cout << "La pila está llena al " << lPila.porcentaje() << "%" << endl;
    cout << "Se eliminaron " << elementos_eliminados << " elementos" << endl;

}
