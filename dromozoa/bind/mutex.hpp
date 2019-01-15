// Copyright (C) 2019 Tomoyuki Fujimori <moyu@dromozoa.com>
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

#ifndef DROMOZOA_THREAD_HPP
#define DROMOZOA_THREAD_HPP

#include <pthread.h>

namespace dromozoa {
  namespace bind {
    class mutex {
    public:
      mutex();
      ~mutex();
      void lock();
      void unlock();
      pthread_mutex_t* native_handle();
    private:
      pthread_mutex_t mutex_;
      mutex(const mutex&);
      mutex& operator=(const mutex&);
    };
  }

  using bind::mutex;
}

#endif
