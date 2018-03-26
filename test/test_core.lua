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

local function check_result(...)
  assert(select("#", ...) == 5)
  if verbose then
    print(...)
  end
  local a, b, c, d, e = ...
  assert(a == nil)
  assert(b == 0.25)
  assert(c == 42)
  assert(d == true)
  assert(e == "foo")
end

check_result(bind.core.result_int())
check_result(bind.core.result_void())

local function check_none_or_nil(n, ...)
  assert(select("#", ...) == n)
  assert(... == nil)
end

check_none_or_nil(0, bind.core.push_none())
check_none_or_nil(1, bind.core.push_nil())

assert(bind.core.push_enum() == 42)

local function check_string(...)
  assert(select("#", ...) == 5)
  if verbose then
    print(...)
  end
  for i = 1, 5 do
    assert(select(i, ...) == "あいうえお")
  end
end

check_string(bind.core.push_string())

assert(bind.core.push_success() == true)
assert(bind.core.push_success(42) == 42)

local function check_error(fn, expect)
  local result, message = pcall(fn)
  if verbose then
    print(result, message)
  end
  assert(not result)
  if expect then
    assert(message:find(expect, 1, true))
  end
end

bind.core.unexpected()
check_error(bind.core.throw, "exception caught: runtime_error")
check_error(bind.core.field_error1, "field nil not an integer")
check_error(bind.core.field_error2, "field userdata:")
check_error(bind.core.field_error3, [[field "\1\2\3\4\5\6\a\b\t\n\v\f\r\14\15\16]])
