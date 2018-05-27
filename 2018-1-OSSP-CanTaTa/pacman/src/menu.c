#include "menu.h"

#include <stdbool.h>

#include <SDL/SDL_keysym.h>

#include "input.h"
#include "ghost.h"
#include "main.h"
#include "renderer.h"


//time till ghost-rows start appearing
#define GHOST_START 500

//time between each ghost-row appearance
#define GHOST_BETWEEN 500
static char tmp[100]={"000.000"};
static int index_num=0;


static int getKey();
static void draw_vanity_screen(MenuSystem *menuSystem);
static void draw_info_screen(void);
static void draw_online_mode(MenuSystem *menuSystem);
static void draw_ghost_line(GhostDisplayRow *row, int y, unsigned int dt);
static void draw_player_info(void);

static GhostDisplayRow enemyRows[4] = {
	{Blinky, "-SHADOW",  "\"BLINKY\"", RedText},
	{Pinky,  "-SPEEDY",  "\"PINKY\"",  PinkText},
	{Inky,   "-BASHFUL", "\"INKY\"",   CyanText},
	{Clyde,  "-POKEY",   "\"CLYDE\"",  OrangeText}
};

void menu_init(MenuSystem *menuSystem)
{
	menuSystem->action = Nothing;
	menuSystem->ticksSinceModeChange = SDL_GetTicks();
	menuSystem->playMode=Single;
}
// #8 Kim : 3. 메뉴 만들기
// int 반환 하도록 함
//  메뉴에서  key up하면 -1  ,down 하면 1 . 아무것도 아니면 0
int menu_tick(MenuSystem *menuSystem)
{
	bool startNew = key_held(SDLK_KP_ENTER) || key_held(SDLK_RETURN);
	if (startNew)
	{
		if(menuSystem->playMode==Online)
			menuSystem->action = GoToJoin;// #19 Kim : 1. 여기서 저게 온라인게임으로 되미녀 엑션 바뀌
		else
			menuSystem->action = GoToGame;
		return 0;
	}
	// #13 Kim : 1 Key held로 하면 계속 눌린거로 되서 released를 사용
	if (key_released(SDLK_UP))
		return -1;
	else if(key_released(SDLK_DOWN))
		return 1;
	else
		return 0;
}

void menu_render(MenuSystem *menuSystem)
{
	if (num_credits() == 0) draw_vanity_screen(menuSystem);
	else draw_info_screen();
}

static void draw_vanity_screen(MenuSystem *menuSystem)
{
	unsigned int dt = SDL_GetTicks() - menuSystem->ticksSinceModeChange;

	draw_player_info();
	draw_vanity_charnickname();

	for (int i = 0; i < 4; i++)
	{
		unsigned int current = GHOST_START + i * GHOST_BETWEEN;
		if (dt < current) break;

		GhostDisplayRow r = enemyRows[i];
		draw_ghost_line(&r, 7 + 3 * i, dt - current);
	}
	//#13 Kim : 일단 이부분좀 빠르게 지바꿔놓을까 생각중
	//if (dt > 3000) draw_vanity_pellet_info(false);
	if(dt>3000)draw_playMode(menuSystem->playMode);
	if (dt > 4000) draw_vanity_corporate_info();
	if (dt > 5000) draw_vanity_animation(dt - 5000);
}
static void draw_online_mode(MenuSystem *menuSystem)//#19 Kim : 1. 일단 메뉴에서 눌렀을때 들어가서 조작하는 화면 만들어보기
{
	unsigned int dt = SDL_GetTicks() - menuSystem->ticksSinceModeChange;


	if(getKey())
		tmp[index_num++] = (char)getKey();

	set_text_color(WhiteText);
	draw_text_coord(get_screen(), "MAKE ROOM", 10, 8);
	draw_text_coord(get_screen(), "JOIN ROOM", 10, 13);
	draw_text_coord(get_screen(), "WRITE SERVER IP", 7, 18);
	draw_text_coord(get_screen(), tmp, 7, 23	);

}
static int getKey()
{
	for(int i = 48 ; i <=57 ; i ++)
	{
		if(key_released(i))
			return i;
	}
	if(key_released(SDLK_PERIOD))
		return '.';

	return 0;
}

void online_mode_render(MenuSystem *menuSystem)
{

		draw_online_mode(menuSystem);

}


static void draw_info_screen(void)
{
	draw_player_info();
	draw_instrc_info();
	draw_instrc_corporate_info();

}

static void draw_player_info(void)
{
	draw_common_oneup(false, 0);
	draw_common_twoup(false, 0);
	draw_common_highscore(0);

	draw_credits(num_credits());
}

static void draw_ghost_line(GhostDisplayRow *row,  int y, unsigned int dt)
{
	bool drawDescription = dt > 1000;
	bool drawName = dt > 1500;

	draw_vanity_ghostline(row, y, drawDescription, drawName);
}
