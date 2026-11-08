// SPDX-FileCopyrightText: 2026 atyxeut <atyxeut@outlook.com>
// SPDX-License-Identifier: LGPL-3.0-or-later

export module creep:data_structure.string.character;

import std;

export namespace creep {

enum class letter_case { uppercase, lowercase };

} // export namespace creep

namespace creep {

template <letter_case, typename>
inline constexpr char radix_digits_impl {};

template <letter_case LetterCase, std::size_t... Is>
inline constexpr char radix_digits_impl<LetterCase, std::index_sequence<Is...>>[sizeof...(Is)] {
  (Is < 10 ? Is + '0' : Is + (LetterCase == letter_case::uppercase ? 'A' : 'a') - 10)...
};

} // namespace creep

export namespace creep {

template <std::size_t N, letter_case LetterCase = letter_case::lowercase>
  requires (2 <= N && N <= 36)
inline constexpr auto& radix_digits = radix_digits_impl<LetterCase, std::make_index_sequence<N>>;

} // export namespace creep

export namespace creep {

[[nodiscard]] constexpr bool is_ascii_digit(char ch) noexcept {
  return '0' <= ch & ch <= '9';
}

} // export namespace creep
