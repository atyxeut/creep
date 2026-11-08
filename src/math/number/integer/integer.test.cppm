// SPDX-FileCopyrightText: 2026 atyxeut <atyxeut@outlook.com>
// SPDX-License-Identifier: GPL-3.0-or-later

export module creep:math.number.integer.test;

import :io;
import :math.number.integer;
import std;

using namespace creep;
using namespace creep::meta;

consteval void check_is_boolean() {
  static_assert(is_boolean_v<int> == false);
  static_assert(is_boolean_v<bool> == true);
  static_assert(is_boolean_v<const bool> == true);
  static_assert(is_boolean_v<volatile bool> == true);
  static_assert(is_boolean_v<const volatile bool> == true);

  static_assert(boolean<int> == false);
  static_assert(boolean<bool> == true);
  static_assert(boolean<const bool> == true);
  static_assert(boolean<volatile bool> == true);
  static_assert(boolean<const volatile bool> == true);
}

consteval void check_make_signed() {
  static_assert(std::same_as<make_signed_t<int>, int> == true);
  static_assert(std::same_as<make_signed_t<const int>, const int> == true);
  static_assert(std::same_as<make_signed_t<const volatile unsigned int>, const volatile int> == true);
  static_assert(std::same_as<make_signed_t<const u64>, const i64> == true);
  static_assert(std::same_as<make_signed_t<i128>, i128> == true);
  static_assert(std::same_as<make_signed_t<u128>, i128> == true);
  static_assert(std::same_as<make_signed_t<integer::u<1024>>, integer::i<1024>> == true);
}

consteval void check_make_unsigned() {
  static_assert(std::same_as<make_unsigned_t<int>, unsigned int> == true);
  static_assert(std::same_as<make_unsigned_t<const int>, const unsigned int> == true);
  static_assert(std::same_as<make_unsigned_t<const volatile unsigned int>, const volatile unsigned int> == true);
  static_assert(std::same_as<make_unsigned_t<const u64>, const u64> == true);
  static_assert(std::same_as<make_unsigned_t<i128>, u128> == true);
  static_assert(std::same_as<make_unsigned_t<u128>, u128> == true);
  static_assert(std::same_as<make_unsigned_t<integer::i<1024>>, integer::u<1024>> == true);
}

export {

void check_int128_input_with_std_istream() {
  i128 a, b;
  u128 c, d;

  std::cin >> a >> b >> c >> d;
  std::println("i128: {}\ni128: {}\nu128: {}\nu128: {}\n", a, b, c, d);
}

void check_int128_output_with_std_ostream_and_std_print() {
  i128 a = -30, b = -40;
  u128 c = 30, d = 40;

  std::cout << a + b << ' ' << c + d << '\n';
  std::println("{:05} {:05}\n", a + b, c + d);
}

} // export

using namespace creep::big_integer;

export {

void check_naive_div() {
  std::cout << "bigint / int:\n";

  std::cout << "group 1:\n";

  const auto a = naive::parse("13847378953985547858791232135235");
  const int b = 2'147'483'647;

  // 6448188312553677787699
  std::cout << "precision 0: ";
  naive::print(naive::div(a, b, 0), iofmt::endl);

  // 6448188312553677787699.2290480687418245
  std::cout << "precision 16 (default): ";
  naive::print(naive::div(a, b), iofmt::endl);

  // 6448188312553677787699.22904806874182450992140197657113986861479462525565
  std::cout << "precision 50: ";
  naive::print(naive::div(a, b, 50), iofmt::endl);

  // 6448188312553677787699.229048068741824509921401976571139868614794625255649269211874003155098298171115246681084506
  std::cout << "precision 90: ";
  naive::print(naive::div(a, b, 90), iofmt::endl);

  std::cout << "group 2:\n";

  const auto c = naive::parse("340282366920938463463374607431768211456");
  const int d = 1'073'741'824;

  // 316912650057057350374175801344
  std::cout << "precision 0: ";
  naive::print(naive::div(c, d, 0), iofmt::endl);

  // 316912650057057350374175801344.00000000000000000000000000000000000000000000000000
  std::cout << "precision 50: ";
  naive::print(naive::div(c, d, 50), iofmt::endl);
}

void check_naive_class_ctor_and_output_with_std_ostream() {
  // 0
  std::cout << naive::digit_storage("0") << '\n';
  // 0 with leading zeros (70 digits)
  std::cout << naive::digit_storage("0000000000000000000000000000000000000000000000000000000000000000000000") << '\n';
  // normal positive integer (36 digits)
  std::cout << naive::digit_storage("123456789123456789123456789123456789") << '\n';
  // normal negative integer (29 digits)
  std::cout << naive::digit_storage("-12345678912345678912345678900") << '\n';
  // positive integer with leading zeros (31 digits)
  std::cout << naive::digit_storage("0000000012345678912345678912345") << '\n';
  // negative integer with leading zeros (45 digits)
  std::cout << naive::digit_storage("-000000123456789123456789123456789123456789123") << '\n';
}

void check_naive_class_input_with_std_istream() {
  naive::digit_storage n;
  std::cin >> n;
  std::cout << n << '\n';
}

} // export
