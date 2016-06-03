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
          std::stringstream buf;
          if (refNumber >= 0 && refNumber < std::tuple_size<Tuple>::value) {
            format_details::variadic_switch(
              std::forward<Tuple>(tuple), refNumber,
              [&buf](auto&& x){ buf << x; });
          }
          else {
            // refNumber is out of range
            throw std::logic_error("A {n} item in format is out of range");
          }

          output += buf.str();
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
