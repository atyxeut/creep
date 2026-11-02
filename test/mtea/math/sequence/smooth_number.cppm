// SPDX-FileCopyrightText: 2026 atyxeut <atyxeut@outlook.com>
// SPDX-License-Identifier: GPL-3.0-or-later

export module test.mtea.math.sequence.smooth_number;

import std;

import mtea.math.sequence.smooth_number;

using namespace mtea;

export {

void print_first_10000_humble_numbers() {
  for (const auto i : smooth_number_list<long long>(10000, std::vector {2, 3, 5, 7}))
    std::print("{} ", i);
}

} // export
