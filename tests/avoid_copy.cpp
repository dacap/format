// Copyright (C) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include <iostream>
#include "format.h"

class A {
public:
  A() { std::cout << "A()\n"; }
  A(const A& a) {
    // We don't expect to make copies using format()
    throw std::runtime_error("Making a copy of a temporal");
  }
  ~A() { std::cout << "~A()\n"; }
  A& operator=(const A& a) { std::cout << "A::=(const A&)\n"; return *this; }
};

std::ostream& operator<<(std::ostream& os, const A& a) {
  return os << "A::<<()";
}

int main() {
  auto fmt = format::string("{0}", A());
  std::cout << "Output:" << fmt << "\n";
}
