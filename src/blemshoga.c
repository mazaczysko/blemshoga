#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "allegro.h"
#include "resmgr.h"
#include "map.h"
#include "log.h"
#include "lapi.h"
#include "tile.h"

/*
	sx/sy - screen x/y
	mx/my - source map x/y
	w, h - map region w/h
	
	
*/
void draw_map(int sx, int sy, int mx, int my, int w, int h)
{
	// Lua context
	lua_State *L = lapi_get();

	//! TEMP
	const int TILE_SIZE = 32;

	for (int x = mx; x < mx + w; x++)
	{
		for (int y = my; y < my + h; y++)
		{
			// Request tiles from lua
			map_get_tile_stack(L, x, y);

			// Iterate tile stack
			lua_pushnil(L);
			while (lua_next(L, -2))
			{
				// k = -2, v = -1
				lua_getfield(L, -1, "sprite");
				const char *sprite_name = lua_tostring(L, -1);
				lua_pop(L, 2);

				al_draw_scaled_bitmap(
					resmgr_get_bitmap( sprite_name ),
					0,
					0,
					TILE_SIZE,
					TILE_SIZE,
					sx + x * TILE_SIZE,
					sy + y * TILE_SIZE,
					TILE_SIZE,
					TILE_SIZE,
					0
				);
			}

			lua_pop(L, 1); // Pop MAP
		}
	}
}

void main_draw(void)
{
	draw_map(0, 0, 0, 0, 24, 20);
}

int main(int argc, char *argv[])
{
	int err;

	// Log init
	log_init(stderr, argv[0]);
	log_info("Starting Blemshoga...");

	// Allegro init
	al_init();
	al_init_image_addon();
	al_install_keyboard();
	log_info("Allegro init done...");

	ALLEGRO_DISPLAY *display = al_create_display(32 * 24, 32 * 20);
	assert(display != NULL);

	ALLEGRO_TIMER *draw_timer = al_create_timer(1.f / 30.f);
	assert(draw_timer != NULL);
	al_start_timer(draw_timer);

	ALLEGRO_FONT *default_font = al_create_builtin_font();
	assert(default_font != NULL);

	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	assert(event_queue != NULL);

	// Event sources
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(draw_timer));

	// Lua API init
	err = lapi_init();
	assert(!err && "Lua init failed!");
	lua_State *L = lapi_get();

	// Run core/init script
	if (safe_lua_dofile(L, "resources/lua/core.lua"))
		return 1;

	//Run load script
	if (safe_lua_dofile(L, "resources/lua/load.lua"))
		return 1;

	// Run post-load script
	if (safe_lua_dofile(L, "resources/lua/post-load.lua"))
		return 1;



	// Main loop
	bool alive = true;
	while (alive)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		do 
		{
			switch (ev.type)
			{
				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					alive = false;
					break;

				case ALLEGRO_EVENT_TIMER:
					if (ev.timer.source == draw_timer)
					{
						al_clear_to_color(al_map_rgb(0, 0, 0));
						main_draw();
						al_flip_display();
					}
					break;

				default:
					break;
			}
		}
		while (al_get_next_event(event_queue, &ev));
	}

	resmgr_destroy_bitmap_storage();
	lapi_destroy();
	log_destroy();

	// Cleanup
	al_destroy_font(default_font);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(draw_timer);
	al_destroy_display(display);

	return 0;
}