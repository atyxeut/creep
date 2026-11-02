// SPDX-FileCopyrightText: 2026 atyxeut <atyxeut@outlook.com>
// SPDX-License-Identifier: LGPL-3.0-or-later

export module mtea.io.format_flag;

import std;

import mtea.meta.enum_flag;

export namespace mtea {

enum class iofmt : std::uint32_t { none = 0, endl = 1 };

} // export namespace mtea

export namespace mtea::meta {

template <>
struct is_enum_flag<iofmt> : std::true_type {};

} // export namespace mtea::meta
