TILES = {}

MAP = setmetatable({}, {
	__call = function(self, x, y)
		local id = tostring(x) .. "_" .. tostring(y)

		if self[id] == nil then
			self[id] = {}
		end
		return self[id]
	end
})
