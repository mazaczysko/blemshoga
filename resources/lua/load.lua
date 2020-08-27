local tile_files = {
	"resources/tiles/floor.lua",
	"resources/tiles/rat.lua",
}

for i = 1, #tile_files do
	load_tile(tile_files[i]);
end