// Copyright (C) 2016 Tomoyuki Fujimori <moyu@dromozoa.com>
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

#ifndef DROMOZOA_BIND_FUNCTION_HPP
#define DROMOZOA_BIND_FUNCTION_HPP

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

#include <exception>

#include "raise_error.hpp"

namespace dromozoa {
  namespace bind {
    template <lua_CFunction T>
    struct function {
      static int value(lua_State* L) {
        try {
          int result = T(L);
          if (get_raise_error()) {
            if (result <= 0 || !lua_toboolean(L, -result)) {
              if (result <= 1) {
                lua_pushliteral(L, "error raised");
              } else if (result > 2) {
                lua_pop(L, result - 2);
              }
              return lua_error(L);
            }
          }
          return result;
        } catch (const std::exception& e) {
          return luaL_error(L, "exception caught: %s", e.what());
        } catch (...) {
          return luaL_error(L, "exception caught");
        }
      }

      static void set_field(lua_State* L, const char* key) {
        lua_pushcfunction(L, value);
        lua_setfield(L, -2, key);
      }
    };
  }
}

#endif
