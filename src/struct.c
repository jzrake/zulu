#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "struct.h"


static int _struct_api_members(lua_State *L);
static int _struct_api_type(lua_State *L);


int luaopen_struct(lua_State *L)
{
  lua_newtable(L);
  luaL_Reg struct_module_funcs[] = {
    {"members", _struct_api_members},
    {"type", _struct_api_type},
    {NULL, NULL}
  };
  luaL_setfuncs(L, struct_module_funcs, 0);
  return 1;
}



int _struct_api_members(lua_State *L)
{
  int n = 0;
  lua_getmetatable(L, 1);
  lua_getfield(L, -1, "__members__");
  lua_remove(L, -2);
  lua_newtable(L);
  lua_pushnil(L);
  while (lua_next(L, -3) != 0) {
    n += 1;
    lua_pushvalue(L, -2);
    lua_rawseti(L, -4, n);
    lua_pop(L, 1);
  }
  lua_remove(L, -3);
  return 1;
}

int _struct_api_type(lua_State *L)
{
  lua_getmetatable(L, 1); /* TOP: mt */
  lua_getfield(L, -1, "__type__"); /* TOP: mt.__type__ */
  lua_struct_t *T = (lua_struct_t*) lua_touserdata(L, -1);
  lua_pushstring(L, T->type_name); /* TOP: type_name */
  lua_remove(L, -2);
  lua_remove(L, -2);
  return 1;
}


static int _struct_constructor(lua_State *L);
static int _struct_mt_index(lua_State *L);
static int _struct_mt_newindex(lua_State *L);
static int _struct_mt_gc(lua_State *L);


int lua_struct_pushmember(lua_State *L, void *obj,
			  const char *type_name,
			  const char *member_name)
{
  luaL_getmetatable(L, type_name); /* TOP: mt */
  lua_getfield(L, -1, "__members__"); /* TOP: __members__ table */
  lua_getfield(L, -1, member_name); /* TOP: mt.__members__[member_name] */
  lua_struct_member_t *M = (lua_struct_member_t*) lua_touserdata(L, -1);
  lua_pop(L, 2); /* TOP: mt */
  lua_getfield(L, -1, "__instances__"); /* TOP: mt.__instances__ */
  lua_rawgetp(L, -1, obj); /* TOP: mt.__instances__[obj] */
  lua_rawgeti(L, -1, M->offset); /* TOP: mt.__instances__[obj][offset] */
  lua_remove(L, -2);
  lua_remove(L, -2);
  lua_remove(L, -2);
  return 1;
}


int lua_struct_register(lua_State *L, lua_struct_t type)
{
  int n;
  luaL_newmetatable(L, type.type_name); /* TOP: mt */


  /* ----------------------------------------------------
   * mt.__type__
   * -------------------------------------------------- */
  lua_struct_t *T = (lua_struct_t*) /* TOP: mt.__type__ */
    lua_newuserdata(L, sizeof(lua_struct_t));
  *T = type;
  T->members = NULL; /* don't use member list directly */
  lua_setfield(L, -2, "__type__"); /* TOP: mt */


  /* ----------------------------------------------------
   * mt.__members__
   * -------------------------------------------------- */
  lua_newtable(L); /* TOP: mt.__members__ */
  n = 0;
  while (type.members[n].member_name) {
    lua_struct_member_t *M = (lua_struct_member_t*) /* TOP: member */
      lua_newuserdata(L, sizeof(lua_struct_member_t));
    *M = type.members[n];
    lua_setfield(L, -2, M->member_name); /* TOP: mt.__members__ */
    n += 1;
  }
  lua_setfield(L, -2, "__members__"); /* TOP: mt */


  /* ----------------------------------------------------
   * mt.__methods__
   * -------------------------------------------------- */
  lua_newtable(L); /* TOP: mt.__methods__ */
  n = 0;
  while (type.methods[n].method_name) {
    lua_struct_method_t *M = (lua_struct_method_t*) /* TOP: method */
      lua_newuserdata(L, sizeof(lua_struct_method_t));
    *M = type.methods[n];
    lua_setfield(L, -2, M->method_name); /* TOP: mt.__methods__ */
    n += 1;
  }
  lua_setfield(L, -2, "__methods__"); /* TOP: mt */


  /* ----------------------------------------------------
   * mt.__instances__ (table of instances of given type)
   * -------------------------------------------------- */
  lua_newtable(L); /* TOP: mt.__instances__ */
  lua_setfield(L, -2, "__instances__"); /* TOP: mt */


  luaL_Reg struct_mt[] = {
    {"__index", _struct_mt_index},
    {"__newindex", _struct_mt_newindex},
    {"__gc", _struct_mt_gc},
    {NULL, NULL}
  };

  luaL_setfuncs(L, struct_mt, 0);
  lua_pop(L, 1); /* TOP: fresh */

  /* set the type name as an upvalue so that the it's part of the constructor
     function's environment */
  lua_pushstring(L, type.type_name); /* TOP: type_name (upvalue index 1) */
  lua_pushcclosure(L, _struct_constructor, 1); /* TOP: constructor */
  lua_setfield(L, -2, type.type_name); /* TOP: fresh */

  return 0;
}


int _struct_constructor(lua_State *L)
{
  lua_getupvalue(L, -1, 1); /* TOP: type name */
  const char *type_name = lua_tostring(L, -1);
  lua_pop(L, 1); /* TOP: fresh */
  luaL_getmetatable(L, type_name); /* TOP: mt */
  lua_getfield(L, -1, "__type__"); /* TOP: mt.__type__ */
  lua_struct_t *T = (lua_struct_t*) lua_touserdata(L, -1);
  lua_pop(L, 2); /* TOP: fresh */
  void *obj = lua_newuserdata(L, T->alloc_size); /* TOP: new obj */
  if (T->new) T->new(obj);
  luaL_setmetatable(L, type_name);
  luaL_getmetatable(L, type_name); /* TOP: mt */
  lua_getfield(L, -1, "__instances__"); /* TOP: mt.__instances__ */
  lua_newtable(L); /* TOP: mt.__instances__[obj] */
  lua_rawsetp(L, -2, obj); /* TOP: mt.__instances__ */
  lua_pop(L, 2); /* TOP: new obj */
  return 1;
}


int _struct_mt_index(lua_State *L)
{
  void *obj = lua_touserdata(L, 1);
  const char *key = luaL_checkstring(L, 2);
  int top = lua_gettop(L);

  lua_getmetatable(L, 1);                   int imetat = ++top;
  lua_getfield(L, imetat, "__type__");      int itypeu = ++top;
  lua_getfield(L, imetat, "__members__");   int imembt = ++top;
  lua_getfield(L, imetat, "__methods__");   int imetht = ++top;
  lua_getfield(L, imetat, "__instances__"); int iinstt = ++top;
  lua_getfield(L, imembt, key);             int ibkeyu = ++top;
  lua_getfield(L, imetht, key);             int itkeyu = ++top;

  lua_struct_t *type = (lua_struct_t *) lua_touserdata(L, itypeu);
  lua_struct_member_t *memb = (lua_struct_member_t*) lua_touserdata(L, ibkeyu);
  lua_struct_method_t *meth = (lua_struct_method_t*) lua_touserdata(L, itkeyu);

  if (memb) {
    switch (memb->data_type) {
    case LSTRUCT_DOUBLE:
      lua_pushnumber(L, *((double *)(obj + memb->offset)));
      break;
    case LSTRUCT_STRING:
      lua_pushstring(L, *((const char **)(obj + memb->offset)));
      break;
    case LSTRUCT_OBJECT:
      lua_rawgetp(L, iinstt, obj);
      lua_rawgeti(L, -1, memb->offset);
      lua_remove(L, -2);
      break;
    }
  }
  else if (meth) {
    lua_pushcfunction(L, meth->method);
  }
  else {
    luaL_error(L, "'%s' object has no member '%s'", type->type_name, key);
  }
  lua_replace(L, imetat);
  lua_settop(L, imetat);
  return 1;
}


int _struct_mt_newindex(lua_State *L)
{
  void *obj = lua_touserdata(L, 1);
  const char *key = luaL_checkstring(L, 2);
  int top = lua_gettop(L);

  lua_getmetatable(L, 1);                   int imetat = ++top;
  lua_getfield(L, imetat, "__type__");      int itypeu = ++top;
  lua_getfield(L, imetat, "__members__");   int imembt = ++top;
  lua_getfield(L, imetat, "__methods__");   int imetht = ++top;
  lua_getfield(L, imetat, "__instances__"); int iinstt = ++top;
  lua_getfield(L, imembt, key);             int ibkeyu = ++top;
  lua_getfield(L, imetht, key);             int itkeyu = ++top;

  lua_struct_t *type = (lua_struct_t *) lua_touserdata(L, itypeu);
  lua_struct_member_t *memb = (lua_struct_member_t*) lua_touserdata(L, ibkeyu);
  lua_struct_method_t *meth = (lua_struct_method_t*) lua_touserdata(L, itkeyu);

  if (memb) {
    switch (memb->data_type) {
    case LSTRUCT_DOUBLE:
      *((double *)(obj + memb->offset)) = luaL_checknumber(L, 3);
      break;
    case LSTRUCT_STRING:
      *((const char **)(obj + memb->offset)) = luaL_checkstring(L, 3);
      break;
    case LSTRUCT_OBJECT:
      *((int *)(obj + memb->offset)) = lua_type(L, 3);
      lua_rawgetp(L, iinstt, obj);
      lua_pushvalue(L, 3);
      lua_rawseti(L, -2, memb->offset);
      lua_pop(L, 1);
      break;
    }
  }
  else if (meth) {
    luaL_error(L, "cannot assign to method '%s.%s'", type->type_name, key);
  }
  else {
    luaL_error(L, "'%s' object has no member '%s'", type->type_name, key);
  }
  lua_settop(L, 3);
  return 0;
}


int _struct_mt_gc(lua_State *L)
{
  void *obj = lua_touserdata(L, 1);
  lua_getmetatable(L, 1); /* TOP: mt */
  lua_getfield(L, -1, "__type__"); /* TOP: mt.__type__ userdata */
  lua_struct_t *T = (lua_struct_t*) lua_touserdata(L, -1);
  lua_pop(L, 2); /* TOP: fresh */
  if (T->del) T->del(obj);
  /* ---------------------------
   * mt.__instances__[obj] = nil
   * --------------------------- */
  lua_getmetatable(L, 1); /* TOP: mt */
  lua_getfield(L, -1, "__instances__"); /* TOP: mt.__instances__ */
  lua_pushnil(L); /* TOP: nil */
  lua_rawsetp(L, -2, obj); /* TOP: mt.__instances__ */
  lua_pop(L, 2);
  return 0;
}
