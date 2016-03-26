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

#ifndef DROMOZOA_LUAX_PUSH_HPP
#define DROMOZOA_LUAX_PUSH_HPP

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <exception>
#include <string>

#include "luaX_nil.hpp"

namespace dromozoa {
  namespace detail {
    template <class T>
    struct luaX_push_impl {};

    template <>
    struct luaX_push_impl<luaX_nil_t> {
      template <class U>
      static void apply(lua_State* L, const U&) {
        lua_pushnil(L);
      }
    };

    struct luaX_push_impl_integer {
      template <class U>
      static void apply(lua_State* L, const U& value) {
        lua_pushinteger(L, value);
      }
    };

    template <> struct luaX_push_impl<char> : luaX_push_impl_integer {};
    template <> struct luaX_push_impl<signed char> : luaX_push_impl_integer {};
    template <> struct luaX_push_impl<unsigned char> : luaX_push_impl_integer {};
    template <> struct luaX_push_impl<short> : luaX_push_impl_integer {};
    template <> struct luaX_push_impl<unsigned short> : luaX_push_impl_integer {};
    template <> struct luaX_push_impl<int> : luaX_push_impl_integer {};
    template <> struct luaX_push_impl<unsigned int> : luaX_push_impl_integer {};
    template <> struct luaX_push_impl<long> : luaX_push_impl_integer {};
    template <> struct luaX_push_impl<unsigned long> : luaX_push_impl_integer {};
    template <> struct luaX_push_impl<long long> : luaX_push_impl_integer {};
    template <> struct luaX_push_impl<unsigned long long> : luaX_push_impl_integer {};

    struct luaX_push_impl_string {
      template <class U>
      static void apply(lua_State* L, const U& value) {
        lua_pushstring(L, value);
      }
    };

    template <> struct luaX_push_impl<char*> : luaX_push_impl_string {};
    template <> struct luaX_push_impl<const char*> : luaX_push_impl_string {};
    template <size_t T> struct luaX_push_impl<char[T]> : luaX_push_impl_string {};

    template <>
    struct luaX_push_impl<std::string> {
      template <class U>
      static void apply(lua_State* L, const U& value) {
        lua_pushlstring(L, value.data(), value.size());
      }
    };

    template <>
    struct luaX_push_impl<bool> {
      template <class U>
      static void apply(lua_State* L, const U& value) {
        lua_pushboolean(L, value);
      }
    };

    template <>
    struct luaX_push_impl<int(lua_State*)> {
      static int closure(lua_State* L) {
        lua_CFunction function = reinterpret_cast<lua_CFunction>(lua_touserdata(L, lua_upvalueindex(1)));
        try {
          return function(L);
        } catch (const std::exception& e) {
          return luaL_error(L, "exception caught: %s", e.what());
        } catch (...) {
          lua_pushliteral(L, "exception caught");
          return lua_error(L);
        }
      }

      template <class U>
      static void apply(lua_State* L, const U& value) {
        lua_pushlightuserdata(L, reinterpret_cast<void*>(value));
        lua_pushcclosure(L, closure, 1);
      }
    };

    template <class T>
    inline void luaX_push(lua_State* L, const T& value) {
      luaX_push_impl<T>::apply(L, value);
    }
  }

  using detail::luaX_push;
}

#endif
