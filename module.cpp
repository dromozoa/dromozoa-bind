// Copyright (C) 2016-2018 Tomoyuki Fujimori <moyu@dromozoa.com>
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

#include <stdexcept>
#include <string>
#include <iostream>

#include "dromozoa/bind.hpp"

namespace dromozoa {
  enum enum_t {
    ENUM42 = 42,
    ENUM69 = 69,
  };

  namespace {
    void impl_set_field(lua_State* L) {
      luaX_set_field(L, -1, 1, 17);
      luaX_set_field(L, -1, 2, 23);
      luaX_set_field(L, -1, 3, 37);
      luaX_set_field(L, -1, 4, 42);
      luaX_set_field(L, -1, 5, luaX_nil);
      luaX_push(L, "foo");
      luaX_set_field(L, -2, "s");
    }

    void impl_opt_range(lua_State* L) {
      size_t size = luaX_check_integer<size_t>(L, 1);
      size_t i = luaX_opt_range_i(L, 2, size);
      size_t j = luaX_opt_range_j(L, 3, size);
      luaX_push(L, i);
      luaX_push(L, j);
    }

    void impl_check_integer(lua_State* L) {
      luaX_check_integer<int16_t>(L, 1);
      luaX_check_integer<uint16_t>(L, 2);
      luaX_check_integer<size_t>(L, 3);
      luaX_check_integer<int>(L, 4, 0, 255);
    }

    void impl_opt_integer(lua_State* L) {
      luaX_opt_integer<int16_t>(L, 1, 0);
      luaX_opt_integer<uint16_t>(L, 2, 0);
      luaX_opt_integer<size_t>(L, 3, 0);
      luaX_opt_integer<int>(L, 4, 0, 0, 255);
    }

    void impl_check_enum(lua_State* L) {
      luaX_check_enum<enum_t>(L, 1);
    }

    void impl_opt_enum(lua_State* L) {
      enum_t value = luaX_opt_enum(L, 1, ENUM42);
      luaX_push<int>(L, value);
    }

    void impl_check_integer_field(lua_State* L) {
      luaX_push(L, luaX_check_integer_field<uint16_t>(L, 1, "foo"));
      luaX_push(L, luaX_check_integer_field<uint16_t>(L, 1, 42));
    }

    void impl_check_integer_field_range(lua_State* L) {
      luaX_push(L, luaX_check_integer_field<int>(L, 1, "nice", -20, 19));
    }

    void impl_opt_integer_field(lua_State* L) {
      luaX_push(L, luaX_opt_integer_field<uint16_t>(L, 1, "foo", 0));
      luaX_push(L, luaX_opt_integer_field<uint16_t>(L, 1, 42, 0));
    }

    void impl_opt_integer_field_range(lua_State* L) {
      luaX_push(L, luaX_opt_integer_field<int32_t>(L, 1, "tv_usec", 0, 0, 999999));
    }

    void impl_set_metafield(lua_State* L) {
      lua_newtable(L);
      luaX_set_metafield(L, -1, "a", "a");
      luaX_push(L, "b");
      luaX_set_metafield(L, -2, "b");
    }

    void impl_is_integer(lua_State* L) {
      luaX_push(L, luaX_is_integer(L, 1));
    }

    void impl_new(lua_State* L) {
      if (lua_isnoneornil(L, 2)) {
        luaX_new<int>(L);
      } else {
        luaX_new<int>(L, luaX_check_integer<int>(L, 2));
      }
      luaX_set_metatable(L, "dromozoa.bind.int");
    }

    void impl_set(lua_State* L) {
      *luaX_check_udata<int>(L, 1, "dromozoa.bind.int") = luaX_check_integer<int>(L, 2);
      luaX_push_success(L);
    }

    void impl_get(lua_State* L) {
      luaX_push(L, *luaX_check_udata<int>(L, 1, "dromozoa.bind.int"));
    }

    void impl_to(lua_State* L) {
      int* data = luaX_to_udata<int>(L, 1,
          luaL_optstring(L, 2, "dromozoa.bind.none"),
          luaL_optstring(L, 3, "dromozoa.bind.none"),
          luaL_optstring(L, 4, "dromozoa.bind.none"),
          luaL_optstring(L, 5, "dromozoa.bind.none"));
      if (data) {
        luaX_push(L, *data);
      } else {
        luaX_push(L, luaX_nil);
      }
    }
  }

  void initialize_callback(lua_State* L);
  void initialize_core(lua_State* L);
  void initialize_handle(lua_State* L);

  void initialize(lua_State* L) {
    luaX_set_field(L, -1, "set_field", impl_set_field);
    luaX_set_field(L, -1, "opt_range", impl_opt_range);
    luaX_set_field(L, -1, "check_integer", impl_check_integer);
    luaX_set_field(L, -1, "opt_integer", impl_opt_integer);
    luaX_set_field(L, -1, "check_enum", impl_check_enum);
    luaX_set_field(L, -1, "opt_enum", impl_opt_enum);
    luaX_set_field(L, -1, "check_integer_field", impl_check_integer_field);
    luaX_set_field(L, -1, "check_integer_field_range", impl_check_integer_field_range);
    luaX_set_field(L, -1, "opt_integer_field", impl_opt_integer_field);
    luaX_set_field(L, -1, "opt_integer_field_range", impl_opt_integer_field_range);
    luaX_set_field(L, -1, "set_metafield", impl_set_metafield);
    luaX_set_field(L, -1, "is_integer", impl_is_integer);

    luaX_set_field<int>(L, -1, "ENUM42", ENUM42);
    luaX_set_field<int>(L, -1, "ENUM69", ENUM69);

    luaX_set_metafield(L, -1, "__call", impl_new);

    luaL_newmetatable(L, "dromozoa.bind.int");
    lua_newtable(L);
    luaX_set_field(L, -1, "set", impl_set);
    luaX_set_field(L, -1, "get", impl_get);
    luaX_set_field(L, -1, "to", impl_to);
    luaX_set_field(L, -2, "__index");
    lua_pop(L, 1);

    luaX_set_field(L, -1, "sizeof_lua_integer", sizeof(lua_Integer));

    initialize_callback(L);
    initialize_core(L);
    initialize_handle(L);
  }
}

extern "C" int luaopen_dromozoa_bind(lua_State* L) {
  lua_newtable(L);
  dromozoa::initialize(L);
  return 1;
}
