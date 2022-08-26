#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <stack>
#include <math.h>

// dentro hacia afuera
//

double resolve(std::string s) {
  std::stack<double> lStack;
  double l1, l2, l3;
  //std::istringstream lexpr("1.0;2.0;+;4.0;*;3.0;+;4.0;^");
  std::istringstream lexpr(s);
  std::string lped;

  while (std::getline(lexpr, lped, ';')) {
    // std::cout << lped << std::endl;
    if (std::isdigit(lped[0])) {
      lStack.push(std::stod(lped)); // stod -> String to double
    } else {
      // Detectar si es resta, numero negativo
      if ((lped.length() > 1) && (lped[0] == '-')) {
        lStack.push(std::stod(lped));
      } else {
        if (lped.length() == 1) {
          l2 = lStack.top(); // Obtenemos el ultimo
          lStack.pop();
          l1 = lStack.top(); // Obtenemos el ultimo
          lStack.pop();

          // Analizamos el tipo de operador        
          switch (lped[0]) {
            case '-': l3 = l1 - l2; break;
            case '+': l3 = l1 + l2; break;
            case '*': l3 = l1 * l2; break;
            case '/': l3 = l1 / l2; break;
            case '%': l3 = (int)l1 % (int)l2; break;
          }
          
        } else {
          l2 = lStack.top(); lStack.pop();
          if (lped == "sin") {
            l3 = sin(l2);
          } else if (lped == "cos") {
            l3 = cos(l2);
          }  else if (lped == "tan") {
            l3 = tan(l2);
          } else if (lped == "pow") {
            // Aqui si obtenemos el valor de l1 (la base)
            l1 = lStack.top(); lStack.pop();
            l3 = pow(l1, l2);
          } 
        }
        lStack.push(l3);
      }
    }
  }

  l3 = lStack.top();
  return l3;
}


int main() {
  std::string s;
  std::cin >> s;

  for (double x=-10.0; x<=10.0; x+=0.25) {
    std::string xss = std::to_string(x);
    for (double y=-10.0; y<=10.0; y+=0.25) {
      std::string replaced("");
      std::string yss = std::to_string(y);
      for (int c=0; c<s.length(); c++) {
        if (s[c] == 'x') {
          replaced.append(xss); 
        } else if (s[c] == 'y') {
          replaced.append(yss); 
        } else {
          replaced.push_back(s[c]); 
        }
      }
      printf("( %7.3lf, %7.3lf, %7.3lf )\n", x, y, resolve(replaced));
    }
  }
  return 0;
}
