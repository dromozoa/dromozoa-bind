package = "dromozoa-bind"
version = "1.58-1"
source = {
  url = "https://github.com/dromozoa/dromozoa-bind/archive/v1.58.tar.gz";
  file = "dromozoa-bind-1.58.tar.gz";
}
description = {
  summary = "Helper library for creating bindings between C++ and Lua";
  license = "GPL-3";
  homepage = "https://github.com/dromozoa/dromozoa-bind/";
  maintainer = "Tomoyuki Fujimori <moyu@dromozoa.com>";
}
build = {
  type = "make";
  build_variables = {
    ROCK_CFLAGS = "$(CFLAGS)";
    ROCK_LIBFLAG = "$(LIBFLAG)";
    ROCK_LUA_INCDIR = "$(LUA_INCDIR)";
  };
  install_variables = {
    ROCK_LIBDIR = "$(LIBDIR)";
  };
}
