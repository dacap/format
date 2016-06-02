# Format Library
*Copyright (C) 2016 David Capello*

> Released under the [MIT license](LICENSE.txt)

Example:

    #include <iostream>
    #include "format.h"

    int main() {
      std::cout << format("{1}*{1} {0} {2}", "is", 4, 16) << "\n";
    }

Prints:

    4*4 is 16
