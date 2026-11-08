-- SPDX-FileCopyrightText: 2026 atyxeut <atyxeut@outlook.com>
-- SPDX-License-Identifier: GPL-3.0-or-later

add_includedirs("../src")

set_default(false)
add_defines("NDEBUG")
set_optimize("fastest")
add_cxxflags("-fcontract-evaluation-semantic=ignore")

if is_mode("debug") then
  target("creep.benchmark")
    set_kind("moduleonly")
    add_files("../src/**.cppm", { public = true })
    remove_files("../src/creep.cppm", "../src/**.test.cppm")
  target_end()

  target("benchmark.hoare_quick_sort_variations")
    set_kind("binary")
    add_deps("creep.benchmark")
    add_files("sort/hoare_quick_sort_variations.cpp")
  target_end()
end
