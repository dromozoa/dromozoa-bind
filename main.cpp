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
}

#include <stdexcept>

#include "dromozoa/bind.hpp"

namespace dromozoa {
  namespace {
    int impl_test_throw(lua_State*) {
      throw std::runtime_error("test");
    }

    int impl_test_throw_int(lua_State*) {
      throw 42;
    }

    int impl_test_raise0(lua_State*) {
      return 0;
    }

    int impl_test_raise1(lua_State* L) {
      lua_pushnil(L);
      return 1;
    }

    int impl_test_raise2(lua_State* L) {
      lua_pushnil(L);
      lua_pushliteral(L, "test");
      return 2;
    }

    int impl_test_raise3(lua_State* L) {
      lua_pushnil(L);
      lua_pushliteral(L, "test");
      lua_pushinteger(L, 42);
      return 3;
    }
  }

  void test_initialize(lua_State* L) {
    bind::function<impl_test_throw>::set_field(L, "test_throw");
    bind::function<impl_test_throw_int>::set_field(L, "test_throw_int");
    bind::function<impl_test_raise0>::set_field(L, "test_raise0");
    bind::function<impl_test_raise1>::set_field(L, "test_raise1");
    bind::function<impl_test_raise2>::set_field(L, "test_raise2");
    bind::function<impl_test_raise3>::set_field(L, "test_raise3");
  }
}

extern "C" int luaopen_dromozoa_bind(lua_State* L) {
  lua_newtable(L);
  dromozoa::bind::initialize(L);
  dromozoa::test_initialize(L);
  return 1;
}
