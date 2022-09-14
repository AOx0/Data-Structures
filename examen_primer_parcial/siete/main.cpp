#include <iostream>

class A {
  protected:
    int aVal;

  public:
     A(int pVal);
     
     // No es necesario el de-constructor
     // porque no hay memoria que liberar
     //~A();
    int getVal();
}; // class A

A::A(int pVal)
{
	aVal= pVal;
} // Constructor

int A::getVal()
{
	return aVal;
} // getVal

int main() {
  A a = A(3);
  std::cout << "El valor es: " << a.getVal()  << std::endl;
  return 0;
}
