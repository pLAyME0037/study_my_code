print("Hello, World");

local function fact(n)
    if n <= 0 then return
    end
    return n * (n - 1)
end

print("enter a number:")
local a = io.read("*number")
print(fact(a))
