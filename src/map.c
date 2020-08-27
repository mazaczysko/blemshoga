#include "allegro.h"
#include "map.h"
#include "utils.h"

void map_draw( )
{
	int width = 800, height = 600;
	
	for( int x = 0; x < width; x++ )
		for( int y = 0; y < height; y++ )
			for( int z = 0; z <= LMAP(x,y).size; z++ )
			{
				al_draw_tinted_scaled_bitmap( LMAP(x,y)[z], 0, 0, 0, TILE_SIZE, TILE_SIZE, x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE, 0 )
			}