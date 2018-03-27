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
    luaX_reference<2> ref_;

    void impl_set(lua_State* L) {
      luaX_reference<2>(L, 1, 2).swap(ref_);
    }

    void impl_run(lua_State* L) {
      lua_State* state = 0;
      if (lua_toboolean(L, 1)) {
        state = L;
      } else {
        state = ref_.state();
      }
      ref_.get_field(state, 0);
      ref_.get_field(state, 1);
      int r = lua_pcall(state, 1, 1, 0);
      luaX_push(L, r);
    }
  }

  void initialize_callback(lua_State* L) {
    lua_newtable(L);
    {
      luaX_set_field(L, -1, "set", impl_set);
      luaX_set_field(L, -1, "run", impl_run);
    }
    luaX_set_field(L, -2, "callback");
  }
}
