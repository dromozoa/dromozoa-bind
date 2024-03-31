// Copyright (C) 2018,2019 Tomoyuki Fujimori <moyu@dromozoa.com>
//
// This file is part of dromozoa-bind.
//
// dromozoa-bind is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// dromozoa-bind is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with dromozoa-bind.  If not, see <http://www.gnu.org/licenses/>.

#ifndef DROMOZOA_COMMON_HPP
#define DROMOZOA_COMMON_HPP

extern "C" {
#include <lua.h>
}

namespace dromozoa {
  bool verbose();
  bool failure_policy_is_error(lua_State*);
}

#define DROMOZOA_FAILURE_POLICY_IS_ERROR(L) failure_policy_is_error(L)

#define DROMOZOA_CHECK(expr) \
  do { \
    if (!(expr)) { \
      luaX_push(L, luaX_nil); \
      luaX_push(L, "assertion failed: " #expr); \
      return; \
    } \
  } while (false)

#endif
