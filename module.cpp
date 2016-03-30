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

#include <iostream>
#include <stdexcept>
#include <string>

#include "dromozoa/bind.hpp"
#include "dromozoa/luacxx.hpp"

namespace dromozoa {
  using bind::function;
  using bind::push_success;
  using bind::translate_range_i;
  using bind::translate_range_j;

  namespace {
    int impl_throw(lua_State*) {
      throw std::runtime_error("test");
    }

    int impl_throw_int(lua_State*) {
      throw 42;
    }

    int impl_raise0(lua_State*) {
      return 0;
    }

    int impl_raise1(lua_State* L) {
      lua_pushnil(L);
      return 1;
    }

    int impl_raise2(lua_State* L) {
      lua_pushnil(L);
      lua_pushliteral(L, "test");
      return 2;
    }

    int impl_raise3(lua_State* L) {
      lua_pushnil(L);
      lua_pushliteral(L, "test");
      lua_pushinteger(L, 42);
      return 3;
    }

    int impl_raise_false(lua_State* L) {
      lua_pushboolean(L, false);
      lua_pushstring(L, "test");
      return 2;
    }

    int impl_new(lua_State* L) {
      *static_cast<int*>(lua_newuserdata(L, sizeof(int))) = 0;
      luaL_getmetatable(L, "dromozoa.bind.test");
      lua_setmetatable(L, -2);
      return 1;
    }

    int impl_set(lua_State* L) {
      *static_cast<int*>(luaL_checkudata(L, 1, "dromozoa.bind.test")) = luaL_checkinteger(L, 2);
      return push_success(L);
    }

    int impl_get(lua_State* L) {
      lua_pushinteger(L, *static_cast<const int*>(luaL_checkudata(L, 1, "dromozoa.bind.test")));
      return 1;
    }

    int impl_translate_range(lua_State* L) {
      size_t size = luaL_checkinteger(L, 1);
      size_t i = translate_range_i(L, 2, size);
      size_t j = translate_range_j(L, 3, size);
      lua_pushinteger(L, i);
      lua_pushinteger(L, j);
      return 2;
    }
  }

  template <bool T_check>
  struct checked {
    static int open_test(lua_State* L, const char* metaname) {
      lua_newtable(L);
      function<impl_throw, T_check>::set_field(L, "throw");
      function<impl_throw_int, T_check>::set_field(L, "throw_int");
      function<impl_raise0, T_check>::set_field(L, "raise0");
      function<impl_raise1, T_check>::set_field(L, "raise1");
      function<impl_raise2, T_check>::set_field(L, "raise2");
      function<impl_raise3, T_check>::set_field(L, "raise3");
      function<impl_raise_false, T_check>::set_field(L, "raise_false");
      function<impl_new, T_check>::set_field(L, "new");
      function<impl_set, T_check>::set_field(L, "set");
      function<impl_get, T_check>::set_field(L, "get");
      function<impl_translate_range, T_check>::set_field(L, "translate_range");

      luaL_newmetatable(L, metaname);
      lua_pushvalue(L, -2);
      lua_setfield(L, -2, "__index");
      lua_pop(L, 1);

      enum {
        zero = 0,
        one = 1,
      };

      DROMOZOA_BIND_SET_FIELD(L, zero);
      DROMOZOA_BIND_SET_FIELD(L, one);

      return 1;
    }
  };

  namespace {
    int impl_luaX_function(lua_State*) {
      throw 42;
      // throw std::runtime_error("a runtime_error");
    }

    void impl_luaX_test(lua_State* L) {
      luaX_push(L, luaX_nil);
      lua_newtable(L);
      luaX_set_table(L, "t", true);
      luaX_set_table(L, 1, false);
      luaX_set_table(L, "f", impl_luaX_function);
    }

    void impl_luaX_test_integer(lua_State* L) {
      int i = 42;
      size_t s = 42;
      lua_Integer l = 42;
      luaX_push(L, i, s, l);
    }

    void impl_luaX_test_string(lua_State* L) {
      char a[] = { 'f', 'o', 'o', '\0' };
      char* p = a;
      const char* c = a;
      luaX_push(L, a);
      luaX_push(L, p);
      luaX_push(L, c);
      luaX_push(L, "foo");
      luaX_push(L, std::string("foo"));
    }
  }

  void initialize_luaX(lua_State* L) {
    luaX_set_table(L, "luaX_test", impl_luaX_test);
    luaX_set_table(L, "luaX_test_integer", impl_luaX_test_integer);
    luaX_set_table(L, "luaX_test_string", impl_luaX_test_string);
  }
}

extern "C" int luaopen_dromozoa_bind(lua_State* L) {
  lua_newtable(L);

  lua_newtable(L);
  dromozoa::bind::initialize(L);
  dromozoa::checked<true>::open_test(L, "dromozoa.bind.test");
  dromozoa::initialize_luaX(L);
  lua_setfield(L, -2, "test");

  lua_newtable(L);
  lua_pushvalue(L, -2);
  lua_setfield(L, -2, "__index");
  lua_setmetatable(L, -3);
  lua_setfield(L, -2, "checked");

  lua_newtable(L);
  dromozoa::bind::initialize(L);
  dromozoa::checked<false>::open_test(L, "dromozoa.bind.test.default");
  lua_setfield(L, -2, "test");
  lua_setfield(L, -2, "default");

  return 1;
}
