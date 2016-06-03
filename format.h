// Copyright (C) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifndef FORMAT_H_INCLUDED
#define FORMAT_H_INCLUDED

#include <sstream>
#include <stdexcept>

namespace format_details {

  // variadic_switch()

  template<typename Pred,
           typename Arg>
  void variadic_switch_0(const std::size_t index,
                         Pred&& pred,
                         Arg&& arg) {
    if (index == 0)
      pred(std::forward<Arg>(arg));
  }

  template<typename Pred,
           typename Arg,
           typename ... Args>
  void variadic_switch_0(const std::size_t index,
                         Pred&& pred,
                         Arg&& arg,
                         Args&& ... args) {
    if (index == 0) {
      pred(std::forward<Arg>(arg));
    }
    else {
      variadic_switch_0(index-1,
                        std::forward<Pred>(pred),
                        std::forward<Args>(args)...);
    }
  }

  template<typename Pred, typename ... Args>
  inline void variadic_switch(const int index,
                              Pred&& pred,
                              Args&& ... args) {
    variadic_switch_0(index,
                      std::forward<Pred>(pred),
                      std::forward<Args>(args)...);
  }

  // format_value()

  template<typename String, typename T>
  inline typename std::enable_if<!std::is_integral<T>::value, void>::type
  format_value(String& output, const T& value) {
    std::ostringstream s;
    s << value;
    output += s.str();
  }

  // Fast integer number -> string
  template<typename String, typename T>
  inline typename std::enable_if<std::is_unsigned<T>::value, void>::type
  format_value(String& output, const T& value) {
    if (value == 0) {
      output.push_back('0');
      return;
    }

    // long long max value is "18446744073709551615" (20 chars)
    int i = 0, values[20];
    T v = value;

    while (true) {
      if (i == sizeof(values)/sizeof(values[0]))
        throw std::logic_error("Too many decimals");

      values[i++] = (v % 10);
      if (v < 10)
        break;

      v /= 10;
    }

    for (int j=i-1; j>=0; --j)
      output.push_back('0' + values[j]);
  }

  // Fast integer number -> string
  template<typename String, typename T>
  inline typename std::enable_if<std::is_signed<T>::value, void>::type
  format_value(String& output, const T& value) {
    if (value == 0) {
      output.push_back('0');
      return;
    }

    int i = 0, values[20];
    T v = value;

    if (v < 0) {
      output.push_back('-');
      values[i++] = -(v % 10);

      // We have to change 'v' sign after dividing by 10 in case that
      // v == numeric_limits<T>::min() (because the absolute value of
      // min() is greater than max() value).
      v /= 10;
      v = -v;
    }

    while (true) {
      if (i == sizeof(values)/sizeof(values[0]))
        throw std::logic_error("Too many decimals");

      values[i++] = (v % 10);
      if (v < 10)
        break;

      v /= 10;
    }

    for (int j=i-1; j>=0; --j)
      output.push_back('0' + values[j]);
  }

  template<typename String>
  inline void format_value(String& output, const char* value) {
    output += value;
  }

  template<typename String>
  inline void format_value(String& output, const String& value) {
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
  std::size_t refNumber;

  for (auto chr : fmt) {
    if (!chr)
      break;

    switch (chr) {
      case '{':
        insideRef = true;
        refNumber = 0;
        break;

      case '}':
        if (refNumber < sizeof...(Args)) {
          format_details::variadic_switch(
            refNumber,
            [&output](auto&& x){
              format_details::format_value(output, x);
            },
            std::forward<Args>(args)...);
        }
        else {
          // refNumber is out of range
          throw std::logic_error("A {n} item in format is out of range");
        }
        insideRef = false;
        break;

      default:
        if (insideRef) {
          if (chr >= '0' && chr <= '9') {
            refNumber *= 10;
            refNumber += (chr - '0');
          }
          else
            throw std::logic_error("Invalid character inside {n} item");
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
