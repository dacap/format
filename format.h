// Copyright (C) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifndef FORMAT_H_INCLUDED
#define FORMAT_H_INCLUDED

#include <sstream>
#include <tuple>
#include <stdexcept>

namespace format_details {

  // variadic_switch()

  template<int Index, typename Tuple, typename Pred>
  inline typename std::enable_if<(Index == 0), void>::type
  variadic_switch_0(Tuple&& tuple, const int index, Pred&& pred) {
    if (index == 0) {
      pred(std::get<0>(std::forward<Tuple>(tuple)));
    }
  }

  template<int Index, typename Tuple, typename Pred>
  inline typename std::enable_if<(Index > 0), void>::type
  variadic_switch_0(Tuple&& tuple, const int index, Pred&& pred) {
    if (index == Index) {
      pred(std::get<Index>(std::forward<Tuple>(tuple)));
    }
    else {
      variadic_switch_0<Index-1>(
        std::forward<Tuple>(tuple), index,
        std::forward<Pred>(pred));
    }
  }

  template<typename Tuple, typename Pred>
  inline void variadic_switch(Tuple&& tuple, const int index, Pred&& pred) {
    variadic_switch_0<(std::tuple_size<typename std::remove_reference<Tuple>::type>::value-1)>(
      std::forward<Tuple>(tuple), index,
      std::forward<Pred>(pred));
  }

  // format_value()

  template<typename String, typename T>
  inline typename std::enable_if<!std::is_integral<T>::value, void>::type
  format_value(String& output, T& value) {
    std::ostringstream s;
    s << value;
    output += s.str();
  }

  // Fast integer number -> string
  template<typename String, typename T>
  inline typename std::enable_if<std::is_integral<T>::value, void>::type
  format_value(String& output, const T& orig_value) {
    T value = orig_value;

    if (value == 0) {
      output.push_back('0');
      return;
    }

    if (value < 0) {
      output.push_back('-');
      value = -value;
    }

    int i = 0, values[20];
    while (true) {
      if (i == sizeof(values)/sizeof(values[0]))
        throw std::logic_error("Too many decimals");

      values[i++] = (value % 10);
      if (value < 10)
        break;
      value /= 10;
    }

    for (int j=i-1; j>=0; --j)
      output.push_back('0' + values[j]);
  }

  template<typename String>
  inline void format_value(String& output, const char* value) {
    output += value;
  }

  template<typename String>
  inline void format_value(String& output, String& value) {
    output += value;
  }

} // namespace format_details

template<typename StringType = std::string,
         typename FormatType,
         typename ... Args>
StringType format(FormatType& fmt, Args&& ... args) {
  typedef std::tuple<Args...> Tuple;

  StringType output;
  bool insideRef = false;
  int refNumber;
  Tuple tuple(std::forward<Args>(args)...);

  for (auto chr : fmt) {
    if (!chr)
      break;

    switch (chr) {
      case '{':
        insideRef = true;
        refNumber = 0;
        break;

      case '}':
        {
          if (refNumber >= 0 && refNumber < std::tuple_size<Tuple>::value) {
            format_details::variadic_switch(
              std::forward<Tuple>(tuple), refNumber,
              [&output](auto&& x){
                format_details::format_value(output, x);
              });
          }
          else {
            // refNumber is out of range
            throw std::logic_error("A {n} item in format is out of range");
          }
        }
        insideRef = false;
        break;

      default:
        if (insideRef) {
          if (chr >= '0' && chr <= '9') {
            refNumber = (refNumber * 10) + (chr - '0');
          }
        }
        else {
          output.push_back(chr);
        }
        break;
    }
  }

  if (insideRef)
    throw std::logic_error("Last {n} item was not closed");

  return output;
}

#endif
