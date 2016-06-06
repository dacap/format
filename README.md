# Format Library
*Copyright (C) 2016 David Capello*

> Released under the [MIT license](LICENSE.txt)

Example:

    #include <iostream>
    #include "format.h"

    int main() {
      std::cout << format::string("{1}*{1} {0} {2}", "is", 4, 16) << "\n";
    }

Prints:

    4*4 is 16

## Format one value

Example:

    #include <iostream>
    #include "format.h"

    int main() {
      std::string out;
      format::append_value(out, 32);

      std::cout << out << "+" << out << " = "
                << format::value(64) << "\n";
    }

Prints:

    32*32 = 64

## Tested Compilers

* Visual Studio 2015
* Xcode 7.3.1 (`-std=c++11`)
* GCC 4.8.4 (`-std=c++11`)
