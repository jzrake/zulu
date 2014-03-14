local hdf5 = require 'hdf5'
local colors = require 'ansicolors'
local class = require 'class'
local array = require 'array'
local argparse = require 'argparse'
local struct = require 'struct'

local M2 = require 'legend'

local MyClass = class.class('MyClass')
function MyClass:__init__()
   self._name = "something"
end
function MyClass:method1()
   print(self._name)
end

local function f()
   print (colors("%{red}hello"))

   local A = array.vector{1, 2, 3, 4}
   print(A)

   local h5f = hdf5.File("myfile.h5", 'w')
   h5f["my_dataset"] = "some string"
   h5f["A"] = A
   h5f:close()
end
local function g()
   f()
end

g()


local c = MyClass()
c:method1()
local mytable = { dog='woof', cat='meow' }
local myarray = {1, 2, 3, 4}
for k,v in pairs(mytable) do
   print(k,v)
end

local m2 = M2.dragon()

m2.length = 10.0
m2.name = "sam"
m2.callback = function(x) return x end

for k,v in pairs(struct.members(m2)) do
   print(k,v,m2[v])
end

m2:speak()

