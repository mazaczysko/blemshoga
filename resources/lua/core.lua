TILES = {}

MAP = setmetatable({}, {
	__call = function(self, x, y)
		local id = tostring(x) .. "_" .. tostring(y)

		if self[id] == nil then
			-- Create tile stack here
			self[id] = {
				
			}
		end
		return self[id]
	end
})