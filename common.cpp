// Copyright (C) 2018,2024 Tomoyuki Fujimori <moyu@dromozoa.com>
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

#include <stdlib.h>

#include "common.hpp"

namespace dromozoa {
  bool verbose() {
    if (const char* p = getenv("VERBOSE")) {
      if (*p == '1') {
        ++p;
        return *p == '\0';
      }
    }
    return false;
  }

  bool failure_policy_is_error(lua_State* L) {
    lua_getglobal(L, "dromozoa_failure_policy_is_error");
    bool result = lua_toboolean(L, -1);
    lua_pop(L, 1);
    return result;
  }
}
