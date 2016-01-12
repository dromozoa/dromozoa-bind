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

assert(bind.get_log_level() == 0)
assert(not bind.get_raise_error())

local result, message = pcall(bind.test_throw)
assert(not result)
assert(message:find("exception caught"))
assert(message:find("test"))

local result, message, code = bind.test_raise3()
assert(not result)
assert(message == "test")
assert(code == 42)

bind.set_raise_error(true)

local result, message = pcall(bind.test_raise0)
assert(not result)
assert(message:find("error raised"))

local result, message = pcall(bind.test_raise1)
assert(not result)
assert(message:find("error raised"))

local result, message = pcall(bind.test_raise2)
assert(not result)
assert(message:find("test"))

local result, message = pcall(bind.test_raise3)
assert(not result)
assert(message:find("test"))
