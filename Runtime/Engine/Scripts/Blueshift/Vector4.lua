local internals = Vector4_internals; Vector4_internals = nil
Vector4 = {}

--Link up Lua side metamethods to some internal methods that work off of lightuserdata
--This is a nice approach because it can easily be extended to work through FFI later.
--Might not be necessary since LuaJIT inlining is already super-fast.

--Good luck getting any kind of cache coherency though. Might not matter; this is in Lua
--after all.

local indices = {
	X = 0,
	Y = 1,
	Z = 2,
	W = 3,

	x = 0,
	y = 1,
	z = 2,
	w = 3
}

local meta; meta = {
	__add = function(a, b)
		return Vector4.from_ptr(internals.__add__(a.ptr, b.ptr))
	end,
	__sub = function(a, b)
		return Vector4.from_ptr(internals.__sub__(a.ptr, b.ptr))
	end,
	__mul = function(a, b)
		return Vector4.from_ptr(internals.__mul__(a.ptr, b.ptr))
	end,
	__div = function(a, b)
		return Vector4.from_ptr(internals.__div__(a.ptr, b.ptr))
	end,

	Dot = function(a, b, with_w) 
		return internals.__dot__(a.ptr, b.ptr, with_w)
	end,
	Cross = function(a, b)
		return Vector4.from_ptr(internals.__cross__(a.ptr, b.ptr))
	end,
	Lerp = function(a, b, m)
		return Vector4.from_ptr(internals.__lerp__(a.ptr, b.ptr, m))
	end,

	Length = function(a, with_w)
		return internals.__length__(a.ptr, with_w)
	end,
	LengthSq = function(a, with_w)
		return internals.__length_sq__(a.ptr, with_w)
	end,

	__index = function(a, idx) 
		local data_idx = indices[idx]
		if data_idx ~= nil then
			return internals.__get__(a.ptr, data_idx)
		end
		if idx == 'length' then
			return internals.__length__(a.ptr, false)
		end
		return meta[idx]
	end,
	__newindex = function(a, idx, v)
		local data_idx = indices[idx]
		if data_idx ~= nil then
			internals.__set__(a.ptr, data_idx, v)
			return
		end
	end
}

function Vector4.from_ptr(ptr)
	local vec = { ptr = x }
	setmetatable(vec, meta)
	return vec
end

function Vector4.new(x, y, z, w)
	local vec = { ptr = internals.__new__() }
	setmetatable(vec, meta)
	vec.x = x
	vec.y = y
	vec.z = z
	vec.w = w
	return vec
end