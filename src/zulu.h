#ifndef ZULU_H
#define ZULU_H

#include <lua.h>

int luaopen_30log(lua_State *L);
int luaopen_ansicolors(lua_State *L);
int luaopen_argparse(lua_State *L);
int luaopen_array(lua_State *L);
int luaopen_buffer(lua_State *L);
int luaopen_class(lua_State *L);
int luaopen_json(lua_State *L);
int luaopen_H5(lua_State *L);
int luaopen_hdf5(lua_State *L);
int luaopen_legend(lua_State *L);
int luaopen_serpent(lua_State *L);
int luaopen_struct(lua_State *L);

int zulu_create_argtable(lua_State *L, int argc, char **argv);
int zulu_runscript(lua_State *L, const char *filename);

#define ZULU_PRINTSTACK(msg)						\
  do {									\
    int n;								\
    printf("Lua stack (%s):\n", msg);					\
    for (n=1; n<=lua_gettop(L); ++n) {					\
      printf("\t%d: <%s>\n", n, lua_typename(L, lua_type(L, n)));	\
    }									\
  } while (0)								\

#endif /* ZULU_H */
