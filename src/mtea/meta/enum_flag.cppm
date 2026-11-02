// SPDX-FileCopyrightText: 2026 atyxeut <atyxeut@outlook.com>
// SPDX-License-Identifier: LGPL-3.0-or-later

export module mtea.meta.enum_flag;

import std;

export namespace mtea::meta {

template <typename>
struct is_enum_flag : std::false_type {};

template <typename T>
inline constexpr bool is_enum_flag_v = is_enum_flag<T>::value;

template <typename T>
concept enum_flag = is_enum_flag_v<T>;

} // export namespace mtea::meta

export namespace mtea::inline operator_overload::inline enum_flag_operator {

template <::mtea::meta::enum_flag T>
[[nodiscard]] constexpr auto operator &(T a, T b) noexcept {
  return static_cast<T>(std::to_underlying(a) & std::to_underlying(b));
};

template <::mtea::meta::enum_flag T>
constexpr auto& operator &=(T& a, T b) noexcept {
  return a = a & b;
};

template <::mtea::meta::enum_flag T>
[[nodiscard]] constexpr auto operator ^(T a, T b) noexcept {
  return static_cast<T>(std::to_underlying(a) ^ std::to_underlying(b));
};

template <::mtea::meta::enum_flag T>
constexpr auto& operator ^=(T& a, T b) noexcept {
  return a = a ^ b;
};

template <::mtea::meta::enum_flag T>
[[nodiscard]] constexpr auto operator |(T a, T b) noexcept {
  return static_cast<T>(std::to_underlying(a) | std::to_underlying(b));
};

template <::mtea::meta::enum_flag T>
constexpr auto& operator |=(T& a, T b) noexcept {
  return a = a | b;
};

} // export namespace mtea::inline operator_overload::inline enum_flag_operator
