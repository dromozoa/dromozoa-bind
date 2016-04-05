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

#include <stddef.h>

#include <stdexcept>
#include <string>

#include "dromozoa/bind.hpp"

namespace dromozoa {
  namespace {
    void impl_throw_int(lua_State*) {
      throw 42;
    }

    void impl_throw_runtime_error(lua_State*) {
      throw std::runtime_error("runtime_error");
    }

    int impl_result_int(lua_State* L) {
      luaX_push(L, true);
      luaX_push(L, 42);
      luaX_push(L, "foo");
      return 3;
    }

    void impl_result_void(lua_State* L) {
      luaX_push(L, true);
      luaX_push(L, 42);
      luaX_push(L, "foo");
    }

    void impl_push_nil(lua_State* L) {
      luaX_push(L, luaX_nil);
    }

    void impl_push_string(lua_State* L) {
      char data[] = { 'f', 'o', 'o', 0 };
      luaX_push(L, "foo");
      luaX_push(L, data);
      luaX_push(L, static_cast<char*>(data));
      luaX_push(L, static_cast<const char*>(data));
      luaX_push(L, std::string("foo"));
    }

    void impl_push_success(lua_State* L) {
      luaX_push_success(L);
    }

    void impl_set_field(lua_State* L) {
      luaX_set_field(L, 1, 17);
      luaX_set_field(L, 2, 23);
      luaX_set_field(L, 3, 37);
      luaX_set_field(L, 4, 42);
      luaX_set_field(L, 5, luaX_nil);
      luaX_push(L, "foo");
      luaX_set_field(L, "s");
    }

    void impl_opt_range(lua_State* L) {
      size_t size = luaL_checkinteger(L, 1);
      size_t i = luaX_opt_range_i(L, 2, size);
      size_t j = luaX_opt_range_j(L, 3, size);
      luaX_push(L, i);
      luaX_push(L, j);
    }

    void impl_new(lua_State* L) {
      if (lua_isnoneornil(L, 2)) {
        luaX_new<int>(L);
      } else {
        luaX_new<int>(L, luaL_checkinteger(L, 2));
      }
      luaX_set_metatable(L, "dromozoa.bind.int");
    }

    void impl_set(lua_State* L) {
      *luaX_check_udata<int>(L, 1, "dromozoa.bind.int") = luaL_checkinteger(L, 2);
      luaX_push_success(L);
    }

    void impl_get(lua_State* L) {
      luaX_push(L, *luaX_check_udata<int>(L, 1, "dromozoa.bind.int"));
    }
  }

  void initialize(lua_State* L) {
    luaX_set_field(L, "throw_int", impl_throw_int);
    luaX_set_field(L, "throw_runtime_error", impl_throw_runtime_error);
    luaX_set_field(L, "result_int", impl_result_int);
    luaX_set_field(L, "result_void", impl_result_void);
    luaX_set_field(L, "push_nil", impl_push_nil);
    luaX_set_field(L, "push_string", impl_push_string);
    luaX_set_field(L, "push_success", impl_push_success);
    luaX_set_field(L, "set_field", impl_set_field);
    luaX_set_field(L, "opt_range", impl_opt_range);
    luaX_set_metafield(L, "__call", impl_new);

    luaL_newmetatable(L, "dromozoa.bind.int");
    lua_newtable(L);
    luaX_set_field(L, "set", impl_set);
    luaX_set_field(L, "get", impl_get);
    luaX_set_field(L, "__index");
    lua_pop(L, 1);
  }
}

extern "C" int luaopen_dromozoa_bind(lua_State* L) {
  lua_newtable(L);
  dromozoa::initialize(L);
  return 1;
}
