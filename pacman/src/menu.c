#include "menu.h"

#include <stdbool.h>

#include <SDL/SDL_keysym.h>

#include "input.h"
#include "ghost.h"
#include "main.h"
#include "renderer.h"
#include "server.h"
#include "client.h"

//time till ghost-rows start appearing
#define GHOST_START 500

//time between each ghost-row appearance
#define GHOST_BETWEEN 500
static char tmp[100]={"000.000"};
static int index_num=0;
static int s_c_num = 0;

static void draw_vanity_screen(MenuSystem *menuSystem);
static void draw_info_screen(void);
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
	bool startNew = key_released(SDLK_KP_ENTER) || key_released(SDLK_RETURN);
	//#19 Kim : 2. 엔터 누르면 keyHeld 라서 다음 장면에서도 엔터가 적용되어서 바로 넘어가버리는 버그 잡기위해 released 로 바꿈

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

int getKey(void)// #19 Kim : 1. 여기서 키값 받아서 와따가따리
{
	for(int i = 48 ; i <=57 ; i ++)
	{
		if(key_released(i))
			return i;
	}
	if(key_released(SDLK_PERIOD))
		return '.';
	else if(key_released(SDLK_UP))
		return SDLK_UP;
	else if(key_released(SDLK_DOWN))
		return SDLK_DOWN;
	else if(key_released(SDLK_KP_ENTER)||key_released(SDLK_RETURN))
		return SDLK_KP_ENTER;// #19 Kim : 2. 엔터가 아니라 SDLK_RETURN 인듯. 엔터치면 ㅇㅅㅇ
	else if(key_released(SDLK_PERIOD))
		return SDLK_PERIOD;
	return -1;
}

int online_mode_render(MenuSystem *menuSystem)// #19 Kim : 2. 여기서 그려줌
{
	int get= getKey();
	if(menuSystem->action==WaitClient)
	{
		makeServer();
		menuSystem->action=GoToGame;
		menuSystem->playMode=Online_Server;// #12 Kim : 2. 잠시 테스트용
		return 1;
	}
	else if(menuSystem->action==JoinServer)
	{
		connectServer(tmp);
		menuSystem->action = GoToGame;
		menuSystem->playMode = Online_Client;//#25 클라이언트쪽 접속하는 코드 추
		return 2;
	}
	if(get==SDLK_UP&&s_c_num==1)
		{
			s_c_num--;
		}
		else if(get==SDLK_DOWN&&s_c_num==0)
		{
			s_c_num++;
		}
		else if(s_c_num==1 && ( (get>=48&&get<=57) ||get==SDLK_PERIOD)) //#25 닷 찍으면 문자열 들어가도록.
			tmp[index_num++] = (char)get;
		else if(get==SDLK_KP_ENTER)
		{
			if(s_c_num==0)//ROOM 만들 때
			{
				draw_input_string("WAITING CLIENT");// #19 Kim : 2. waiting client 그려줌 이름을 맞게 바꿔줌
				menuSystem->action=WaitClient;
				return 0;
			}
			else if(s_c_num==1)
			{
				draw_input_string("CONNECT SERVER");
				menuSystem->action = JoinServer;
				return 0;
			}
		}
		draw_online_mode(&s_c_num,tmp);
		return 1;
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
