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

#ifndef DROMOZOA_LUAX_HPP
#define DROMOZOA_LUAX_HPP

#include "luaX_push.hpp"
#include "luaX_set_field.hpp"

namespace dromozoa {
  namespace detail {
    class luaX_State {
    public:
      explicit luaX_State(lua_State* L)
        : L_(L), top_(lua_gettop(L)) {}

      luaX_State& new_table() {
        lua_newtable(L_);
        return *this;
      }

      template <class U>
      luaX_State& push(const U& value) {
        luaX_push(L_, value);
        return *this;
      }

      template <class U1, class U2>
      luaX_State& set_field(const U1& key, const U2& value) {
        luaX_set_field(L_, key, value);
        return *this;
      }

      int end() {
        return lua_gettop(L_) - top_;
      }

    private:
      lua_State* L_;
      int top_;
    };
  }

  using detail::luaX_State;
}

#endif
