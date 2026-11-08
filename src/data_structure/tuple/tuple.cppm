// SPDX-FileCopyrightText: 2026 atyxeut <atyxeut@outlook.com>
// SPDX-License-Identifier: LGPL-3.0-or-later

export module creep:data_structure.tuple;

import std;

export namespace creep {

template <typename T1, typename T2, std::convertible_to<std::string> Delim = std::string>
void print(std::ostream& ostr, const std::pair<T1, T2>& p, Delim&& delim = std::string(1, ' '), bool new_line = false) {
  ostr << p.first << std::forward<Delim>(delim) << p.second;

  if (new_line)
    ostr << '\n';
}

} // export namespace creep

export namespace creep::inline operator_overload::inline io_operator {

template <typename T1, typename T2>
auto& operator <<(std::ostream& ostr, const std::pair<T1, T2>& p) {
  ::creep::print(ostr, p);
  return ostr;
}

} // export namespace creep::inline operator_overload::inline io_operator

export namespace creep {

template <typename... Ts, std::convertible_to<std::string> Delim = std::string>
void print(std::ostream& ostr, const std::tuple<Ts...>& t, Delim&& delim = std::string(1, ' '), bool new_line = false) {
  [&]<std::size_t... Is>(std::index_sequence<Is...>) {
    ((ostr << std::get<Is>(t) << (Is + 1 == sizeof...(Ts) ? std::string {} : delim)), ...);
  }(std::index_sequence_for<Ts...> {});

  if (new_line)
    ostr << '\n';
}

} // export namespace creep

export namespace creep::inline operator_overload::inline io_operator {

template <typename... Ts>
auto& operator <<(std::ostream& ostr, const std::tuple<Ts...>& t) {
  ::creep::print(ostr, t);
  return ostr;
}

} // export namespace creep::inline operator_overload::inline io_operator
