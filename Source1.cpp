#include <stdio.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_audio.h>
#include<allegro5\allegro_acodec.h>
#include "object.h"

/*screen constants*/
const int screen_h = 600;
const int screen_w = 800;
const int FPS = 60;

/*keys enumeration*/
enum KEYS { UP, DOWN, LEFT, RIGHT, E, ESC, SPACE };
bool keys[7] = { false,false,false,false,false,false,false };

/*state enumeration*/
enum STATE { MENU, PLAYING, BOARD, PLAYER_SELECTION };


/*mouse variables*/
int pos_x = screen_w / 2;
int pos_y = screen_h / 2;

/*player variables*/
int player_x = 50;
int player_y = 50;
int player_speed = 3;
int player_height = 75;
int player_width = 50;
int player_boundx = player_width / 2;
int player_boundy = player_height / 2;
int player_score = 0;

/*enemy variables*/
int enemy_width = 25;
int enemy_height = 35;
int enemy_boundx = enemy_width;
int enemy_boundy = enemy_height;

int enemy1_x = 50;
int enemy1_y = 250;
int enemy1_speed = 3;
bool enemy1_collision = false;

int enemy2_x = 190;
int enemy2_y = 380;
int enemy2_speed = 2;
bool enemy2_collision = false;

int enemy3_x = 50;
int enemy3_y = 580;
int enemy3_speed = 2;
bool enemy3_collision = false;

/*item variables*/
int item_width = 10;
int item_height = 10;
int item_boundx = item_width;
int item_boundy = item_height;
bool item1_collision = false;

int item1_x = 50;
int item1_y = 190;

int item2_x = 300;
int item2_y = 60;
bool item2_collision = false;

int item3_x = 50;
int item3_y = 350;
bool item3_collision = false;

int item4_x = 710;
int item4_y = 380;
bool item4_collision = false;

int item5_x = 290;
int item5_y = 480;
bool item5_collision = false;

/*messages*/
bool message1 = false;

int main()
{
	bool done = false;
	bool redraw = false;

	int state = MENU;

	/*allegro initialisation*/
	al_init();
	if (!al_init())
	{
		printf("Init Error");
	}

	/*initialisation*/
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_init_acodec_addon();

	/*installations*/
	al_install_keyboard();
	al_install_mouse();
	al_install_audio();

	/*allegro*/
	ALLEGRO_DISPLAY *display = al_create_display(screen_w, screen_h);
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);

	/*fonts*/
	ALLEGRO_FONT *font8bit = al_load_ttf_font("8-BIT WONDER.ttf", 25, 0);
	ALLEGRO_FONT *title_font = al_load_ttf_font("8-BIT WONDER.ttf", 20, 0);
	ALLEGRO_FONT *talk_font = al_load_font("8-BIT WONDER.ttf", 10, 0);

	/*bitmaps*/
	ALLEGRO_BITMAP *player_bitmap = al_load_bitmap("Jakub.png");
	int image_w = al_get_bitmap_width(player_bitmap);
	int image_h = al_get_bitmap_height(player_bitmap);
	ALLEGRO_BITMAP *bottle_bitmap = al_load_bitmap("bottle.png");
	int image_bottle_w = al_get_bitmap_width(bottle_bitmap);
	int image_bottle_h = al_get_bitmap_height(bottle_bitmap);
	ALLEGRO_BITMAP *enemy_bitmap = al_load_bitmap("enemy.png");
	int image_enemy_w = al_get_bitmap_width(enemy_bitmap);
	int image_enemy_h = al_get_bitmap_height(enemy_bitmap);
	ALLEGRO_BITMAP *map = al_load_bitmap("map.png");
	int map_w = al_get_bitmap_width(map);
	int map_h = al_get_bitmap_height(map);
	ALLEGRO_BITMAP *menu_background = al_load_bitmap("background.png");
	ALLEGRO_BITMAP *popup = al_load_bitmap("popup.png");

	/*audio*/
	al_reserve_samples(10);
	ALLEGRO_SAMPLE *song = al_load_sample("song.ogg");
	ALLEGRO_SAMPLE_INSTANCE *songInstance = al_create_sample_instance(song);
	al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());

	/*checking initialisation*/
	if (!display)
	{
		printf("Display Error");
	}
	if (!event_queue)
	{
		printf("Event Queue Error");
	}
	if (!timer)
	{
		printf("Timer Error");
	}
	if (!font8bit)
	{
		printf("Font Error");
	}
	if (!title_font)
	{
		printf("Font Error");
	}

	/*event sources*/
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_start_timer(timer);
	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		/*keyboard*/
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				keys[ESC] = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_E:
				keys[E] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
				break;


			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				keys[ESC] = false;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_E:
				keys[E] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			}

		}

		else if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;
			al_play_sample_instance(songInstance);

			/*player movement*/
			if (keys[UP])
			{
				player_y -= player_speed;
				if (player_y < 0)
				{
					player_y = 0;
				}
			}
			if (keys[DOWN])
			{
				player_y += player_speed;
				if (player_y > screen_h)
				{
					player_y = screen_h;

				}
			}
			if (keys[LEFT])
			{
				player_x -= player_speed;
				if (player_x < 0)
				{
					player_x = 0;

				}
			}
			if (keys[RIGHT])
			{
				player_x += player_speed;
				if (player_x > screen_w)
				{
					player_x = screen_w;

				}
			}

			/*game functions during playing*/
			if (state == PLAYING)
			{
				if (keys[ESC])
				{
					state = BOARD;
				}
				/*player movement on the map*/
				/*if (player_x <20)
				{
					player_x = 20;
				}
				if (player_x > 80)
				{
					player_x = 80;
				}
				if (player_y > 215 && player_y < 280)
				{
					player_x = player_x;
				}*/

				/*area affecting player*/
				if (player_x > 20 && player_x < 80 && player_y>280 && player_y < 320)
				{
					player_x = 50;
					player_y = 250;
				}
				if (player_x > 685 && player_x < 745 && player_y < 30 && player_y>0)
				{
					player_x = 710;
					player_y = 60;
				}


				/*enemy movement*/
				if (enemy1_x > 710 || enemy1_x < 50)
				{
					enemy1_speed = -enemy1_speed;
				}
				enemy1_x += enemy1_speed;

				if (enemy2_x > 700 || enemy2_x < 190)
				{
					enemy2_speed = -enemy2_speed;
				}
				enemy2_x += enemy2_speed;

				if (enemy3_x > 575 || enemy3_x < 50)
				{
					enemy3_speed = -enemy3_speed;
				}
				enemy3_x += enemy3_speed;

				/*enemy collision*/
				if (player_x + player_boundx > enemy1_x - enemy_boundx &&
					player_x - player_boundx < enemy1_x + enemy_boundx &&
					player_y + player_boundy > enemy1_y - enemy_boundy &&
					player_y - player_boundy < enemy1_y + enemy_boundy)
				{
					printf("collision\n");
				}
				if (player_x + player_boundx > enemy2_x - enemy_boundx &&
					player_x - player_boundx < enemy2_x + enemy_boundx &&
					player_y + player_boundy > enemy2_y - enemy_boundy &&
					player_y - player_boundy < enemy2_y + enemy_boundy)
				{
					printf("collision\n");
				}
				if (player_x + player_boundx > enemy3_x - enemy_boundx &&
					player_x - player_boundx < enemy3_x + enemy_boundx &&
					player_y + player_boundy > enemy3_y - enemy_boundy &&
					player_y - player_boundy < enemy3_y + enemy_boundy)
				{
					printf("collision\n");
				}

				/*item collision*/
				if (!item1_collision)
				{
					if (player_x + player_boundx > item1_x - item_boundx &&
						player_x - player_boundx < item1_x + item_boundx &&
						player_y + player_boundy > item1_y - item_boundy &&
						player_y - player_boundy < item1_y + item_boundy)
					{
						message1 = true;
						printf("item1 collision\n");
					}
					if (keys[E] && player_x + player_boundx > item1_x - item_boundx &&
						player_x - player_boundx < item1_x + item_boundx &&
						player_y + player_boundy > item1_y - item_boundy &&
						player_y - player_boundy < item1_y + item_boundy)
					{
						item1_collision = true;
						message1 = false;
						player_score++;
						printf("item1 collision\n");
					}
				}

				if (!item2_collision)
				{
					if (keys[E] && player_x + player_boundx > item2_x - item_boundx &&
						player_x - player_boundx < item2_x + item_boundx &&
						player_y + player_boundy > item2_y - item_boundy &&
						player_y - player_boundy < item2_y + item_boundy)
					{
						item2_collision = true;
						player_score++;
						printf("item2 collision\n");
					}
				}

				if (!item3_collision)
				{
					if (keys[E] && player_x + player_boundx > item3_x - item_boundx &&
						player_x - player_boundx < item3_x + item_boundx &&
						player_y + player_boundy > item3_y - item_boundy &&
						player_y - player_boundy < item3_y + item_boundy)
					{
						item3_collision = true;
						player_score++;
						printf("item3 collision\n");
					}
				}

				if (!item4_collision)
				{
					if (keys[E] && player_x + player_boundx > item4_x - item_boundx &&
						player_x - player_boundx < item4_x + item_boundx &&
						player_y + player_boundy > item4_y - item_boundy &&
						player_y - player_boundy < item4_y + item_boundy)
					{
						item4_collision = true;
						player_score++;
						printf("item4 collision\n");
					}
				}

				if (!item5_collision)
				{
					if (keys[E] && player_x + player_boundx > item5_x - item_boundx &&
						player_x - player_boundx < item5_x + item_boundx &&
						player_y + player_boundy > item5_y - item_boundy &&
						player_y - player_boundy < item5_y + item_boundy)
					{
						item5_collision = true;
						player_score++;
						printf("item5 collision\n");
					}
				}





			}
		}
		/*window control*/
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}

		/*mouse control*/
		else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
				printf("%i , %i\n", pos_x, pos_y);
			pos_x = ev.mouse.x;
			pos_y = ev.mouse.y;
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (state == MENU)
			{
				if (pos_x>289 && pos_x <501 && pos_y > 424 && pos_y < 441)
				{
					if (ev.mouse.button & 1)
						state = PLAYING;
				}
				if (pos_x>302 && pos_x <489 && pos_y > 454 && pos_y < 474)
				{
					if (ev.mouse.button & 1)
						state = BOARD;
				}
				if (pos_x>354 && pos_x <436 && pos_y > 485 && pos_y < 505)
				{
					if (ev.mouse.button & 1)
						done = true;
				}

			}
			else if (state == BOARD)
			{
				if (pos_x > 354 && pos_x < 432 && pos_y > 304 && pos_y < 322)
				{
					if (ev.mouse.button & 1)
						state = MENU;
				}
			}
		}

		/*rendering*/
		if (redraw && al_event_queue_is_empty(event_queue))
		{
			/*MENU*/
		if (state == MENU)
		{

			al_draw_bitmap(menu_background, 0, 0, 0);

			//colour change of menu buttons beta version
			if (pos_x > 289 && pos_x < 501 && pos_y > 424 && pos_y < 441)
			{
				al_draw_text(font8bit, al_map_rgb(255, 255, 255), screen_w / 2, screen_h / 2 + 120, ALLEGRO_ALIGN_CENTER, "New Game");
			}
			else
			{
				al_draw_text(font8bit, al_map_rgb(255, 0, 0), screen_w / 2, screen_h / 2 + 120, ALLEGRO_ALIGN_CENTER, "New Game");
			}


			if (pos_x > 302 && pos_x < 489 && pos_y > 454 && pos_y < 474)
			{
				al_draw_text(font8bit, al_map_rgb(255, 255, 255), screen_w / 2, screen_h / 2 + 150, ALLEGRO_ALIGN_CENTER, "LifeBoard");
			}
			else
			{
				al_draw_text(font8bit, al_map_rgb(255, 0, 0), screen_w / 2, screen_h / 2 + 150, ALLEGRO_ALIGN_CENTER, "LifeBoard");
			}

			if (pos_x > 354 && pos_x < 436 && pos_y > 485 && pos_y < 505)
			{
				al_draw_text(font8bit, al_map_rgb(255, 255, 255), screen_w / 2, screen_h / 2 + 180, ALLEGRO_ALIGN_CENTER, "Exit");
			}
			else
			{
				al_draw_text(font8bit, al_map_rgb(255, 0, 0), screen_w / 2, screen_h / 2 + 180, ALLEGRO_ALIGN_CENTER, "Exit");
			}



		}
			/*PLAYING*/
		else if (state == PLAYING)
		{
			al_draw_bitmap(map, screen_w - map_w, screen_h - map_h, 0);
			al_draw_bitmap(player_bitmap, player_x - image_w / 2, player_y - image_h / 2, 0);
			al_draw_bitmap(enemy_bitmap, enemy1_x - image_enemy_w / 2, enemy1_y - image_enemy_h / 2, 0);
			al_draw_bitmap(enemy_bitmap, enemy2_x - image_enemy_w / 2, enemy2_y - image_enemy_h / 2, 0);
			al_draw_bitmap(enemy_bitmap, enemy3_x - image_enemy_w / 2, enemy3_y - image_enemy_h / 2, 0);
			if (message1)
			{
				al_draw_bitmap(popup, 590, 0, 0);
				al_draw_textf(talk_font, al_map_rgb(255, 255, 255), 700, 30, ALLEGRO_ALIGN_CENTER, "this is a bottle");
				al_draw_textf(talk_font, al_map_rgb(255, 255, 255), 700, 65, ALLEGRO_ALIGN_CENTER, "press e to pick up");
				al_draw_textf(talk_font, al_map_rgb(255, 255, 255), 700, 85, ALLEGRO_ALIGN_CENTER, "bottles give you points");
				al_draw_textf(talk_font, al_map_rgb(255, 255, 255), 700, 100, ALLEGRO_ALIGN_CENTER, "collect them all");
				
			}

			if (!item1_collision)
			{
				al_draw_bitmap(bottle_bitmap, item1_x - image_bottle_w / 2, item1_y - image_bottle_h / 2, 0);
			}

			if (!item2_collision)
			{
				al_draw_bitmap(bottle_bitmap, item2_x - image_bottle_w / 2, item2_y - image_bottle_h / 2, 0);
			}

			if (!item3_collision)
			{
				al_draw_bitmap(bottle_bitmap, item3_x - image_bottle_w / 2, item3_y - image_bottle_h / 2, 0);
			}

			if (!item4_collision)
			{
				al_draw_bitmap(bottle_bitmap, item4_x - image_bottle_w / 2, item4_y - image_bottle_h / 2, 0);
			}

			if (!item5_collision)
			{
				al_draw_bitmap(bottle_bitmap, item5_x - image_bottle_w / 2, item5_y - image_bottle_h / 2, 0);
			}


		}
			/*BOARD*/
		else if (state == BOARD)
		{
			if (pos_x > 354 && pos_x < 432 && pos_y > 304 && pos_y < 322)
			{
				al_draw_text(font8bit, al_map_rgb(255, 255, 255), screen_w / 2, screen_h / 2, ALLEGRO_ALIGN_CENTER, "Back to Menu");
			}
			else
			{
				al_draw_text(font8bit, al_map_rgb(255, 0, 0), screen_w / 2, screen_h / 2, ALLEGRO_ALIGN_CENTER, "Back to Menu");
			}
		}
		
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}

	al_destroy_display(display);
	al_destroy_font(font8bit);
	al_destroy_font(talk_font);
	al_destroy_font(title_font);
	al_destroy_bitmap(bottle_bitmap);
	al_destroy_bitmap(enemy_bitmap);
	al_destroy_bitmap(player_bitmap);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
}