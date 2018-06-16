#pragma once

#include <stdbool.h>

#include "board.h"
#include "fruit.h"
#include "ghost.h"
#include "pacman.h"
#include "pellet.h"
#include "text.h"
#include "object.h"
#include "game.h"
#include "main.h"

//Holds data relating to a single rendered ghost-row.
typedef struct
{
	GhostType  type;
	const char *description;
	const char *name;
	TextColor color;
} GhostDisplayRow;

//Class handles the drawing of all entities in the game

//
// Vanity screen renderering
//

void draw_vanity_charnickname(void);
void draw_vanity_ghostline(GhostDisplayRow *row, int y, bool description, bool name);
void draw_vanity_corporate_info(void);
void draw_vanity_pellet_info(bool flashing);
void draw_vanity_animation(int dt);

//
// Instructions screen renderering
//

void draw_instrc_info(void);
void draw_instrc_corporate_info(void);

//
// Common menu rendering
//

void draw_common_oneup(bool flashing, int score);
void draw_common_twoup(bool flashing, int score, int flag);
void draw_common_highscore(int highscore);
void draw_credits(int numCredits);

//
// Game-only word rendering
//

void draw_game_playerone_start(void);
void draw_game_playertwo_start(void);
void draw_game_ready(void);
void draw_game_gameover(void);
void draw_game_time(int time);
//#30 Yang :winner 표시 뷰
void draw_game_playerone_win(int flag);
void draw_game_playertwo_win(int flag);
//
// Fruit renderering
//

void draw_fruit_indicators(int currentLevel);
void draw_fruit_game(int currentLevel, GameFruit *gameFruit);


//#5 Yang : 3. obeject renderering
void draw_object_indicators(int currentLevel);
void draw_object_game(int currentLevel, GameObject *gameObject);
//
// Pellet renderering
//

void draw_pellet_small(Pellet *p);
void draw_pellet_power(Pellet *p, bool flashing);

void draw_small_pellets(PelletHolder *ph);
void draw_large_pellets(PelletHolder *ph, bool flashing);

//
// Pacman renderering
//

void draw_pacman(Pacman *pacman, int num);
void draw_pacman_static(Pacman *pacman, int num);
void draw_pacman_death(Pacman *pacman, unsigned int timeSinceDeath, int num);
void draw_pacman_lives(int numLives1, int numLives2, int flag);

//
// Ghost rendering
//

void draw_ghost(Ghost *ghost);
bool draw_scared_ghost(Ghost *ghost, unsigned int timeGhostScared);
void draw_eyes(Ghost *ghost);

//
// Points rendering
//

void draw_fruit_pts(GameFruit *gameFruit);

//
// Board rendering
//

void draw_board(Board *board);
void draw_board_flash(Board *board);

void draw_mutil_mode(int *s_c_num); // # 9 Dong : 확장 맵 구현을 위한 추가

void draw_playMode(PlayMode playMode);//#13 kim : 메뉴에다가 그플레이ㄷ모드 그려죽ㅣ

void draw_online_mode(int *s_c_num ,char* tmp);//#19 Kim : 2. menu.c에있는거 옮겼음
void draw_input_string(const char tmp[],int x,int y);//#19 Kim : 2. 클라이언트 기다리는거 화면 추가.
void draw_multi_mode(int *s_c_num);
