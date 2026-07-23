local err = "lua: compile_langs.lua:129: attempt to index a nil value (global 'vim')"
local file, err_line = err:match("^lua: ([^:]+):(%d+):")
-- ([^:]+) — filename (chars that aren't :)
-- (%d+) — line number
print (file)
print (err_line)

vim.cmd("e " .. file)
vim.cmd(tostring(err_line))
