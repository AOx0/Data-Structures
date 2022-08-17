/// 12 Agosto 2022
/// Programación y estructura de datos


// #include <cstdlib>
#include <iostream>

// void swap1(int x, int y) {}

void swap(int * x, int * y) {
  int lm = *x;
  *x = *y;
  *y = lm;
}

/*
template<typename T> 
T * Arreglo(int len) {
  T * res = (T *)malloc(sizeof(T)*len);
  return res;
}

// Nótese como pasamos un apuntador a un apuntador.
// No es lo mismo tener un apuntador a un entero a un apuntador a un apuntador a un entero.
// Si no pongo ** hay error señal 11;
template<typename T>
void ArregloDir(T ** target, int len) {
  *target = (T *)malloc(sizeof(T)*len);
}
*/

int main() {
  int x=10, y=5;
  // swap1(x,y); // Parametros con valor copiado. Las modificaciones no afectan.
  // std::cout << x << " " << y << std::endl;
  swap(&x,&y); // Por referencia
  printf("%d %d\n", x, y);
  //std::cout << x << " " << y << std::endl;

/**
  double * z;
  ArregloDir(&z, 5);
  z[0] = 5.0;

  std::cout << z[0] << std::endl;

  free(z);
*/

  return 0;
}
