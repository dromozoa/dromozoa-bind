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

#ifndef DROMOZOA_BIND_HPP
#define DROMOZOA_BIND_HPP

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <stddef.h>
#include <stdint.h>

#include <exception>
#include <new>
#include <string>

namespace dromozoa {
  namespace bind {
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

    template <class T, class T_key>
    inline void luaX_set_metafield(lua_State* L, const T_key& key, const T& value) {
      if (!lua_getmetatable(L, -1)) {
        lua_newtable(L);
      }
      luaX_set_field(L, key, value);
      lua_setmetatable(L, -2);
    }

    inline void luaX_set_metatable(lua_State* L, const char* name) {
#if LUA_VERSION_NUM+0 >= 502
      return luaL_setmetatable(L, name);
#else
      luaL_getmetatable(L, name);
      lua_setmetatable(L, -2);
#endif
    }

    inline bool luaX_to_udata_impl(lua_State* L, const char* name) {
      luaL_getmetatable(L, name);
      bool result = lua_rawequal(L, -1, -2);
      lua_pop(L, 1);
      return result;
    }

    template <class T>
    inline T* luaX_to_udata(lua_State* L, int index, const char* name) {
      if (T* data = static_cast<T*>(lua_touserdata(L, index))) {
        if (lua_getmetatable(L, index)) {
          if (!luaX_to_udata_impl(L, name)) {
            data = 0;
          }
          lua_pop(L, 1);
          return data;
        }
      }
      return 0;
    }

    template <class T>
    inline T* luaX_to_udata(lua_State* L, int index, const char* name1, const char* name2) {
      if (T* data = static_cast<T*>(lua_touserdata(L, index))) {
        if (lua_getmetatable(L, index)) {
          if (!(luaX_to_udata_impl(L, name1) || luaX_to_udata_impl(L, name2))) {
            data = 0;
          }
          lua_pop(L, 1);
          return data;
        }
      }
      return 0;
    }

    template <class T>
    inline T* luaX_to_udata(lua_State* L, int index, const char* name1, const char* name2, const char* name3) {
      if (T* data = static_cast<T*>(lua_touserdata(L, index))) {
        if (lua_getmetatable(L, index)) {
          if (!(luaX_to_udata_impl(L, name1) || luaX_to_udata_impl(L, name2) || luaX_to_udata_impl(L, name3))) {
            data = 0;
          }
          lua_pop(L, 1);
          return data;
        }
      }
      return 0;
    }

    template <class T>
    inline T* luaX_to_udata(lua_State* L, int index, const char* name1, const char* name2, const char* name3, const char* name4) {
      if (T* data = static_cast<T*>(lua_touserdata(L, index))) {
        if (lua_getmetatable(L, index)) {
          if (!(luaX_to_udata_impl(L, name1) || luaX_to_udata_impl(L, name2) || luaX_to_udata_impl(L, name3) || luaX_to_udata_impl(L, name4))) {
            data = 0;
          }
          lua_pop(L, 1);
          return data;
        }
      }
      return 0;
    }

    template <class T>
    inline T* luaX_check_udata(lua_State* L, int n, const char* name) {
      return static_cast<T*>(luaL_checkudata(L, n, name));
    }

    template <class T>
    inline T* luaX_check_udata(lua_State* L, int n, const char* name1, const char* name2) {
      if (T* data = luaX_to_udata<T>(L, n, name1)) {
        return data;
      } else {
        return luaX_check_udata<T>(L, n, name2);
      }
    }

    template <class T>
    inline T* luaX_check_udata(lua_State* L, int n, const char* name1, const char* name2, const char* name3) {
      if (T* data = luaX_to_udata<T>(L, n, name1, name2)) {
        return data;
      } else {
        return luaX_check_udata<T>(L, n, name3);
      }
    }

    template <class T>
    inline T* luaX_check_udata(lua_State* L, int n, const char* name1, const char* name2, const char* name3, const char* name4) {
      if (T* data = luaX_to_udata<T>(L, n, name1, name2, name3)) {
        return data;
      } else {
        return luaX_check_udata<T>(L, n, name4);
      }
    }

    template <class T>
    inline T* luaX_new(lua_State* L) {
      T* data = static_cast<T*>(lua_newuserdata(L, sizeof(T)));
      new(data) T();
      return data;
    }

    template <class T, class T1>
    inline T* luaX_new(lua_State* L, const T1& v1) {
      T* data = static_cast<T*>(lua_newuserdata(L, sizeof(T)));
      new(data) T(v1);
      return data;
    }

    template <class T, class T1, class T2>
    inline T* luaX_new(lua_State* L, const T1& v1, const T2& v2) {
      T* data = static_cast<T*>(lua_newuserdata(L, sizeof(T)));
      new(data) T(v1, v2);
      return data;
    }

    template <class T, class T1, class T2, class T3>
    inline T* luaX_new(lua_State* L, const T1& v1, const T2& v2, const T3& v3) {
      T* data = static_cast<T*>(lua_newuserdata(L, sizeof(T)));
      new(data) T(v1, v2, v3);
      return data;
    }

    template <class T, class T1, class T2, class T3, class T4>
    inline T* luaX_new(lua_State* L, const T1& v1, const T2& v2, const T3& v3, const T4& v4) {
      T* data = static_cast<T*>(lua_newuserdata(L, sizeof(T)));
      new(data) T(v1, v2, v3, v4);
      return data;
    }

    template <class T, bool T_is_integer, bool T_is_signed>
    struct luaX_check_integer_impl {};

    template <class T>
    inline T luaX_check_integer(lua_State* L, int n) {
      return luaX_check_integer_impl<T, std::numeric_limits<T>::is_integer, std::numeric_limits<T>::is_signed>::apply(L, n);
    }

    inline size_t luaX_opt_range_i(lua_State* L, int n, size_t size) {
      lua_Integer i = luaL_optinteger(L, n, 0);
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

    inline size_t luaX_opt_range_j(lua_State* L, int n, size_t size) {
      lua_Integer j = luaL_optinteger(L, n, size);
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

#define DROMOZOA_BIND_LUAX_PUSH_IMPL(PP_lua_type, PP_cxx_type) \
    template <> \
    struct luaX_push_impl<PP_cxx_type> { \
      template <class U> \
      static void apply(lua_State* L, const U& value) { \
        lua_push##PP_lua_type(L, value); \
      } \
    };

    DROMOZOA_BIND_LUAX_PUSH_IMPL(boolean, bool)
    DROMOZOA_BIND_LUAX_PUSH_IMPL(integer, char)
    DROMOZOA_BIND_LUAX_PUSH_IMPL(integer, signed char)
    DROMOZOA_BIND_LUAX_PUSH_IMPL(integer, unsigned char)
    DROMOZOA_BIND_LUAX_PUSH_IMPL(integer, short)
    DROMOZOA_BIND_LUAX_PUSH_IMPL(integer, unsigned short)
    DROMOZOA_BIND_LUAX_PUSH_IMPL(integer, int)
    DROMOZOA_BIND_LUAX_PUSH_IMPL(integer, unsigned int)
    DROMOZOA_BIND_LUAX_PUSH_IMPL(integer, long)
    DROMOZOA_BIND_LUAX_PUSH_IMPL(integer, unsigned long)
    DROMOZOA_BIND_LUAX_PUSH_IMPL(integer, long long)
    DROMOZOA_BIND_LUAX_PUSH_IMPL(integer, unsigned long long)
    DROMOZOA_BIND_LUAX_PUSH_IMPL(string, char*)
    DROMOZOA_BIND_LUAX_PUSH_IMPL(string, const char*)

#undef DROMOZOA_BIND_LUAX_PUSH_IMPL

    template <>
    struct luaX_check_integer_impl<bool, true, false> {
      static bool apply(lua_State* L, int n) {
        return luaL_checkinteger(L, n);
      }
    };

    template <class T>
    struct luaX_check_integer_impl<T, true, true> {
      static T apply(lua_State* L, int n) {
        static const intmax_t min = std::numeric_limits<T>::min();
        static const intmax_t max = std::numeric_limits<T>::max();
        intmax_t v = luaL_checkinteger(L, n);
        if (min <= v && v <= max) {
          return v;
        }
        return luaL_argerror(L, n, "out of range");
      }
    };

    template <class T>
    struct luaX_check_integer_impl<T, true, false> {
      static T apply(lua_State* L, int n) {
        static const uintmax_t min = std::numeric_limits<T>::min();
        static const uintmax_t max = std::numeric_limits<T>::max();
        intmax_t v = luaL_checkinteger(L, n);
        if (0 <= v) {
          uintmax_t u = v;
          if (min <= u && u <= max) {
            return u;
          }
        }
        return luaL_argerror(L, n, "out of range");
      }
    };
  }

  using bind::luaX_check_integer;
  using bind::luaX_check_udata;
  using bind::luaX_new;
  using bind::luaX_nil;
  using bind::luaX_opt_range_i;
  using bind::luaX_opt_range_j;
  using bind::luaX_push;
  using bind::luaX_push_success;
  using bind::luaX_set_field;
  using bind::luaX_set_metafield;
  using bind::luaX_set_metatable;
  using bind::luaX_to_udata;
}

#endif
