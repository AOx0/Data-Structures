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

    bool num_in(int * n, int size, int num) {
      for (int i=0; i<size; i++) {
        if (num == n[i]) {
          return true;
        }
      }

      return false;
    }

    void push_prime(int n) {
      if (num_primos == size_num_primos) {
        size_num_primos += 100;
        int * new_primos = new int[size_num_primos];

        for (int i=0; i<num_primos; i++) {
          new_primos[i] = primos[i];
        }

        delete [] primos;

        primos = new_primos;
      }

      primo_max = n;
      primos[num_primos] = n;
      num_primos++;
    }


    void gen_primos(int hasta) {
      delete [] primos;
      size_num_primos = 0;
      num_primos = 0;

      bool * no_primos = new bool[hasta+1];

      for (int i=0; i<=hasta; i++) no_primos[i] = false;

      for (int i=2; i<=hasta; i++) {
        if (no_primos[i] != true) {
          push_prime(i);
          int j = 1;
          while (true) {
            int b = pow(i, 1) * j;
            if (b > hasta) break;
            no_primos[b] = true;
            j++;
          }
        }
      }

      delete [] no_primos;
    }

    bool is_prime(int n) {
      if (num_primos == 0 || n > primo_max) {
        gen_primos(n < 10000 ? 10000 : n);
      }
      
      return num_in(primos, size_num_primos, n); 
    }

  protected:
    int suma;
    int * data;
    int size;
    int curr;
    Result<uint8_t, uint8_t> resize(int new_size);

    friend ostream& operator<< (ostream &os, const Pila &e) {
      os << "[";
      for (int i=0; i<e.curr; i++) {
        os << " " <<  e.data[i];
      }
      os << " ]";
      return os;
    }

  public:
    Result<uint8_t, uint8_t> get(size_t idx);
    int operator[](size_t idx);
    Pila(int size);
    ~Pila();
    bool is_empty();
    bool is_full();
    Result<uint8_t, uint8_t> push(int valor);
    Result<uint8_t, uint8_t> pop(int valor);

    int numero_primos() {
      return primos_ingresados;
    }
};

