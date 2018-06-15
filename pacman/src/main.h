#pragma once

//28 across, 36 down, 16 pixels each square.
#define SCREEN_TITLE "Tphakumahn"
#define SCREEN_WIDTH 448
#define SCREEN_HEIGHT 576

//Defines the main states the program can be in.
typedef enum
{
	Menu,
	Game,
	Intermission,
	Joinmulti, // # 9 Dong : 확장맵 사용을 위한 수정사항
	Join// #19 Kim : 1. 방 만들고 들어가기 위한 새로운 State 추가.

} ProgramState;


//Returns the number of credits the user currently has.
int num_credits(void);
int Multi_flags(void);
//static void resource_init_Multi(void); // # 9 Dong : 2P 맵 연동을 위한 함수
void game_init2(int level); // # 9 Dong : 2.
void game_init(int level); // # 9 Dong : 레벨별 맵 연동을위한 자리이동
