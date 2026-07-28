local function ptime(...)
    local cmd = table.concat({...}, " ")
    local start = io.popen("date +%s%3N"):read("*a")
                                         :gsub("%s+", "")
    os.execute(cmd)
    local stop = io.popen("date +%s%3N"):read("*a")
                                        :gsub("%s+", "")
    local elapsed = tonumber(stop) - tonumber(start)
    io.write(string.format("[%sms] %s\n", elapsed, cmd))
end

ptime('lua', './1_hello_world.lua')
