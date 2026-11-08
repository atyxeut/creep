// SPDX-FileCopyrightText: 2026 atyxeut <atyxeut@outlook.com>
// SPDX-License-Identifier: LGPL-3.0-or-later

export module creep:io.format_flag;

import :meta.enum_flag;
import std;

export namespace creep {

enum class iofmt : std::uint32_t { none = 0, endl = 1 };

} // export namespace creep

export namespace creep::meta {

template <>
struct is_enum_flag<iofmt> : std::true_type {};

} // export namespace creep::meta
