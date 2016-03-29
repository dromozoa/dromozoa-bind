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

#ifndef DROMOZOA_LUACXX_HPP
#define DROMOZOA_LUACXX_HPP

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <exception>
#include <string>

namespace dromozoa {
  namespace luacxx {
    struct luaX_Nil_impl {};
    typedef int luaX_Nil_impl::*luaX_Nil;

    static const luaX_Nil luaX_nil = 0;

    template <class T>
    struct luaX_push_impl {};

    template <class T>
    inline void luaX_push(lua_State* L, const T& value) {
      luaX_push_impl<T>::apply(L, value);
    }

    template <class T, class T_key>
    inline void luaX_set_table(lua_State* L, const T_key& key, const T& value) {
      luaX_push(L, key);
      luaX_push(L, value);
      lua_settable(L, -3);
    }

    class luaX_State {
    public:
      explicit luaX_State(lua_State* L) : L_(L), top_(lua_gettop(L)) {}

      lua_State* get() {
        return L_;
      }

      int end() {
        return lua_gettop(L_) - top_;
      }

      template <class U>
      luaX_State& push(const U& value) {
        luaX_push(L_, value);
        return *this;
      }

      luaX_State& push_value(int index) {
        lua_pushvalue(L_, index);
        return *this;
      }

      luaX_State& pop(int n = 1) {
        lua_pop(L_, n);
        return *this;
      }

      luaX_State& new_table() {
        lua_newtable(L_);
        return *this;
      }

      template <class U, class U_key>
      luaX_State& set_table(const U_key& key, const U& value) {
        luaX_set_table(L_, key, value);
        return *this;
      }

    private:
      lua_State* L_;
      int top_;
    };

    typedef void (*luaX_CxxFunction)(luaX_State&);

    template <>
    struct luaX_push_impl<luaX_Nil> {
      template <class U>
      static void apply(lua_State* L, const U&) {
        lua_pushnil(L);
      }
    };

    template <size_t T>
    struct luaX_push_impl<char[T]> {
      template <class U>
      static void apply(lua_State* L, const U& value) {
        lua_pushstring(L, value);
      }
    };

    template <>
    struct luaX_push_impl<std::string> {
      template <class U>
      static void apply(lua_State* L, const U& value) {
        lua_pushlstring(L, value.data(), value.size());
      }
    };

    typedef int (*luaX_Closure)(lua_State*, void*);

    template <luaX_Closure T>
    struct luaX_push_closure {
      static int closure(lua_State* L) {
        try {
          return T(L, lua_touserdata(L, lua_upvalueindex(1)));
        } catch (const std::exception& e) {
          return luaL_error(L, "exception caught: %s", e.what());
        } catch (...) {
          return luaL_error(L, "exception caught");
        }
      }

      template <class U>
      static void apply(lua_State* L, const U& value) {
        lua_pushlightuserdata(L, reinterpret_cast<void*>(value));
        lua_pushcclosure(L, closure, 1);
      }
    };

    inline int luaX_closure_lua_CFunction(lua_State* L, void* data) {
      lua_CFunction f = reinterpret_cast<lua_CFunction>(data);
      return f(L);
    }

    inline int luaX_closure_luaX_CxxFunction(lua_State* L, void* data) {
      luaX_State state(L);
      luaX_CxxFunction f = reinterpret_cast<luaX_CxxFunction>(data);
      f(state);
      return state.end();
    }

    template <>
    struct luaX_push_impl<int(lua_State*)>
      : luaX_push_closure<luaX_closure_lua_CFunction> {};

    template <>
    struct luaX_push_impl<void(luaX_State&)>
      : luaX_push_closure<luaX_closure_luaX_CxxFunction> {} ;

#define DROMOZOA_DETAIL_LUAX_PUSH_IMPL(PP_lua_type, PP_cxx_type) \
    template <> \
    struct luaX_push_impl<PP_cxx_type> { \
      template <class U> \
      static void apply(lua_State* L, const U& value) { \
        lua_push##PP_lua_type(L, value); \
      } \
    };

    DROMOZOA_DETAIL_LUAX_PUSH_IMPL(boolean, bool)
    DROMOZOA_DETAIL_LUAX_PUSH_IMPL(integer, char)
    DROMOZOA_DETAIL_LUAX_PUSH_IMPL(integer, signed char)
    DROMOZOA_DETAIL_LUAX_PUSH_IMPL(integer, unsigned char)
    DROMOZOA_DETAIL_LUAX_PUSH_IMPL(integer, short)
    DROMOZOA_DETAIL_LUAX_PUSH_IMPL(integer, unsigned short)
    DROMOZOA_DETAIL_LUAX_PUSH_IMPL(integer, int)
    DROMOZOA_DETAIL_LUAX_PUSH_IMPL(integer, unsigned int)
    DROMOZOA_DETAIL_LUAX_PUSH_IMPL(integer, long)
    DROMOZOA_DETAIL_LUAX_PUSH_IMPL(integer, unsigned long)
    DROMOZOA_DETAIL_LUAX_PUSH_IMPL(integer, long long)
    DROMOZOA_DETAIL_LUAX_PUSH_IMPL(integer, unsigned long long)
    DROMOZOA_DETAIL_LUAX_PUSH_IMPL(string, char*)
    DROMOZOA_DETAIL_LUAX_PUSH_IMPL(string, const char*)

#undef DROMOZOA_DETAIL_LUAX_PUSH_IMPL
  }

  using luacxx::luaX_Nil;
  using luacxx::luaX_State;
  using luacxx::luaX_nil;
  using luacxx::luaX_push;
  using luacxx::luaX_set_table;
}

#endif
