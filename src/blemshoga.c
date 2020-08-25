#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

void main_draw(void)
{

}

int main(int argc, char *argv[])
{
	// Allegro init
	al_init();
	al_install_keyboard();

	ALLEGRO_DISPLAY *display = al_create_display(800, 600);
	assert(display != NULL);

	ALLEGRO_TIMER *draw_timer = al_create_timer(1.f / 60.f);
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

	// Cleanup
	al_destroy_font(default_font);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(draw_timer);
	al_destroy_display(display);

	return 0;
}