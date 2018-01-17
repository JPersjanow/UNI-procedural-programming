#include <stdio.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_audio.h>
#include<allegro5\allegro_acodec.h>

/*screen constants*/
const int screen_h = 600;
const int screen_w = 800;
const int FPS = 60;

/*keys enumeration*/
enum KEYS { UP, DOWN, LEFT, RIGHT, E, ESC, SPACE };
bool keys[7] = { false,false,false,false,false,false,false };

/*state enumeration*/
enum STATE { MENU, PLAYING, ENDGAME, OPTIONS, ENDSCREEN };


/*mouse variables*/
int pos_x = screen_w / 2;
int pos_y = screen_h / 2;

/*player variables*/
int player_x = 50;
int player_y = 50;
int player_speed = 3;
int player_height = 60;
int player_width = 40;
int player_boundx = player_width / 2;
int player_boundy = player_height / 2;
int player_score = 0;

/*enemy variables*/
int enemy_width = 40;
int enemy_height = 60;
int enemy_boundx = enemy_width/2;
int enemy_boundy = enemy_height/2;

int enemy1_x = 100;
int enemy1_y = 230;
int enemy1_speed = 3;
bool enemy1_collision = false;

int enemy2_x = 430;
int enemy2_y = 410;
int enemy2_speed = 2;
bool enemy2_collision = false;

int enemy3_x = 90;
int enemy3_y = 550;
int enemy3_speed = 2;
bool enemy3_collision = false;

/*NPC variable*/
int npc_width = 40;
int npc_height = 60;
int npc_boundx = npc_width / 2;
int npc_boundy = npc_height / 2;

int npc1_x = 80;
int npc1_y = 80;
bool npc1_collision = false;

int npc2_x = 140;
int npc2_y = 330;
bool npc2_collision = false;

int npc3_x = 440;
int npc3_y = 50;
bool npc3_collision = false;

int npc_score = 20;

/*item variables*/
int item_width = 10;
int item_height = 10;
int item_boundx = item_width;
int item_boundy = item_height;
bool item1_collision = false;

int item1_x = 50;
int item1_y = 230;

int item2_x = 360;
int item2_y = 50;
bool item2_collision = false;

int item3_x = 750;
int item3_y = 350;
bool item3_collision = false;

int item4_x = 50;
int item4_y = 360;
bool item4_collision = false;

int item5_x = 290;
int item5_y = 480;
bool item5_collision = false;

int bottle_score = 20;

/*messages*/
bool message_npc1 = false;
bool message_npc2 = false;
bool message_npc3 = false;


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
	ALLEGRO_FONT *font8bit = al_load_ttf_font("Benguiat Bold.ttf", 25, 0);
	ALLEGRO_FONT *title_font = al_load_ttf_font("Benguiat Bold.ttf", 20, 0);
	ALLEGRO_FONT *talk_font = al_load_font("Benguiat Bold.ttf", 10, 0);

	/*bitmaps*/
	ALLEGRO_BITMAP *player_bitmap = al_load_bitmap("player.png");
	int image_w = al_get_bitmap_width(player_bitmap);
	int image_h = al_get_bitmap_height(player_bitmap);
	ALLEGRO_BITMAP *bottle_bitmap = al_load_bitmap("bottle.png");
	int image_bottle_w = al_get_bitmap_width(bottle_bitmap);
	int image_bottle_h = al_get_bitmap_height(bottle_bitmap);
	ALLEGRO_BITMAP *enemy_bitmap = al_load_bitmap("enemy.png");
	int image_enemy_w = al_get_bitmap_width(enemy_bitmap);
	int image_enemy_h = al_get_bitmap_height(enemy_bitmap);
	ALLEGRO_BITMAP *npc1_bitmap = al_load_bitmap("npc1.png");
	int image_npc_w = al_get_bitmap_width(npc1_bitmap);
	int image_npc_h = al_get_bitmap_height(npc1_bitmap);
	ALLEGRO_BITMAP *npc2_bitmap = al_load_bitmap("npc2.png");
	ALLEGRO_BITMAP *npc3_bitmap = al_load_bitmap("npc3.png");
	ALLEGRO_BITMAP *map = al_load_bitmap("map.png");
	int map_w = al_get_bitmap_width(map);
	int map_h = al_get_bitmap_height(map);
	ALLEGRO_BITMAP *menu_background = al_load_bitmap("background.png");
	ALLEGRO_BITMAP *message_npc1_bitmap = al_load_bitmap("message npc1.png");
	ALLEGRO_BITMAP *message_npc2_bitmap = al_load_bitmap("message npc2.png");
	ALLEGRO_BITMAP *message_npc3_bitmap = al_load_bitmap("message npc3.png");
	int image_npc_message_w = al_get_bitmap_width(message_npc1_bitmap);
	int image_npc_message_h = al_get_bitmap_height(message_npc1_bitmap);

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
	if (!songInstance)
	{
		printf("failed to initialize songInstance\n");
	}
	if (!song)
	{
		printf("failed to initialize song\n");
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
				if ((player_x >= 80 && player_x <= 720) && (player_y <= 200 && player_y >= 190))
				{
					player_y -= 0;
				}
				else if ((player_x >= 330 && player_x <= 720) && (player_y <= 20 && player_y >= 10))
				{
					player_y -= 0;
				}
				else if ((player_x >= 430 && player_x <= 720) && (player_y <= 380 && player_y >= 370))
				{
					player_y -= 0;
				}
				else if ((player_x >= 80 && player_x <= 370) && (player_y <= 265 && player_y >= 270))
				{
					player_y -= 0;
				}
				else if ((player_x >= 80 && player_x <= 370) && (player_y <= 515 && player_y >= 520))
				{
					player_y -= 0;
				}
				else if ((player_x >= 430 && player_x <= 800) && (player_y <= 520 && player_y >= 510))
				{
					player_y -= 0;
				}
				else if ((player_x >= 720 && player_x <= 780) && (player_y <= 320 && player_y >= 310))
				{
					player_y -= 0;
				}
				else
					player_y -= player_speed;
				if (player_y < 0)
				{
					player_y = 0;
				}
			}
			if (keys[DOWN])
			{
				if ((player_x >= 330 && player_x <= 720) && (player_y >= 80  && player_y <= 90))
				{
					player_y -= 0;
				}
				else if ((player_x >= 80 && player_x <= 370) && (player_y >= 260 && player_y <= 265))
				{
					player_y -= 0;
				}
				else if ((player_x >= 430 && player_x <= 780) && (player_y >= 260 && player_y <= 265))
				{
					player_y -= 0;
				}
				else if ((player_x >= 430 && player_x <= 780) && (player_y >= 440 && player_y <= 450))
				{
					player_y -= 0;
				}
				else if ((player_x >= 80 && player_x <= 370) && (player_y >= 520 && player_y <= 525))
				{
					player_y -= 0;
				}
				else if ((player_x >= 20 && player_x <= 800) && (player_y >= 580 && player_y <= 590))
				{
					player_y -= 0;
				}
				else
				player_y += player_speed;
				if (player_y > screen_h)
				{
					player_y = screen_h;

				}
			}
			if (keys[LEFT])
			{
				if ((player_x >= 10 && player_x <= 20) && (player_y >= 0 && player_y <= 580))
				{
					player_x -= 0;
				}
				else if ((player_x >= 320 && player_x <= 330) && (player_y >= 20 && player_y <= 80))
				{
					player_x -= 0;
				}
				else if ((player_x >= 710 && player_x <= 720) && (player_y >= 80 && player_y <= 200))
				{
					player_x -= 0;
				}
				else if ((player_x >= 365 && player_x <= 370) && (player_y >= 260 && player_y <= 380))
				{
					player_x -= 0;
				}
				else if ((player_x >= 365 && player_x <= 370) && (player_y >= 440 && player_y <= 520))
				{
					player_x -= 0;
				}
				else if ((player_x >= 85 && player_x <= 90) && (player_y >= 260 && player_y <= 520))
				{
					player_x -= 0;
				}
				else if ((player_x >= 710 && player_x <= 720) && (player_y >= 320 && player_y <= 380))
				{
					player_x -= 0;
				}
				else
				{
					player_x -= player_speed;
				}
				
				if (player_x < 0)
				{
					player_x = 0;

				}
			}
			if (keys[RIGHT])
			{
				if ((player_x >= 80 && player_x <= 90) && (player_y >= 0 && player_y <= 200))
				{
					player_x -= 0;
				}
				else if ((player_x >= 80 && player_x <= 85) && (player_y >= 260 && player_y <= 520))
				{
					player_x -= 0;
				}
				else if ((player_x >= 360 && player_x <= 365) && (player_y >= 260 && player_y <= 380))
				{
					player_x -= 0;
				}
				else if ((player_x >= 430 && player_x <= 440) && (player_y >= 260 && player_y <= 380))
				{
					player_x -= 0;
				}
				else if ((player_x >= 780 && player_x <= 790) && (player_y >= 0 && player_y <= 260))
				{
					player_x -= 0;
				}
				else if ((player_x >= 780 && player_x <= 790) && (player_y >= 320 && player_y <= 440))
				{
					player_x -= 0;
				}
				else if ((player_x >= 430 && player_x <= 440) && (player_y >= 440 && player_y <= 520))
				{
					player_x -= 0;
				}
				else if ((player_x >= 360 && player_x <= 365) && (player_y >= 440 && player_y <= 520))
				{
					player_x -= 0;
				}
				else
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
					state = ENDGAME;
				}

				/*endgame*/
				if (player_score < -20)
				{

					state = ENDGAME;
					/*restart*/
					player_score = 0;
					player_x = 50;
					player_y = 50;

					item1_collision = false;
					item2_collision = false;
					item3_collision = false;
					item4_collision = false;
					item5_collision = false;
				}

				/*if(po³o¿enie gracza==obszar koñcowy)
				{
					state = ENDSCREEN;
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
				if (enemy1_x > 700 || enemy1_x < 100)
				{
					enemy1_speed = -enemy1_speed;
				}
				enemy1_x += enemy1_speed;

				if (enemy2_x > 700 || enemy2_x < 430)
				{
					enemy2_speed = -enemy2_speed;
				}
				enemy2_x += enemy2_speed;

				if (enemy3_x > 660 || enemy3_x < 90)
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
					player_score = player_score - 1;
					printf("collision\n");
				}
				if (player_x + player_boundx > enemy2_x - enemy_boundx &&
					player_x - player_boundx < enemy2_x + enemy_boundx &&
					player_y + player_boundy > enemy2_y - enemy_boundy &&
					player_y - player_boundy < enemy2_y + enemy_boundy)
				{
					player_score = player_score - 1;
					printf("collision\n");
				}
				if (player_x + player_boundx > enemy3_x - enemy_boundx &&
					player_x - player_boundx < enemy3_x + enemy_boundx &&
					player_y + player_boundy > enemy3_y - enemy_boundy &&
					player_y - player_boundy < enemy3_y + enemy_boundy)
				{
					player_score = player_score - 1;
					printf("collision\n");
				}
				

				/*npc collision*/
				if (!message_npc1 && keys[E] && player_x + player_boundx > npc1_x - npc_boundx &&
					player_x - player_boundx < npc1_x + npc_boundx &&
					player_y + player_boundy > npc1_y - npc_boundy &&
					player_y - player_boundy < npc1_y + npc_boundy)
				{
					message_npc1 = true;
					printf("npc 1 collision\n");
				}
				else if (message_npc1 && keys[E] && player_x + player_boundx > npc1_x - npc_boundx &&
					player_x - player_boundx < npc1_x + npc_boundx &&
					player_y + player_boundy > npc1_y - npc_boundy &&
					player_y - player_boundy < npc1_y + npc_boundy)
				{
					message_npc1 = false;
				}
				if (!message_npc2 && keys[E] && player_x + player_boundx > npc2_x - npc_boundx &&
					player_x - player_boundx < npc2_x + npc_boundx &&
					player_y + player_boundy > npc2_y - npc_boundy &&
					player_y - player_boundy < npc2_y + npc_boundy)
				{
					message_npc2 = true;
					printf("npc 2 collision\n");
				}
				else if (message_npc2 && keys[E] && player_x + player_boundx > npc2_x - npc_boundx &&
					player_x - player_boundx < npc2_x + npc_boundx &&
					player_y + player_boundy > npc2_y - npc_boundy &&
					player_y - player_boundy < npc2_y + npc_boundy)
				{
					message_npc2 = false;
				}
				if (!message_npc3 && keys[E] && player_x + player_boundx > npc3_x - npc_boundx &&
					player_x - player_boundx < npc3_x + npc_boundx &&
					player_y + player_boundy > npc3_y - npc_boundy &&
					player_y - player_boundy < npc3_y + npc_boundy)
				{
					message_npc3 = true;
					printf("npc 3 collision\n");
				}
				else if (message_npc3 && keys[E] && player_x + player_boundx > npc3_x - npc_boundx &&
					player_x - player_boundx < npc3_x + npc_boundx &&
					player_y + player_boundy > npc3_y - npc_boundy &&
					player_y - player_boundy < npc3_y + npc_boundy)
				{
					message_npc3 = false;
				}
				

				/*item collision*/
				if (!item1_collision)
				{
					if (keys[E] && player_x + player_boundx > item1_x - item_boundx &&
						player_x - player_boundx < item1_x + item_boundx &&
						player_y + player_boundy > item1_y - item_boundy &&
						player_y - player_boundy < item1_y + item_boundy)
					{
						item1_collision = true;
						player_score = player_score + bottle_score;
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
						player_score = player_score + bottle_score;
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
						player_score = player_score + bottle_score;
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
						player_score = player_score + bottle_score;
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
						player_score = player_score + bottle_score;
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
						state = OPTIONS;
				}
				if (pos_x>354 && pos_x <436 && pos_y > 485 && pos_y < 505)
				{
					if (ev.mouse.button & 1)
						done = true;
				}

			}
			else if (state == ENDGAME)
			{
				if (pos_x > 354 && pos_x < 432 && pos_y > 304 && pos_y < 322)
				{
					if (ev.mouse.button & 1)
						state = MENU;
				}
			}
			else if (state == ENDSCREEN)
			{

			}
			else if (state == OPTIONS)
			{
				if (pos_x > 354 && pos_x < 432 && pos_y > 304 && pos_y < 322)
				{
					if (ev.mouse.button & 1)
					{
						state = MENU;
					}
						
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
				al_draw_text(font8bit, al_map_rgb(255, 255, 255), screen_w / 2, screen_h / 2 + 150, ALLEGRO_ALIGN_CENTER, "About");
			}
			else
			{
				al_draw_text(font8bit, al_map_rgb(255, 0, 0), screen_w / 2, screen_h / 2 + 150, ALLEGRO_ALIGN_CENTER, "About");
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
			al_draw_textf(talk_font, al_map_rgb(255, 255, 255), 5, 30, 0, "SCORE %i", player_score);
			al_draw_bitmap(player_bitmap, player_x - image_w / 2, player_y - image_h / 2, 0);
			al_draw_bitmap(enemy_bitmap, enemy1_x - image_enemy_w / 2, enemy1_y - image_enemy_h / 2, 0);
			al_draw_bitmap(enemy_bitmap, enemy2_x - image_enemy_w / 2, enemy2_y - image_enemy_h / 2, 0);
			al_draw_bitmap(enemy_bitmap, enemy3_x - image_enemy_w / 2, enemy3_y - image_enemy_h / 2, 0);
			al_draw_bitmap(npc1_bitmap, npc1_x - image_npc_w / 2, npc1_y - image_npc_h / 2, 0);
			al_draw_bitmap(npc2_bitmap, npc2_x - image_npc_w / 2, npc2_y - image_npc_h / 2, 0);
			al_draw_bitmap(npc3_bitmap, npc3_x - image_npc_w / 2, npc3_y - image_npc_h / 2, 0);
		
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

			if (message_npc1)
			{
				al_draw_bitmap(message_npc1_bitmap, screen_w - image_npc_message_w, screen_h - image_npc_message_h, 0);
			}
			if (message_npc2)
			{
				al_draw_bitmap(message_npc2_bitmap, screen_w - image_npc_message_w, screen_h - image_npc_message_h, 0);
			}
			if (message_npc3)
			{
				al_draw_bitmap(message_npc3_bitmap, screen_w - image_npc_message_w, screen_h - image_npc_message_h, 0);
			}

		}
			/*OPTIONS MENU*/
		else if (state == OPTIONS)
		{
			if (pos_x > 354 && pos_x < 432 && pos_y > 304 && pos_y < 322 )
			{
				al_draw_multiline_text(font8bit, al_map_rgb(255, 255, 255), screen_w / 2, screen_h / 2, 760,10, ALLEGRO_ALIGN_CENTER, "JOP");

			}
			else
			{
				al_draw_text(font8bit, al_map_rgb(255, 0, 0), screen_w / 2, screen_h / 2, ALLEGRO_ALIGN_CENTER, "MUSIC ON");
			}
		}
			/*ENDGAME*/
		else if (state == ENDGAME)
		{
			if (pos_x > 354 && pos_x < 432 && pos_y > 304 && pos_y < 322)
			{
				al_draw_text(font8bit, al_map_rgb(255, 255, 255), screen_w / 2, screen_h / 2, ALLEGRO_ALIGN_CENTER, "Back to Menu");
			}
			else
			{
				al_draw_text(font8bit, al_map_rgb(255, 0, 0), screen_w / 2, screen_h / 2, ALLEGRO_ALIGN_CENTER, "END GAME");
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