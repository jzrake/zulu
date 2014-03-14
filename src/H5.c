#include <stdlib.h>
#include <string.h>
#include <hdf5.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#ifndef H5_VERSION_GE
#define H5_VERSION_GE(Maj,Min,Rel)					\
  (((H5_VERS_MAJOR==Maj) && (H5_VERS_MINOR==Min) && (H5_VERS_RELEASE>=Rel)) || \
   ((H5_VERS_MAJOR==Maj) && (H5_VERS_MINOR>Min)) ||			\
   (H5_VERS_MAJOR>Maj))
#endif
#ifndef H5_VERSION_LE
#define H5_VERSION_LE(Maj,Min,Rel)					\
  (((H5_VERS_MAJOR==Maj) && (H5_VERS_MINOR==Min) && (H5_VERS_RELEASE<=Rel)) || \
   ((H5_VERS_MAJOR==Maj) && (H5_VERS_MINOR<Min)) ||			\
   (H5_VERS_MAJOR<Maj))
#endif

// -----------------------------------------------------------------------------
// hid_t
// -----------------------------------------------------------------------------
static void lh5_push_hid_t(lua_State *L, hid_t id)
{
  *((hid_t*) lua_newuserdata(L, sizeof(hid_t))) = id;
  luaL_setmetatable(L, "HDF5::hid_t");
}
static int _new_hid_t(lua_State *L)
{
  lh5_push_hid_t(L, 0);
  return 1;
}
static int _hid_t__len(lua_State *L)
{
  hid_t *h = (hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t");
  lua_pushnumber(L, *h);
  return 1;
}


// -----------------------------------------------------------------------------
// herr_t
// -----------------------------------------------------------------------------
static void lh5_push_herr_t(lua_State *L, herr_t id)
{
  *((herr_t*) lua_newuserdata(L, sizeof(herr_t))) = id;
  luaL_setmetatable(L, "HDF5::herr_t");
}
static int _new_herr_t(lua_State *L)
{
  lh5_push_herr_t(L, 0);
  return 1;
}
static int _herr_t__len(lua_State *L)
{
  herr_t *h = (herr_t*) luaL_checkudata(L, 1, "HDF5::herr_t");
  lua_pushnumber(L, *h);
  return 1;
}


// -----------------------------------------------------------------------------
// H5O_info_t
// -----------------------------------------------------------------------------
static void lh5_push_H5O_info_t(lua_State *L, H5O_info_t id)
{
  *((H5O_info_t*) lua_newuserdata(L, sizeof(H5O_info_t))) = id;
  luaL_setmetatable(L, "HDF5::H5O_info_t");
}
static int _new_H5O_info_t(lua_State *L)
{
  H5O_info_t id = { };
  lh5_push_H5O_info_t(L, id);
  return 1;
}
static int _H5O_info_t__index(lua_State *L)
{
  H5O_info_t *i = (H5O_info_t*) luaL_checkudata(L, 1, "HDF5::H5O_info_t");
  const char *key = luaL_checkstring(L, 2);
  if (0) { }
  else if (strcmp(key, "fileno") == 0) { lua_pushnumber(L, i->fileno); }
  else if (strcmp(key, "addr") == 0) { lua_pushnumber(L, i->addr); }
  else if (strcmp(key, "type") == 0) { lua_pushnumber(L, i->type); }
  else if (strcmp(key, "rc") == 0) { lua_pushnumber(L, i->rc); }
  else if (strcmp(key, "atime") == 0) { lua_pushnumber(L, i->atime); }
  else if (strcmp(key, "mtime") == 0) { lua_pushnumber(L, i->mtime); }
  else if (strcmp(key, "ctime") == 0) { lua_pushnumber(L, i->ctime); }
  else if (strcmp(key, "btime") == 0) { lua_pushnumber(L, i->btime); }
  else if (strcmp(key, "num_attrs") == 0) { lua_pushnumber(L, i->num_attrs); }
  else { lua_pushnil(L); }
  return 1;
}
static int _H5O_info_t__newindex(lua_State *L)
{
  luaL_checkudata(L, 1, "HDF5::H5O_info_t");
  luaL_error(L, "object does not support item assignment");
  return 0;
}

// -----------------------------------------------------------------------------
// hsize_t_arr
// -----------------------------------------------------------------------------
static void lh5_push_hsize_t_arr(lua_State *L, hsize_t *hs, unsigned int N)
{
  memcpy(lua_newuserdata(L, sizeof(hsize_t) * N), hs, sizeof(hsize_t) * N);
  luaL_setmetatable(L, "HDF5::hsize_t_arr");
}
static int _new_hsize_t_arr(lua_State *L)
{
  luaL_checktype(L, 1, LUA_TTABLE);
  int n = 0, N = lua_rawlen(L, 1);
  hsize_t *hs = (hsize_t*) malloc(sizeof(hsize_t) * N);

  while (n < N) {
    lua_rawgeti(L, 1, n+1);
    hsize_t val = lua_tointeger(L, -1);
    if (val == -1) val = H5S_UNLIMITED;
    hs[n] = val;
    lua_pop(L, 1);
    ++n;
  }

  lh5_push_hsize_t_arr(L, hs, N);
  free(hs);
  return 1;
}
static int _hsize_t_arr__index(lua_State *L)
{
  hsize_t *lhs = (hsize_t*) luaL_checkudata(L, 1, "HDF5::hsize_t_arr");
  unsigned int n = luaL_checkunsigned(L, 2);
  unsigned int N = lua_rawlen(L, 1) / sizeof(hsize_t);
  if (n < N) {
    lua_pushnumber(L, lhs[n]);
  }
  else {
    lua_pushnil(L);
  }
  return 1;
}
static int _hsize_t_arr__newindex(lua_State *L)
{
  hsize_t *lhs = (hsize_t*) luaL_checkudata(L, 1, "HDF5::hsize_t_arr");
  unsigned int n = luaL_checkunsigned(L, 2);
  hsize_t val = luaL_checkunsigned(L, 3);
  unsigned int N = lua_rawlen(L, 1) / sizeof(hsize_t);
  if (n < N) {
    lhs[n] = val;
  }
  else {
    luaL_error(L, "index %d out of range on array of length %d", n, N);
  }
  return 1;
}


// -----------------------------------------------------------------------------
// By-hand wrappers
// -----------------------------------------------------------------------------

static int _H5_VERSION_GE(lua_State *L)
{
  int Maj = luaL_checkinteger(L, 1);
  int Min = luaL_checkinteger(L, 2);
  int Rel = luaL_checkinteger(L, 3);
  lua_pushboolean(L, H5_VERSION_GE(Maj,Min,Rel));
  return 1;
}

static int _H5_VERSION_LE(lua_State *L)
{
  int Maj = luaL_checkinteger(L, 1);
  int Min = luaL_checkinteger(L, 2);
  int Rel = luaL_checkinteger(L, 3);
  lua_pushboolean(L, H5_VERSION_LE(Maj,Min,Rel));
  return 1;
}

static int _H5_VERS_INFO(lua_State *L)
{
  lua_pushstring(L, H5_VERS_INFO);
  return 1;
}

static herr_t _H5Literate_cb(hid_t g_id, const char *name,
			     const H5L_info_t *info, void *op_data)
{
  /*
   *   function expects
   *
   * + op_data is a lua_State*
   * + function on top of Lua stack
   */
  lua_State *L = (lua_State*) op_data;
  lua_pushvalue(L, -1);
  lua_pushstring(L, name);
  lua_call(L, 1, 0);
  return 0;
}
int _H5Literate(lua_State *L)
{
  hid_t group_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  H5_index_t index_type = luaL_checkinteger(L, 2);
  H5_iter_order_t order = luaL_checkinteger(L, 3);
  hsize_t *idx = (hsize_t*) luaL_checkudata(L, 4, "HDF5::hsize_t_arr");
  luaL_checktype(L, 5, LUA_TFUNCTION);
  H5L_iterate_t op = _H5Literate_cb;
  int ret = H5Literate(group_id, index_type, order, idx, op, L);
  return ret;
}

static int _H5Pget_mpio_actual_chunk_opt_mode(lua_State *L)
{
#ifdef H5_HAVE_PARALLEL
#if (H5_VERSION_GE(1,8,8))
  hid_t dxpl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  H5D_mpio_actual_chunk_opt_mode_t actual_chunk_opt_mode;
  H5Pget_mpio_actual_chunk_opt_mode(dxpl_id, &actual_chunk_opt_mode);
  lua_pushnumber(L, actual_chunk_opt_mode);
  return 1;
#else
  return 0;
#endif // H5_VERSION_GE(1,8,8)
#else
  return 0;
#endif // H5_HAVE_PARALLEL
}

static int _H5Pget_mpio_actual_io_mode(lua_State *L)
{
#ifdef H5_HAVE_PARALLEL
#if (H5_VERSION_GE(1,8,8))
  hid_t dxpl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  H5D_mpio_actual_io_mode_t actual_io_mode;
  H5Pget_mpio_actual_io_mode(dxpl_id, &actual_io_mode);
  lua_pushnumber(L, actual_io_mode);
  return 1;
#else
  return 0;
#endif // H5_VERSION_GE(1,8,8)
#else
  return 0;
#endif // H5_HAVE_PARALLEL
}

static int _H5Pget_mpio_no_collective_cause(lua_State *L)
{
#ifdef H5_HAVE_PARALLEL
#if (H5_VERSION_GE(1,8,10))
  hid_t dxpl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  uint32_t local_no_collective_cause;
  uint32_t global_no_collective_cause;
  H5Pget_mpio_no_collective_cause(dxpl_id, &local_no_collective_cause,
				  &global_no_collective_cause);
  lua_pushnumber(L, local_no_collective_cause);
  lua_pushnumber(L, global_no_collective_cause);
  return 2;
#else
  return 0;
#endif // H5_VERSION_GE(1,8,10)
#else
  return 0;
#endif // H5_HAVE_PARALLEL
}


// -----------------------------------------------------------------------------
// Python-generated wrappers
// -----------------------------------------------------------------------------
static int _H5Acreate2(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *attr_name = luaL_checkstring(L, 2);
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t acpl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t aapl_id = *((hid_t*) luaL_checkudata(L, 6, "HDF5::hid_t"));
  hid_t res = H5Acreate2(loc_id, attr_name, type_id, space_id, acpl_id, aapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Acreate_by_name(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *obj_name = luaL_checkstring(L, 2);
  const char *attr_name = luaL_checkstring(L, 3);
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t acpl_id = *((hid_t*) luaL_checkudata(L, 6, "HDF5::hid_t"));
  hid_t aapl_id = *((hid_t*) luaL_checkudata(L, 7, "HDF5::hid_t"));
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 8, "HDF5::hid_t"));
  hid_t res = H5Acreate_by_name(loc_id, obj_name, attr_name, type_id, space_id, acpl_id, aapl_id, lapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Aopen(lua_State *L)
{
  hid_t obj_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *attr_name = luaL_checkstring(L, 2);
  hid_t aapl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t res = H5Aopen(obj_id, attr_name, aapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Aopen_by_name(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *obj_name = luaL_checkstring(L, 2);
  const char *attr_name = luaL_checkstring(L, 3);
  hid_t aapl_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t res = H5Aopen_by_name(loc_id, obj_name, attr_name, aapl_id, lapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Awrite(lua_State *L)
{
  hid_t attr_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  const void *buf = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  herr_t res = H5Awrite(attr_id, type_id, buf);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Aread(lua_State *L)
{
  hid_t attr_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  void *buf = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  herr_t res = H5Aread(attr_id, type_id, buf);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Aclose(lua_State *L)
{
  hid_t attr_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Aclose(attr_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Aget_space(lua_State *L)
{
  hid_t attr_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Aget_space(attr_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Aget_type(lua_State *L)
{
  hid_t attr_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Aget_type(attr_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Aget_create_plist(lua_State *L)
{
  hid_t attr_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Aget_create_plist(attr_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Arename(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *old_name = luaL_checkstring(L, 2);
  const char *new_name = luaL_checkstring(L, 3);
  herr_t res = H5Arename(loc_id, old_name, new_name);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Arename_by_name(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *obj_name = luaL_checkstring(L, 2);
  const char *old_attr_name = luaL_checkstring(L, 3);
  const char *new_attr_name = luaL_checkstring(L, 4);
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  herr_t res = H5Arename_by_name(loc_id, obj_name, old_attr_name, new_attr_name, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Adelete(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  herr_t res = H5Adelete(loc_id, name);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Adelete_by_name(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *obj_name = luaL_checkstring(L, 2);
  const char *attr_name = luaL_checkstring(L, 3);
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  herr_t res = H5Adelete_by_name(loc_id, obj_name, attr_name, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Aexists(lua_State *L)
{
  hid_t obj_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *attr_name = luaL_checkstring(L, 2);
  htri_t res = H5Aexists(obj_id, attr_name);
  lua_pushboolean(L, res);
  return 1;
}
static int _H5Aexists_by_name(lua_State *L)
{
  hid_t obj_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *obj_name = luaL_checkstring(L, 2);
  const char *attr_name = luaL_checkstring(L, 3);
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  htri_t res = H5Aexists_by_name(obj_id, obj_name, attr_name, lapl_id);
  lua_pushboolean(L, res);
  return 1;
}
static int _H5Acreate1(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t acpl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t res = H5Acreate1(loc_id, name, type_id, space_id, acpl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Aopen_name(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t res = H5Aopen_name(loc_id, name);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Aopen_idx(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned idx = luaL_checkunsigned(L, 2);
  hid_t res = H5Aopen_idx(loc_id, idx);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Aget_num_attrs(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int res = H5Aget_num_attrs(loc_id);
  lua_pushnumber(L, res);
  return 1;
}
static luaL_Reg H5A_funcs[] = {
  {"H5Acreate2", _H5Acreate2},
  {"H5Acreate_by_name", _H5Acreate_by_name},
  {"H5Aopen", _H5Aopen},
  {"H5Aopen_by_name", _H5Aopen_by_name},
  {"H5Awrite", _H5Awrite},
  {"H5Aread", _H5Aread},
  {"H5Aclose", _H5Aclose},
  {"H5Aget_space", _H5Aget_space},
  {"H5Aget_type", _H5Aget_type},
  {"H5Aget_create_plist", _H5Aget_create_plist},
  {"H5Arename", _H5Arename},
  {"H5Arename_by_name", _H5Arename_by_name},
  {"H5Adelete", _H5Adelete},
  {"H5Adelete_by_name", _H5Adelete_by_name},
  {"H5Aexists", _H5Aexists},
  {"H5Aexists_by_name", _H5Aexists_by_name},
  {"H5Acreate1", _H5Acreate1},
  {"H5Aopen_name", _H5Aopen_name},
  {"H5Aopen_idx", _H5Aopen_idx},
  {"H5Aget_num_attrs", _H5Aget_num_attrs},
  {NULL,NULL}};

static int _H5Dcreate2(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t lcpl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t dcpl_id = *((hid_t*) luaL_checkudata(L, 6, "HDF5::hid_t"));
  hid_t dapl_id = *((hid_t*) luaL_checkudata(L, 7, "HDF5::hid_t"));
  hid_t res = H5Dcreate2(loc_id, name, type_id, space_id, lcpl_id, dcpl_id, dapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Dcreate_anon(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t dapl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t res = H5Dcreate_anon(file_id, type_id, space_id, plist_id, dapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Dopen2(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t dapl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t res = H5Dopen2(file_id, name, dapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Dclose(lua_State *L)
{
  hid_t dset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Dclose(dset_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Dget_space(lua_State *L)
{
  hid_t dset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Dget_space(dset_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Dget_type(lua_State *L)
{
  hid_t dset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Dget_type(dset_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Dget_create_plist(lua_State *L)
{
  hid_t dset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Dget_create_plist(dset_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Dget_access_plist(lua_State *L)
{
  hid_t dset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Dget_access_plist(dset_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Dread(lua_State *L)
{
  hid_t dset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t mem_type_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  hid_t mem_space_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t file_space_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  void *buf = lua_touserdata(L, 6); luaL_checktype(L, 6, LUA_TUSERDATA);
  herr_t res = H5Dread(dset_id, mem_type_id, mem_space_id, file_space_id, plist_id, buf);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Dwrite(lua_State *L)
{
  hid_t dset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t mem_type_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  hid_t mem_space_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t file_space_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  const void *buf = lua_touserdata(L, 6); luaL_checktype(L, 6, LUA_TUSERDATA);
  herr_t res = H5Dwrite(dset_id, mem_type_id, mem_space_id, file_space_id, plist_id, buf);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Dvlen_reclaim(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  void *buf = lua_touserdata(L, 4); luaL_checktype(L, 4, LUA_TUSERDATA);
  herr_t res = H5Dvlen_reclaim(type_id, space_id, plist_id, buf);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Dvlen_get_buf_size(lua_State *L)
{
  hid_t dataset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hsize_t *size = (hsize_t*) luaL_checkudata(L, 4, "HDF5::hsize_t_arr");
  herr_t res = H5Dvlen_get_buf_size(dataset_id, type_id, space_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Dfill(lua_State *L)
{
  const void *fill = lua_touserdata(L, 1); luaL_checktype(L, 1, LUA_TUSERDATA);
  hid_t fill_type = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  void *buf = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  hid_t buf_type = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t space = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  herr_t res = H5Dfill(fill, fill_type, buf, buf_type, space);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Dset_extent(lua_State *L)
{
  hid_t dset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const hsize_t *size = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  herr_t res = H5Dset_extent(dset_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Ddebug(lua_State *L)
{
  hid_t dset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Ddebug(dset_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Dcreate1(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t dcpl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t res = H5Dcreate1(file_id, name, type_id, space_id, dcpl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Dopen1(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t res = H5Dopen1(file_id, name);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Dextend(lua_State *L)
{
  hid_t dset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const hsize_t *size = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  herr_t res = H5Dextend(dset_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static luaL_Reg H5D_funcs[] = {
  {"H5Dcreate2", _H5Dcreate2},
  {"H5Dcreate_anon", _H5Dcreate_anon},
  {"H5Dopen2", _H5Dopen2},
  {"H5Dclose", _H5Dclose},
  {"H5Dget_space", _H5Dget_space},
  {"H5Dget_type", _H5Dget_type},
  {"H5Dget_create_plist", _H5Dget_create_plist},
  {"H5Dget_access_plist", _H5Dget_access_plist},
  {"H5Dread", _H5Dread},
  {"H5Dwrite", _H5Dwrite},
  {"H5Dvlen_reclaim", _H5Dvlen_reclaim},
  {"H5Dvlen_get_buf_size", _H5Dvlen_get_buf_size},
  {"H5Dfill", _H5Dfill},
  {"H5Dset_extent", _H5Dset_extent},
  {"H5Ddebug", _H5Ddebug},
  {"H5Dcreate1", _H5Dcreate1},
  {"H5Dopen1", _H5Dopen1},
  {"H5Dextend", _H5Dextend},
  {NULL,NULL}};

static int _H5Eregister_class(lua_State *L)
{
  const char *cls_name = luaL_checkstring(L, 1);
  const char *lib_name = luaL_checkstring(L, 2);
  const char *version = luaL_checkstring(L, 3);
  hid_t res = H5Eregister_class(cls_name, lib_name, version);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Eunregister_class(lua_State *L)
{
  hid_t class_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Eunregister_class(class_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Eclose_msg(lua_State *L)
{
  hid_t err_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Eclose_msg(err_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Eclose_stack(lua_State *L)
{
  hid_t stack_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Eclose_stack(stack_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Eset_current_stack(lua_State *L)
{
  hid_t err_stack_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Eset_current_stack(err_stack_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Epop(lua_State *L)
{
  hid_t err_stack = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t count = luaL_checkunsigned(L, 2);
  herr_t res = H5Epop(err_stack, count);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Eclear2(lua_State *L)
{
  hid_t err_stack = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Eclear2(err_stack);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Eget_num(lua_State *L)
{
  hid_t error_stack_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  ssize_t res = H5Eget_num(error_stack_id);
  lua_pushnumber(L, res);
  return 1;
}
static luaL_Reg H5E_funcs[] = {
  {"H5Eregister_class", _H5Eregister_class},
  {"H5Eunregister_class", _H5Eunregister_class},
  {"H5Eclose_msg", _H5Eclose_msg},
  {"H5Eclose_stack", _H5Eclose_stack},
  {"H5Eset_current_stack", _H5Eset_current_stack},
  {"H5Epop", _H5Epop},
  {"H5Eclear2", _H5Eclear2},
  {"H5Eget_num", _H5Eget_num},
  {NULL,NULL}};

static int _H5Fis_hdf5(lua_State *L)
{
  const char *filename = luaL_checkstring(L, 1);
  htri_t res = H5Fis_hdf5(filename);
  lua_pushboolean(L, res);
  return 1;
}
static int _H5Fcreate(lua_State *L)
{
  const char *filename = luaL_checkstring(L, 1);
  unsigned flags = luaL_checkunsigned(L, 2);
  hid_t create_plist = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t access_plist = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t res = H5Fcreate(filename, flags, create_plist, access_plist);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Fopen(lua_State *L)
{
  const char *filename = luaL_checkstring(L, 1);
  unsigned flags = luaL_checkunsigned(L, 2);
  hid_t access_plist = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t res = H5Fopen(filename, flags, access_plist);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Freopen(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Freopen(file_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Fclose(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Fclose(file_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Fget_create_plist(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Fget_create_plist(file_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Fget_access_plist(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Fget_access_plist(file_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Fget_obj_count(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned types = luaL_checkunsigned(L, 2);
  ssize_t res = H5Fget_obj_count(file_id, types);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Fmount(lua_State *L)
{
  hid_t loc = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t child = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t plist = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  herr_t res = H5Fmount(loc, name, child, plist);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Funmount(lua_State *L)
{
  hid_t loc = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  herr_t res = H5Funmount(loc, name);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Fget_freespace(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hssize_t res = H5Fget_freespace(file_id);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Fget_filesize(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t *size = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  herr_t res = H5Fget_filesize(file_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Fget_file_image(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  void *buf_ptr = lua_touserdata(L, 2); luaL_checktype(L, 2, LUA_TUSERDATA);
  size_t buf_len = luaL_checkunsigned(L, 3);
  ssize_t res = H5Fget_file_image(file_id, buf_ptr, buf_len);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Freset_mdc_hit_rate_stats(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Freset_mdc_hit_rate_stats(file_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Fclear_elink_file_cache(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Fclear_elink_file_cache(file_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static luaL_Reg H5F_funcs[] = {
  {"H5Fis_hdf5", _H5Fis_hdf5},
  {"H5Fcreate", _H5Fcreate},
  {"H5Fopen", _H5Fopen},
  {"H5Freopen", _H5Freopen},
  {"H5Fclose", _H5Fclose},
  {"H5Fget_create_plist", _H5Fget_create_plist},
  {"H5Fget_access_plist", _H5Fget_access_plist},
  {"H5Fget_obj_count", _H5Fget_obj_count},
  {"H5Fmount", _H5Fmount},
  {"H5Funmount", _H5Funmount},
  {"H5Fget_freespace", _H5Fget_freespace},
  {"H5Fget_filesize", _H5Fget_filesize},
  {"H5Fget_file_image", _H5Fget_file_image},
  {"H5Freset_mdc_hit_rate_stats", _H5Freset_mdc_hit_rate_stats},
  {"H5Fclear_elink_file_cache", _H5Fclear_elink_file_cache},
  {NULL,NULL}};

static int _H5Gcreate2(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t lcpl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t gcpl_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t gapl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t res = H5Gcreate2(loc_id, name, lcpl_id, gcpl_id, gapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Gcreate_anon(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t gcpl_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  hid_t gapl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t res = H5Gcreate_anon(loc_id, gcpl_id, gapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Gopen2(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t gapl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t res = H5Gopen2(loc_id, name, gapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Gget_create_plist(lua_State *L)
{
  hid_t group_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Gget_create_plist(group_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Gclose(lua_State *L)
{
  hid_t group_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Gclose(group_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Gcreate1(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  size_t size_hint = luaL_checkunsigned(L, 3);
  hid_t res = H5Gcreate1(loc_id, name, size_hint);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Gopen1(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t res = H5Gopen1(loc_id, name);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Gmove(lua_State *L)
{
  hid_t src_loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *src_name = luaL_checkstring(L, 2);
  const char *dst_name = luaL_checkstring(L, 3);
  herr_t res = H5Gmove(src_loc_id, src_name, dst_name);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Gmove2(lua_State *L)
{
  hid_t src_loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *src_name = luaL_checkstring(L, 2);
  hid_t dst_loc_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  const char *dst_name = luaL_checkstring(L, 4);
  herr_t res = H5Gmove2(src_loc_id, src_name, dst_loc_id, dst_name);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Gunlink(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  herr_t res = H5Gunlink(loc_id, name);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Gset_comment(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  const char *comment = luaL_checkstring(L, 3);
  herr_t res = H5Gset_comment(loc_id, name, comment);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Gget_num_objs(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t *num_objs = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  herr_t res = H5Gget_num_objs(loc_id, num_objs);
  lh5_push_herr_t(L, res);
  return 1;
}
static luaL_Reg H5G_funcs[] = {
  {"H5Gcreate2", _H5Gcreate2},
  {"H5Gcreate_anon", _H5Gcreate_anon},
  {"H5Gopen2", _H5Gopen2},
  {"H5Gget_create_plist", _H5Gget_create_plist},
  {"H5Gclose", _H5Gclose},
  {"H5Gcreate1", _H5Gcreate1},
  {"H5Gopen1", _H5Gopen1},
  {"H5Gmove", _H5Gmove},
  {"H5Gmove2", _H5Gmove2},
  {"H5Gunlink", _H5Gunlink},
  {"H5Gset_comment", _H5Gset_comment},
  {"H5Gget_num_objs", _H5Gget_num_objs},
  {NULL,NULL}};

static int _H5Iget_file_id(lua_State *L)
{
  hid_t id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Iget_file_id(id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Iinc_ref(lua_State *L)
{
  hid_t id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int res = H5Iinc_ref(id);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Idec_ref(lua_State *L)
{
  hid_t id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int res = H5Idec_ref(id);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Iget_ref(lua_State *L)
{
  hid_t id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int res = H5Iget_ref(id);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Iis_valid(lua_State *L)
{
  hid_t id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  htri_t res = H5Iis_valid(id);
  lua_pushboolean(L, res);
  return 1;
}
static luaL_Reg H5I_funcs[] = {
  {"H5Iget_file_id", _H5Iget_file_id},
  {"H5Iinc_ref", _H5Iinc_ref},
  {"H5Idec_ref", _H5Idec_ref},
  {"H5Iget_ref", _H5Iget_ref},
  {"H5Iis_valid", _H5Iis_valid},
  {NULL,NULL}};

static int _H5Lmove(lua_State *L)
{
  hid_t src_loc = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *src_name = luaL_checkstring(L, 2);
  hid_t dst_loc = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  const char *dst_name = luaL_checkstring(L, 4);
  hid_t lcpl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 6, "HDF5::hid_t"));
  herr_t res = H5Lmove(src_loc, src_name, dst_loc, dst_name, lcpl_id, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Lcopy(lua_State *L)
{
  hid_t src_loc = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *src_name = luaL_checkstring(L, 2);
  hid_t dst_loc = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  const char *dst_name = luaL_checkstring(L, 4);
  hid_t lcpl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 6, "HDF5::hid_t"));
  herr_t res = H5Lcopy(src_loc, src_name, dst_loc, dst_name, lcpl_id, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Lcreate_hard(lua_State *L)
{
  hid_t cur_loc = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *cur_name = luaL_checkstring(L, 2);
  hid_t dst_loc = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  const char *dst_name = luaL_checkstring(L, 4);
  hid_t lcpl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 6, "HDF5::hid_t"));
  herr_t res = H5Lcreate_hard(cur_loc, cur_name, dst_loc, dst_name, lcpl_id, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Lcreate_soft(lua_State *L)
{
  const char *link_target = luaL_checkstring(L, 1);
  hid_t link_loc_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  const char *link_name = luaL_checkstring(L, 3);
  hid_t lcpl_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  herr_t res = H5Lcreate_soft(link_target, link_loc_id, link_name, lcpl_id, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Ldelete(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  herr_t res = H5Ldelete(loc_id, name, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Lget_val(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  void *buf = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  size_t size = luaL_checkunsigned(L, 4);
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  herr_t res = H5Lget_val(loc_id, name, buf, size, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Lexists(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  htri_t res = H5Lexists(loc_id, name, lapl_id);
  lua_pushboolean(L, res);
  return 1;
}
static int _H5Lcreate_external(lua_State *L)
{
  const char *file_name = luaL_checkstring(L, 1);
  const char *obj_name = luaL_checkstring(L, 2);
  hid_t link_loc_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  const char *link_name = luaL_checkstring(L, 4);
  hid_t lcpl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 6, "HDF5::hid_t"));
  herr_t res = H5Lcreate_external(file_name, obj_name, link_loc_id, link_name, lcpl_id, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static luaL_Reg H5L_funcs[] = {
  {"H5Lmove", _H5Lmove},
  {"H5Lcopy", _H5Lcopy},
  {"H5Lcreate_hard", _H5Lcreate_hard},
  {"H5Lcreate_soft", _H5Lcreate_soft},
  {"H5Ldelete", _H5Ldelete},
  {"H5Lget_val", _H5Lget_val},
  {"H5Lexists", _H5Lexists},
  {"H5Lcreate_external", _H5Lcreate_external},
  {"H5Literate", _H5Literate},
  {NULL,NULL}};

static int _H5Oopen(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t res = H5Oopen(loc_id, name, lapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Oexists_by_name(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  htri_t res = H5Oexists_by_name(loc_id, name, lapl_id);
  lua_pushboolean(L, res);
  return 1;
}
static int _H5Oget_info(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  H5O_info_t *oinfo = (H5O_info_t*) luaL_checkudata(L, 2, "HDF5::H5O_info_t");
  herr_t res = H5Oget_info(loc_id, oinfo);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Oget_info_by_name(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  H5O_info_t *oinfo = (H5O_info_t*) luaL_checkudata(L, 3, "HDF5::H5O_info_t");
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  herr_t res = H5Oget_info_by_name(loc_id, name, oinfo, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Olink(lua_State *L)
{
  hid_t obj_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t new_loc_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  const char *new_name = luaL_checkstring(L, 3);
  hid_t lcpl_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  herr_t res = H5Olink(obj_id, new_loc_id, new_name, lcpl_id, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Oincr_refcount(lua_State *L)
{
  hid_t object_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Oincr_refcount(object_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Odecr_refcount(lua_State *L)
{
  hid_t object_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Odecr_refcount(object_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Ocopy(lua_State *L)
{
  hid_t src_loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *src_name = luaL_checkstring(L, 2);
  hid_t dst_loc_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  const char *dst_name = luaL_checkstring(L, 4);
  hid_t ocpypl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t lcpl_id = *((hid_t*) luaL_checkudata(L, 6, "HDF5::hid_t"));
  herr_t res = H5Ocopy(src_loc_id, src_name, dst_loc_id, dst_name, ocpypl_id, lcpl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Oset_comment(lua_State *L)
{
  hid_t obj_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *comment = luaL_checkstring(L, 2);
  herr_t res = H5Oset_comment(obj_id, comment);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Oset_comment_by_name(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  const char *comment = luaL_checkstring(L, 3);
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  herr_t res = H5Oset_comment_by_name(loc_id, name, comment, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Oclose(lua_State *L)
{
  hid_t object_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Oclose(object_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static luaL_Reg H5O_funcs[] = {
  {"H5Oopen", _H5Oopen},
  {"H5Oexists_by_name", _H5Oexists_by_name},
  {"H5Oget_info", _H5Oget_info},
  {"H5Oget_info_by_name", _H5Oget_info_by_name},
  {"H5Olink", _H5Olink},
  {"H5Oincr_refcount", _H5Oincr_refcount},
  {"H5Odecr_refcount", _H5Odecr_refcount},
  {"H5Ocopy", _H5Ocopy},
  {"H5Oset_comment", _H5Oset_comment},
  {"H5Oset_comment_by_name", _H5Oset_comment_by_name},
  {"H5Oclose", _H5Oclose},
  {NULL,NULL}};

static int _H5Pcreate(lua_State *L)
{
  hid_t cls_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Pcreate(cls_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Pset(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  void *value = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  herr_t res = H5Pset(plist_id, name, value);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pexist(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  htri_t res = H5Pexist(plist_id, name);
  lua_pushboolean(L, res);
  return 1;
}
static int _H5Pget_class(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Pget_class(plist_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Pget_class_parent(lua_State *L)
{
  hid_t pclass_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Pget_class_parent(pclass_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Pget(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  void *value = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  herr_t res = H5Pget(plist_id, name, value);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pequal(lua_State *L)
{
  hid_t id1 = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t id2 = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  htri_t res = H5Pequal(id1, id2);
  lua_pushboolean(L, res);
  return 1;
}
static int _H5Pisa_class(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t pclass_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  htri_t res = H5Pisa_class(plist_id, pclass_id);
  lua_pushboolean(L, res);
  return 1;
}
static int _H5Pcopy_prop(lua_State *L)
{
  hid_t dst_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t src_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 3);
  herr_t res = H5Pcopy_prop(dst_id, src_id, name);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Premove(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  herr_t res = H5Premove(plist_id, name);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Punregister(lua_State *L)
{
  hid_t pclass_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  herr_t res = H5Punregister(pclass_id, name);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pclose_class(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Pclose_class(plist_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pclose(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Pclose(plist_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pcopy(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Pcopy(plist_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Pset_attr_phase_change(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned max_compact = luaL_checkunsigned(L, 2);
  unsigned min_dense = luaL_checkunsigned(L, 3);
  herr_t res = H5Pset_attr_phase_change(plist_id, max_compact, min_dense);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_attr_creation_order(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned crt_order_flags = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_attr_creation_order(plist_id, crt_order_flags);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pget_nfilters(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int res = H5Pget_nfilters(plist_id);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Pall_filters_avail(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  htri_t res = H5Pall_filters_avail(plist_id);
  lua_pushboolean(L, res);
  return 1;
}
static int _H5Pset_deflate(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned aggression = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_deflate(plist_id, aggression);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_fletcher32(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Pset_fletcher32(plist_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_userblock(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t size = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_userblock(plist_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pget_userblock(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t *size = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  herr_t res = H5Pget_userblock(plist_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_sizes(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t sizeof_addr = luaL_checkunsigned(L, 2);
  size_t sizeof_size = luaL_checkunsigned(L, 3);
  herr_t res = H5Pset_sizes(plist_id, sizeof_addr, sizeof_size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_sym_k(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned ik = luaL_checkunsigned(L, 2);
  unsigned lk = luaL_checkunsigned(L, 3);
  herr_t res = H5Pset_sym_k(plist_id, ik, lk);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_istore_k(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned ik = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_istore_k(plist_id, ik);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_shared_mesg_nindexes(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned nindexes = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_shared_mesg_nindexes(plist_id, nindexes);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_shared_mesg_index(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned index_num = luaL_checkunsigned(L, 2);
  unsigned mesg_type_flags = luaL_checkunsigned(L, 3);
  unsigned min_mesg_size = luaL_checkunsigned(L, 4);
  herr_t res = H5Pset_shared_mesg_index(plist_id, index_num, mesg_type_flags, min_mesg_size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_shared_mesg_phase_change(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned max_list = luaL_checkunsigned(L, 2);
  unsigned min_btree = luaL_checkunsigned(L, 3);
  herr_t res = H5Pset_shared_mesg_phase_change(plist_id, max_list, min_btree);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_alignment(lua_State *L)
{
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t threshold = luaL_checkunsigned(L, 2);
  hsize_t alignment = luaL_checkunsigned(L, 3);
  herr_t res = H5Pset_alignment(fapl_id, threshold, alignment);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pget_alignment(lua_State *L)
{
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t *threshold = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  hsize_t *alignment = (hsize_t*) luaL_checkudata(L, 3, "HDF5::hsize_t_arr");
  herr_t res = H5Pget_alignment(fapl_id, threshold, alignment);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_driver(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t driver_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  const void *driver_info = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  herr_t res = H5Pset_driver(plist_id, driver_id, driver_info);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pget_driver(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Pget_driver(plist_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Pset_family_offset(lua_State *L)
{
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t offset = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_family_offset(fapl_id, offset);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pget_family_offset(lua_State *L)
{
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t *offset = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  herr_t res = H5Pget_family_offset(fapl_id, offset);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_gc_references(lua_State *L)
{
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned gc_ref = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_gc_references(fapl_id, gc_ref);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_meta_block_size(lua_State *L)
{
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t size = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_meta_block_size(fapl_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pget_meta_block_size(lua_State *L)
{
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t *size = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  herr_t res = H5Pget_meta_block_size(fapl_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_sieve_buf_size(lua_State *L)
{
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t size = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_sieve_buf_size(fapl_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_small_data_block_size(lua_State *L)
{
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t size = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_small_data_block_size(fapl_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pget_small_data_block_size(lua_State *L)
{
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t *size = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  herr_t res = H5Pget_small_data_block_size(fapl_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_elink_file_cache_size(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned efc_size = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_elink_file_cache_size(plist_id, efc_size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_file_image(lua_State *L)
{
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  void *buf_ptr = lua_touserdata(L, 2); luaL_checktype(L, 2, LUA_TUSERDATA);
  size_t buf_len = luaL_checkunsigned(L, 3);
  herr_t res = H5Pset_file_image(fapl_id, buf_ptr, buf_len);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pget_layout(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  H5D_layout_t res = H5Pget_layout(plist_id);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Pset_chunk(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int ndims = luaL_checkinteger(L, 2);
  const hsize_t *dim = (hsize_t*) luaL_checkudata(L, 3, "HDF5::hsize_t_arr");
  herr_t res = H5Pset_chunk(plist_id, ndims, dim);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pget_chunk(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int max_ndims = luaL_checkinteger(L, 2);
  hsize_t *dim = (hsize_t*) luaL_checkudata(L, 3, "HDF5::hsize_t_arr");
  int res = H5Pget_chunk(plist_id, max_ndims, dim);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Pget_external_count(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int res = H5Pget_external_count(plist_id);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Pset_szip(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned options_mask = luaL_checkunsigned(L, 2);
  unsigned pixels_per_block = luaL_checkunsigned(L, 3);
  herr_t res = H5Pset_szip(plist_id, options_mask, pixels_per_block);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_shuffle(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Pset_shuffle(plist_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_nbit(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Pset_nbit(plist_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_fill_value(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  const void *value = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  herr_t res = H5Pset_fill_value(plist_id, type_id, value);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pget_fill_value(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  void *value = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  herr_t res = H5Pget_fill_value(plist_id, type_id, value);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_buffer(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t size = luaL_checkunsigned(L, 2);
  void *tconv = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  void *bkg = lua_touserdata(L, 4); luaL_checktype(L, 4, LUA_TUSERDATA);
  herr_t res = H5Pset_buffer(plist_id, size, tconv, bkg);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pget_preserve(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int res = H5Pget_preserve(plist_id);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Pset_hyper_vector_size(lua_State *L)
{
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t size = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_hyper_vector_size(fapl_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_create_intermediate_group(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned crt_intmd = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_create_intermediate_group(plist_id, crt_intmd);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_local_heap_size_hint(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t size_hint = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_local_heap_size_hint(plist_id, size_hint);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_link_phase_change(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned max_compact = luaL_checkunsigned(L, 2);
  unsigned min_dense = luaL_checkunsigned(L, 3);
  herr_t res = H5Pset_link_phase_change(plist_id, max_compact, min_dense);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_est_link_info(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned est_num_entries = luaL_checkunsigned(L, 2);
  unsigned est_name_len = luaL_checkunsigned(L, 3);
  herr_t res = H5Pset_est_link_info(plist_id, est_num_entries, est_name_len);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_link_creation_order(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned crt_order_flags = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_link_creation_order(plist_id, crt_order_flags);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_nlinks(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t nlinks = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_nlinks(plist_id, nlinks);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_elink_prefix(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *prefix = luaL_checkstring(L, 2);
  herr_t res = H5Pset_elink_prefix(plist_id, prefix);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pget_elink_fapl(lua_State *L)
{
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Pget_elink_fapl(lapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Pset_elink_fapl(lua_State *L)
{
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  herr_t res = H5Pset_elink_fapl(lapl_id, fapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_elink_acc_flags(lua_State *L)
{
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned flags = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_elink_acc_flags(lapl_id, flags);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pset_copy_object(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned crt_intmd = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_copy_object(plist_id, crt_intmd);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Padd_merge_committed_dtype_path(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *path = luaL_checkstring(L, 2);
  herr_t res = H5Padd_merge_committed_dtype_path(plist_id, path);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Pfree_merge_committed_dtype_paths(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Pfree_merge_committed_dtype_paths(plist_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static luaL_Reg H5P_funcs[] = {
  {"H5Pcreate", _H5Pcreate},
  {"H5Pset", _H5Pset},
  {"H5Pexist", _H5Pexist},
  {"H5Pget_class", _H5Pget_class},
  {"H5Pget_class_parent", _H5Pget_class_parent},
  {"H5Pget", _H5Pget},
  {"H5Pequal", _H5Pequal},
  {"H5Pisa_class", _H5Pisa_class},
  {"H5Pcopy_prop", _H5Pcopy_prop},
  {"H5Premove", _H5Premove},
  {"H5Punregister", _H5Punregister},
  {"H5Pclose_class", _H5Pclose_class},
  {"H5Pclose", _H5Pclose},
  {"H5Pcopy", _H5Pcopy},
  {"H5Pset_attr_phase_change", _H5Pset_attr_phase_change},
  {"H5Pset_attr_creation_order", _H5Pset_attr_creation_order},
  {"H5Pget_nfilters", _H5Pget_nfilters},
  {"H5Pall_filters_avail", _H5Pall_filters_avail},
  {"H5Pset_deflate", _H5Pset_deflate},
  {"H5Pset_fletcher32", _H5Pset_fletcher32},
  {"H5Pset_userblock", _H5Pset_userblock},
  {"H5Pget_userblock", _H5Pget_userblock},
  {"H5Pset_sizes", _H5Pset_sizes},
  {"H5Pset_sym_k", _H5Pset_sym_k},
  {"H5Pset_istore_k", _H5Pset_istore_k},
  {"H5Pset_shared_mesg_nindexes", _H5Pset_shared_mesg_nindexes},
  {"H5Pset_shared_mesg_index", _H5Pset_shared_mesg_index},
  {"H5Pset_shared_mesg_phase_change", _H5Pset_shared_mesg_phase_change},
  {"H5Pset_alignment", _H5Pset_alignment},
  {"H5Pget_alignment", _H5Pget_alignment},
  {"H5Pset_driver", _H5Pset_driver},
  {"H5Pget_driver", _H5Pget_driver},
  {"H5Pset_family_offset", _H5Pset_family_offset},
  {"H5Pget_family_offset", _H5Pget_family_offset},
  {"H5Pset_gc_references", _H5Pset_gc_references},
  {"H5Pset_meta_block_size", _H5Pset_meta_block_size},
  {"H5Pget_meta_block_size", _H5Pget_meta_block_size},
  {"H5Pset_sieve_buf_size", _H5Pset_sieve_buf_size},
  {"H5Pset_small_data_block_size", _H5Pset_small_data_block_size},
  {"H5Pget_small_data_block_size", _H5Pget_small_data_block_size},
  {"H5Pset_elink_file_cache_size", _H5Pset_elink_file_cache_size},
  {"H5Pset_file_image", _H5Pset_file_image},
  {"H5Pget_layout", _H5Pget_layout},
  {"H5Pset_chunk", _H5Pset_chunk},
  {"H5Pget_chunk", _H5Pget_chunk},
  {"H5Pget_external_count", _H5Pget_external_count},
  {"H5Pset_szip", _H5Pset_szip},
  {"H5Pset_shuffle", _H5Pset_shuffle},
  {"H5Pset_nbit", _H5Pset_nbit},
  {"H5Pset_fill_value", _H5Pset_fill_value},
  {"H5Pget_fill_value", _H5Pget_fill_value},
  {"H5Pset_buffer", _H5Pset_buffer},
  {"H5Pget_preserve", _H5Pget_preserve},
  {"H5Pset_hyper_vector_size", _H5Pset_hyper_vector_size},
  {"H5Pset_create_intermediate_group", _H5Pset_create_intermediate_group},
  {"H5Pset_local_heap_size_hint", _H5Pset_local_heap_size_hint},
  {"H5Pset_link_phase_change", _H5Pset_link_phase_change},
  {"H5Pset_est_link_info", _H5Pset_est_link_info},
  {"H5Pset_link_creation_order", _H5Pset_link_creation_order},
  {"H5Pset_nlinks", _H5Pset_nlinks},
  {"H5Pset_elink_prefix", _H5Pset_elink_prefix},
  {"H5Pget_elink_fapl", _H5Pget_elink_fapl},
  {"H5Pset_elink_fapl", _H5Pset_elink_fapl},
  {"H5Pset_elink_acc_flags", _H5Pset_elink_acc_flags},
  {"H5Pset_copy_object", _H5Pset_copy_object},
  {"H5Padd_merge_committed_dtype_path", _H5Padd_merge_committed_dtype_path},
  {"H5Pfree_merge_committed_dtype_paths", _H5Pfree_merge_committed_dtype_paths},
  {"H5Pget_mpio_actual_chunk_opt_mode", _H5Pget_mpio_actual_chunk_opt_mode},
  {"H5Pget_mpio_actual_io_mode", _H5Pget_mpio_actual_io_mode},
  {"H5Pget_mpio_no_collective_cause", _H5Pget_mpio_no_collective_cause},
  {NULL,NULL}};

static int _H5Rcreate(lua_State *L)
{
  void *ref = lua_touserdata(L, 1); luaL_checktype(L, 1, LUA_TUSERDATA);
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 3);
  int ref_type = luaL_checkinteger(L, 4);
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  herr_t res = H5Rcreate(ref, loc_id, name, ref_type, space_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Rdereference(lua_State *L)
{
  hid_t dataset = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int ref_type = luaL_checkinteger(L, 2);
  const void *ref = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  hid_t res = H5Rdereference(dataset, ref_type, ref);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Rget_region(lua_State *L)
{
  hid_t dataset = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int ref_type = luaL_checkinteger(L, 2);
  const void *ref = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  hid_t res = H5Rget_region(dataset, ref_type, ref);
  lh5_push_hid_t(L, res);
  return 1;
}
static luaL_Reg H5R_funcs[] = {
  {"H5Rcreate", _H5Rcreate},
  {"H5Rdereference", _H5Rdereference},
  {"H5Rget_region", _H5Rget_region},
  {NULL,NULL}};

static int _H5Screate(lua_State *L)
{
  int type = luaL_checkinteger(L, 1);
  hid_t res = H5Screate(type);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Screate_simple(lua_State *L)
{
  int rank = luaL_checkinteger(L, 1);
  const hsize_t *dims = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  const hsize_t *maxdims = (hsize_t*) luaL_checkudata(L, 3, "HDF5::hsize_t_arr");
  hid_t res = H5Screate_simple(rank, dims, maxdims);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Sset_extent_simple(lua_State *L)
{
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int rank = luaL_checkinteger(L, 2);
  const hsize_t *dims = (hsize_t*) luaL_checkudata(L, 3, "HDF5::hsize_t_arr");
  const hsize_t *max = (hsize_t*) luaL_checkudata(L, 4, "HDF5::hsize_t_arr");
  herr_t res = H5Sset_extent_simple(space_id, rank, dims, max);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Scopy(lua_State *L)
{
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Scopy(space_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Sclose(lua_State *L)
{
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Sclose(space_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Sdecode(lua_State *L)
{
  const void *buf = lua_touserdata(L, 1); luaL_checktype(L, 1, LUA_TUSERDATA);
  hid_t res = H5Sdecode(buf);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Sget_simple_extent_npoints(lua_State *L)
{
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hssize_t res = H5Sget_simple_extent_npoints(space_id);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Sget_simple_extent_ndims(lua_State *L)
{
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int res = H5Sget_simple_extent_ndims(space_id);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Sget_simple_extent_dims(lua_State *L)
{
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t *dims = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  hsize_t *maxdims = (hsize_t*) luaL_checkudata(L, 3, "HDF5::hsize_t_arr");
  int res = H5Sget_simple_extent_dims(space_id, dims, maxdims);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Sis_simple(lua_State *L)
{
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  htri_t res = H5Sis_simple(space_id);
  lua_pushboolean(L, res);
  return 1;
}
static int _H5Sget_select_npoints(lua_State *L)
{
  hid_t spaceid = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hssize_t res = H5Sget_select_npoints(spaceid);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Sselect_hyperslab(lua_State *L)
{
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int op = luaL_checkinteger(L, 2);
  const hsize_t *start = (hsize_t*) luaL_checkudata(L, 3, "HDF5::hsize_t_arr");
  const hsize_t *_stride = (hsize_t*) luaL_checkudata(L, 4, "HDF5::hsize_t_arr");
  const hsize_t *count = (hsize_t*) luaL_checkudata(L, 5, "HDF5::hsize_t_arr");
  const hsize_t *_block = (hsize_t*) luaL_checkudata(L, 6, "HDF5::hsize_t_arr");
  herr_t res = H5Sselect_hyperslab(space_id, op, start, _stride, count, _block);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Sselect_elements(lua_State *L)
{
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int op = luaL_checkinteger(L, 2);
  size_t num_elem = luaL_checkunsigned(L, 3);
  const hsize_t *coord = (hsize_t*) luaL_checkudata(L, 4, "HDF5::hsize_t_arr");
  herr_t res = H5Sselect_elements(space_id, op, num_elem, coord);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Sset_extent_none(lua_State *L)
{
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Sset_extent_none(space_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Sextent_copy(lua_State *L)
{
  hid_t dst_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t src_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  herr_t res = H5Sextent_copy(dst_id, src_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Sextent_equal(lua_State *L)
{
  hid_t sid1 = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t sid2 = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  htri_t res = H5Sextent_equal(sid1, sid2);
  lua_pushboolean(L, res);
  return 1;
}
static int _H5Sselect_all(lua_State *L)
{
  hid_t spaceid = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Sselect_all(spaceid);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Sselect_none(lua_State *L)
{
  hid_t spaceid = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Sselect_none(spaceid);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Sselect_valid(lua_State *L)
{
  hid_t spaceid = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  htri_t res = H5Sselect_valid(spaceid);
  lua_pushboolean(L, res);
  return 1;
}
static int _H5Sget_select_hyper_nblocks(lua_State *L)
{
  hid_t spaceid = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hssize_t res = H5Sget_select_hyper_nblocks(spaceid);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Sget_select_elem_npoints(lua_State *L)
{
  hid_t spaceid = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hssize_t res = H5Sget_select_elem_npoints(spaceid);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Sget_select_hyper_blocklist(lua_State *L)
{
  hid_t spaceid = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t startblock = luaL_checkunsigned(L, 2);
  hsize_t numblocks = luaL_checkunsigned(L, 3);
  hsize_t *buf = (hsize_t*) luaL_checkudata(L, 4, "HDF5::hsize_t_arr");
  herr_t res = H5Sget_select_hyper_blocklist(spaceid, startblock, numblocks, buf);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Sget_select_elem_pointlist(lua_State *L)
{
  hid_t spaceid = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t startpoint = luaL_checkunsigned(L, 2);
  hsize_t numpoints = luaL_checkunsigned(L, 3);
  hsize_t *buf = (hsize_t*) luaL_checkudata(L, 4, "HDF5::hsize_t_arr");
  herr_t res = H5Sget_select_elem_pointlist(spaceid, startpoint, numpoints, buf);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Sget_select_bounds(lua_State *L)
{
  hid_t spaceid = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t *start = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  hsize_t *end = (hsize_t*) luaL_checkudata(L, 3, "HDF5::hsize_t_arr");
  herr_t res = H5Sget_select_bounds(spaceid, start, end);
  lh5_push_herr_t(L, res);
  return 1;
}
static luaL_Reg H5S_funcs[] = {
  {"H5Screate", _H5Screate},
  {"H5Screate_simple", _H5Screate_simple},
  {"H5Sset_extent_simple", _H5Sset_extent_simple},
  {"H5Scopy", _H5Scopy},
  {"H5Sclose", _H5Sclose},
  {"H5Sdecode", _H5Sdecode},
  {"H5Sget_simple_extent_npoints", _H5Sget_simple_extent_npoints},
  {"H5Sget_simple_extent_ndims", _H5Sget_simple_extent_ndims},
  {"H5Sget_simple_extent_dims", _H5Sget_simple_extent_dims},
  {"H5Sis_simple", _H5Sis_simple},
  {"H5Sget_select_npoints", _H5Sget_select_npoints},
  {"H5Sselect_hyperslab", _H5Sselect_hyperslab},
  {"H5Sselect_elements", _H5Sselect_elements},
  {"H5Sset_extent_none", _H5Sset_extent_none},
  {"H5Sextent_copy", _H5Sextent_copy},
  {"H5Sextent_equal", _H5Sextent_equal},
  {"H5Sselect_all", _H5Sselect_all},
  {"H5Sselect_none", _H5Sselect_none},
  {"H5Sselect_valid", _H5Sselect_valid},
  {"H5Sget_select_hyper_nblocks", _H5Sget_select_hyper_nblocks},
  {"H5Sget_select_elem_npoints", _H5Sget_select_elem_npoints},
  {"H5Sget_select_hyper_blocklist", _H5Sget_select_hyper_blocklist},
  {"H5Sget_select_elem_pointlist", _H5Sget_select_elem_pointlist},
  {"H5Sget_select_bounds", _H5Sget_select_bounds},
  {NULL,NULL}};

static int _H5Tcreate(lua_State *L)
{
  H5T_class_t type = luaL_checkinteger(L, 1);
  size_t size = luaL_checkunsigned(L, 2);
  hid_t res = H5Tcreate(type, size);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Tcopy(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Tcopy(type_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Tclose(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Tclose(type_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Tequal(lua_State *L)
{
  hid_t type1_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t type2_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  htri_t res = H5Tequal(type1_id, type2_id);
  lua_pushboolean(L, res);
  return 1;
}
static int _H5Tlock(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Tlock(type_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Tcommit2(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t lcpl_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t tcpl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t tapl_id = *((hid_t*) luaL_checkudata(L, 6, "HDF5::hid_t"));
  herr_t res = H5Tcommit2(loc_id, name, type_id, lcpl_id, tcpl_id, tapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Topen2(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t tapl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t res = H5Topen2(loc_id, name, tapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Tcommit_anon(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  hid_t tcpl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t tapl_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  herr_t res = H5Tcommit_anon(loc_id, type_id, tcpl_id, tapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Tget_create_plist(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Tget_create_plist(type_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Tcommitted(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  htri_t res = H5Tcommitted(type_id);
  lua_pushboolean(L, res);
  return 1;
}
static int _H5Tdecode(lua_State *L)
{
  const void *buf = lua_touserdata(L, 1); luaL_checktype(L, 1, LUA_TUSERDATA);
  hid_t res = H5Tdecode(buf);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Tinsert(lua_State *L)
{
  hid_t parent_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  size_t offset = luaL_checkunsigned(L, 3);
  hid_t member_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  herr_t res = H5Tinsert(parent_id, name, offset, member_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Tpack(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Tpack(type_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Tenum_create(lua_State *L)
{
  hid_t base_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Tenum_create(base_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Tenum_insert(lua_State *L)
{
  hid_t type = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  const void *value = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  herr_t res = H5Tenum_insert(type, name, value);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Tenum_valueof(lua_State *L)
{
  hid_t type = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  void *value = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  herr_t res = H5Tenum_valueof(type, name, value);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Tvlen_create(lua_State *L)
{
  hid_t base_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Tvlen_create(base_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Tarray_create2(lua_State *L)
{
  hid_t base_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned ndims = luaL_checkunsigned(L, 2);
  const hsize_t *dim = (hsize_t*) luaL_checkudata(L, 3, "HDF5::hsize_t_arr");
  hid_t res = H5Tarray_create2(base_id, ndims, dim);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Tget_array_ndims(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int res = H5Tget_array_ndims(type_id);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Tget_array_dims2(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t *dims = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  int res = H5Tget_array_dims2(type_id, dims);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Tset_tag(lua_State *L)
{
  hid_t type = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *tag = luaL_checkstring(L, 2);
  herr_t res = H5Tset_tag(type, tag);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Tget_super(lua_State *L)
{
  hid_t type = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Tget_super(type);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Tget_class(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  H5T_class_t res = H5Tget_class(type_id);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Tdetect_class(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  H5T_class_t cls = luaL_checkinteger(L, 2);
  htri_t res = H5Tdetect_class(type_id, cls);
  lua_pushboolean(L, res);
  return 1;
}
static int _H5Tget_size(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t res = H5Tget_size(type_id);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Tget_precision(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t res = H5Tget_precision(type_id);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Tget_offset(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int res = H5Tget_offset(type_id);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Tget_ebias(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t res = H5Tget_ebias(type_id);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Tget_nmembers(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int res = H5Tget_nmembers(type_id);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Tget_member_index(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  int res = H5Tget_member_index(type_id, name);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Tget_member_offset(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned membno = luaL_checkunsigned(L, 2);
  size_t res = H5Tget_member_offset(type_id, membno);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Tget_member_class(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned membno = luaL_checkunsigned(L, 2);
  H5T_class_t res = H5Tget_member_class(type_id, membno);
  lua_pushnumber(L, res);
  return 1;
}
static int _H5Tget_member_type(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned membno = luaL_checkunsigned(L, 2);
  hid_t res = H5Tget_member_type(type_id, membno);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Tget_member_value(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned membno = luaL_checkunsigned(L, 2);
  void *value = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  herr_t res = H5Tget_member_value(type_id, membno, value);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Tis_variable_str(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  htri_t res = H5Tis_variable_str(type_id);
  lua_pushboolean(L, res);
  return 1;
}
static int _H5Tget_native_type(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  H5T_direction_t direction = luaL_checkinteger(L, 2);
  hid_t res = H5Tget_native_type(type_id, direction);
  lh5_push_hid_t(L, res);
  return 1;
}
static int _H5Tset_size(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t size = luaL_checkunsigned(L, 2);
  herr_t res = H5Tset_size(type_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Tset_precision(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t prec = luaL_checkunsigned(L, 2);
  herr_t res = H5Tset_precision(type_id, prec);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Tset_offset(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t offset = luaL_checkunsigned(L, 2);
  herr_t res = H5Tset_offset(type_id, offset);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Tset_fields(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t spos = luaL_checkunsigned(L, 2);
  size_t epos = luaL_checkunsigned(L, 3);
  size_t esize = luaL_checkunsigned(L, 4);
  size_t mpos = luaL_checkunsigned(L, 5);
  size_t msize = luaL_checkunsigned(L, 6);
  herr_t res = H5Tset_fields(type_id, spos, epos, esize, mpos, msize);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Tset_ebias(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t ebias = luaL_checkunsigned(L, 2);
  herr_t res = H5Tset_ebias(type_id, ebias);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Tcompiler_conv(lua_State *L)
{
  hid_t src_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t dst_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  htri_t res = H5Tcompiler_conv(src_id, dst_id);
  lua_pushboolean(L, res);
  return 1;
}
static int _H5Tconvert(lua_State *L)
{
  hid_t src_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t dst_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  size_t nelmts = luaL_checkunsigned(L, 3);
  void *buf = lua_touserdata(L, 4); luaL_checktype(L, 4, LUA_TUSERDATA);
  void *background = lua_touserdata(L, 5); luaL_checktype(L, 5, LUA_TUSERDATA);
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 6, "HDF5::hid_t"));
  herr_t res = H5Tconvert(src_id, dst_id, nelmts, buf, background, plist_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Tcommit1(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  herr_t res = H5Tcommit1(loc_id, name, type_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int _H5Topen1(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t res = H5Topen1(loc_id, name);
  lh5_push_hid_t(L, res);
  return 1;
}
static luaL_Reg H5T_funcs[] = {
  {"H5Tcreate", _H5Tcreate},
  {"H5Tcopy", _H5Tcopy},
  {"H5Tclose", _H5Tclose},
  {"H5Tequal", _H5Tequal},
  {"H5Tlock", _H5Tlock},
  {"H5Tcommit2", _H5Tcommit2},
  {"H5Topen2", _H5Topen2},
  {"H5Tcommit_anon", _H5Tcommit_anon},
  {"H5Tget_create_plist", _H5Tget_create_plist},
  {"H5Tcommitted", _H5Tcommitted},
  {"H5Tdecode", _H5Tdecode},
  {"H5Tinsert", _H5Tinsert},
  {"H5Tpack", _H5Tpack},
  {"H5Tenum_create", _H5Tenum_create},
  {"H5Tenum_insert", _H5Tenum_insert},
  {"H5Tenum_valueof", _H5Tenum_valueof},
  {"H5Tvlen_create", _H5Tvlen_create},
  {"H5Tarray_create2", _H5Tarray_create2},
  {"H5Tget_array_ndims", _H5Tget_array_ndims},
  {"H5Tget_array_dims2", _H5Tget_array_dims2},
  {"H5Tset_tag", _H5Tset_tag},
  {"H5Tget_super", _H5Tget_super},
  {"H5Tget_class", _H5Tget_class},
  {"H5Tdetect_class", _H5Tdetect_class},
  {"H5Tget_size", _H5Tget_size},
  {"H5Tget_precision", _H5Tget_precision},
  {"H5Tget_offset", _H5Tget_offset},
  {"H5Tget_ebias", _H5Tget_ebias},
  {"H5Tget_nmembers", _H5Tget_nmembers},
  {"H5Tget_member_index", _H5Tget_member_index},
  {"H5Tget_member_offset", _H5Tget_member_offset},
  {"H5Tget_member_class", _H5Tget_member_class},
  {"H5Tget_member_type", _H5Tget_member_type},
  {"H5Tget_member_value", _H5Tget_member_value},
  {"H5Tis_variable_str", _H5Tis_variable_str},
  {"H5Tget_native_type", _H5Tget_native_type},
  {"H5Tset_size", _H5Tset_size},
  {"H5Tset_precision", _H5Tset_precision},
  {"H5Tset_offset", _H5Tset_offset},
  {"H5Tset_fields", _H5Tset_fields},
  {"H5Tset_ebias", _H5Tset_ebias},
  {"H5Tcompiler_conv", _H5Tcompiler_conv},
  {"H5Tconvert", _H5Tconvert},
  {"H5Tcommit1", _H5Tcommit1},
  {"H5Topen1", _H5Topen1},
  {NULL,NULL}};

static int _H5Zregister(lua_State *L)
{
  const void *cls = lua_touserdata(L, 1); luaL_checktype(L, 1, LUA_TUSERDATA);
  herr_t res = H5Zregister(cls);
  lh5_push_herr_t(L, res);
  return 1;
}
static luaL_Reg H5Z_funcs[] = {
  {"H5Zregister", _H5Zregister},
  {NULL,NULL}};
static luaL_Reg H5P_MPI_funcs[] = {{NULL,NULL}};static void register_constants(lua_State *L)
{
#define REG_NUMBER(s) lua_pushnumber(L, s); lua_setfield(L, -2, #s)
#define REG_NUMBER2(s,t) lua_pushnumber(L, t); lua_setfield(L, -2, #s)
#define REG_HID(s) lh5_push_hid_t(L, s); lua_setfield(L, -2, #s)
  REG_HID(H5P_DEFAULT);
  REG_NUMBER(H5_VERS_RELEASE);
  REG_NUMBER(H5_ITER_UNKNOWN);
  REG_NUMBER(H5_ITER_INC);
  REG_NUMBER(H5_ITER_DEC);
  REG_NUMBER(H5_ITER_NATIVE);
  REG_NUMBER(H5_ITER_N);
  REG_NUMBER(H5_INDEX_UNKNOWN);
  REG_NUMBER(H5_INDEX_NAME);
  REG_NUMBER(H5_INDEX_CRT_ORDER);
  REG_NUMBER(H5_INDEX_N);
  REG_NUMBER(H5D_LAYOUT_ERROR);
  REG_NUMBER(H5D_COMPACT);
  REG_NUMBER(H5D_CONTIGUOUS);
  REG_NUMBER(H5D_CHUNKED);
  REG_NUMBER(H5D_NLAYOUTS);
  REG_NUMBER(H5D_CHUNK_BTREE);
  REG_NUMBER(H5D_ALLOC_TIME_ERROR);
  REG_NUMBER(H5D_ALLOC_TIME_DEFAULT);
  REG_NUMBER(H5D_ALLOC_TIME_EARLY);
  REG_NUMBER(H5D_ALLOC_TIME_LATE);
  REG_NUMBER(H5D_ALLOC_TIME_INCR);
  REG_NUMBER(H5D_SPACE_STATUS_ERROR);
  REG_NUMBER(H5D_SPACE_STATUS_NOT_ALLOCATED);
  REG_NUMBER(H5D_SPACE_STATUS_PART_ALLOCATED);
  REG_NUMBER(H5D_SPACE_STATUS_ALLOCATED);
  REG_NUMBER(H5D_FILL_TIME_ERROR);
  REG_NUMBER(H5D_FILL_TIME_ALLOC);
  REG_NUMBER(H5D_FILL_TIME_NEVER);
  REG_NUMBER(H5D_FILL_TIME_IFSET);
  REG_NUMBER(H5D_FILL_VALUE_ERROR);
  REG_NUMBER(H5D_FILL_VALUE_UNDEFINED);
  REG_NUMBER(H5D_FILL_VALUE_DEFAULT);
  REG_NUMBER(H5D_FILL_VALUE_USER_DEFINED);
  REG_NUMBER(H5F_ACC_RDONLY);
  REG_NUMBER(H5F_ACC_RDWR);
  REG_NUMBER(H5F_ACC_TRUNC);
  REG_NUMBER(H5F_ACC_EXCL);
  REG_NUMBER(H5F_ACC_DEBUG);
  REG_NUMBER(H5F_ACC_CREAT);
  REG_NUMBER(H5F_ACC_DEFAULT);
  REG_NUMBER(H5F_OBJ_FILE);
  REG_NUMBER(H5F_OBJ_DATASET);
  REG_NUMBER(H5F_OBJ_GROUP);
  REG_NUMBER(H5F_OBJ_DATATYPE);
  REG_NUMBER(H5F_OBJ_ATTR);
  REG_NUMBER(H5F_OBJ_ALL);
  REG_NUMBER(H5F_OBJ_LOCAL);
  REG_NUMBER(H5F_FAMILY_DEFAULT);
  REG_NUMBER(H5F_UNLIMITED);
  REG_NUMBER(H5F_LIBVER_18);
  REG_NUMBER(H5S_NO_CLASS);
  REG_NUMBER(H5S_SCALAR);
  REG_NUMBER(H5S_SIMPLE);
  REG_NUMBER(H5S_NULL);
  REG_NUMBER(H5S_SELECT_NOOP);
  REG_NUMBER(H5S_SELECT_SET);
  REG_NUMBER(H5S_SELECT_OR);
  REG_NUMBER(H5S_SELECT_AND);
  REG_NUMBER(H5S_SELECT_XOR);
  REG_NUMBER(H5S_SELECT_NOTB);
  REG_NUMBER(H5S_SELECT_NOTA);
  REG_NUMBER(H5S_SELECT_APPEND);
  REG_NUMBER(H5S_SELECT_PREPEND);
  REG_NUMBER(H5S_SELECT_INVALID);
  REG_NUMBER(H5S_SEL_ERROR);
  REG_NUMBER(H5S_SEL_NONE);
  REG_NUMBER(H5S_SEL_POINTS);
  REG_NUMBER(H5S_SEL_HYPERSLABS);
  REG_NUMBER(H5S_SEL_ALL);
  REG_NUMBER(H5S_SEL_N);
  REG_NUMBER(H5S_ALL);
  REG_NUMBER2(H5S_UNLIMITED,-1);
  REG_NUMBER(H5S_MAX_RANK);
  REG_HID(H5T_NCSET);
  REG_HID(H5T_NSTR);
  REG_NUMBER2(H5T_VARIABLE,-1);
  REG_HID(H5T_OPAQUE_TAG_MAX);
  REG_HID(H5T_IEEE_F32BE);
  REG_HID(H5T_IEEE_F32LE);
  REG_HID(H5T_IEEE_F64BE);
  REG_HID(H5T_IEEE_F64LE);
  REG_HID(H5T_STD_I8BE);
  REG_HID(H5T_STD_I8LE);
  REG_HID(H5T_STD_I16BE);
  REG_HID(H5T_STD_I16LE);
  REG_HID(H5T_STD_I32BE);
  REG_HID(H5T_STD_I32LE);
  REG_HID(H5T_STD_I64BE);
  REG_HID(H5T_STD_I64LE);
  REG_HID(H5T_STD_U8BE);
  REG_HID(H5T_STD_U8LE);
  REG_HID(H5T_STD_U16BE);
  REG_HID(H5T_STD_U16LE);
  REG_HID(H5T_STD_U32BE);
  REG_HID(H5T_STD_U32LE);
  REG_HID(H5T_STD_U64BE);
  REG_HID(H5T_STD_U64LE);
  REG_HID(H5T_STD_B8BE);
  REG_HID(H5T_STD_B8LE);
  REG_HID(H5T_STD_B16BE);
  REG_HID(H5T_STD_B16LE);
  REG_HID(H5T_STD_B32BE);
  REG_HID(H5T_STD_B32LE);
  REG_HID(H5T_STD_B64BE);
  REG_HID(H5T_STD_B64LE);
  REG_HID(H5T_STD_REF_OBJ);
  REG_HID(H5T_STD_REF_DSETREG);
  REG_HID(H5T_UNIX_D32BE);
  REG_HID(H5T_UNIX_D32LE);
  REG_HID(H5T_UNIX_D64BE);
  REG_HID(H5T_UNIX_D64LE);
  REG_HID(H5T_C_S1);
  REG_HID(H5T_FORTRAN_S1);
  REG_HID(H5T_INTEL_I8);
  REG_HID(H5T_INTEL_I16);
  REG_HID(H5T_INTEL_I32);
  REG_HID(H5T_INTEL_I64);
  REG_HID(H5T_INTEL_U8);
  REG_HID(H5T_INTEL_U16);
  REG_HID(H5T_INTEL_U32);
  REG_HID(H5T_INTEL_U64);
  REG_HID(H5T_INTEL_B8);
  REG_HID(H5T_INTEL_B16);
  REG_HID(H5T_INTEL_B32);
  REG_HID(H5T_INTEL_B64);
  REG_HID(H5T_INTEL_F32);
  REG_HID(H5T_INTEL_F64);
  REG_HID(H5T_ALPHA_I8);
  REG_HID(H5T_ALPHA_I16);
  REG_HID(H5T_ALPHA_I32);
  REG_HID(H5T_ALPHA_I64);
  REG_HID(H5T_ALPHA_U8);
  REG_HID(H5T_ALPHA_U16);
  REG_HID(H5T_ALPHA_U32);
  REG_HID(H5T_ALPHA_U64);
  REG_HID(H5T_ALPHA_B8);
  REG_HID(H5T_ALPHA_B16);
  REG_HID(H5T_ALPHA_B32);
  REG_HID(H5T_ALPHA_B64);
  REG_HID(H5T_ALPHA_F32);
  REG_HID(H5T_ALPHA_F64);
  REG_HID(H5T_MIPS_I8);
  REG_HID(H5T_MIPS_I16);
  REG_HID(H5T_MIPS_I32);
  REG_HID(H5T_MIPS_I64);
  REG_HID(H5T_MIPS_U8);
  REG_HID(H5T_MIPS_U16);
  REG_HID(H5T_MIPS_U32);
  REG_HID(H5T_MIPS_U64);
  REG_HID(H5T_MIPS_B8);
  REG_HID(H5T_MIPS_B16);
  REG_HID(H5T_MIPS_B32);
  REG_HID(H5T_MIPS_B64);
  REG_HID(H5T_MIPS_F32);
  REG_HID(H5T_MIPS_F64);
  REG_HID(H5T_VAX_F32);
  REG_HID(H5T_VAX_F64);
  REG_HID(H5T_NATIVE_CHAR);
  REG_HID(H5T_NATIVE_SCHAR);
  REG_HID(H5T_NATIVE_UCHAR);
  REG_HID(H5T_NATIVE_SHORT);
  REG_HID(H5T_NATIVE_USHORT);
  REG_HID(H5T_NATIVE_INT);
  REG_HID(H5T_NATIVE_UINT);
  REG_HID(H5T_NATIVE_LONG);
  REG_HID(H5T_NATIVE_ULONG);
  REG_HID(H5T_NATIVE_LLONG);
  REG_HID(H5T_NATIVE_ULLONG);
  REG_HID(H5T_NATIVE_FLOAT);
  REG_HID(H5T_NATIVE_DOUBLE);
  REG_HID(H5T_NATIVE_LDOUBLE);
  REG_HID(H5T_NATIVE_B8);
  REG_HID(H5T_NATIVE_B16);
  REG_HID(H5T_NATIVE_B32);
  REG_HID(H5T_NATIVE_B64);
  REG_HID(H5T_NATIVE_OPAQUE);
  REG_HID(H5T_NATIVE_HADDR);
  REG_HID(H5T_NATIVE_HSIZE);
  REG_HID(H5T_NATIVE_HSSIZE);
  REG_HID(H5T_NATIVE_HERR);
  REG_HID(H5T_NATIVE_HBOOL);
  REG_HID(H5T_NATIVE_INT8);
  REG_HID(H5T_NATIVE_UINT8);
  REG_HID(H5T_NATIVE_INT_LEAST8);
  REG_HID(H5T_NATIVE_UINT_LEAST8);
  REG_HID(H5T_NATIVE_INT_FAST8);
  REG_HID(H5T_NATIVE_UINT_FAST8);
  REG_HID(H5T_NATIVE_INT16);
  REG_HID(H5T_NATIVE_UINT16);
  REG_HID(H5T_NATIVE_INT_LEAST16);
  REG_HID(H5T_NATIVE_UINT_LEAST16);
  REG_HID(H5T_NATIVE_INT_FAST16);
  REG_HID(H5T_NATIVE_UINT_FAST16);
  REG_HID(H5T_NATIVE_INT32);
  REG_HID(H5T_NATIVE_UINT32);
  REG_HID(H5T_NATIVE_INT_LEAST32);
  REG_HID(H5T_NATIVE_UINT_LEAST32);
  REG_HID(H5T_NATIVE_INT_FAST32);
  REG_HID(H5T_NATIVE_UINT_FAST32);
  REG_HID(H5T_NATIVE_INT64);
  REG_HID(H5T_NATIVE_UINT64);
  REG_HID(H5T_NATIVE_INT_LEAST64);
  REG_HID(H5T_NATIVE_UINT_LEAST64);
  REG_HID(H5T_NATIVE_INT_FAST64);
  REG_HID(H5T_NATIVE_UINT_FAST64);
  REG_NUMBER(H5T_NO_CLASS);
  REG_NUMBER(H5T_INTEGER);
  REG_NUMBER(H5T_FLOAT);
  REG_NUMBER(H5T_TIME);
  REG_NUMBER(H5T_STRING);
  REG_NUMBER(H5T_BITFIELD);
  REG_NUMBER(H5T_OPAQUE);
  REG_NUMBER(H5T_COMPOUND);
  REG_NUMBER(H5T_REFERENCE);
  REG_NUMBER(H5T_ENUM);
  REG_NUMBER(H5T_VLEN);
  REG_NUMBER(H5T_ARRAY);
  REG_NUMBER(H5T_NCLASSES);
  REG_NUMBER(H5T_ORDER_ERROR);
  REG_NUMBER(H5T_ORDER_LE);
  REG_NUMBER(H5T_ORDER_BE);
  REG_NUMBER(H5T_ORDER_VAX);
  REG_NUMBER(H5T_ORDER_MIXED);
  REG_NUMBER(H5T_ORDER_NONE);
  REG_NUMBER(H5T_SGN_ERROR);
  REG_NUMBER(H5T_SGN_NONE);
  REG_NUMBER(H5T_SGN_2);
  REG_NUMBER(H5T_NSGN);
  REG_NUMBER(H5T_NORM_ERROR);
  REG_NUMBER(H5T_NORM_IMPLIED);
  REG_NUMBER(H5T_NORM_MSBSET);
  REG_NUMBER(H5T_NORM_NONE);
  REG_NUMBER(H5T_CSET_ERROR);
  REG_NUMBER(H5T_CSET_ASCII);
  REG_NUMBER(H5T_CSET_UTF8);
  REG_NUMBER(H5T_CSET_RESERVED_2);
  REG_NUMBER(H5T_CSET_RESERVED_3);
  REG_NUMBER(H5T_CSET_RESERVED_4);
  REG_NUMBER(H5T_CSET_RESERVED_5);
  REG_NUMBER(H5T_CSET_RESERVED_6);
  REG_NUMBER(H5T_CSET_RESERVED_7);
  REG_NUMBER(H5T_CSET_RESERVED_8);
  REG_NUMBER(H5T_CSET_RESERVED_9);
  REG_NUMBER(H5T_CSET_RESERVED_10);
  REG_NUMBER(H5T_CSET_RESERVED_11);
  REG_NUMBER(H5T_CSET_RESERVED_12);
  REG_NUMBER(H5T_CSET_RESERVED_13);
  REG_NUMBER(H5T_CSET_RESERVED_14);
  REG_NUMBER(H5T_CSET_RESERVED_15);
  REG_NUMBER(H5T_STR_ERROR);
  REG_NUMBER(H5T_STR_NULLTERM);
  REG_NUMBER(H5T_STR_NULLPAD);
  REG_NUMBER(H5T_STR_SPACEPAD);
  REG_NUMBER(H5T_STR_RESERVED_3);
  REG_NUMBER(H5T_STR_RESERVED_4);
  REG_NUMBER(H5T_STR_RESERVED_5);
  REG_NUMBER(H5T_STR_RESERVED_6);
  REG_NUMBER(H5T_STR_RESERVED_7);
  REG_NUMBER(H5T_STR_RESERVED_8);
  REG_NUMBER(H5T_STR_RESERVED_9);
  REG_NUMBER(H5T_STR_RESERVED_10);
  REG_NUMBER(H5T_STR_RESERVED_11);
  REG_NUMBER(H5T_STR_RESERVED_12);
  REG_NUMBER(H5T_STR_RESERVED_13);
  REG_NUMBER(H5T_STR_RESERVED_14);
  REG_NUMBER(H5T_STR_RESERVED_15);
  REG_NUMBER(H5T_PAD_ERROR);
  REG_NUMBER(H5T_PAD_ZERO);
  REG_NUMBER(H5T_PAD_ONE);
  REG_NUMBER(H5T_PAD_BACKGROUND);
  REG_NUMBER(H5T_NPAD);
  REG_NUMBER(H5T_CONV_INIT);
  REG_NUMBER(H5T_CONV_CONV);
  REG_NUMBER(H5T_CONV_FREE);
  REG_NUMBER(H5T_BKG_NO);
  REG_NUMBER(H5T_BKG_TEMP);
  REG_NUMBER(H5T_BKG_YES);
  REG_NUMBER(H5T_PERS_DONTCARE);
  REG_NUMBER(H5T_PERS_HARD);
  REG_NUMBER(H5T_PERS_SOFT);
  REG_NUMBER(H5T_DIR_DEFAULT);
  REG_NUMBER(H5T_DIR_ASCEND);
  REG_NUMBER(H5T_DIR_DESCEND);
  REG_NUMBER(H5T_CONV_EXCEPT_RANGE_HI);
  REG_NUMBER(H5T_CONV_EXCEPT_RANGE_LOW);
  REG_NUMBER(H5T_CONV_EXCEPT_PRECISION);
  REG_NUMBER(H5T_CONV_EXCEPT_TRUNCATE);
  REG_NUMBER(H5T_CONV_EXCEPT_PINF);
  REG_NUMBER(H5T_CONV_EXCEPT_NINF);
  REG_NUMBER(H5T_CONV_EXCEPT_NAN);
  REG_NUMBER(H5T_CONV_ABORT);
  REG_NUMBER(H5T_CONV_UNHANDLED);
  REG_NUMBER(H5T_CONV_HANDLED);
  REG_HID(H5P_ROOT);
  REG_HID(H5P_OBJECT_CREATE);
  REG_HID(H5P_FILE_CREATE);
  REG_HID(H5P_FILE_ACCESS);
  REG_HID(H5P_DATASET_CREATE);
  REG_HID(H5P_DATASET_ACCESS);
  REG_HID(H5P_DATASET_XFER);
  REG_HID(H5P_FILE_MOUNT);
  REG_HID(H5P_GROUP_CREATE);
  REG_HID(H5P_GROUP_ACCESS);
  REG_HID(H5P_DATATYPE_CREATE);
  REG_HID(H5P_DATATYPE_ACCESS);
  REG_HID(H5P_STRING_CREATE);
  REG_HID(H5P_ATTRIBUTE_CREATE);
  REG_HID(H5P_OBJECT_COPY);
  REG_HID(H5P_LINK_CREATE);
  REG_HID(H5P_LINK_ACCESS);
  REG_HID(H5P_FILE_CREATE_DEFAULT);
  REG_HID(H5P_FILE_ACCESS_DEFAULT);
  REG_HID(H5P_DATASET_CREATE_DEFAULT);
  REG_HID(H5P_DATASET_ACCESS_DEFAULT);
  REG_HID(H5P_DATASET_XFER_DEFAULT);
  REG_HID(H5P_FILE_MOUNT_DEFAULT);
  REG_HID(H5P_GROUP_CREATE_DEFAULT);
  REG_HID(H5P_GROUP_ACCESS_DEFAULT);
  REG_HID(H5P_DATATYPE_CREATE_DEFAULT);
  REG_HID(H5P_DATATYPE_ACCESS_DEFAULT);
  REG_HID(H5P_ATTRIBUTE_CREATE_DEFAULT);
  REG_HID(H5P_OBJECT_COPY_DEFAULT);
  REG_HID(H5P_LINK_CREATE_DEFAULT);
  REG_HID(H5P_LINK_ACCESS_DEFAULT);
  REG_HID(H5P_CRT_ORDER_TRACKED);
  REG_HID(H5P_CRT_ORDER_INDEXED);
  REG_HID(H5P_NO_CLASS);
  REG_HID(H5O_COPY_SHALLOW_HIERARCHY_FLAG);
  REG_HID(H5O_COPY_EXPAND_SOFT_LINK_FLAG);
  REG_HID(H5O_COPY_EXPAND_EXT_LINK_FLAG);
  REG_HID(H5O_COPY_EXPAND_REFERENCE_FLAG);
  REG_HID(H5O_COPY_WITHOUT_ATTR_FLAG);
  REG_HID(H5O_COPY_PRESERVE_NULL_FLAG);
  REG_HID(H5O_COPY_MERGE_COMMITTED_DTYPE_FLAG);
  REG_HID(H5O_COPY_ALL);
  REG_HID(H5O_SHMESG_NONE_FLAG);
  REG_HID(H5O_SHMESG_SDSPACE_FLAG);
  REG_HID(H5O_SHMESG_DTYPE_FLAG);
  REG_HID(H5O_SHMESG_FILL_FLAG);
  REG_HID(H5O_SHMESG_PLINE_FLAG);
  REG_HID(H5O_SHMESG_ATTR_FLAG);
  REG_HID(H5O_SHMESG_ALL_FLAG);
  REG_HID(H5O_HDR_CHUNK0_SIZE);
  REG_HID(H5O_HDR_ATTR_CRT_ORDER_TRACKED);
  REG_HID(H5O_HDR_ATTR_CRT_ORDER_INDEXED);
  REG_HID(H5O_HDR_ATTR_STORE_PHASE_CHANGE);
  REG_HID(H5O_HDR_STORE_TIMES);
  REG_HID(H5O_HDR_ALL_FLAGS);
  REG_HID(H5O_SHMESG_MAX_NINDEXES);
  REG_HID(H5O_SHMESG_MAX_LIST_SIZE);
  REG_NUMBER(H5O_TYPE_UNKNOWN);
  REG_NUMBER(H5O_TYPE_GROUP);
  REG_NUMBER(H5O_TYPE_DATASET);
  REG_NUMBER(H5O_TYPE_NAMED_DATATYPE);
  REG_NUMBER(H5O_TYPE_NTYPES);
  REG_NUMBER(H5O_MCDT_SEARCH_ERROR);
  REG_NUMBER(H5O_MCDT_SEARCH_CONT);
  REG_NUMBER(H5O_MCDT_SEARCH_STOP);
  REG_NUMBER(H5L_TYPE_ERROR);
  REG_NUMBER(H5L_TYPE_HARD);
  REG_NUMBER(H5L_TYPE_SOFT);
  REG_NUMBER(H5L_TYPE_EXTERNAL);
  REG_NUMBER(H5L_TYPE_MAX);
#undef REG_NUMBER

#undef REG_HID
}


const unsigned char *luagetbytes_hdf5(lua_State *L);

int luaopen_H5(lua_State *L)
{
  luaL_Reg hdf5_auxf[] = {
    {"new_hid_t", _new_hid_t},
    {"new_herr_t", _new_herr_t},
    {"new_H5O_info_t", _new_H5O_info_t},
    {"new_hsize_t_arr", _new_hsize_t_arr},
    {"H5_VERSION_GE", _H5_VERSION_GE},
    {"H5_VERSION_LE", _H5_VERSION_LE},
    {"H5_VERS_INFO", _H5_VERS_INFO},
    {NULL, NULL}};

  luaL_Reg H5O_info_t_meta[] = {
    {"__index", _H5O_info_t__index},
    {"__newindex", _H5O_info_t__newindex},
    {NULL, NULL}};
  luaL_newmetatable(L, "HDF5::H5O_info_t");
  luaL_setfuncs(L, H5O_info_t_meta, 0);
  lua_pop(L, 1);

  luaL_Reg hsize_t_arr_meta[] = {
    {"__index", _hsize_t_arr__index},
    {"__newindex", _hsize_t_arr__newindex},
    {NULL, NULL}};
  luaL_newmetatable(L, "HDF5::hsize_t_arr");
  luaL_setfuncs(L, hsize_t_arr_meta, 0);
  lua_pop(L, 1);

  luaL_Reg herr_t_meta[] = {
    {"__len", _herr_t__len}, // really the number value
    {NULL, NULL}};
  luaL_newmetatable(L, "HDF5::herr_t");
  luaL_setfuncs(L, herr_t_meta, 0);
  lua_pop(L, 1);

  luaL_Reg hid_t_meta[] = {
    {"__len", _hid_t__len}, // really the number value
    {NULL, NULL}};
  luaL_newmetatable(L, "HDF5::hid_t");
  luaL_setfuncs(L, hid_t_meta, 0);
  lua_pop(L, 1);


  // ---------------------------------------------------------------------------
  // Module definition
  // ---------------------------------------------------------------------------
  lua_newtable(L);
  luaL_setfuncs(L, hdf5_auxf, 0);
  luaL_setfuncs(L, H5A_funcs, 0);
  luaL_setfuncs(L, H5D_funcs, 0);
  luaL_setfuncs(L, H5E_funcs, 0);
  luaL_setfuncs(L, H5F_funcs, 0);
  luaL_setfuncs(L, H5G_funcs, 0);
  luaL_setfuncs(L, H5I_funcs, 0);
  luaL_setfuncs(L, H5L_funcs, 0);
  luaL_setfuncs(L, H5O_funcs, 0);
  luaL_setfuncs(L, H5P_funcs, 0);
  luaL_setfuncs(L, H5R_funcs, 0);
  luaL_setfuncs(L, H5S_funcs, 0);
  luaL_setfuncs(L, H5T_funcs, 0);
  luaL_setfuncs(L, H5Z_funcs, 0);
  luaL_setfuncs(L, H5P_MPI_funcs, 0);
  register_constants(L);

  return 1;
}
