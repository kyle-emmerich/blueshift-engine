--Grab the internal functions from C++, then remove the global reference
--to them since they're not needed elsewhere.
local internals = Matrix4_internals; Matrix4_internals = nil
--Define a global table for our Lua-side interface
Matrix4 = {}

local pointers = setmetatable({}, { __mode = 'k' })
local references = setmetatable({}, { __mode = 'k' })

--There is absolutely zero cost (other than a few bytes over the whole runtime)
--to having all these aliases, so why not?
local axes = {
	x = 1, X = 1,	right = 1,
	y = 2, Y = 2,	up = 2,
	z = 3, Z = 3,	forward = 3,
	p = 4, position = 4, translation = 4
}

local meta; meta = {

	Copy = function(self)
		local new = Matrix4.new()
		for i = 1, 4 do
			for j = 1, 4 do
				new[i][j] = self[i][j]
			end
		end
	end,

	__mul = function(a, b)
		return Matrix4.from_ptr(internals.__mul__(pointers[a], pointers[b]))
	end,

	Inverse = function(a)
		return Matrix.from_ptr(internals.__inverse__(pointers[a]))
	end,

	Invert = function(self)
		internals.__invert__(pointers[self])
		return self
	end,

	Transpose = function(self)
		return Matrix4.new(
			self[1][1], self[2][1], self[3][1], self[4][1],
			self[1][2], self[2][2], self[3][2], self[4][2],
			self[1][3], self[2][3], self[3][3], self[4][3], 
			self[1][4], self[2][4], self[3][4], self[4][4])
	end,

	TransposeInplace = function(self)
		internals.__transpose__(pointers[self])
		return self
	end,

	__index = function(a, idx)
		if type(idx) == 'number' then
			local vec_ptr = internals.__get__(pointers[a], idx)
			return Vector4.from_ptr(vec_ptr, true)
		end

		if axes[idx] then
			return a[axes[idx]]
		end
		
		
		return meta[idx]
	end,

	__gc = function(a) 
		internals.__delete__(pointers[a])
		pointers[a] = nil
	end,

	__tostring = function(a)
		return string.format("Matrix4(%.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f)",
			a[1][1], a[1][2], a[1][3], a[1][4],
			a[2][1], a[2][2], a[2][3], a[2][4],
			a[3][1], a[3][2], a[3][3], a[3][4],
			a[4][1], a[4][2], a[4][3], a[4][4]
		)
	end,

	getpointer = function(a)
		return pointers[a]
	end,

	__type = "Matrix4"
}

function Matrix4.from_ptr(ptr, no_delete)
	local mat = newproxy(true)
	pointers[mat] = ptr
	references[mat] = no_delete
	debug.setmetatable(mat, meta)
	return mat
end

function Matrix4.new(...)
	local mat = newproxy(true)
	pointers[mat] = internals.__new__()
	debug.setmetatable(mat, meta)

	local vals = {...}
	local n = 1
	for i = 1, 4 do
		for j = 1, 4 do
			if #vals >= n then
				mat[i][j] = vals[n]
			else
				mat[i][j] = i==j and 1 or 0
			end
			n = n + 1
		end
	end
	return mat
end

function Matrix4.FromAxisAngle(axis, angle)
	if type(axis) ~= "Vector4" then
		error("incorrect argument 1 to Matrix4.FromAxisAngle: 'Vector4' expected")
	end
	axis_ptr = axis:getpointer()

	return Matrix4.from_ptr(internals.__axisangle__(axis_ptr, angle))
end