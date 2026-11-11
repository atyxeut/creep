// SPDX-FileCopyrightText: 2026 atyxeut <atyxeut@outlook.com>
// SPDX-License-Identifier: LGPL-3.0-or-later

export module creep:meta.enum_flag;

import std;

export namespace creep::meta {

template <typename>
struct is_enum_flag : std::false_type {};

template <typename T>
inline constexpr bool is_enum_flag_v = is_enum_flag<T>::value;

template <typename T>
concept enum_flag = is_enum_flag_v<T>;

} // export namespace creep::meta

export namespace creep::inline operator_overload::inline enum_flag_operator {

template <::creep::meta::enum_flag T>
[[nodiscard]] constexpr
auto operator &(T a, T b) noexcept {
  return static_cast<T>(std::to_underlying(a) & std::to_underlying(b));
};

template <::creep::meta::enum_flag T>
constexpr
auto& operator &=(T& a, T b) noexcept {
  return a = a & b;
};

template <::creep::meta::enum_flag T>
[[nodiscard]] constexpr
auto operator ^(T a, T b) noexcept {
  return static_cast<T>(std::to_underlying(a) ^ std::to_underlying(b));
};

template <::creep::meta::enum_flag T>
constexpr
auto& operator ^=(T& a, T b) noexcept {
  return a = a ^ b;
};

template <::creep::meta::enum_flag T>
[[nodiscard]] constexpr
auto operator |(T a, T b) noexcept {
  return static_cast<T>(std::to_underlying(a) | std::to_underlying(b));
};

template <::creep::meta::enum_flag T>
constexpr
auto& operator |=(T& a, T b) noexcept {
  return a = a | b;
};

} // export namespace creep::inline operator_overload::inline enum_flag_operator
