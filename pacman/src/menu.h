#pragma once
#include "game.h"
//Defines what action we should take at a particular time while on the menu.
typedef enum
{
	Nothing,
	GoToGame,
	TimeAttackMode, // # 9 Dong : 확장맵 테스트
	ScoreMode, // # 9 Dong : 확장맵 테스트 //#32 Yang : smallMap LargeMap 이름 스코어모드 타임어택모드로 바꿈
	GoToJoin,// #19 Kim : 1. 메뉴에서 눌렀을때 열로가려고 ..만들었음일단..
	GoToMulti, // # 9 Dong : 확장맵 테스트
	WaitClient,// #19 Kim : 2. 방만들기 했을 때 클라이언트 기다리기
	JoinServer, // #25 join room 선택시 이액션
	ReturnMenu // #35 make 룸에서 백스페이스 키시 이액션
} MenuAction;

//Defines the menu system.
typedef struct
{
	MenuAction action;
	unsigned int ticksSinceModeChange;
	PlayMode playMode;//# 메뉴화면에 넣기위한 playMode...
} MenuSystem;

//Performs a single tick on the menu system.
int menu_tick(MenuSystem *menuSystem);
int multi_mode_render(MenuSystem *menuSystem); // # 9 Dong : 확장맵을 위한 구현
int online_mode_render(MenuSystem *menuSystem);// #20 Kim : 1. int 로 반환형 바꾸가


//Renders the menu system in its current state.
void menu_render(MenuSystem *menuSystem);

void menu_init(MenuSystem *menuSystem);
int getKey(void);// #19 Kim : 키 받아오기위해서
