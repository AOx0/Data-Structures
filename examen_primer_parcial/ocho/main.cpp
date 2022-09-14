#include <iostream>

using namespace std;

int main() {
  // Por buena practica se debe usar new T[] cuando se usa delete
  // como metodo para liberar la memoria
  //int *lVal = malloc(5 * sizeof(int *));
  int *lVal = new int[5];
  int *lPtr = lVal;
  *lPtr = (int) NULL;

  for (int lIdx = 0; lIdx < 5; lIdx++) {
    // No es necesario castizar pues lIdx ya es int
    lVal[lIdx] = lIdx * (*lPtr);
    cout << lVal[lIdx];
  }
  
  delete[] lPtr;
  return 0;
}
