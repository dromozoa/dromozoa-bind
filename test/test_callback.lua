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

local verbose = os.getenv "VERBOSE" == "1"

bind.handle.clear_destructed()

assert(not bind.handle.is_destructed(42))
bind.callback.set(function (handle)
  if verbose then
    print(handle)
    print(handle:get())
    print(coroutine.running())
  end
  assert(handle:get() == 42)
end, bind.handle(42))
collectgarbage()
collectgarbage()
assert(not bind.handle.is_destructed(42))

bind.callback.run(true)
bind.callback.run(false)

local thread = coroutine.create(function ()
  bind.callback.set(function (handle)
    if verbose then
      print(handle)
      print(handle:get())
      print(coroutine.running())
    end
    assert(handle:get() == 69)
  end, bind.handle(69))
end)

assert(not bind.handle.is_destructed(69))
assert(coroutine.resume(thread))
collectgarbage()
collectgarbage()
assert(not bind.handle.is_destructed(69))
assert(bind.handle.is_destructed(42))

bind.callback.run(true)
bind.callback.run(false)

bind.callback.set(nil, nil)
collectgarbage()
collectgarbage()
assert(bind.handle.is_destructed(69))
