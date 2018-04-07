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

#include <iostream>

#include "dromozoa/bind.hpp"
#include "common.hpp"

namespace dromozoa {
  void initialize_callback(lua_State* L);
  void initialize_core(lua_State* L);
  void initialize_handle(lua_State* L);
  void initialize_util(lua_State* L);

  void initialize(lua_State* L) {
    static int count = 0;
    luaX_set_field(L, -1, "count", ++count);

    initialize_callback(L);
    initialize_core(L);
    initialize_handle(L);
    initialize_util(L);
  }
}

extern "C" int luaopen_dromozoa_bind(lua_State* L) {
  if (dromozoa::verbose()) {
    std::cout << "[VERBOSE] luaopen_dromozoa_bind\n";
  }
  lua_newtable(L);
  dromozoa::initialize(L);
  return 1;
}
