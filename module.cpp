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
  }

  void initialize_callback(lua_State* L);
  void initialize_core(lua_State* L);
  void initialize_handle(lua_State* L);
  void initialize_util(lua_State* L);

  void initialize(lua_State* L) {
    luaX_set_field(L, -1, "set_field", impl_set_field);
    luaX_set_field(L, -1, "check_enum", impl_check_enum);
    luaX_set_field(L, -1, "opt_enum", impl_opt_enum);
    luaX_set_field(L, -1, "check_integer_field", impl_check_integer_field);
    luaX_set_field(L, -1, "check_integer_field_range", impl_check_integer_field_range);
    luaX_set_field(L, -1, "opt_integer_field", impl_opt_integer_field);
    luaX_set_field(L, -1, "opt_integer_field_range", impl_opt_integer_field_range);
    luaX_set_field(L, -1, "set_metafield", impl_set_metafield);

    luaX_set_field<int>(L, -1, "ENUM42", ENUM42);
    luaX_set_field<int>(L, -1, "ENUM69", ENUM69);

    initialize_callback(L);
    initialize_core(L);
    initialize_handle(L);
    initialize_util(L);
  }
}

extern "C" int luaopen_dromozoa_bind(lua_State* L) {
  lua_newtable(L);
  dromozoa::initialize(L);
  return 1;
}
