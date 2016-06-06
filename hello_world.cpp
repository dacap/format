// Copyright (C) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include <iostream>
#include "format.h"

int main() {
  std::cout << format::string("{1} {0}!", "world", "hello") << "\n";
}
