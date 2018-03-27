-- Copyright (C) 2016-2018 Tomoyuki Fujimori <moyu@dromozoa.com>
--
-- This file is part of dromozoa-bind.
--
-- dromozoa-bind is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- dromozoa-bind is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with dromozoa-bind.  If not, see <http://www.gnu.org/licenses/>.

local bind = require "dromozoa.bind"

local t = {}
bind.set_field(t)
assert(t[1] == 17)
assert(t[2] == 23)
assert(t[3] == 37)
assert(t[4] == 42)
assert(t[5] == nil)
assert(t.s == "foo")

local t = { 42, 42, 42, 42, 42, s = "bar" }
bind.set_field(t)
assert(t[1] == 17)
assert(t[2] == 23)
assert(t[3] == 37)
assert(t[4] == 42)
assert(t[5] == nil)
assert(t.s == "foo")

bind.check_integer(0, 0, 0, 0)
assert(not pcall(bind.check_integer))
assert(not pcall(bind.check_integer, 0))
assert(not pcall(bind.check_integer, 0, 0))
assert(not pcall(bind.check_integer, 0, 0, 0))
assert(not pcall(bind.check_integer, 32768, 0, 0, 0));
assert(not pcall(bind.check_integer, -32769, 0, 0, 0));
assert(not pcall(bind.check_integer, 0, -1, 0, 0));
assert(not pcall(bind.check_integer, 0, 65536, 0, 0));
assert(not pcall(bind.check_integer, 0, 0, -1, 0));
assert(not pcall(bind.check_integer, 0, 0, 0, -1));
assert(not pcall(bind.check_integer, 0, 0, 0, 256));

bind.opt_integer()
bind.opt_integer(0)
bind.opt_integer(0, 0)
bind.opt_integer(0, 0, 0)
bind.opt_integer(0, 0, 0, 0)
assert(not pcall(bind.opt_integer, 32768))
assert(not pcall(bind.opt_integer, -32769))
assert(not pcall(bind.opt_integer, 0, -1))
assert(not pcall(bind.opt_integer, 0, 65536))
assert(not pcall(bind.opt_integer, 0, 0, -1))
assert(not pcall(bind.opt_integer, 0, 0, 0, -1))
assert(not pcall(bind.opt_integer, 0, 0, 0, 256))

bind.check_enum(bind.ENUM42)
bind.check_enum(bind.ENUM69)
assert(not pcall(bind.check_enum, "foo"))

assert(bind.opt_enum() == bind.ENUM42)
assert(bind.opt_enum(bind.ENUM42) == bind.ENUM42)
assert(bind.opt_enum(bind.ENUM69) == bind.ENUM69)
assert(not pcall(bind.check_enum, "foo"))

local a, b = assert(bind.check_integer_field({ foo = 42, [42] = "42" }))
assert(a == 42)
assert(b == 42)
assert(not pcall(bind.check_integer_field, {}))
assert(not pcall(bind.check_integer_field, { foo = "bar" }))
assert(not pcall(bind.check_integer_field, { foo = 42 }))
assert(not pcall(bind.check_integer_field, { foo = 42, [42] = "bar" }))

assert(bind.check_integer_field_range({ nice = 0 }) == 0)
assert(not pcall(bind.check_integer_field_range, { nice = 42 }))
assert(not pcall(bind.check_integer_field_range, { nice = "bar" }))

assert(bind.opt_integer_field({}) == 0)
assert(bind.opt_integer_field({ foo = 42 }) == 42)
assert(not pcall(bind.opt_integer_field, { foo = "bar" }))
assert(not pcall(bind.opt_integer_field, { foo = -1 }))
assert(not pcall(bind.opt_integer_field, { [42] = -1 }))

assert(bind.opt_integer_field_range({}) == 0)
assert(bind.opt_integer_field_range({ tv_usec = 42 }) == 42)
assert(not pcall(bind.opt_integer_field_range, { tv_usec = "foo" }))
assert(not pcall(bind.opt_integer_field_range, { tv_usec = -1 }))
assert(not pcall(bind.opt_integer_field_range, { tv_usec = 1000000 }))

local a, b = bind.set_metafield()
assert(getmetatable(a).a == "a")
assert(getmetatable(a).b == "b")
assert(b == nil)
