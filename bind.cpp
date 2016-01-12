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

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include "dromozoa/bind.hpp"

namespace dromozoa {
  namespace bind {
    namespace {
      int log_level = 0;

      int impl_set_log_level(lua_State* L) {
        log_level = luaL_checkinteger(L, 1);
        lua_pushboolean(L, true);
        return 1;
      }

      int impl_get_log_level(lua_State* L) {
        lua_pushinteger(L, log_level);
        return 1;
      }

      bool raise_error = false;

      int impl_set_raise_error(lua_State* L) {
        raise_error = lua_toboolean(L, 1);
        lua_pushboolean(L, true);
        return 1;
      }

      int impl_get_raise_error(lua_State* L) {
        lua_pushboolean(L, raise_error);
        return 1;
      }

      void set_cfunction(lua_State* L, const char* key, lua_CFunction cfunction) {
        lua_pushcfunction(L, cfunction);
        lua_setfield(L, -2, key);
      }
    }

    int get_log_level() {
      return log_level;
    }

    bool get_raise_error() {
      return raise_error;
    }

    void initialize(lua_State* L) {
      set_cfunction(L, "set_log_level", impl_set_log_level);
      set_cfunction(L, "get_log_level", impl_get_log_level);
      set_cfunction(L, "set_raise_error", impl_set_raise_error);
      set_cfunction(L, "get_raise_error", impl_get_raise_error);
    }
  }
}
