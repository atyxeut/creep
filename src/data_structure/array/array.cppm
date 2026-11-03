// SPDX-FileCopyrightText: 2026 atyxeut <atyxeut@outlook.com>
// SPDX-License-Identifier: LGPL-3.0-or-later

export module mtea:data_structure.array;

import :memory;
import :meta;
import std;

// multidimensional std::array
namespace mtea {

template <typename, std::size_t...>
struct array_impl;

template <typename T, std::size_t Dim>
struct array_impl<T, Dim> {
  using type = std::array<T, Dim>;
};

template <typename T, std::size_t Dim, std::size_t... Dims>
struct array_impl<T, Dim, Dims...> : array_impl<typename array_impl<T, Dims...>::type, Dim> {};

} // namespace mtea

export namespace mtea {

// mtea::array<int, 3, 5, 2> arr3d {};
// same as: std::array<std::array<std::array<int, 2>, 5>, 3> arr3d {};
template <typename T, std::size_t... Dims>
using array = array_impl<T, Dims...>::type;

template <typename>
struct is_no_cv_std_array : std::false_type {};

template <typename T, std::size_t N>
struct is_no_cv_std_array<std::array<T, N>> : std::true_type {};

template <typename T>
inline constexpr bool is_no_cv_std_array_v = is_no_cv_std_array<T>::value;

template <typename T>
using is_std_array = is_no_cv_std_array<std::remove_cv_t<T>>;

template <typename T>
inline constexpr bool is_std_array_v = is_std_array<T>::value;

// sets every element of a mtea::array to val
// mtea::array<int, 3, 5, 2, 10> arr4d;
// int val = -1;
// mtea::fill_array(arr4d, val);
template <typename Elem, std::size_t Dim, typename T>
constexpr void fill_array(std::array<Elem, Dim>& arr, const T& val) {
  if constexpr (is_no_cv_std_array_v<Elem>)
    for (auto& inner_arr : arr)
      fill_array(inner_arr, val);
  else
    arr.fill(static_cast<Elem>(val));
}

// same as above, for C-style arrays
template <typename Elem, std::size_t N, typename T>
constexpr void fill_array(Elem (&arr)[N], const T& val) {
  if constexpr (std::is_array_v<Elem>)
    for (auto& inner_arr : arr)
      fill_array(inner_arr, val);
  else
    std::fill(arr, arr + N, static_cast<Elem>(val));
}

// auto arr4d = mtea::make_array<int, 5, 8, 3, 2>(val);
// combines `mtea::array<int, 5, 8, 3, 2> arr4d` and `mtea::fill_array(arr4d, val)`
template <typename Elem, std::size_t... Dims, typename T>
[[nodiscard]] constexpr array<Elem, Dims...> make_array(const T& val) {
  array<Elem, Dims...> arr;
  fill_array(arr, static_cast<Elem>(val));
  return arr;
}

} // export namespace mtea

// multidimensional std::vector
namespace mtea {

template <typename, std::size_t DimCnt, typename>
  requires (DimCnt != 0)
class vector_impl;

template <typename T, std::size_t DimCnt, typename FirstDimAllocator, typename... Allocators>
class vector_impl<T, DimCnt, meta::type_list<FirstDimAllocator, Allocators...>> {
private:
  using allocator_list_ = meta::type_list<FirstDimAllocator, Allocators...>;
  using adjusted_allocator_list_ = meta::snoc_if_t<allocator_list_, std_allocator_tag, (1 + sizeof...(Allocators) < DimCnt)>;
  using element_type_ = vector_impl<T, DimCnt - 1, meta::init_t<adjusted_allocator_list_>>::type;

public:
  using type = std::vector<element_type_, eval_allocator_t<meta::last_t<adjusted_allocator_list_>, element_type_>>;
};

template <typename T, typename Allocator>
class vector_impl<T, 1, meta::type_list<Allocator>> {
public:
  using type = std::vector<T, eval_allocator_t<Allocator, T>>;
};

} // namespace mtea

export namespace mtea {

// mtea::vector<int> vec1d;
// same as: std::vector<int> vec1d;
//
// mtea::vector<int, 4> vec4d;
// same as: std::vector<std::vector<std::vector<std::vector<int>>>> vec4d;
template <typename T, std::size_t DimCnt = 1, typename FirstDimAllocator = std_allocator_tag, typename... Allocators>
  requires (sizeof...(Allocators) < DimCnt)
using vector = vector_impl<T, DimCnt, meta::type_list<FirstDimAllocator, Allocators...>>::type;

} // export namespace mtea

namespace mtea {

template <typename Elem, typename AllocatorList, typename Dim, typename... Ts>
[[nodiscard]] constexpr auto make_vector_impl(Dim first_dim_size, Ts&&... args) {
  using adjusted_allocator_list = meta::snoc_if_t<AllocatorList, std_allocator_tag, (meta::length_v<AllocatorList> < sizeof...(Ts))>;
  if constexpr (sizeof...(Ts) == 1) {
    using allocator_type = eval_allocator_t<meta::last_t<adjusted_allocator_list>, Elem>;
    return vector<Elem, 1, allocator_type>(static_cast<std::size_t>(first_dim_size), static_cast<Elem>(args)...);
  } else {
    using inner_allocator_list = meta::init_t<adjusted_allocator_list>;
    using element_type = vector_impl<Elem, sizeof...(Ts) - 1, inner_allocator_list>::type;
    using allocator_type = eval_allocator_t<meta::last_t<adjusted_allocator_list>, element_type>;
    return vector<element_type, 1, allocator_type>(
      static_cast<std::size_t>(first_dim_size), make_vector_impl<Elem, inner_allocator_list>(std::forward<Ts>(args)...)
    );
  }
}

} // namespace mtea

export namespace mtea {

// auto vec3d = mtea::make_vector<int>(x, y, z, 1);
// same as: auto vec3d = std::vector<std::vector<std::vector<int>>>(
//                         x,
//                         std::vector<std::vector<int>>(y, std::vector<int>(z, 1))
//                       );
//
// constexpr usage example (error if mtea::make_vector is not constexpr):
// std::cout << [](const std::vector<int>& vec, int sum = 0) consteval {
//   std::ranges::for_each(vec, [&sum](int elem) { return sum += elem; });
//   return sum;
// }(mtea::make_vector<int>(10, -1)) << "\n";
template <typename Elem, typename FirstDimAllocator = std_allocator_tag, typename... Allocators, std::integral Dim, typename... Ts>
  requires (sizeof(Dim) <= sizeof(std::size_t) && sizeof...(Ts) > 0 && sizeof...(Allocators) < sizeof...(Ts))
[[nodiscard]] constexpr auto make_vector(Dim first_dim_size, Ts&&... args) {
  return make_vector_impl<Elem, meta::type_list<FirstDimAllocator, Allocators...>>(first_dim_size, std::forward<Ts>(args)...);
}

} // export namespace mtea
