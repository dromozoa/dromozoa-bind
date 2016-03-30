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

#include <stddef.h>

#include <exception>
#include <string>

namespace dromozoa {
  namespace luacxx {
    struct luaX_nil_impl {};
    typedef int luaX_nil_impl::*luaX_nil_t;

    static const luaX_nil_t luaX_nil = 0;

    template <class T>
    struct luaX_push_impl {};

    template <class T>
    inline void luaX_push(lua_State* L, const T& value) {
      luaX_push_impl<T>::apply(L, value);
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

    inline void luaX_push_success(lua_State* L) {
      if (lua_toboolean(L, 1)) {
        lua_pushvalue(L, 1);
      } else {
        lua_pushboolean(L, true);
      }
    }

    template <class T, class T_key>
    inline void luaX_set_field(lua_State* L, const T_key& key, const T& value) {
      luaX_push(L, key, value);
      lua_settable(L, -3);
    }

    template <class T_key>
    inline void luaX_set_field(lua_State* L, const T_key& key) {
      luaX_push(L, key);
      lua_pushvalue(L, -2);
      lua_settable(L, -4);
      lua_pop(L, 1);
    }

    inline void luaX_set_metatable(lua_State* L, const char* name) {
#if LUA_VERSION_NUM+0 >= 502
      return luaL_setmetatable(L, name);
#else
      luaL_getmetatable(L, name);
      lua_setmetatable(L, -2);
#endif
    }

    template <class T, class T_key>
    inline void luaX_set_metafield(lua_State* L, const T_key& key, const T& value) {
      if (!lua_getmetatable(L, -1)) {
        lua_newtable(L);
      }
      luaX_set_field(L, key, value);
      lua_setmetatable(L, -2);
    }

    inline void* luaX_test_udata(lua_State* L, int index, const char* name) {
#if LUA_VERSION_NUM+0 >= 502
      return luaL_testudata(L, index, name);
#else
      if (void* data = lua_touserdata(L, index)) {
        if (lua_getmetatable(L, index)) {
          luaL_getmetatable(L, name);
          if (!lua_rawequal(L, -1, -2)) {
            data = 0;
          }
        }
        lua_pop(L, 2);
        return data;
      } else {
        return 0;
      }
#endif
    }

    inline size_t luaX_range_i(lua_State* L, int index, size_t size) {
      lua_Integer i = luaL_optinteger(L, index, 0);
      if (i < 0) {
        i += size;
        if (i < 0) {
          i = 0;
        }
      } else if (i > 0) {
        --i;
      }
      return i;
    }

    inline size_t luaX_range_j(lua_State* L, int index, size_t size) {
      lua_Integer j = luaL_optinteger(L, index, size);
      if (j < 0) {
        j += size + 1;
      } else if (j > static_cast<lua_Integer>(size)) {
        j = size;
      }
      return j;
    }

    inline int luaX_closure(lua_State* L, lua_CFunction function) {
      return function(L);
    }

    inline int luaX_closure(lua_State* L, void (*function)(lua_State*)) {
      int top = lua_gettop(L);
      function(L);
      return lua_gettop(L) - top;
    }

    template <>
    struct luaX_push_impl<luaX_nil_t> {
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

    template <class T_result, class T>
    struct luaX_push_impl<T_result(T)> {
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

#define DROMOZOA_LUACXX_LUAX_PUSH_IMPL(PP_lua_type, PP_cxx_type) \
    template <> \
    struct luaX_push_impl<PP_cxx_type> { \
      template <class U> \
      static void apply(lua_State* L, const U& value) { \
        lua_push##PP_lua_type(L, value); \
      } \
    };

    DROMOZOA_LUACXX_LUAX_PUSH_IMPL(boolean, bool)
    DROMOZOA_LUACXX_LUAX_PUSH_IMPL(integer, char)
    DROMOZOA_LUACXX_LUAX_PUSH_IMPL(integer, signed char)
    DROMOZOA_LUACXX_LUAX_PUSH_IMPL(integer, unsigned char)
    DROMOZOA_LUACXX_LUAX_PUSH_IMPL(integer, short)
    DROMOZOA_LUACXX_LUAX_PUSH_IMPL(integer, unsigned short)
    DROMOZOA_LUACXX_LUAX_PUSH_IMPL(integer, int)
    DROMOZOA_LUACXX_LUAX_PUSH_IMPL(integer, unsigned int)
    DROMOZOA_LUACXX_LUAX_PUSH_IMPL(integer, long)
    DROMOZOA_LUACXX_LUAX_PUSH_IMPL(integer, unsigned long)
    DROMOZOA_LUACXX_LUAX_PUSH_IMPL(integer, long long)
    DROMOZOA_LUACXX_LUAX_PUSH_IMPL(integer, unsigned long long)
    DROMOZOA_LUACXX_LUAX_PUSH_IMPL(string, char*)
    DROMOZOA_LUACXX_LUAX_PUSH_IMPL(string, const char*)

#undef DROMOZOA_LUACXX_LUAX_PUSH_IMPL
  }

  using luacxx::luaX_nil;
  using luacxx::luaX_push;
  using luacxx::luaX_push_success;
  using luacxx::luaX_range_i;
  using luacxx::luaX_range_j;
  using luacxx::luaX_set_field;
  using luacxx::luaX_set_metafield;
  using luacxx::luaX_set_metatable;
  using luacxx::luaX_test_udata;
}

#endif
