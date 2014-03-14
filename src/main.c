#include <stdio.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "struct.h"
#include "zulu.h"




typedef struct {
  double weight;
  double length;
  int callback;
  const char *name;
} dragon;
int dragon_new(void *obj)
{
  dragon *d = (dragon*) obj;
  d->weight = 0.0;
  d->length = 0.0;
  return 0;
}
int dragon_del(void *obj)
{
  //  printf("cleaning up after a dragon\n");
  return 0;
}
int dragon_speak(lua_State *L)
{
  dragon *d = (dragon*) luaL_checkudata(L, 1, "dragon");
  printf("%s-dragon says ROAR!\n", d->name);
  lua_struct_pushmember(L, d, "dragon", "callback");
  return 1;
}
int luaopen_legend(lua_State *L)
{
  lua_newtable(L);
  lua_struct_member_t lua_dragon_members[] = {
    {"weight", offsetof(dragon, weight), LSTRUCT_DOUBLE},
    {"length", offsetof(dragon, length), LSTRUCT_DOUBLE},
    {"callback", offsetof(dragon, callback), LSTRUCT_OBJECT},
    {"name", offsetof(dragon, name), LSTRUCT_STRING},
    {NULL, 0, 0},
  };
  lua_struct_method_t lua_dragon_methods[] = {
    {"speak", dragon_speak},
    {NULL, NULL},
  };
  lua_struct_t lua_dragon = {
    .type_name = "dragon",
    .alloc_size = sizeof(dragon),
    .new = dragon_new,
    .del = dragon_del,
    .members = lua_dragon_members,
    .methods = lua_dragon_methods,
  };
  lua_struct_register(L, lua_dragon);
  lua_pushcfunction(L, dragon_speak);
  lua_setfield(L, -2, "speak");
  lua_pushstring(L, "Legend version 0.0.1");
  lua_setfield(L, -2, "version");
  return 1;
}





int main(int argc, char **argv)
{
  lua_State *L = luaL_newstate();
  zulu_create_argtable(L, argc, argv);

  luaL_openlibs(L);
  luaL_requiref(L, "buffer", luaopen_buffer, 0); lua_pop(L, 1);
  luaL_requiref(L, "H5", luaopen_H5, 0); lua_pop(L, 1);

  luaL_requiref(L, "30log", luaopen_30log, 0); lua_pop(L, 1);
  luaL_requiref(L, "ansicolors", luaopen_ansicolors, 0); lua_pop(L, 1);
  luaL_requiref(L, "argparse", luaopen_argparse, 0); lua_pop(L, 1);
  luaL_requiref(L, "array", luaopen_array, 0); lua_pop(L, 1);
  luaL_requiref(L, "class", luaopen_class, 0); lua_pop(L, 1);
  luaL_requiref(L, "hdf5", luaopen_hdf5, 0); lua_pop(L, 1);
  luaL_requiref(L, "json", luaopen_json, 0); lua_pop(L, 1);
  luaL_requiref(L, "serpent", luaopen_serpent, 0); lua_pop(L, 1);
  luaL_requiref(L, "struct", luaopen_struct, 0); lua_pop(L, 1);

  luaL_requiref(L, "legend", luaopen_legend, 0); lua_pop(L, 1);

  if (argc == 1) {
    printf("usage: main script.lua [arg1=val1 arg2=val2]\n");
  }
  else {
    zulu_runscript(L, argv[1]);
  }

  lua_close(L);

  return 0;
}
