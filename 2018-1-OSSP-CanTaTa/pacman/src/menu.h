#pragma once
#include "game.h"
//Defines what action we should take at a particular time while on the menu.
typedef enum
{
	Nothing,
	GoToGame,
	GoToJoin// #19 Kim : 1. 메뉴에서 눌렀을때 열로가려고 ..만들었음일단..
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
void online_mode_render(MenuSystem *menuSystem);


//Renders the menu system in its current state.
void menu_render(MenuSystem *menuSystem);

void menu_init(MenuSystem *menuSystem);
