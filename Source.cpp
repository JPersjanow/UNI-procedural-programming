#include <stdio.h>
#include <time.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_audio.h>
#include<allegro5\allegro_acodec.h>
#include "object.h"


//constants
const int screen_h = 600;
const int screen_w = 800;
const int FPS = 60;
const int ENEMY_NUMBER = 5;
const int ITEM_NUMBER = 4;

//player choice variables
bool Jakub = false;
bool Bojan = false;
bool Mati = false;
bool Bocian = false;



//keyboard and keys
enum KEYS { UP, DOWN, LEFT, RIGHT, E, ESC, SPACE };
bool keys[7] = { false,false,false,false,false,false,false };

int pos_x = screen_w / 2;
int pos_y = screen_h / 2;

//states
enum STATE { MENU, PLAYING, BOARD, PLAYER_SELECTION };



//object functions
void InitPlayer(Student &player);
void DrawPlayer(Student &player);
void MovePlayerUp(Student &player);
void MovePlayerDown(Student &player);
void MovePlayerLeft(Student &player);
void MovePlayerRight(Student &player);

void InitItem(Bottle item[], int size);
void DrawItem(Bottle item[], int size);

int main()
{
	//main variables
	bool done = false;
	bool redraw = false;

	//collision_enemy detection
	bool bound = false;
	bool collision_enemy1 = false;
	bool collision_enemy2 = false;
	bool collision_enemy3 = false;
	bool collision_enemy4 = false;
	bool collision_item1 = false;
	bool collision_item2 = false;
	bool collision_item3 = false;
	bool collision_item4 = false;

	//game variables
	Student player;
	Sebix enemy[ENEMY_NUMBER];
	Bottle item[ITEM_NUMBER];

	//state variable
	int state = MENU;

	int score = 0;

	//***************Allegro initialisation*************//
	//**************************************************//

	al_init();
	if (!al_init())
	{
		printf("Init Error");
	}

	//***************Initialisation********************//
	//************************************************//

	//addons
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_init_acodec_addon();

	//keyboard and/or mouse
	al_install_keyboard();
	al_install_mouse();
	al_install_audio();

	//objects
	InitPlayer(player);

	InitItem(item, ITEM_NUMBER);
	float enemy_x = 10;
	float enemy_y = screen_h / 2;
	float enemy_speed = 3;


	//*****************Allegro variables****************//
	//*************************************************//

	//creating allegro variables
	ALLEGRO_DISPLAY *display = al_create_display(screen_w, screen_h);
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);

	ALLEGRO_BITMAP *menu_background = al_load_bitmap("background.png");
	ALLEGRO_BITMAP * player_selection_background = al_load_bitmap("player_selection.png");
	ALLEGRO_BITMAP *map = al_load_bitmap("map.png");

	int map_w = al_get_bitmap_width(map);
	int map_h = al_get_bitmap_height(map);
	ALLEGRO_BITMAP *player_Bocian = al_load_bitmap("Bocian.png");
	ALLEGRO_BITMAP *player_Dolny = al_load_bitmap("Dolny.png");
	ALLEGRO_BITMAP *player_Jakub = al_load_bitmap("Jakub.png");
	

	//audio
	al_reserve_samples(10);
	ALLEGRO_SAMPLE *song = al_load_sample("song.ogg");
	ALLEGRO_SAMPLE_INSTANCE *songInstance = al_create_sample_instance(song);
	al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());


	ALLEGRO_FONT *font8bit = al_load_ttf_font("8-BIT WONDER.ttf", 25, 0);
	ALLEGRO_FONT *title_font = al_load_ttf_font("8-BIT WONDER.ttf", 20, 0);
	ALLEGRO_FONT *talk_font = al_load_font("8-BIT WONDER.ttf", 10, 0);

	//checking allegro variables
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


	//registering event source
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));


	//main game loop
	al_start_timer(timer);
	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

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

			//player movement
			if (keys[UP])
				MovePlayerUp(player);
			if (keys[DOWN])
				MovePlayerDown(player);
			if (keys[RIGHT])
				MovePlayerRight(player);
			if (keys[LEFT])
				MovePlayerLeft(player);
			//developer
			/*if (state == PLAYING)
			{
				printf("player x = %i\n player.y = %i\n", player.x, player.y);
			}*/

			//enemy movemenet
			if (enemy_x>355 || enemy_x<0)
			{
				enemy_speed = -enemy_speed;
			}
			enemy_x += enemy_speed;

			/*************************************/
			/***************MAP******************/

					if (player.x < 40)
					{
						player.x = player.x;
					}
					/*else if (player.x > 60 && player.y < 250)
					{
						player.x = j;
						player.y = i;
					}
					else if (player.x > 60 && player.y > 270)
					{
						player.x = j;
						player.y = i;
					}*/
			

			/***********************************/
					

			//collision_enemy detection
			if (keys[E])
				bound = true;
			else
				bound = false;
			
			
				if (keys[E] && player.x == enemy_x)
				{

					player.score++;
					//develloper
					printf("colission enemy \n");
				}

			if (enemy[1].live)
			{
					if (keys[E] && player.x + player.boundx > enemy[1].x - enemy[1].boundx &&
					player.x - player.boundx < enemy[1].x + enemy[1].boundx &&
					player.y + player.boundy > enemy[1].y - enemy[1].boundy &&
					player.y - player.boundy < enemy[1].y + enemy[1].boundy)
				{
					collision_enemy1 = true;
					enemy[1].live = false;
					player.score++;
					//develloper
					printf("colission enemy 1\n");
				}
			}
			if (enemy[2].live)
			{
					if (keys[E] && player.x + player.boundx > enemy[2].x - enemy[2].boundx &&
					player.x - player.boundx < enemy[2].x + enemy[2].boundx &&
					player.y + player.boundy > enemy[2].y - enemy[2].boundy &&
					player.y - player.boundy < enemy[2].y + enemy[2].boundy)
				{
					collision_enemy2 = true;
					enemy[2].live = false;
					player.score++;
					//develloper
					printf("colission enemy 2\n");
				}
			}
			if (enemy[3].live)
			{
				if (keys[E] &&  player.x + player.boundx > enemy[3].x - enemy[3].boundx &&
					player.x - player.boundx < enemy[3].x + enemy[3].boundx &&
					player.y + player.boundy > enemy[3].y - enemy[3].boundy &&
					player.y - player.boundy < enemy[3].y + enemy[3].boundy)
				{
					collision_enemy3 = true;
					enemy[3].live = false;
					player.score++;
					//develloper
					printf("colission enemy 3\n");
				}
			}
			if (enemy[4].live)
			{	
					if (keys[E] && player.x + player.boundx > enemy[4].x - enemy[4].boundx &&
					player.x - player.boundx < enemy[4].x + enemy[4].boundx &&
					player.y + player.boundy > enemy[4].y - enemy[4].boundy &&
					player.y - player.boundy < enemy[4].y + enemy[4].boundy)
				{
					collision_enemy4 = true;
					enemy[4].live = false;
					player.score++;
					//develloper
					printf("colission enemy 4\n");
				}

			}
			
			else
			{
				collision_enemy1 = false;
				collision_enemy2 = false;
				collision_enemy3 = false;
				collision_enemy4 = false;
			}


			//collision item detection
			if (keys[E] && player.x + player.boundx > item[1].x - item[1].boundx &&
				player.x - player.boundx < item[1].x + item[1].boundx &&
				player.y + player.boundy > item[1].y - item[1].boundy &&
				player.y - player.boundy < item[1].y + item[1].boundy)
			{
				collision_item1 = true;
				item[1].live = false;
				player.score++;
				//develloper
				printf("colission item 1\n");
			}
			if (keys[E] && player.x + player.boundx > item[2].x - item[2].boundx &&
				player.x - player.boundx < item[2].x + item[2].boundx &&
				player.y + player.boundy > item[2].y - item[2].boundy &&
				player.y - player.boundy < item[2].y + item[2].boundy)
			{
				collision_item2 = true;
				//develloper
				printf("colission item 2\n");
			}
			if (keys[E] && player.x + player.boundx > item[3].x - item[3].boundx &&
				player.x - player.boundx < item[3].x + item[3].boundx &&
				player.y + player.boundy > item[3].y - item[3].boundy &&
				player.y - player.boundy < item[3].y + item[3].boundy)
			{
				collision_item3 = true;
				//develloper
				printf("colission item 3\n");
			}
			if (keys[E] && player.x + player.boundx > item[4].x - item[4].boundx &&
				player.x - player.boundx < item[4].x + item[4].boundx &&
				player.y + player.boundy > item[4].y - item[4].boundy &&
				player.y - player.boundy < item[4].y + item[4].boundy)
			{
				collision_item4 = true;
				//develloper
				printf("colission item 4\n");
			}
			else
			{
				collision_item1 = false;
				collision_item2 = false;
				collision_item3 = false;
				collision_item4 = false;
			}


			//game functions during playing
			if (state == PLAYING)
			{
				if (keys[ESC])
				{
					state = BOARD;
				}
			}

		}

		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}



		//Mouse operations in menu and settings
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (state == MENU)
			{
				if (pos_x>289 && pos_x <501 && pos_y > 424 && pos_y < 441)
				{
					if (ev.mouse.button & 1)
						state = PLAYER_SELECTION;
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
			else if (state == PLAYER_SELECTION)
			{
				if (keys[E])
				{
					state = PLAYING;
				}
				else if (pos_x > 75 && pos_x < 125 && pos_y>300 && pos_y < 375)
				{
					if (ev.mouse.button & 1)
					{
						Bocian = true;
						state = PLAYING;
					}

				}
				else if (pos_x > 275 && pos_x < 325 && pos_y>300 && pos_y < 375)
				{
					if (ev.mouse.button & 1)
					{
						Mati = true;
						state = PLAYING;
					}

				}
				else if (pos_x > 475 && pos_x < 525 && pos_y>300 && pos_y < 375)
				{
					if (ev.mouse.button & 1)
					{
						Jakub = true;
						state = PLAYING;
					}

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

		//mouse position develloper settings
		else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			if (state == MENU || state == PLAYER_SELECTION || state == BOARD || state==PLAYING)
			{
				printf("%i , %i\n", pos_x, pos_y);
			}

			pos_x = ev.mouse.x;
			pos_y = ev.mouse.y;
		}




		//*********************Rendering**********************//
		//****************************************************//

		if (redraw && al_event_queue_is_empty(event_queue)) //used to avoid flickering
		{

			redraw = false;

			//*************************Game State************************//
			//***********************************************************//
			if (state == PLAYING)
			{

				al_draw_bitmap(map, screen_w - map_w, screen_h - map_h, 0);

				DrawPlayer(player);
				DrawItem(item,ITEM_NUMBER);
				al_draw_circle(enemy_x, enemy_y, 6, al_map_rgb(255, 0, 0), 10);
				//DrawEnemy(enemy, ENEMY_NUMBER);
				al_draw_text(font8bit, al_map_rgb(255, 255, 255), screen_w / 2, screen_h - 30, ALLEGRO_ALIGN_CENTER, "press esc to settings");
				al_draw_textf(talk_font, al_map_rgb(255, 255, 255), screen_w - 100, 35, ALLEGRO_ALIGN_CENTER, "score %i", player.score);


				//collision_enemy detection - developer
				if (bound)
				{
					al_draw_filled_rectangle(player.x - player.boundx, player.y - player.boundy, player.x + player.boundx, player.y + player.boundy, al_map_rgba_f(.6, 0, .6, .6));

					for (int i = 0; i < ENEMY_NUMBER; i++)
					{
						al_draw_filled_rectangle(enemy[i].x - enemy[i].boundx, enemy[i].y - enemy[i].boundy, enemy[i].x + enemy[i].boundx, enemy[i].y + enemy[i].boundy, al_map_rgba_f(.6, 0, .6, .6));
					}

					for (int i = 0; i < ITEM_NUMBER; i++)
					{
						al_draw_filled_rectangle(item[i].x - item[i].boundx, item[i].y - item[i].boundy, item[i].x + item[i].boundx, item[i].y + item[i].boundy, al_map_rgba_f(.6, 0, .6, .6));
					}
					

				}

			}

			//**************************MENU**********************//
			//****************************************************//

			else if (state == MENU)
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
			//*********************PLAYER SELECTION******************//
			//******************************************************//
			else if (state == PLAYER_SELECTION)
			{
				al_draw_bitmap(player_selection_background, 0, 0, 0);
				al_draw_text(title_font, al_map_rgb(255, 255, 255), screen_w / 2, 150, ALLEGRO_ALIGN_CENTER, "Choose player");


				if (pos_x > 75 && pos_x < 125 && pos_y>300 && pos_y < 375)
				{
					al_draw_rectangle(75, 300, 125, 375, al_map_rgb(255, 255, 255), 5);
					al_draw_bitmap(player_Bocian, 75, 300, ALLEGRO_FLIP_VERTICAL);
					al_draw_text(talk_font, al_map_rgb(255, 255, 255), 75, 400, 0, "BOCIAN");
				}
				else
				{
					al_draw_rectangle(75, 300, 125, 375, al_map_rgb(255, 0, 0), 5);
					al_draw_bitmap(player_Bocian, 75, 300, 0);
					al_draw_text(talk_font, al_map_rgb(255, 0, 0), 75, 400, 0, "BOCIAN");
				}

				if (pos_x > 275 && pos_x < 325 && pos_y>300 && pos_y < 375)
				{
					al_draw_rectangle(275, 300, 325, 375, al_map_rgb(255, 255, 255), 5);
					al_draw_bitmap(player_Dolny, 275, 300, ALLEGRO_FLIP_VERTICAL);
					al_draw_text(talk_font, al_map_rgb(255, 255, 255), 275, 400, 0, "DOLNY");
				}
				else
				{
					al_draw_rectangle(275, 300, 325, 375, al_map_rgb(255, 0, 0), 5);
					al_draw_bitmap(player_Dolny, 275, 300, 0);
					al_draw_text(talk_font, al_map_rgb(255, 0, 0), 275, 400, 0, "DOLNY");
				}

				if (pos_x > 475 && pos_x < 525 && pos_y>300 && pos_y < 375)
				{
					al_draw_rectangle(475, 300, 525, 375, al_map_rgb(255, 255, 255), 5);
					al_draw_bitmap(player_Jakub, 475, 300, ALLEGRO_FLIP_VERTICAL);
					al_draw_text(talk_font, al_map_rgb(255, 255, 255), 475, 400, 0, "JAKUB");
				}
				else
				{
					al_draw_rectangle(475, 300, 525, 375, al_map_rgb(255, 0, 0), 5);
					al_draw_bitmap(player_Jakub, 475, 300, 0);
					al_draw_text(talk_font, al_map_rgb(255, 0, 0), 475, 400, 0, "JAKUB");
				}

				al_draw_rectangle(675, 300, 725, 375, al_map_rgb(255, 0, 0), 5);

			}
			//***********************BOARD**************************//
			//******************************************************//

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
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);

}


//*****************Objects Functions************************//
//*********************************************************//


//Player
void InitPlayer(Student &player)
{
	player.x = 50;
	player.y = 50;
	player.ID = PLAYER;
	player.score = 0;
	player.speed = 3;
	player.width = 50;
	player.height = 75;
	player.boundx = player.width / 2;
	player.boundy = player.height / 2;

}
void DrawPlayer(Student &player)
{
	if (Bocian)
	{
		ALLEGRO_BITMAP *player_bitmap = al_load_bitmap("Bocian.png");
		int image_w = al_get_bitmap_width(player_bitmap);
		int image_h = al_get_bitmap_height(player_bitmap);
		al_draw_bitmap(player_bitmap, player.x - image_w / 2, player.y - image_h / 2, 0);
	}
	else if (Mati)
	{
		ALLEGRO_BITMAP *player_bitmap = al_load_bitmap("Dolny.png");
		int image_w = al_get_bitmap_width(player_bitmap);
		int image_h = al_get_bitmap_height(player_bitmap);
		al_draw_bitmap(player_bitmap, player.x - image_w / 2, player.y - image_h / 2, 0);
	}
	else if (Jakub)
	{
		ALLEGRO_BITMAP *player_bitmap = al_load_bitmap("Jakub.png");
		int image_w = al_get_bitmap_width(player_bitmap);
		int image_h = al_get_bitmap_height(player_bitmap);
		al_draw_bitmap(player_bitmap, player.x - image_w / 2, player.y - image_h / 2, 0);
	}


	//al_draw_filled_circle(player.x, player.y, 40, al_map_rgb(0, 255, 0));

}
//Player Movement
void MovePlayerUp(Student &player)
{
	player.y -= player.speed;
	if (player.y < 0)
	{
		player.y = 0;
	}
}
void MovePlayerDown(Student &player)
{
	player.y += player.speed;
	if (player.y > screen_h)
	{
		player.y = screen_h;

	}
}
void MovePlayerLeft(Student &player)
{
	player.x -= player.speed;
	if (player.x < 0)
	{
		player.x = 0;

	}
}
void MovePlayerRight(Student &player)
{
	player.x += player.speed;
	if (player.x > screen_w)
	{
		player.x = screen_w;

	}
}


//Item
void InitItem(Bottle item[], int size)
{
	for (int i = 1; i < size; i++)
	{
		item[i].ID = OBJECT;
		item[i].live = true;
		item[i].width = 10;
		item[i].height = 10;
		item[i].boundx = item[i].width;
		item[i].boundy = item[i].height;
	}
	
}
void DrawItem(Bottle item[],int size)
{
	item[1].x = 62;
	item[1].y = 125;

	item[2].x = 301;
	item[2].y = 62;

	item[3].x = 710;
	item[3].y = 248;

	item[4].x = 710;
	item[4].y = 379;

	ALLEGRO_BITMAP *bottle_bitmap = al_load_bitmap("bottle.png");
	int image_w = al_get_bitmap_width(bottle_bitmap);
	int image_h = al_get_bitmap_height(bottle_bitmap);

	for (int i = 0; i < size; i++)
	{
		al_draw_bitmap(bottle_bitmap, item[i].x - image_w / 2, item[i].y - image_h / 2, 0);
	}
	


	//al_draw_filled_circle(item.x, item.y, 10, al_map_rgb(0, 255, 255));
}