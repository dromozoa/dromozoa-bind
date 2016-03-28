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

#ifndef DROMOZOA_LUAX_SET_TABLE_HPP
#define DROMOZOA_LUAX_SET_TABLE_HPP

extern "C" {
#include <lua.h>
}

#include "luaX_push.hpp"

namespace dromozoa {
  namespace detail {
    template <class T1, class T2>
    inline void luaX_set_table(lua_State* L, const T1& key, const T2& value) {
      luaX_push(L, key);
      luaX_push(L, value);
      lua_settable(L, -3);
    }
  }

  using detail::luaX_set_table;
}

#endif
