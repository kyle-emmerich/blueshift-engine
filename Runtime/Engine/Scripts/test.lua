print("Hello from LuaJIT 2.0.4")


local foo = Vector4.new(10, 10, 10, 1)
local bar = Vector4.new(0, 5, 20, 1)

print("foo", foo.X, foo.Y, foo.Z, foo.W)
print("bar", bar.X, bar.Y, bar.Z, bar.W)

local baz = foo * bar

print("baz", baz.X, baz.Y, baz.Z, baz.W)


print(foo.Lerp)
local fup = foo:Lerp(bar, 0.5)
print("fup", fup.X, fup.Y, fup.Z, fup.W)