#include <stdio.h>
#include <lauxlib.h>
#include "zulu.h"


int zulu_create_argtable(lua_State *L, int argc, char **argv)
/*
 * Create a global table called 'arg' in the same way as the Lua interpreter
 */
{
  int n;
  lua_newtable(L);
  for (n=1; n<argc; ++n) {
    lua_pushstring(L, argv[n]);
    lua_rawseti(L, -2, n-1);
  }
  lua_setglobal(L, "arg");
  return 0;
}

int zulu_runscript(lua_State *L, const char *filename)
{
  lua_getglobal(L, "debug");
  lua_getfield(L, -1, "traceback");

  int msgh = lua_gettop(L);
  int errc = luaL_loadfile(L, filename) || lua_pcall(L, 0, LUA_MULTRET, msgh);

  lua_remove(L, msgh);

  if (errc == LUA_OK) {
    return 0;
  }
  else {
    printf("error running %s\n", filename);
    printf("%s\n", lua_tostring(L, -1));
    return errc;
  }
}
