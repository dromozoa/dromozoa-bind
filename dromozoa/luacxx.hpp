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
    struct luaX_Push {};

    template <class T>
    inline void luaX_push(lua_State* L, const T& value) {
      luaX_Push<T>::apply(L, value);
    }

    template <class T1, class T2>
    inline void luaX_push(lua_State* L, const T1& v1, const T2& v2) {
      luaX_push(L, v1);
      luaX_push(L, v2);
    }

    template <class T1, class T2, class T3>
    inline void luaX_push(lua_State* L, const T1& v1, const T2& v2, const T3& v3) {
      luaX_push(L, v1);
      luaX_push(L, v2);
      luaX_push(L, v3);
    }

    template <class T1, class T2, class T3, class T4>
    inline void luaX_push(lua_State* L, const T1& v1, const T2& v2, const T3& v3, const T4& v4) {
      luaX_push(L, v1);
      luaX_push(L, v2);
      luaX_push(L, v3);
      luaX_push(L, v4);
    }

    template <class T, class T_key>
    inline void luaX_set_table(lua_State* L, const T_key& key, const T& value) {
      luaX_push(L, key, value);
      lua_settable(L, -3);
    }

    class luaX_State {
    public:
      explicit luaX_State(lua_State* L) : L_(L) {}

      lua_State* get() {
        return L_;
      }

      template <class U>
      luaX_State& push(const U& value) {
        luaX_push(L_, value);
        return *this;
      }

      template <class U1, class U2>
      luaX_State& push(const U1& v1, const U2& v2) {
        luaX_push(L_, v1, v2);
        return *this;
      }

      template <class U1, class U2, class U3>
      luaX_State& push(const U1& v1, const U2& v2, const U3& v3) {
        luaX_push(L_, v1, v2, v3);
        return *this;
      }

      template <class U1, class U2, class U3, class U4>
      luaX_State& push(const U1& v1, const U2& v2, const U3& v3, const U4& v4) {
        luaX_push(L_, v1, v2, v3, v4);
        return *this;
      }

      luaX_State& push_value(int index) {
        lua_pushvalue(L_, index);
        return *this;
      }

      luaX_State& push_success() {
        if (lua_toboolean(L_, 1)) {
          lua_pushvalue(L_, 1);
        } else {
          lua_pushboolean(L_, 1);
        }
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
    };

    inline int luaX_closure(lua_State* L, lua_CFunction function) {
      return function(L);
    }

    inline int luaX_closure(lua_State* L, void (*function)(luaX_State&)) {
      int top = lua_gettop(L);
      luaX_State LX(L);
      function(LX);
      return lua_gettop(L) - top;
    }

    template <>
    struct luaX_Push<luaX_Nil> {
      template <class U>
      static void apply(lua_State* L, const U&) {
        lua_pushnil(L);
      }
    };

    template <size_t T>
    struct luaX_Push<char[T]> {
      template <class U>
      static void apply(lua_State* L, const U& value) {
        lua_pushstring(L, value);
      }
    };

    template <>
    struct luaX_Push<std::string> {
      template <class U>
      static void apply(lua_State* L, const U& value) {
        lua_pushlstring(L, value.data(), value.size());
      }
    };

    template <class T_result, class T>
    struct luaX_Push<T_result(T)> {
      typedef T_result (*function_type)(T);

      static int closure(lua_State* L) {
        try {
          return luaX_closure(L, reinterpret_cast<function_type>(lua_touserdata(L, lua_upvalueindex(1))));
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

#define DROMOZOA_LUACXX_LUAX_PUSH(PP_lua_type, PP_cxx_type) \
    template <> \
    struct luaX_Push<PP_cxx_type> { \
      template <class U> \
      static void apply(lua_State* L, const U& value) { \
        lua_push##PP_lua_type(L, value); \
      } \
    };

    DROMOZOA_LUACXX_LUAX_PUSH(boolean, bool)
    DROMOZOA_LUACXX_LUAX_PUSH(integer, char)
    DROMOZOA_LUACXX_LUAX_PUSH(integer, signed char)
    DROMOZOA_LUACXX_LUAX_PUSH(integer, unsigned char)
    DROMOZOA_LUACXX_LUAX_PUSH(integer, short)
    DROMOZOA_LUACXX_LUAX_PUSH(integer, unsigned short)
    DROMOZOA_LUACXX_LUAX_PUSH(integer, int)
    DROMOZOA_LUACXX_LUAX_PUSH(integer, unsigned int)
    DROMOZOA_LUACXX_LUAX_PUSH(integer, long)
    DROMOZOA_LUACXX_LUAX_PUSH(integer, unsigned long)
    DROMOZOA_LUACXX_LUAX_PUSH(integer, long long)
    DROMOZOA_LUACXX_LUAX_PUSH(integer, unsigned long long)
    DROMOZOA_LUACXX_LUAX_PUSH(string, char*)
    DROMOZOA_LUACXX_LUAX_PUSH(string, const char*)

#undef DROMOZOA_LUACXX_LUAX_PUSH
  }

  using luacxx::luaX_nil;
  using luacxx::luaX_push;
  using luacxx::luaX_set_table;
  using luacxx::luaX_State;
}

#endif
