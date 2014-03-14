local argparse = require 'argparse'

local parser = argparse()
   :description "An example."
parser:argument "input"
   :description "Input file."
parser:option "-o" "--output"
   :default "a.out"
   :description "Output file."
parser:option "-I" "--include"
   :count "*"
   :description "Include locations."
local args = parser:parse()
prettyprint(args)

