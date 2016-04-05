-- Copyright (C) 2016 Tomoyuki Fujimori <moyu@dromozoa.com>
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

local result, message = pcall(bind.throw_int)
assert(not result)
assert(message == "exception caught")

local result, message = pcall(bind.throw_runtime_error)
assert(not result)
assert(message:find("exception caught:") == 1)
assert(message:find("runtime_error"))

local a, b, c = bind.result_int()
assert(a == true)
assert(b == 42)
assert(c == "foo")

local a, b, c = bind.result_void()
assert(a == true)
assert(b == 42)
assert(c == "foo")

assert(bind.push_nil() == nil)

local a, b, c, d, e = bind.push_string()
assert(a == "foo")
assert(b == "foo")
assert(c == "foo")
assert(d == "foo")
assert(e == "foo")

assert(bind.push_success() == true)
assert(bind.push_success(42) == 42)

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

local function test_opt_range(a, b, i, j)
  assert(a == i)
  assert(b == j)
end

test_opt_range(0, 3, bind.opt_range(3))
test_opt_range(3, 3, bind.opt_range(3, 4))
test_opt_range(2, 3, bind.opt_range(3, 3))
test_opt_range(1, 3, bind.opt_range(3, 2))
test_opt_range(0, 3, bind.opt_range(3, 1))
test_opt_range(0, 3, bind.opt_range(3, 0))
test_opt_range(2, 3, bind.opt_range(3, -1))
test_opt_range(1, 3, bind.opt_range(3, -2))
test_opt_range(0, 3, bind.opt_range(3, -3))
test_opt_range(0, 3, bind.opt_range(3, -4))
test_opt_range(0, 3, bind.opt_range(3, 1, 4))
test_opt_range(0, 3, bind.opt_range(3, 1, 3))
test_opt_range(0, 2, bind.opt_range(3, 1, 2))
test_opt_range(0, 1, bind.opt_range(3, 1, 1))
test_opt_range(0, 0, bind.opt_range(3, 1, 0))
test_opt_range(0, 3, bind.opt_range(3, 1, -1))
test_opt_range(0, 2, bind.opt_range(3, 1, -2))
test_opt_range(0, 1, bind.opt_range(3, 1, -3))
test_opt_range(0, 0, bind.opt_range(3, 1, -4))

assert(bind():get() == 0)
assert(bind():set(42):get() == 42)
assert(bind(42):get() == 42)
