#include "lifo.hpp"
#include <utility>

#define Res Result<uint8_t, uint8_t> 

Pila::Pila(int size) 
  : data((int *)NULL)
  , primos((int *)NULL)
  , num_primos(0)
  , size_num_primos(0)
  , primo_max(0)
  , primos_ingresados(0)
  , size(size)
  , curr(0)
{
  data = new int[size];
}

Res Pila::get(size_t idx) {
  if (curr < idx) {
    return Res::Err(0);
  } else if (idx < 0) {
    return Res::Err(1);
  } else {
    return Res::Ok(data[idx]);
  }
}

int Pila::operator[](size_t idx) {
  Res res = get(idx);
  res.is_ok();
  int r = res.unwrap();
  return r;
}

Res Pila::pop(int valor) {
    if (curr == 0) {
      return Res::Err(1);
    } else {
      curr--;
      return Res::Ok(data[curr]);
    }
}

Res Pila::push(int valor) {
  if (size == curr) {
    Res result = resize(size + 5);
    if (result.is_err()) {
      return result;
    }
  }


  if (is_prime(valor)) {
    primos_ingresados++;
  }

  data[curr++] = valor;
  return Res::Ok(0); 
}

Pila::~Pila() {
  delete [] data;
  delete [] primos;
}

Res Pila::resize(int new_size) {
  if (new_size < size) {
    return Res::Err(1);
  }
  int * new_data = new int[new_size];

  for (int i=0; i<curr; i++) {
    new_data[i] = data[i];
  }

  size = new_size;

  delete [] data;
  data = new_data;

  return Res::Ok(0);
}


bool Pila::is_empty() {
  return curr == 0;
};

bool Pila::is_full() {
  return curr == size;
};
