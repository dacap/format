# Copyright (C) 2016 David Capello
#
# This file is released under the terms of the MIT license.
# Read LICENSE.txt for more information.

function(add_format_test name)
  add_executable(${name} ${name}.cpp)
  add_test(NAME ${name} COMMAND ${name})
endfunction()

add_format_test(avoid_copy)
add_format_test(ints)
