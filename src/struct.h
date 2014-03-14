#ifndef LUA_STRUCT_H
#define LUA_STRUCT_H


enum {
  LSTRUCT_INT    = 1200001,
  LSTRUCT_DOUBLE = 1300001,
  LSTRUCT_STRING = 1400001,
  LSTRUCT_OBJECT = 1500001,
  LSTRUCT_STRUCT = 1600001,
} ;


typedef struct {
  const char *method_name;
  lua_CFunction method;
} lua_struct_method_t;


typedef struct {
  const char *member_name;
  ptrdiff_t offset;
  int data_type;
  const char *type_name; /* if type is LSTRUCT_STRUCT */
} lua_struct_member_t;


typedef struct {
  const char *type_name;
  size_t alloc_size;
  int (*new)(void*);
  int (*del)(void*);
  lua_struct_member_t *members;
  lua_struct_method_t *methods;
} lua_struct_t;


int luaopen_struct(lua_State *L);
int lua_struct_register(lua_State *L, lua_struct_t type);
int lua_struct_pushmember(lua_State *L, void *obj,
			  const char *type_name,
			  const char *member_name);
int lua_struct_pushstruct(lua_State *L, void *obj,
			  const char *type_name);


#endif /* LUA_STRUCT_H */
