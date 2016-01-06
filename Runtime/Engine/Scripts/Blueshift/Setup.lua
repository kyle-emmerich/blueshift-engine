local old_type = type
type = function(val)
	local mt = debug.getmetatable(val)
	if mt and mt.__type then
		local t = mt.__type
		if old_type(t) == 'function' then
			return t()
		else
			return t
		end
	end
	return old_type(val)
end