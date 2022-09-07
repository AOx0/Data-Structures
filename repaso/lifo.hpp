/*
 Realice un clase pila (LiFo) con los métodos estándar push, pop, isEmpty y isFull, de enteros. Cada que se haga un push de en la pila, verificar si el número es primo, y si lo es añadir 1 a un atributo privado de la clase. Haga el método que pueda entregar el valor del número de primos que han entrado en la pila.
*/

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <system_error>
#include <math.h>
#include "lib/result.hpp"

using namespace std;

struct Pila {
  private:
    int * primos;
    int primo_max;
    size_t num_primos;
    size_t size_num_primos;
    int primos_ingresados;

    static bool num_in(int * n, int size, int num);
    void push_prime(int n);
    void gen_primos(int hasta);
    bool is_prime(int n);

  protected:
    int suma;
    int * data;
    int size;
    int curr;
    Result<uint8_t, uint8_t> resize(int new_size);

    friend ostream& operator<< (ostream &os, const Pila &e);

  public:
    Result<uint8_t, uint8_t> get(size_t idx);
    int operator[](size_t idx);
    Pila(int size);
    ~Pila();
    bool is_empty();
    bool is_full();
    Result<uint8_t, uint8_t> push(int valor);
    Result<uint8_t, uint8_t> pop(int valor);
    int numero_primos() const;
};

