-- Copyright (C) 2018 Tomoyuki Fujimori <moyu@dromozoa.com>
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

local verbose = true

--
-- is_integer
--

assert(bind.util.is_integer(42))
assert(bind.util.is_integer(42 / 2))
assert(not bind.util.is_integer(1.25))
assert(bind.util.is_integer(1.25e6))
if bind.util.sizeof_integer == 4 then
  assert(not bind.util.is_integer(1.25e12))
elseif bind.util.sizeof_integer == 8 then
  assert(bind.util.is_integer(1.25e12))
  assert(bind.util.is_integer(1.25e18))
  assert(not bind.util.is_integer(1.25e24))
end

local DBL_MAX = 1.7976931348623157e+308
local DBL_DENORM_MIN = 4.9406564584124654e-324
local DBL_MIN = 2.2250738585072014e-308
local DBL_EPSILON = 2.2204460492503131e-16

assert(not bind.util.is_integer(DBL_MAX))
assert(not bind.util.is_integer(DBL_DENORM_MIN))
assert(not bind.util.is_integer(DBL_MIN))
assert(not bind.util.is_integer(DBL_EPSILON))
assert(not bind.util.is_integer(math.pi))

assert(bind.util.is_integer(-1 / math.huge)) -- -0
assert(not bind.util.is_integer(math.huge))  -- inf
assert(not bind.util.is_integer(-math.huge)) -- -inf
assert(not bind.util.is_integer(0 / 0))      -- nan

--
-- check_integer
--

local function check_integer(fn, source, expect)
  local status, result = pcall(fn, source)
  if status then
    assert(not expect)
    assert(result == tonumber(source))
  else
    if verbose then
      print(result)
    end
    assert(expect)
    if type(expect) == "string" then
      assert(result:find(expect, 1, true))
    end
  end
end

check_integer(bind.util.check_int16, -32769, "out of bounds")
check_integer(bind.util.check_int16, -32768)
check_integer(bind.util.check_int16, 0)
check_integer(bind.util.check_int16, 32767)
check_integer(bind.util.check_int16, 32768, "out of bounds")
check_integer(bind.util.check_int16, 0.25, true)
check_integer(bind.util.check_int16, "42")

check_integer(bind.util.check_uint16, -1, "out of bounds")
check_integer(bind.util.check_uint16, 0)
check_integer(bind.util.check_uint16, 65535)
check_integer(bind.util.check_uint16, 65536, "out of bounds")
check_integer(bind.util.check_uint16, 0.25, true)
check_integer(bind.util.check_uint16, "42")

check_integer(bind.util.check_int_range, -1, "out of bounds")
check_integer(bind.util.check_int_range, 0)
check_integer(bind.util.check_int_range, 255)
check_integer(bind.util.check_int_range, 256, "out of bounds")
check_integer(bind.util.check_int_range, 0.25, true)
check_integer(bind.util.check_int_range, "42")

--
-- opt_range
--

local function check_opt_range(a, b, i, j)
  assert(a == i)
  assert(b == j)
end

check_opt_range(0, 3, bind.util.opt_range(3))
check_opt_range(3, 3, bind.util.opt_range(3, 4))
check_opt_range(2, 3, bind.util.opt_range(3, 3))
check_opt_range(1, 3, bind.util.opt_range(3, 2))
check_opt_range(0, 3, bind.util.opt_range(3, 1))
check_opt_range(0, 3, bind.util.opt_range(3, 0))
check_opt_range(2, 3, bind.util.opt_range(3, -1))
check_opt_range(1, 3, bind.util.opt_range(3, -2))
check_opt_range(0, 3, bind.util.opt_range(3, -3))
check_opt_range(0, 3, bind.util.opt_range(3, -4))
check_opt_range(0, 3, bind.util.opt_range(3, 1, 4))
check_opt_range(0, 3, bind.util.opt_range(3, 1, 3))
check_opt_range(0, 2, bind.util.opt_range(3, 1, 2))
check_opt_range(0, 1, bind.util.opt_range(3, 1, 1))
check_opt_range(0, 0, bind.util.opt_range(3, 1, 0))
check_opt_range(0, 3, bind.util.opt_range(3, 1, -1))
check_opt_range(0, 2, bind.util.opt_range(3, 1, -2))
check_opt_range(0, 1, bind.util.opt_range(3, 1, -3))
check_opt_range(0, 0, bind.util.opt_range(3, 1, -4))
