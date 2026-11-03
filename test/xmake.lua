-- SPDX-FileCopyrightText: 2026 atyxeut <atyxeut@outlook.com>
-- SPDX-License-Identifier: GPL-3.0-or-later

add_includedirs("../src")

set_optimize("none")
add_cxxflags("-fcontract-evaluation-semantic=enforce")

if is_mode("debug") then
  target("mtea.test")
    set_kind("moduleonly")
    add_files("../src/**.cppm", { public = true })
    remove_files("../src/mtea.cppm", "../src/**.benchmark.cppm")
  target_end()

  target("test.1d_sparse_table")
    set_kind("binary")
    add_deps("mtea.test")
    add_files("data_structure/1d_sparse_table.cpp")
  target_end()

  target("test.kmp_search")
    set_kind("binary")
    add_deps("mtea.test")
    add_files("data_structure/kmp_search.cpp")
  target_end()

  target("test.make_vector")
    set_kind("binary")
    add_deps("mtea.test")
    add_files("data_structure/make_vector.cpp")
  target_end()

  target("test.naive_bigint_div")
    set_kind("binary")
    add_deps("mtea.test")
    add_files("math/naive_bigint_div.cpp")
  target_end()

  target("test.std_pair_output")
    set_kind("binary")
    add_deps("mtea.test")
    add_files("output/std_pair.cpp")
  target_end()

  target("test.std_tuple_output")
    set_kind("binary")
    add_deps("mtea.test")
    add_files("output/std_tuple.cpp")
  target_end()

  target("test.ranges_output")
    set_kind("binary")
    add_deps("mtea.test")
    add_files("output/ranges.cpp")
  target_end()

  target("test.int128_input")
    set_kind("binary")
    add_deps("mtea.test")
    add_files("input/int128.cpp")
  target_end()

  target("test.naive_bigint_input")
    set_kind("binary")
    add_deps("mtea.test")
    add_files("input/naive_bigint.cpp")
  target_end()

  target("test.int128_output")
    set_kind("binary")
    add_deps("mtea.test")
    add_files("output/int128.cpp")
  target_end()

  target("test.naive_bigint_output")
    set_kind("binary")
    add_deps("mtea.test")
    add_files("output/naive_bigint.cpp")
  target_end()

  target("test.first_10000_humble_numbers_output")
    set_kind("binary")
    add_deps("mtea.test")
    add_files("output/first_10000_humble_numbers.cpp")
  target_end()

  target("test.internal_sorting_algorithms")
    set_kind("binary")
    add_deps("mtea.test")
    add_files("sort/internal.cpp")
  target_end()

  target("test.update_extrema_functions")
    set_kind("binary")
    add_deps("mtea.test")
    add_files("util/update_extrema_functions.cpp")
  target_end()
end
