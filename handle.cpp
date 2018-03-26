// Copyright (C) 2018 Tomoyuki Fujimori <moyu@dromozoa.com>
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

#include "dromozoa/bind.hpp"

namespace dromozoa {
  namespace {
    int count = 0;

    class handle {
    public:
      handle(int value) : value_(value) {}

      ~handle() {
        ++count;
      }

      int get() {
        return value_;
      }

    private:
      int value_;
      handle(const handle&);
      handle& operator=(const handle&);
    };

    void new_handle_ref(lua_State* L, int value) {
      luaX_new<handle>(L, value);
      luaX_set_metatable(L, "dromozoa.bind.handle_ref");
    }

    void new_handle(lua_State* L, int value) {
      luaX_new<handle>(L, value);
      luaX_set_metatable(L, "dromozoa.bind.handle");
    }

    handle* check_handle(lua_State* L, int arg) {
      return luaX_check_udata<handle>(L, arg, "dromozoa.bind.handle_ref", "dromozoa.bind.handle");
    }

    void impl_call(lua_State* L) {
      int value = luaX_check_integer<int>(L, 2);
      new_handle(L, value);
    }

    void impl_gc(lua_State* L) {
      check_handle(L, 1)->~handle();
    }

    void impl_get(lua_State* L) {
      luaX_push(L, check_handle(L, 1)->get());
    }

    void impl_get_count(lua_State* L) {
      luaX_push(L, count);
    }

    void impl_handle_ref(lua_State* L) {
      int value = luaX_check_integer<int>(L, 1);
      new_handle_ref(L, value);
    }
  }

  void initialize_handle(lua_State* L) {
    lua_newtable(L);
    {
      luaL_newmetatable(L, "dromozoa.bind.handle_ref");
      lua_pushvalue(L, -2);
      luaX_set_field(L, -2, "__index");
      lua_pop(L, 1);

      luaL_newmetatable(L, "dromozoa.bind.handle");
      lua_pushvalue(L, -2);
      luaX_set_field(L, -2, "__index");
      luaX_set_field(L, -1, "__gc", impl_gc);
      lua_pop(L, 1);

      luaX_set_metafield(L, -1, "__call", impl_call);
      luaX_set_field(L, -1, "get", impl_get);
      luaX_set_field(L, -1, "get_count", impl_get_count);
    }
    luaX_set_field(L, -2, "handle");

    luaX_set_field(L, -1, "handle_ref", impl_handle_ref);
  }
}
