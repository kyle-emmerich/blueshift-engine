--Grab the internal functions from C++, then remove the global reference
--to them since they're not needed elsewhere.
local internals = Vector4_internals; Vector4_internals = nil
--Define a global table for our Lua-side interface.
Vector4 = {}

--Link up Lua side metamethods to some internal methods that work off of lightuserdata
--This is a nice approach because it can easily be extended to work through FFI later.
--Might not be necessary since LuaJIT inlining is already super-fast.

--Good luck getting any kind of cache coherency though. Might not matter; this is in Lua
--after all.

--A lookup table mapping a string member index to an index into the Vector's internal data
--This way is nice because it allows for multiple indices for each piece of data.
local indices = {
	x = 1, X = 1, right = 1,
	y = 2, Y = 2, up = 2,
	z = 3, Z = 3, forward = 3,
	w = 4, W = 4, clip = 4
}

--This is a lookup table where the keys are userdata and the values are the actual internal
--pointers to Vector4 objects in the C++ code. It's a weak table because we don't want the 
--garbage collector to hold up over the references to the keys.
local pointers = setmetatable({}, { __mode = 'k' })
local pointees = setmetatable({}, { __mode = 'k' })
local references = setmetatable({}, { __mode = 'k' })

--A single metatable contains all the methods we need for the Vector4 type.
local meta; meta = {
	--Our math operators
	__add = function(a, b)
		return Vector4.from_ptr(internals.__add__(pointers[a], pointers[b]))
	end,
	__sub = function(a, b)
		return Vector4.from_ptr(internals.__sub__(pointers[a], pointers[b]))
	end,
	__mul = function(a, b)
		return Vector4.from_ptr(internals.__mul__(pointers[a], pointers[b]))
	end,
	__div = function(a, b)
		return Vector4.from_ptr(internals.__div__(pointers[a], pointers[b]))
	end,

	--Some vector math functions
	--Dot Product
	--	Parameters:
	--		a : Vector4			- The first vector
	--		b : Vector4			- The second vector
	--		with_w : boolean	- Treat this like a Vector3 if false (default = false)
	--	Returns:
	--		cosine : number		- The cosine of the angle between a and b
	Dot = function(a, b, with_w) 
		return internals.__dot__(pointers[a], pointers[b], with_w)
	end,
	--Angle Between
	--	Parameters
	--		a : Vector4			- The first vector
	--		b : Vector4			- The second vector
	--	Returns:
	--		radians : number	- The angle between the two vectors a and b in radians
	AngleBetween = function(a, b) 
		return math.acos(internals.__dot__(pointers[a], pointers[b], false))
	end,
	--Cross Product
	--	Parameters
	--		a : Vector4			- The first vector
	--		b : Vector4			- The second vector
	--	Returns:
	--		normal : Vector4	- If a and b define a plane, this is the normal vector of that plane.
	Cross = function(a, b)
		return Vector4.from_ptr(internals.__cross__(pointers[a], pointers[b]))
	end,
	--Linear Interpolate
	--	Parameters
	--		a : Vector4			- The first point
	--		b : Vector4			- The second point
	--		m : Vector4			- The fraction between a and b to interpolate to
	--	Returns:
	--		lerped : Vector4	- The point lying (m*100)% between a and b in a straight line.
	Lerp = function(a, b, m)
		return Vector4.from_ptr(internals.__lerp__(pointers[a], pointers[b], m))
	end,

	--Length
	--	Parameters
	--		a : Vector4			- The vector
	--		with_w : boolean	- If false, a is treated like a Vector3 (default = false)
	--	Returns:
	--		length : number		- The length or magnitude of the vector a.
	Length = function(a, with_w)
		return internals.__length__(pointers[a], with_w)
	end,
	--LengthSq
	--	Parameters
	--		a : Vector4			- The vector
	--		with_w : boolean	- If false, a is treated like a Vector3 (default = false)
	--	Returns:
	--		length : number		- The squared length or magnitude of the vector a.
	--	Note: Use whenever possible to save on a sqrt() call.
	LengthSq = function(a, with_w)
		return internals.__length_sq__(pointers[a], with_w)
	end,

	--Check to see if idx refers to a data member or property, otherwise
	--return whatever is in the metatable for that index.
	__index = function(a, idx) 
		local data_idx = indices[idx]
		if data_idx ~= nil then
			return internals.__get__(pointers[a], data_idx)
		end
		if idx == 'length' then
			return internals.__length__(pointers[a], false)
		end
		if type(idx) == 'number' then
			return internals.__get__(pointers[a], idx)
		end
		return meta[idx]
	end,
	--Same as __index, but using __set__ instead of __get__. This means
	--the X,Y,Z,W properties are writable.
	__newindex = function(a, idx, v)
		local data_idx = indices[idx]
		if data_idx ~= nil then
			internals.__set__(pointers[a], data_idx, v)
			return
		end
		if type(idx) == 'number' then
			internals.__set__(pointers[a], idx, v)
			return
		end
	end,
	--This gets executed when there are no references left for this userdata.
	--It means we can delete the pointer and kick it out of our lookup table.
	__gc = function(a)
		if not references[a] then
			internals.__delete__(pointers[a])
		end
		pointees[pointers[a]] = nil
		pointers[a] = nil
	end,

	--Converts a to a string so it can be printed or concatenated
	__tostring = function(a)
		return string.format("Vector4(%.3f, %.3f, %.3f, %.3f)", a.x, a.y, a.z, a.w)
	end,

	getpointer = function(a)
		return pointers[a]
	end,

	__type = "Vector4"
}

--Creates a userdata from a pointer passed to us by the C++ side. Any time
--a C++ function returns a Vector4 to us, we need to do this instead of Vector4.new.
function Vector4.from_ptr(ptr, no_delete)
	if pointees[ptr] ~= nil then
		return pointees[ptr]
	end
	local vec = newproxy(true)
	pointers[vec] = ptr
	pointees[ptr] = vec
	references[vec] = no_delete
	debug.setmetatable(vec, meta)

	return vec
end

--This is our Lua-side method for creating Vector4s. It creates a userdata, assigns
--a pointer to it given to us by the C++ side (so that the pointer can be allocated
--with the necessary alignment so SSE instructions don't blow up), and then initializes
--it with default values if necessary.
function Vector4.new(x, y, z, w)
	local vec = newproxy(true)
	pointers[vec] = internals.__new__()
	pointees[pointers[vec]] = vec
	debug.setmetatable(vec, meta)
	vec.x = x
	vec.y = y or 0
	vec.z = z or 0 
	vec.w = w or 1
	return vec
end