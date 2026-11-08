-- SPDX-FileCopyrightText: 2026 atyxeut <atyxeut@outlook.com>
-- SPDX-License-Identifier: GPL-3.0-or-later

add_includedirs("../src")

if is_mode("release") then
  target("creep")
    set_kind("moduleonly")
    add_files("**.cppm", { public = true })
    remove_files("**.test.cppm", "**.benchmark.cppm")
    add_defines("NDEBUG")
    set_optimize("fastest")
    add_cxxflags("-fcontract-evaluation-semantic=ignore")
  target_end()
end
