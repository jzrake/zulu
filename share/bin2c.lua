local description = [=[
usage: lua bin2c.lua [+]my_module.lua [my_module_name]

Write a C source file containing compiled code for the Lua module
'my_module.lua'. The source file can then be used to load that module within an
executable embedding Lua:

luaL_requiref(L, "my_module", luaopen_my_module, 0);

If 'my_module_name' is not given, the Lua file without the extension is used. If
+ is used, then the contents of 'filename' are first compiled before being used
to generate the C output.

http://lua-users.org/wiki/BinTwoCee
Original author: Mark Edgar
Licensed under the same terms as Lua (MIT license).

Modified by Jonathan Zrake (2014) to output C source that defines a function to
open a Lua module.

]=]

if not arg or not arg[1] then
  io.stderr:write(description)
  return
end

local compile, filename = arg[1]:match"^(+?)(.*)"
local modulename = arg[2] or filename:gsub(".lua", ""):gsub("-", "_")
local numtab = { }
for i=0,255 do numtab[string.char(i)]=("%3d,"):format(i) end

local content = compile=="+"
  and string.dump(assert(loadfile(filename)))
  or assert(io.open(filename,"rb")):read"*a"

function dump(str)
   return (str:gsub(".", numtab):gsub(("."):rep(80), "%0\n"))
end

io.write(([=[
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

int luaopen_%s(lua_State *L)
{
 static const unsigned char B1[] = {
 %s
 } ;
 if (luaL_loadbuffer(L,(const char*)B1,sizeof(B1),%q)==0) {
   lua_pushnumber(L, 0); /* so that the module doesn't think it's __main__ */
   lua_call(L, 1, 1);
 }
 else {
   luaL_error(L, "failed to load");
 }
 return 1;
}
]=]):format(modulename, dump(content), filename, modulename))
