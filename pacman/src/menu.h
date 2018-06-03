#pragma once
#include "game.h"
//Defines what action we should take at a particular time while on the menu.
typedef enum
{
	Nothing,
	GoToGame,
	GoToJoin,// #19 Kim : 1. 메뉴에서 눌렀을때 열로가려고 ..만들었음일단..
	GoToMulti, // # 9 Dong : 확장맵 테스트
	WaitClient// #19 Kim : 2. 방만들기 했을 때 클라이언트 기다리기
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
