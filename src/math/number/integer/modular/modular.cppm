// SPDX-FileCopyrightText: 2026 atyxeut <atyxeut@outlook.com>
// SPDX-License-Identifier: LGPL-3.0-or-later

export module creep:math.number.integer.modular;

import :math.number.integer.bit;
import std;

export namespace creep::meta {

// unsigned integers implicitly modulo 2^(Bits)
template <typename T>
struct is_no_cv_modular_integral : std::bool_constant<std::same_as<T, std::remove_cv_t<T>> && unsigned_integral<T>> {};

template <typename T>
inline constexpr bool is_no_cv_modular_integral_v = is_no_cv_modular_integral<T>::value;

template <typename T>
using is_modular_integral = is_no_cv_modular_integral<std::remove_cv_t<T>>;

template <typename T>
inline constexpr bool is_modular_integral_v = is_modular_integral<T>::value;

template <typename T>
concept modular_integral = is_modular_integral_v<T>;

} // export namespace creep::meta
