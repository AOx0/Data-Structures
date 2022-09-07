#include <iostream>

#include "lifo.hpp"

using namespace std;

int main() {
  Pila pila = Pila(20);
  
  for (int i=1; i<=100; i++) {
    pila.push(i);
  }
  
  cout << pila << endl;
  cout << "Primer elemento: " << pila[0] << endl;
  cout << "Quinto elemento: " << pila[4] << endl;
  cout << "Primos ingresados: " << pila.numero_primos() << endl;
  return 0;
}
