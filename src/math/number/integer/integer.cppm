// SPDX-FileCopyrightText: 2026 atyxeut <atyxeut@outlook.com>
// SPDX-License-Identifier: LGPL-3.0-or-later

module;

// https://stackoverflow.com/a/76440171
#if _MSC_VER >= 1934
  #include <__msvc_int128.hpp>
#endif

#include <macro/wconversion_flag.hpp>

export module creep:math.number.integer;

import :data_structure.string.character;
import :io.format_flag;
import :meta.cvref;
import :meta.type_list;
import std;

// clang-format off

export namespace creep {

using i8  = std::int8_t;
using u8  = std::uint8_t;
using i16 = std::int16_t;
using u16 = std::uint16_t;
using i32 = std::int32_t;
using u32 = std::uint32_t;
using i64 = std::int64_t;
using u64 = std::uint64_t;

using idx32 = i32;
using idx64 = i64;

using isize = std::ptrdiff_t;
using usize = std::size_t;

} // export namespace creep

// clang-format on

export namespace creep::meta {

template <typename T>
struct is_no_cv_boolean : std::bool_constant<std::same_as<T, bool>> {};

template <typename T>
inline constexpr bool is_no_cv_boolean_v = is_no_cv_boolean<T>::value;

template <typename T>
using is_boolean = is_no_cv_boolean<std::remove_cv_t<T>>;

template <typename T>
inline constexpr bool is_boolean_v = is_boolean<T>::value;

template <typename T>
concept boolean = is_boolean_v<std::remove_cv_t<T>>;

template <typename T>
concept nonbool_standard_unsigned_integral = std::unsigned_integral<T> && !boolean<T>;

template <typename T>
concept nonbool_standard_integral = std::integral<T> && !boolean<T>;

template <typename T>
concept index_integral = std::same_as<T, idx32> || std::same_as<T, idx64> || std::same_as<T, isize>;

template <typename T>
concept standard_size_integral = std::same_as<T, usize>;

template <typename T>
concept size_integral = standard_size_integral<T> || std::same_as<T, u32>;

} // export namespace creep::meta

export namespace creep::meta {

template <typename T>
concept state_integral = is_any_of_v<T, i32, u32, i64, u64>;

} // export namespace creep::meta

namespace creep {

// for 32-bit integers:
//   00000000 ... 00000000
//   31 ...       7 ...  0
//
// for 64-bit integers:
//   00000000 ... 00000000
//   63 ...       7 ...  0

template <meta::state_integral T>
[[nodiscard]] constexpr bool is_valid_bit_index_for_state(int index) noexcept {
  if (index < 0)
    return false;
  if constexpr (meta::is_any_of_v<T, i32, u32>)
    return index < 32;
  if constexpr (meta::is_any_of_v<T, i64, u64>)
    return index < 64;
}

} // namespace creep

export namespace creep {

template <meta::state_integral T>
[[nodiscard]] constexpr bool is_bit_set(T state, int index) noexcept pre(is_valid_bit_index_for_state<T>(index)) {
  return static_cast<bool>(state & std::make_unsigned_t<T>(1) << index);
}

template <meta::state_integral T>
constexpr void set_bit(T& state, int index) noexcept pre(is_valid_bit_index_for_state<T>(index)) {
  state |= std::make_unsigned_t<T>(1) << index;
}

template <meta::state_integral T>
[[nodiscard]] constexpr T after_set_bit(T state, int index) noexcept pre(is_valid_bit_index_for_state<T>(index)) {
  return state | std::make_unsigned_t<T>(1) << index;
}

template <meta::state_integral T>
constexpr void reset_bit(T& state, int index) noexcept pre(is_valid_bit_index_for_state<T>(index)) {
  state &= ~(std::make_unsigned_t<T>(1) << index);
}

template <meta::state_integral T>
[[nodiscard]] constexpr T after_reset_bit(T state, int index) noexcept pre(is_valid_bit_index_for_state<T>(index)) {
  return state & ~(std::make_unsigned_t<T>(1) << index);
}

template <meta::state_integral T>
constexpr void flip_bit(T& state, int index) noexcept pre(is_valid_bit_index_for_state<T>(index)) {
  state ^= std::make_unsigned_t<T>(1) << index;
}

template <meta::state_integral T>
[[nodiscard]] constexpr T after_flip_bit(T state, int index) noexcept pre(is_valid_bit_index_for_state<T>(index)) {
  return state ^ std::make_unsigned_t<T>(1) << index;
}

} // export namespace creep

namespace creep::integer {

#if defined(__SIZEOF_INT128__) || _MSC_VER >= 1934
constexpr std::size_t min_precision_bits = 256;
#else
constexpr std::size_t min_precision_bits = 128;
#endif

} // namespace creep::integer

// forward declarations
export namespace creep::integer {

template <std::size_t Bits>
  requires (Bits >= min_precision_bits && std::has_single_bit(Bits))
class i;

template <std::size_t Bits>
  requires (Bits >= min_precision_bits && std::has_single_bit(Bits))
class u;

} // export namespace creep::integer

// clang-format off

#if defined(__SIZEOF_INT128__) || _MSC_VER >= 1934
  export namespace creep {

  #ifdef __SIZEOF_INT128__
    // https://gcc.gnu.org/onlinedocs/gcc/_005f_005fint128.html
    __extension__ using i128 = __int128;
    __extension__ using u128 = unsigned __int128;
  #else
    using i128 = std::_Signed128;
    using u128 = std::_Unsigned128;
  #endif

  } // export namespace creep

  export namespace creep::inline operator_overload::inline io_operator {

  auto& operator >>(std::istream& istr, u128& n)
  {
    std::string buffer;
    istr >> buffer;

    n = 0;
    for (char ch : buffer)
      n = n * 10 + static_cast<u128>(ch - '0');

    return istr;
  }

  auto& operator >>(std::istream& istr, i128& n)
  {
    std::string buffer;
    istr >> buffer;

    const int sgn = buffer[0] == '-' ? -1 : 1;

    u128 mag = 0;
    for (usize i = sgn < 0; i < buffer.size(); ++i)
      mag = mag * 10 + static_cast<u128>(buffer[i] - '0');

    if (mag > std::numeric_limits<i128>::max() || sgn > 0)
      n = static_cast<i128>(mag);
    else
      n = -static_cast<i128>(mag);

    return istr;
  }

  auto& operator <<(std::ostream& ostr, u128 n)
  {
    if (n == 0)
      return ostr << '0';

    std::string buffer;
    for (; n; n /= 10)
      buffer += static_cast<char>(n % 10 + '0');
    std::reverse(buffer.begin(), buffer.end());
    return ostr << buffer;
  }

  auto& operator <<(std::ostream& ostr, i128 n)
  {
    if (n == std::numeric_limits<i128>::min())
      ostr << '-' << static_cast<u128>(n);
    else if (n < 0)
      ostr << '-' << static_cast<u128>(-n);
    else
      ostr << static_cast<u128>(n);
    return ostr;
  }

  } // export namespace creep::inline operator_overload::inline io_operator
#else
  export namespace creep {

    using i128 = ::creep::integer::i<128>;
    using u128 = ::creep::integer::u<128>;

  } // export namespace creep
#endif

// clang-format on

export namespace creep::meta {

// whether std::integral<T> is true for T = i128/u128 depends on the compiler
template <typename T>
concept potential_standard_integral =
  std::integral<T> || std::same_as<std::remove_cv_t<T>, i128> || std::same_as<std::remove_cv_t<T>, u128>;

template <typename T>
concept nonbool_potential_standard_integral = potential_standard_integral<T> && !boolean<T>;

template <typename>
struct is_no_cv_custom_twos_complement_signed_integral : std::false_type {};

template <usize Bits>
struct is_no_cv_custom_twos_complement_signed_integral<integer::i<Bits>> : std::true_type {};

template <typename T>
inline constexpr bool is_no_cv_custom_twos_complement_signed_integral_v = is_no_cv_custom_twos_complement_signed_integral<T>::value;

template <typename T>
using is_custom_twos_complement_signed_integral = is_no_cv_custom_twos_complement_signed_integral<std::remove_cv_t<T>>;

template <typename T>
inline constexpr bool is_custom_twos_complement_signed_integral_v = is_custom_twos_complement_signed_integral<T>::value;

template <typename T>
concept twos_complement_signed_integral =
  std::signed_integral<T> || std::same_as<std::remove_cv_t<T>, i128> || is_custom_twos_complement_signed_integral_v<T>;

template <typename>
struct is_no_cv_custom_unsigned_integral : std::false_type {};

template <usize Bits>
struct is_no_cv_custom_unsigned_integral<integer::u<Bits>> : std::true_type {};

template <typename T>
inline constexpr bool is_no_cv_custom_unsigned_integral_v = is_no_cv_custom_unsigned_integral<T>::value;

template <typename T>
using is_custom_unsigned_integral = is_no_cv_custom_unsigned_integral<std::remove_cv_t<T>>;

template <typename T>
inline constexpr bool is_custom_unsigned_integral_v = is_custom_unsigned_integral<T>::value;

template <typename T>
concept custom_twos_complement_signed_integral = is_custom_twos_complement_signed_integral_v<T>;

template <typename T>
concept unsigned_integral = std::unsigned_integral<T> || std::same_as<std::remove_cv_t<T>, u128> || is_custom_unsigned_integral_v<T>;

template <typename T>
concept nonbool_unsigned_integral = unsigned_integral<T> && !boolean<T>;

template <typename T>
concept fixed_precision_integral = twos_complement_signed_integral<T> || unsigned_integral<T>;

template <typename T>
concept nonbool_fixed_precision_integral = fixed_precision_integral<T> && !boolean<T>;

template <typename T>
concept nothrow_integral = fixed_precision_integral<T>;

} // export namespace creep::meta

namespace creep::meta {

template <typename T, typename = std::remove_cv_t<T>>
struct make_signed_selector : std::make_signed<T> {};

template <typename T>
struct make_signed_selector<T, i128> : claim_cv<T, i128> {};

template <typename T>
struct make_signed_selector<T, u128> : claim_cv<T, i128> {};

template <typename T, usize Bits>
struct make_signed_selector<T, integer::i<Bits>> : claim_cv<T, integer::i<Bits>> {};

template <typename T, usize Bits>
struct make_signed_selector<T, integer::u<Bits>> : claim_cv<T, integer::i<Bits>> {};

} // namespace creep::meta

export namespace creep::meta {

template <fixed_precision_integral T>
using make_signed = make_signed_selector<T>;

template <fixed_precision_integral T>
using make_signed_t = make_signed<T>::type;

} // export namespace creep::meta

namespace creep::meta {

template <typename T, typename = std::remove_cv_t<T>>
struct make_unsigned_selector : std::make_unsigned<T> {};

template <typename T>
struct make_unsigned_selector<T, i128> : claim_cv<T, u128> {};

template <typename T>
struct make_unsigned_selector<T, u128> : claim_cv<T, u128> {};

template <typename T, usize Bits>
struct make_unsigned_selector<T, integer::i<Bits>> : claim_cv<T, integer::u<Bits>> {};

template <typename T, usize Bits>
struct make_unsigned_selector<T, integer::u<Bits>> : claim_cv<T, integer::u<Bits>> {};

} // namespace creep::meta

export namespace creep::meta {

template <fixed_precision_integral T>
using make_unsigned = make_unsigned_selector<T>;

template <fixed_precision_integral T>
using make_unsigned_t = make_unsigned<T>::type;

} // export namespace creep::meta

export namespace creep {

// an extended version of std::abs
// no compilation error: unsigned int _ = abs(1u);
// no overflow: unsigned int _ = abs(-2147483647 - 1);
template <meta::nonbool_potential_standard_integral T>
[[nodiscard]] constexpr auto abs(T x) noexcept {
  // for negative x, ~x + 1 is |x| if the bit representation is 2's complement
  return x >= 0 ? static_cast<meta::make_unsigned_t<T>>(x) : ~static_cast<meta::make_unsigned_t<T>>(x) + 1;
}

// an extended version of std::countl_zero
template <meta::nonbool_potential_standard_integral T>
[[nodiscard]] constexpr int countl_zero(T x) noexcept {
  if (x < 0)
    return 0;

  if constexpr (meta::nonbool_standard_integral<T>)
    return std::countl_zero<std::make_unsigned_t<T>>(x);
  else
    return x >> 64 ? std::countl_zero<u64>(x >> 64) : 64 + std::countl_zero<u64>(x);
}

// an extended version of std::countr_zero
template <meta::nonbool_potential_standard_integral T>
[[nodiscard]] constexpr int countr_zero(T x) noexcept {
  // -x has the same lowbit as x in 2's complement
  if constexpr (meta::nonbool_standard_integral<T>)
    return std::countr_zero<std::make_unsigned_t<T>>(x);
  else
    return static_cast<u64>(x) ? std::countr_zero<u64>(x) : 64 + std::countr_zero<u64>(x >> 64);
}

// an extended version of std::bit_width
template <meta::nonbool_potential_standard_integral T>
[[nodiscard]] constexpr int bit_width(T x) noexcept pre(x >= 0) {
  return std::numeric_limits<meta::make_unsigned_t<T>>::digits - countl_zero(x);
}

// calculate floor(log_2 x), ilog2(0) has defined behavior and returns -1
template <meta::nonbool_potential_standard_integral T>
[[nodiscard]] constexpr int ilog2(T x) noexcept pre(x >= 0) {
  return bit_width(x) - 1;
}

template <std::unsigned_integral T, std::ranges::random_access_range R>
[[nodiscard]] std::string to_string(T n, R&& digits) pre(std::ranges::size(digits) >= 2) {
  if (n == 0)
    return std::string(1, digits[0]);

  std::string ans;
  for (const auto radix = std::ranges::size(digits); n != 0; n /= radix)
    ans += digits[n % radix];
  std::ranges::reverse(ans);
  return ans;
}

} // export namespace creep

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

export namespace creep::meta {

template <typename>
struct is_no_cv_arbitrary_precision_integral : std::false_type {};

template <typename T>
inline constexpr bool is_no_cv_arbitrary_precision_integral_v = is_no_cv_arbitrary_precision_integral<T>::value;

template <typename T>
using is_arbitrary_precision_integral = is_no_cv_arbitrary_precision_integral<std::remove_cv_t<T>>;

template <typename T>
inline constexpr bool is_arbitrary_precision_integral_v = is_arbitrary_precision_integral<T>::value;

template <typename T>
concept arbitrary_precision_integral = is_arbitrary_precision_integral_v<T>;

template <typename T>
concept signed_integral = twos_complement_signed_integral<T> || arbitrary_precision_integral<T>;

template <typename T>
concept integral = fixed_precision_integral<T> || arbitrary_precision_integral<T>;

template <typename T>
concept nonbool_integral = integral<T> && !boolean<T>;

} // export namespace creep::meta

export namespace creep::big_integer::naive {

// basic utils for unsigned operands in radix 10^k

template <std::ranges::bidirectional_range R>
void print(R&& number, iofmt format = iofmt::none) {
  for (const auto limb : number | std::views::reverse)
    std::print("{}", limb);
  if (format == iofmt::endl)
    std::println();
}

// s must be an unsigned integer, otherwise the behavior is undefined
template <std::integral Limb = int>
[[nodiscard]] constexpr std::vector<Limb> parse(std::string_view s) {
  const auto n = s.size();
  if (n == 1 && s[0] == '0')
    return {};

  std::vector<Limb> res(n);
  for (auto i = 0uz; i < n; ++i)
    res[i] = s[n - i - 1] - '0';
  return res;
}

template <std::integral Limb = int>
[[nodiscard]] std::vector<Limb> input() {
  std::string s;
  std::cin >> s;
  return parse<Limb>(s);
}

template <std::integral Limb>
[[nodiscard]] constexpr bool is_zero(const std::vector<Limb>& number) noexcept {
  return number.empty();
}

// -1: a < b, 0: a = b, 1: a > b
template <std::integral Limb>
[[nodiscard]] constexpr int compare(std::span<const Limb> a, std::span<const Limb> b) noexcept {
  const auto al = a.size();
  const auto bl = b.size();
  if (al != bl)
    return (al > bl) - (al < bl);
  for (auto i = al; i > 0;) {
    --i;
    if (a[i] != b[i])
      return (a[i] > b[i]) - (a[i] < b[i]);
  }
  return 0;
}

} // export namespace creep::big_integer::naive

namespace creep::big_integer::naive {

enum class carry_policy { none, assume_unsigned };

template <carry_policy Policy, typename Limb, Limb Radix, typename Range>
constexpr Limb carry_impl(Range&& number) noexcept {
  Limb carry = 0;
  for (auto& limb : number) {
    limb += carry;
    carry = limb / Radix;
    limb %= Radix;
    if (Policy == carry_policy::none && limb < 0) {
      --carry;
      limb += Radix;
    }
  }
  return carry;
}

} // namespace creep::big_integer::naive

export namespace creep::big_integer::naive {

template <std::integral Limb, Limb Radix = 10, std::ranges::forward_range R>
constexpr auto carry_unsigned(R&& number) noexcept {
  return carry_impl<carry_policy::assume_unsigned, Limb, Radix>(std::forward<R>(number));
}

template <std::integral Limb, Limb Radix = 10, std::ranges::forward_range R>
constexpr auto carry(R&& number) noexcept {
  return carry_impl<carry_policy::none, Limb, Radix>(std::forward<R>(number));
}

template <std::integral Limb>
constexpr void remove_lz(std::vector<Limb>& number) noexcept {
  while (!number.empty() && number.back() == 0)
    number.pop_back();
}

template <std::integral Limb>
[[nodiscard]] constexpr std::vector<Limb> add(const std::vector<Limb>& a, const std::vector<Limb>& b) {
  if (is_zero(a))
    return b;
  if (is_zero(b))
    return a;

  std::vector<Limb> ans(std::max(a.size(), b.size()) + 1);
  for (auto i = 0uz; i < a.size(); ++i)
    ans[i] += a[i];
  for (auto i = 0uz; i < b.size(); ++i)
    ans[i] += b[i];
  carry_unsigned<Limb>(ans);
  remove_lz(ans);
  return ans;
}

template <std::integral Limb>
struct sub_result {
  bool negative;
  std::vector<Limb> mag;
};

template <std::integral Limb>
void print(const sub_result<Limb>& result, iofmt format = iofmt::none) {
  if (result.negative)
    std::print("-");
  print(result.mag, format);
}

template <std::integral Limb>
[[nodiscard]] constexpr sub_result<Limb> sub(const std::vector<Limb>& a, const std::vector<Limb>& b) {
  if (is_zero(a))
    return {true, b};
  if (is_zero(b))
    return {false, a};

  std::vector<Limb> ans(std::max(a.size(), b.size()));
  const int sgn = compare<int>(a, b);
  for (auto i = 0uz; i < a.size(); ++i)
    ans[i] += a[i] * sgn;
  for (auto i = 0uz; i < b.size(); ++i)
    ans[i] -= b[i] * sgn;
  carry<Limb>(ans);
  remove_lz(ans);
  return {sgn < 0, std::move(ans)};
}

template <std::integral Limb>
[[nodiscard]] constexpr std::vector<Limb> mul(const std::vector<Limb>& a, const std::vector<Limb>& b) {
  if (is_zero(a) || is_zero(b))
    return std::vector<Limb> {};

  // delayed carry is always safe here, ans[k] accumulates at most min(la, lb) additions, assume that every addition is ans[k] += 9 * 9,
  // it requires (2^31 - 1) / 81 > 1e7 additions to overflow, such inputs are far beyond the capability of this O(n^2) algorithm
  std::vector<Limb> ans(a.size() + b.size());

  for (auto i = 0uz; i < a.size(); ++i)
    for (auto j = 0uz; j < b.size(); ++j)
      ans[i + j] += a[i] * b[j];

  carry_unsigned<Limb>(ans);
  remove_lz(ans);
  return ans;
}

template <std::integral Limb, typename Remainder>
struct floor_div_result {
  std::vector<Limb> q;
  Remainder r;
};

CREEP_WCONVERSION_PUSH()

// used when b is way smaller than a
template <std::integral Limb>
[[nodiscard]] constexpr floor_div_result<Limb, long long> floor_div(const std::vector<Limb>& a, int b)
// ICE in debug mode
// pre(b != 0)
{
  std::vector<Limb> q(a.size());
  long long r = 0;
  for (auto i = q.size(); i > 0;) {
    --i;
    r = r * 10 + a[i];
    q[i] = r / b;
    r %= b;
  }
  remove_lz(q);
  return {std::move(q), r};
}

template <std::integral Limb>
[[nodiscard]] constexpr floor_div_result<Limb, std::vector<Limb>> floor_div(const std::vector<Limb>& a, const std::vector<Limb>& b) pre(
  !is_zero(b)
) {
  const int comp_result = compare<Limb>(a, b);
  if (comp_result < 0)
    return {{}, a};
  if (comp_result == 0)
    return {{1}, {}};

  std::vector<Limb> q(a.size() - b.size() + 1);
  std::vector<Limb> r(a);
  bool not_first_digit = false;
  for (auto i = q.size(); i > 0;) {
    --i;
    while ((not_first_digit && r[i + b.size()] != 0) || compare<Limb>(std::span(r.begin() + i, b.size()), b) >= 0) {
      ++q[i];
      for (auto j = 0uz; j < b.size(); ++j)
        r[i + j] -= b[j];
      carry<Limb>(std::span(r.begin() + i, b.size() + not_first_digit));
    }
    not_first_digit = true;
  }
  remove_lz(q);
  r.resize(b.size());
  remove_lz(r);
  return {std::move(q), std::move(r)};
}

CREEP_WCONVERSION_POP()

template <std::integral Limb>
struct div_result {
  std::vector<Limb> q_int;
  std::vector<Limb> q_frac; // big endian
};

template <std::integral Limb>
void print(const div_result<Limb>& result, iofmt format = iofmt::none) {
  print(result.q_int);
  if (!result.q_frac.empty())
    std::print(".");
  print(std::views::reverse(result.q_frac), format);
}

CREEP_WCONVERSION_PUSH()

// will calculate to precision + 1 decimal digits and round to the nearest
template <std::integral Limb>
[[nodiscard]] constexpr div_result<Limb> div(const std::vector<Limb>& a, int b, usize precision = 16) {
  auto [q_int, r] = floor_div(a, b);
  if (precision == 0) {
    if (r * 10 / b >= 5) {
      ++q_int[0];
      if (Limb carry = carry_unsigned<Limb>(q_int); carry > 0)
        q_int.resize(q_int.size() + 1, carry);
    }
    return {std::move(q_int), std::vector<Limb> {}};
  }

  ++precision;
  std::vector<Limb> q_frac(precision);
  for (auto i = 0uz; i < precision; ++i) {
    r *= 10;
    q_frac[i] = r / b;
    r %= b;
  }
  if (q_frac.back() >= 5) {
    ++q_frac[precision - 2];
    if (carry_unsigned<int>(std::views::reverse(std::span(q_frac.begin(), q_frac.begin() + precision - 1))) > 0) {
      ++q_int[0];
      if (Limb carry = carry_unsigned<Limb>(q_int); carry != 0)
        q_int.resize(q_int.size() + 1, carry);
    }
  }

  q_frac.pop_back();
  return {std::move(q_int), std::move(q_frac)};
}

CREEP_WCONVERSION_POP()

template <std::integral Limb>
[[nodiscard]] constexpr std::vector<Limb> pow(std::vector<Limb> a, int n) pre(n >= 0) {
  if (n == 0)
    return {1};

  std::vector<Limb> ans {1};
  while (true) {
    if (n & 1)
      ans = mul(ans, a);
    n >>= 1;
    if (n == 0)
      break;
    a = mul(a, a);
  }
  return ans;
}

CREEP_WCONVERSION_PUSH()

[[nodiscard]] constexpr std::vector<int> floor_root(const std::vector<int>& a, int n) pre(n > 0) {
  if (n == 1 || is_zero(a))
    return a;

  std::vector<int> l;
  std::vector<int> r(pow(std::vector<int> {0, 1}, a.size() / n + 1));
  while (compare<int>(add(l, std::vector<int> {1}), r) < 0) {
    std::vector<int> mid(floor_div(add(l, r), 2).q);
    if (compare<int>(pow(mid, n), a) <= 0)
      l = std::move(mid);
    else
      r = std::move(mid);
  }
  return l;
}

CREEP_WCONVERSION_POP()

[[nodiscard]] constexpr auto floor_sqrt(const std::vector<int>& a) {
  return floor_root(a, 2);
}

[[nodiscard]] constexpr auto floor_cbrt(const std::vector<int>& a) {
  return floor_root(a, 3);
}

} // export namespace creep::big_integer::naive

namespace creep::big_integer {

[[nodiscard]] constexpr auto preprocess_input_string(std::string_view s) {
  if (s.empty() || (s.size() == 1) & !is_ascii_digit(s[0]))
    throw std::invalid_argument("invalid integer: no digits");

  if (s.size() > 1 & s[0] != '-' & !is_ascii_digit(s[0]))
    throw std::invalid_argument("invalid integer: not a number");

  auto i = 0uz;
  for (; i + 1 < s.size(); ++i)
    if ('1' <= s[i] & s[i] <= '9')
      break;
  return s.substr(i);
}

template <typename Limb, Limb DigitsPerLimb>
[[nodiscard]] constexpr auto get_digit_limb_count(std::string_view s_after_preprocess) {
  return (s_after_preprocess.size() + DigitsPerLimb - 1) / DigitsPerLimb;
}

template <typename Limb, Limb DigitsPerLimb>
constexpr void set_digit_limbs(std::string_view s_after_preprocess, std::span<Limb> dest) {
  for (const char *l, *r = s_after_preprocess.data() + s_after_preprocess.size(); auto& limb : dest) {
    l = std::max(r - DigitsPerLimb, s_after_preprocess.data());
    if (const auto res = std::from_chars(l, r, limb); res.ptr != r)
      throw std::invalid_argument("invalid integer: not a number");
    r = l;
  }
}

template <typename Limb, Limb DigitsPerLimb>
void print_digit_limbs(std::ostream& ostr, std::span<const Limb> num) {
  auto i = num.size();
  ostr << num[--i]; // the first limb doesn't need padding zeros

  ostr << std::setfill('0');
  while (i > 0)
    ostr << std::setw(DigitsPerLimb) << num[--i];
  ostr << std::setfill(' ');
}

} // namespace creep::big_integer

export namespace creep::big_integer::naive {

class digit_storage {
public:
  using limb_type = i64;

  // (2^63 - 1) / (1e6 * 1e6) > 9e6
  static constexpr limb_type radix = 1e6;
  static constexpr limb_type digits_per_limb = 6;

private:
  bool negative_;
  std::vector<limb_type> mag_;

public:
  constexpr digit_storage() noexcept = default;

  explicit constexpr digit_storage(std::string_view s) {
    const auto ns = preprocess_input_string(s);
    negative_ = s[0] == '-';
    mag_.resize(get_digit_limb_count<limb_type, digits_per_limb>(ns));
    set_digit_limbs<limb_type, digits_per_limb>(ns, mag_);
  }

  friend auto& operator >>(std::istream& istr, digit_storage& n) {
    std::string s;
    istr >> s;
    n = digit_storage(s);
    return istr;
  }

  friend auto& operator <<(std::ostream& ostr, const digit_storage& n) {
    if (n.negative_)
      ostr << '-';
    print_digit_limbs<limb_type, digits_per_limb>(ostr, n.mag_);
    return ostr;
  }
};

} // export namespace creep::big_integer::naive

export namespace creep::meta {

template <>
struct is_no_cv_arbitrary_precision_integral<big_integer::naive::digit_storage> : std::true_type {};

} // export namespace creep::meta
