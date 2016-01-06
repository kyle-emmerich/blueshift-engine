
local internals = Quaternion_internals; Quaternion_internals = nil

Quaternion = {}

local indices = {
	X = 1,
	Y = 2,
	Z = 3,
	W = 4,

	x = 1,
	y = 2,
	z = 3,
	w = 4
}

local pointers = setmetatable({}, { __mode = 'k' })
local pointees = setmetatable({}, { __mode = 'k' })
local references = setmetatable({}, { __mode = 'k' })

local meta; meta = {

	__index = function(a, idx) 
		local data_idx = indices[idx]
		if data_idx ~= nil then
			return internals.__get__(pointers[a], data_idx)
		end
		if type(idx) == 'number' then
			return internals.__get__(pointers[a], idx)
		end
		return meta[idx]
	end,

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

	__gc = function(a)
		if not references[a] then
			internals.__delete__(pointers[a])
		end
		pointees[pointers[a]] = nil
		pointers[a] = nil
	end,
	
	__tostring = function(a)
		return string.format("Quaternion(%.3f, %.3f, %.3f, %.3f)", a.x, a.y, a.z, a.w)
	end,

	__type = "Quaternion"
}

function Quaternion.from_ptr(ptr, no_delete)
	if pointees[ptr] ~= nil then
		return pointees[ptr]
	end
	local quat = newproxy(true)
	pointers[quat] = ptr
	pointees[ptr] = quat
	references[quat] = no_delete
	debug.setmetatable(quat, meta)

	return vec
end

function Quaternion.new(x, y, z, w)
	local quat = newproxy(true)
	pointers[quat] = internals.__new__()
	pointees[pointers[quat]] = quat
	debug.setmetatable(quat, meta)
	quat.x = x or 0
	quat.y = y or 0
	quat.z = z or 0
	quat.w = w or 1
	return quat
end