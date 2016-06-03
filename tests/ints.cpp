// Copyright (C) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "format.h"
#include "test.h"
#include <sstream>
#include <string>
#include <limits>

template<typename T>
void test_limits() {
  std::stringstream str;
  str << std::numeric_limits<T>::min() << " "
      << std::numeric_limits<T>::max();

  std::string expected = str.str();
  std::string actual = format("{0} {1}",
                              std::numeric_limits<T>::min(),
                              std::numeric_limits<T>::max());

  std::cout << expected << "\n"
            << actual << "\n";

  EXPECT_EQ(expected, actual);
}

int main() {
  test_limits<bool>();
  test_limits<int>();
  test_limits<long>();
  test_limits<long long>();
  test_limits<unsigned int>();
  test_limits<unsigned long>();
  test_limits<unsigned long long>();
}
