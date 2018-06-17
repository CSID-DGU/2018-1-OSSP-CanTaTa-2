#include "game.h"

#include "animation.h"
#include "board.h"
#include "fps.h"
#include "input.h"
#include "main.h"
#include "pacman.h"
#include "pellet.h"
#include "physics.h"
#include "renderer.h"
#include "sound.h"
#include "text.h"
#include "window.h"
#include <stdlib.h>
#include <time.h>
#include "server.h"// #20 Kim : 1. 헤더 추가해주기
#include "ghost.h"

static void process_player(PacmanGame *game,int player_num);// #8 Kim 2. player num 추가
static void process_fruit(PacmanGame *game, int player_num);//#5 Yang : 5. playernum 추가
static void process_ghosts(PacmanGame *game, int flag);
static void process_pellets(PacmanGame *game,int player_num);// #8 Kim 3. player num 추가
static void process_object(PacmanGame *game, int player_num); //Yang #5: 2. object

static bool check_pacghost_collision(PacmanGame *game,int player_num);     //return true if pacman collided with any ghosts
//	//#14 Kim : 1. 흠 어케하지 이거를. 일단 player 2개로 추가해보도록 함

static void enter_state(PacmanGame *game, GameState state); //transitions to/ from a state
// #13 Kim : 1. 이 부분 collision 일어난 뒤에 state 부분에 DeathState 붙어서 가게됨.
//는 다시 수정.

static bool resolve_telesquare(PhysicsBody *body, int flag);          //wraps the body around if they've gone tele square

void game_tick(PacmanGame *game)
{
	unsigned dt = ticks_game() - game->ticksSinceModeChange;

	switch (game->gameState)
	{
		case GameBeginState:
			// plays the sound, has the "player 1" image, has the "READY!" thing

			break;
		case LevelBeginState:
			// similar to game begin mode except no sound, no "Player 1", and slightly shorter duration

			break;
		case GamePlayState:
			// everyone can move and this is the standard 'play' game mode
			//#8 2. player 두개로 늘림
			process_player(game,0);
			if(game->playMode!=Single)// #13 Kim : 1. play Mode 에 따라서 추가
				process_player(game,1);


			process_ghosts(game,0);
			if(game->playMode==Multi_TA)
				process_ghosts(game,1);

			process_fruit(game,0);
			process_object(game,0);

			//#8 3. collusion pellet check 2개로
			process_pellets(game,0);

			if(game->playMode!=Single){// #13 Kim : 1. play Mode 에 따라서 추가
				process_pellets(game,1);
				process_object(game,1);//#5 Yang : 5. 2p일 때 변수추
				process_fruit(game,1);
			}
			if (game->pacman[0].score > game->highscore ) game->highscore = game->pacman[0].score;// #8 Kim : 1.
			if(game->playMode!=Single)// #13 Kim : 1. play Mode 에 따라서 추가
				if (game->pacman[1].score > game->highscore ) game->highscore = game->pacman[1].score;// #8 Kim : 2. 만약 p2가 최고점수면 ㅇㅇ

			break;
		case WinState:
			//pacman eats last pellet, immediately becomes full circle
			//everything stays still for a second or so
			//monsters + pen gate disappear
			//level flashes between normal color and white 4 times
			//screen turns dark (pacman still appears to be there for a second before disappearing)
			//full normal map appears again
			//pellets + ghosts + pacman appear again
			//go to start level mode

			break;
		case DeathState:case DeathState2: case ReviveState1:case ReviveState2:
			// pacman has been eaten by a ghost and everything stops moving
			// he then does death animation and game starts again

			//everything stops for 1ish second

			//ghosts disappear
			//death animation starts
			//empty screen for half a second

			break;
		case GameoverState:
			// pacman has lost all his lives
			//it displays "game over" briefly, then goes back to main menu
			break;
	}

	//
	// State Transitions - refer to gameflow for descriptions
	//

	bool allPelletsEaten = game->pelletHolder.numLeft == 0;
	bool collidedWithGhost = check_pacghost_collision(game,0);	//#14 Kim : 1. 흠 어케하지 이거를. 일단 player 선택하여 넣도록 추가해보도록 함
	bool collidedWithGhost2=false;
	if(game->playMode!=Single)
		collidedWithGhost2 = check_pacghost_collision(game,1);
	int lives1 = game->pacman[0].livesLeft;
	int lives2 = game->pacman[1].livesLeft;

	switch (game->gameState)
	{
		case GameBeginState:
			if (dt > 2200) enter_state(game, LevelBeginState);

			break;
		case LevelBeginState:
			if (dt > 1800) enter_state(game, GamePlayState);
			game->pacman[0].godMode = false;// #8 Kim : 1. 흠..
			break;
		case ReviveState1:// #14 Kim : 2. ReviveState라는걸 추가해서 죽었을때 Level BeginState가 아니라 Revive로 가게했음
			if (dt > 1800) enter_state(game, GamePlayState);
			//game->pacman[0].godMode = false;// #8 Kim : 1. 흠..
			break;
		case ReviveState2:
			if (dt > 1800) enter_state(game, GamePlayState);
			//game->pacman[0].godMode = false;// #8 Kim : 1. 흠..
			break;
		case GamePlayState:

			//TODO: remove this hacks
			if (key_held(SDLK_k)) enter_state(game, WinState);

			else if (allPelletsEaten) enter_state(game, WinState);
			else if (collidedWithGhost) enter_state(game, DeathState);//#14 일단 이때. 열로 들어가는데... 현제 스테이트는 GamePlayState고..
			if(game->playMode!=Single&&collidedWithGhost2)enter_state(game,DeathState2);//#14 Kim : 2. 2p가 죽었을때는 DeathState2로 간다.

			break;
		case WinState:
			//if (transitionLevel) //do transition here
			if (dt > 4000) enter_state(game, LevelBeginState);

			break;
		case DeathState:
			if (dt > 4000)
			{
				if (lives1 == 0) enter_state(game, GameoverState);
				else enter_state(game, ReviveState1);// #14 Kim : 2. ReviveState라는걸 추가해서 죽었을때 Level BeginState가 아니라 Revive로 가게했음
			}
			break;
		case DeathState2:
			if (dt > 4000)
			{
				if (lives2 == 0) enter_state(game, GameoverState); //#8 Yang : 2p 라이프 0이되도 게임 끝나지 않는 부분 수정
				else enter_state(game, ReviveState2);// #14 Kim : 2. ReviveState라는걸 추가해서 죽었을때 Level BeginState가 아니라 Revive로 가게했음
			}
				break;
		case GameoverState:
			if (dt > 2000)
			{
				//TODO: go back to main menu

			}
			break;
	}
}

void game_render(PacmanGame *game)
{
	unsigned dt = ticks_game() - game->ticksSinceModeChange;
	static unsigned godDt = 0;
	static bool godChange = false;


	//common stuff that is rendered in every mode:
	// 1up + score, highscore, base b록oard, lives, small pellets, fruit indicators
	draw_common_oneup(true, game->pacman[0].score);// #8 Kim : 1.
	if(game->playMode==Multi_TA) draw_common_twoup(true, game->pacman[1].score,0);//#37 Yang: 2P UI 추가 - 점수 나오도록
	else if(game->playMode!=Single)draw_common_twoup(true,game->pacman[1].score,1);
	if(game->playMode!=Multi_TA)draw_common_highscore(game->highscore);
	if(game->playMode==Multi_TA&&(game->time-ticks_game()+game->get_ticks)/1000>0) draw_game_time((game->time-ticks_game()+game->get_ticks)/1000);
	//if(game->playMode==Multi_TA&&(game->time-ticks_game()+game->get_ticks)/1000<=0) draw_game_time(0);
	//#37 Yang :2P UI 추가 생명 나오도
	if(game->playMode==Multi_TA) draw_pacman_lives(game->pacman[0].livesLeft,game->pacman[1].livesLeft,0);
	else if(game->playMode!=Single)draw_pacman_lives(game->pacman[0].livesLeft,game->pacman[1].livesLeft,1);
	else draw_pacman_lives(game->pacman[0].livesLeft,0,0);// #8 Kim : 1. 2p도 추가해줘야할듯!

	draw_small_pellets(&game->pelletHolder);
	if(game->playMode!=Multi)draw_fruit_indicators(game->currentLevel);

	//in gameover state big pellets don't render
	//in gamebegin + levelbegin big pellets don't flash
	//in all other states they flash at normal rate

	switch (game->gameState)
	{
		case GameBeginState:
			draw_game_playerone_start();
			draw_game_ready();
			game->time=11000;
			game->get_ticks=ticks_game();

			draw_large_pellets(&game->pelletHolder, false);
			draw_board(&game->board);
			break;
		case LevelBeginState:
			draw_game_ready();

			//we also draw pacman and ghosts (they are idle currently though)
			draw_pacman_static(&game->pacman[0],0);// #8 Kim : 1.
			if(game->playMode!=Single)// #13 Kim : 1. play Mode 에 따라서 추가
				draw_pacman_static(&game->pacman[1],1);// #8 Kim : 2. pacman 2도 그려보자~~
			//#28 Yang : 1.난이도 조절 ghost 수 조절
			for(int i = 0; i < ghost_number(game->currentLevel); i++) draw_ghost(&game->ghosts[0][i]);
			if(game->playMode==Multi_TA)
				for(int i=0;i<ghost_number(game->currentLevel);i++) draw_ghost(&game->ghosts[1][i]);
			draw_large_pellets(&game->pelletHolder, false);
			draw_board(&game->board);
			break;
		case ReviveState1://#14 Kim : 2. ghost collision 후의 그냥 계속 진행 하게끔!!
			draw_pacman_static(&game->pacman[0],0);// #8 Kim : 1.
			if (game->playMode!=Single)
				draw_pacman_static(&game->pacman[1],1);
			for (int i = 0; i < ghost_number(game->currentLevel); i++) draw_ghost(&game->ghosts[0][i]);
			if(game->playMode==Multi_TA)
				for(int i=0;i<ghost_number(game->currentLevel);i++) draw_ghost(&game->ghosts[1][i]);
			draw_large_pellets(&game->pelletHolder, false);
			draw_board(&game->board);
			break;
		case ReviveState2:
			draw_pacman_static(&game->pacman[1],1);// #8 Kim : 1.
			draw_pacman_static(&game->pacman[0],0);
			for (int i = 0; i < ghost_number(game->currentLevel); i++) draw_ghost(&game->ghosts[0][i]);
			if(game->playMode==Multi_TA)
				for(int i=0;i<ghost_number(game->currentLevel);i++) draw_ghost(&game->ghosts[1][i]);
			draw_large_pellets(&game->pelletHolder, false);
			draw_board(&game->board);
			break;
		case GamePlayState:

			if(ticks_game()%100==0)game->time--;
			draw_large_pellets(&game->pelletHolder, true);
			draw_board(&game->board);

			for(int i=0;i<5;i++){
				if (game->gameFruit[0][i].fruitMode == Displaying) draw_fruit_game(game->currentLevel, &game->gameFruit[0][i]);
				if (game->gameFruit[0][i].eaten && ticks_game() - game->gameFruit[0][i].eatenAt < 2000) draw_fruit_pts(&game->gameFruit[0][i]);
			}

			// #5 Yang : 3.object 표시
			for(int i=0;i<3;i++){
				if (game->gameObject[0][i].objectMode == Displaying_obj) draw_object_game(game->currentLevel, &game->gameObject[0][i]);
			}
			// #8 Kim : 1.
			draw_pacman(&game->pacman[0],0);
			// #8 Kim : 2.
			if(game->playMode!=Single)// #13 Kim : 1. play Mode 에 따라서 추가
				draw_pacman(&game->pacman[1],1);


			if(game->pacman[0].godMode == false) {
				for (int i = 0; i < ghost_number(game->currentLevel); i++) {
					if(game->ghosts[0][i].isDead == 1) {
						draw_eyes(&game->ghosts[0][i]);
					} else
						draw_ghost(&game->ghosts[0][i]);
					//if(game->playMode==Multi_TA) draw_ghost(&game->ghosts[1][i]);
				}

			}
			else {
				if(godChange == false) {
					game->pacman[0].originDt = ticks_game();
					godChange = true;
				}
				godDt = ticks_game() - game->pacman[0].originDt;
				for (int i = 0; i < ghost_number(game->currentLevel); i++) {
					if(game->ghosts[0][i].isDead == 1) {
						draw_eyes(&game->ghosts[0][i]);
					} else if(draw_scared_ghost(&game->ghosts[0][i], godDt)){
						// nothing
						if(game->ghosts[0][i].isDead == 2) {
							draw_ghost(&game->ghosts[0][i]);
						}
					} else {
						game->pacman[0].godMode = false;
						godChange = false;
						if(game->ghosts[0][i].isDead == 2)
							game->ghosts[0][i].isDead = 0;
					}
				}
			}
			if(game->playMode==Multi_TA){
				if(game->pacman[1].godMode == false) {
					for (int i = 0; i < ghost_number(game->currentLevel); i++) {
						if(game->ghosts[1][i].isDead == 1) {
							draw_eyes(&game->ghosts[1][i]);
						} else
							draw_ghost(&game->ghosts[1][i]);
						//if(game->playMode==Multi_TA) draw_ghost(&game->ghosts[1][i]);
					}

				}
				else {
					if(godChange == false) {
						game->pacman[1].originDt = ticks_game();
						godChange = true;
					}
					godDt = ticks_game() - game->pacman[1].originDt;
					for (int i = 0; i < ghost_number(game->currentLevel); i++) {
						if(game->ghosts[1][i].isDead == 1) {
							draw_eyes(&game->ghosts[1][i]);
						} else if(draw_scared_ghost(&game->ghosts[1][i], godDt)){
							// nothing
							if(game->ghosts[0][i].isDead == 2) {
								draw_ghost(&game->ghosts[1][i]);
							}
						} else {
							game->pacman[1].godMode = false;
							godChange = false;
							if(game->ghosts[1][i].isDead == 2)
								game->ghosts[1][i].isDead = 0;
						}
					}
				}
				if((int)(game->time-ticks_game()+game->get_ticks)<0) game->gameState=GameoverState;
			}

			break;
		case WinState:

			draw_pacman_static(&game->pacman[0],0);
			if(game->playMode!=Single) game->gameState=GameoverState;
			else{
			if (dt < 2000)
			{
				for (int i = 0; i < ghost_number(game->currentLevel); i++) draw_ghost(&game->ghosts[0][i]);
				if(game->playMode==Multi_TA)
					for(int i=0;i<ghost_number(game->currentLevel);i++) draw_ghost(&game->ghosts[1][i]);
				draw_board(&game->board);
			}
			else
			{
				//stop rendering the pen, and do the flash animation
				draw_board_flash(&game->board);
			}
			}
			break;
		case DeathState: // #14 Kim : 2. 여기 통쨰로임 ㅇㅅㅇ
			//draw everything the same for 1ish second
			if (dt < 500)
			{
				//draw everything normally
				//TODO: this actually draws the last frame pacman was on when he died
				draw_pacman_static(&game->pacman[0],0);
				if (game->playMode!=Single)// #14
					draw_pacman_static(&game->pacman[1],1);
				for (int i = 0; i < ghost_number(game->currentLevel); i++) draw_ghost(&game->ghosts[0][i]);
				if(game->playMode==Multi_TA)
					for(int i=0;i<ghost_number(game->currentLevel);i++) draw_ghost(&game->ghosts[1][i]);
			}
			else
			{
				//draw the death animation
				draw_pacman_death(&game->pacman[0], dt - 500,0);
				if(game->playMode!=Single)//#14
					draw_pacman_static(&game->pacman[1],1);
			}
			//#14 Kim : 2. 이 부분에서 ~ pac맨 그려준다요 그래서 부딪히고 죽는 모션에서 다른 플레이어도 출력하게끔 했음
			draw_large_pellets(&game->pelletHolder, true);
			draw_board(&game->board);
			break;
		case DeathState2://#14 Kim : 2. 2P가 죽었을 때. 여기도 마찬가지~
			//draw everything the same for 1ish second
			if (dt < 500)
			{
				//draw everything normally
				//TODO: this actually draws the last frame pacman was on when he died
				draw_pacman_static(&game->pacman[1],1);
				draw_pacman_static(&game->pacman[0],0);
				for (int i = 0; i < 4; i++) draw_ghost(&game->ghosts[0][i]);
				if(game->playMode==Multi_TA)
					for(int i=0;i<ghost_number(game->currentLevel);i++) draw_ghost(&game->ghosts[1][i]);
			}
			else
			{	//#14 Kim : 2. 이 부분에서 ~ pac맨 그려준다요 그래서 부딪히고 죽는 모션에서 다른 플레이어도 출력하게끔 했음
				//draw the death animation
				draw_pacman_death(&game->pacman[1], dt - 500,1);
				draw_pacman_static(&game->pacman[0],0);
			}
			draw_large_pellets(&game->pelletHolder, true);
			draw_board(&game->board);
			break;

		case GameoverState:
			//#31 Yang : 점수로 승부판정모드 - 일단 멀티모드 자체를 점수 높으면 이기는 걸로 수정
			if(game->playMode!=Single){
				if(game->pacman[0].score>game->pacman[1].score&&game->pacman[0].livesLeft)
					if(game->playMode==Multi_TA) draw_game_playerone_win(1);
					else draw_game_playerone_win(0);
				else if(game->pacman[0].score<game->pacman[1].score&&game->pacman[1].livesLeft){
					if(game->playMode==Multi_TA) draw_game_playertwo_win(1);
					else draw_game_playertwo_win(0);
				}
				else if(game->pacman[0].livesLeft==0) {
					if(game->playMode==Multi_TA) draw_game_playertwo_win(1);
					else draw_game_playertwo_win(0);
				}
				else {
					if(game->playMode==Multi_TA) draw_game_playerone_win(1);
					else draw_game_playerone_win(0);
				}
				break;
			}
			draw_game_gameover();
			draw_board(&game->board);
			draw_credits(num_credits());
			break;
	}
}

static void enter_state(PacmanGame *game, GameState state)
{// #14 Kim : 1. 이 부분 collision 일어난 뒤에 state 부분에 DeathState 붙어서 가게됨.
	//process leaving a state
	switch (game->gameState)
	{
		case GameBeginState:
			game->pacman[0].livesLeft--;
			if (game->playMode!=Single)
				game->pacman[1].livesLeft--;//#14 Kim : 2. Player_num 없애고 그냥 이거로 대체

			break;
		case WinState:
			//#31 Yang : 점수 승부판정모드
			if(game->playMode!=Single)
			{
				game->gameState=GameoverState;
			}else{
			game->currentLevel++;
			game->gameState = LevelBeginState;
			level_init(game);
			}
			break;
		case DeathState:
			// Player died and is starting a new game, subtract a life
			if (state == LevelBeginState)
			{
				game->pacman[0].livesLeft--;
				pacdeath_init(game,0);// #14 Kim : 2. 이거 한번 시도해보자 이거 주석처리하고 아래에서 처리해보자
			}
			else if(state==ReviveState1)// #14 Kim : 2. ReviveState라는걸 추가해서 죽었을때 Level BeginState가 아니라 Revive로 가게했음
			{
				game->pacman[0].livesLeft--;
				pacdeath_init(game,0);// #14 Kim : 2. 이거 한번 시도해보자 이거 주석처리하고 아래에서 처리해보자
			}
			break;
		case DeathState2://#14 Kim : 2. 2P가 죽었을 때.
				// Player died and is starting a new game, subtract a life
				if (state == LevelBeginState)
				{
					game->pacman[1].livesLeft--;
					pacdeath_init(game,1);// #14 Kim : 2. 이거 한번 시도해보자 이거 주석처리하고 아래에서 처리해보자
				}
				else if(state==ReviveState2)// #14 Kim : 2. ReviveState라는걸 추가해서 죽었을때 Level BeginState가 아니라 Revive로 가게했음
				{
					game->pacman[1].livesLeft--;
					pacdeath_init(game,1);// #14 Kim : 2. 이거 한번 시도해보자 이거 주석처리하고 아래에서 처리해보자
				}
				break;

		default: ; //do nothing
	}

	//process entering a state
	switch (state)
	{
		case GameBeginState:
			play_sound(LevelStartSound);

			break;
		case LevelBeginState:

			break;
		case GamePlayState:
			break;
		case WinState:

			break;
		case DeathState:case DeathState2: case ReviveState1:case ReviveState2:
		//	pacdeath_init(game); //#14 Kim : 2. 해보잣!
			break;
		case GameoverState:
			break;
	}

	game->ticksSinceModeChange = ticks_game();
	game->gameState = state;
}

//checks if it's valid that pacman could move in this direction at this point in time
bool can_move(Pacman *pacman, Board *board, Direction dir)
{
	//easy edge cases, tile has to be parallal with a direction to move it
	if ((dir == Up   || dir == Down ) && !on_vert(&pacman->body)) return false;
	if ((dir == Left || dir == Right) && !on_horo(&pacman->body)) return false;

	//if pacman wants to move on an axis and he is already partially on that axis (not 0)
	//it is always a valid move
	if ((dir == Left || dir == Right) && !on_vert(&pacman->body)) return true;
	if ((dir == Up   || dir == Down ) && !on_horo(&pacman->body)) return true;

	//pacman is at 0/0 and moving in the requested direction depends on if there is a valid tile there
	int x = 0;
	int y = 0;

	dir_xy(dir, &x, &y);

	int newX = pacman->body.x + x;
	int newY = pacman->body.y + y;

	return is_valid_square(board, newX, newY) || is_tele_square(newX, newY);
}

static void process_player(PacmanGame *game,int player_num)
{// #8 Kim : 1.
	Pacman *pacman = &game->pacman[player_num]; // #8 Kim : 2. 이거로 두개의 플레이어 방향 셋팅
	Board *board = &game->board;

	if (pacman->missedFrames != 0)
	{
		pacman->missedFrames--;
		return;
	}

	Direction oldLastAttemptedDir = pacman->lastAttemptedMoveDirection;

	Direction newDir;

	bool dirPressed;
	if(player_num==1&&game->playMode==Online_Server)
	{
		dirPressed = Client_dir_pressed_now(&newDir,player_num);
	}
	else
		dirPressed = dir_pressed_now(&newDir,player_num);//#8 Kim : 2.플레이어마다 키가 달라짐.

	if (dirPressed)
	{
		//user wants to move in a direction
		pacman->lastAttemptedMoveDirection = newDir;

		//if player holds opposite direction to current walking dir
		//we can always just switch current walking direction
		//since we're on parallel line
		if (newDir == dir_opposite(pacman->body.curDir))
		{
			pacman->body.curDir = newDir;
			pacman->body.nextDir = newDir;
		}

		//if pacman was stuck before just set his current direction as pressed
		if (pacman->movementType == Stuck)
		{
			pacman->body.curDir = newDir;
		}

		pacman->body.nextDir = newDir;
	}
	else if (pacman->movementType == Stuck)
	{
		//pacman is stuck and player didn't move - player should still be stuck.
		//don't do anything
		return;
	}
	else
	{
		//user doesn't want to change direction and pacman isn't stuck
		//pacman can move like normal

		//just set the next dir to current dir
		pacman->body.nextDir = pacman->body.curDir;
	}

	pacman->movementType = Unstuck;

	int curDirX = 0;
	int curDirY = 0;
	int nextDirX = 0;
	int nextDirY = 0;

	dir_xy(pacman->body.curDir, &curDirX, &curDirY);
	dir_xy(pacman->body.nextDir, &nextDirX, &nextDirY);

	int newCurX = pacman->body.x + curDirX;
	int newCurY = pacman->body.y + curDirY;
	int newNextX = pacman->body.x + nextDirX;
	int newNextY = pacman->body.y + nextDirY;

	bool canMoveCur =  is_valid_square(board, newCurX, newCurY) || is_tele_square(newCurX, newCurY);
	bool canMoveNext = is_valid_square(board, newNextX, newNextY) || is_tele_square(newNextX, newNextY);

	//if pacman is currently on a center tile and can't move in either direction
	//don't move him
	if (on_center(&pacman->body) && !canMoveCur && !canMoveNext)
	{
		pacman->movementType = Stuck;
		pacman->lastAttemptedMoveDirection = oldLastAttemptedDir;

		return;
	}

	move_pacman(&pacman->body, canMoveCur, canMoveNext);

	//if pacman is on the center, and he couldn't move either of  his last directions
	//he must be stuck now
	if (on_center(&pacman->body) && !canMoveCur && !canMoveNext)
	{
		pacman->movementType = Stuck;
		return;
	}

	resolve_telesquare(&pacman->body,player_num);
}

static void process_ghosts(PacmanGame *game, int flag)
{
	for (int i = 0; i < ghost_number(game->currentLevel); i++)//#26 Yang : 1. 난이도 조절 - 고스트 수 조절
	{
		Ghost *g = &game->ghosts[flag][i];

		if (g->movementMode == InPen)
		{
			//ghosts bob up and down - move in direction. If they hit a square, change direction
			bool moved = move_ghost(&g->body);

			if (moved && (g->body.y == 13 || g->body.y == 15))
			{
				g->body.nextDir = g->body.curDir;
				g->body.curDir = dir_opposite(g->body.curDir);
			}


			continue;
		}

		if (g->movementMode == LeavingPen)
		{
			//ghost is in center of tile
			//move em to the center of the pen (in x axis)
			//then more em up out the gate
			//when they are out of the gate, set them to be in normal chase mode then set them off on their way

			continue;
		}

		//all other modes can move normally (I think)
		MovementResult result = move_ghost(&g->body);
		resolve_telesquare(&g->body,flag);

		if (result == NewSquare)
		{
			//if they are in a new tile, rerun their target update logic
			execute_ghost_logic(g, g->ghostType, &game->ghosts[0], &game->pacman[flag]);// #8 Kim : 1.

			g->nextDirection = next_direction(g, &game->board);
		}
		else if (result == OverCenter)
		{
			//they've hit the center of a tile, so change their current direction to the new direction
			g->body.curDir = g->transDirection;
			g->body.nextDir = g->nextDirection;
			g->transDirection = g->nextDirection;
		}
	}
}

static void process_fruit(PacmanGame *game, int playernum)//#5 Yang : 5. playernum 추가
{
	int pelletsEaten = game->pelletHolder.totalNum - game->pelletHolder.numLeft;
	GameFruit *f[5];
	unsigned int fdt[5];
	for(int i=0;i<5;i++){
		f[i]=&game->gameFruit[0][i];
	}


	int curLvl = game->currentLevel;

	if (pelletsEaten >= 30 && f[0]->fruitMode == NotDisplaying)
	{
		f[0]->fruitMode = Displaying;
		regen_fruit(f[0], curLvl);
	}
	else if (pelletsEaten == 60 && f[1]->fruitMode == NotDisplaying)
	{
		f[1]->fruitMode = Displaying;
		regen_fruit(f[1], curLvl);
	}
	else if (pelletsEaten == 90 && f[2]->fruitMode == NotDisplaying)
	{
		f[2]->fruitMode = Displaying;
		regen_fruit(f[2], curLvl);
	}
	else if (pelletsEaten == 120 && f[3]->fruitMode == NotDisplaying)
	{
		f[3]->fruitMode = Displaying;
		regen_fruit(f[3], curLvl);
	}
	else if (pelletsEaten == 150 && f[4]->fruitMode == NotDisplaying)
	{
		f[4]->fruitMode = Displaying;
		regen_fruit(f[4], curLvl);
	}

	for(int i=0;i<5;i++){
		fdt[i]= ticks_game()-f[i]->startedAt;
	}

	Pacman *pac = &game->pacman[playernum];

	if (f[0]->fruitMode == Displaying)
	{
		if (fdt[0] > f[0]->displayTime) f[0]->fruitMode = Displayed;
	}
	if (f[1]->fruitMode == Displaying)
	{
		if (fdt[1] > f[1]->displayTime) f[1]->fruitMode = Displayed;
	}
	if (f[2]->fruitMode == Displaying)
		{
			if (fdt[2] > f[2]->displayTime) f[2]->fruitMode = Displayed;
		}
	if (f[3]->fruitMode == Displaying)
		{
			if (fdt[3] > f[3]->displayTime) f[3]->fruitMode = Displayed;
		}
	if (f[4]->fruitMode == Displaying)
		{
			if (fdt[4] > f[4]->displayTime) f[4]->fruitMode = Displayed;
		}

	//check for collisions

	if (f[0]->fruitMode == Displaying && collides_obj(&pac->body, f[0]->x, f[0]->y))
	{
		f[0]->fruitMode = Displayed;
		f[0]->eaten = true;
		f[0]->eatenAt = ticks_game();
		pac->score += fruit_points(f[0]->fruit);
	}

	if (f[1]->fruitMode == Displaying && collides_obj(&pac->body, f[1]->x, f[1]->y))
	{
		f[1]->fruitMode = Displayed;
		f[1]->eaten = true;
		f[1]->eatenAt = ticks_game();
		pac->score += fruit_points(f[1]->fruit);
	}
	if (f[2]->fruitMode == Displaying && collides_obj(&pac->body, f[2]->x, f[2]->y))
	{
		f[2]->fruitMode = Displayed;
		f[2]->eaten = true;
		f[2]->eatenAt = ticks_game();
		pac->score += fruit_points(f[2]->fruit);
	}
	if (f[3]->fruitMode == Displaying && collides_obj(&pac->body, f[3]->x, f[3]->y))
	{
		f[3]->fruitMode = Displayed;
		f[3]->eaten = true;
		f[3]->eatenAt = ticks_game();
		pac->score += fruit_points(f[3]->fruit);
	}
	if (f[4]->fruitMode == Displaying && collides_obj(&pac->body, f[4]->x, f[4]->y))
	{
		f[4]->fruitMode = Displayed;
		f[4]->eaten = true;
		f[4]->eatenAt = ticks_game();
		pac->score += fruit_points(f[4]->fruit);
	}

}

//#5 Yang : 프로세스 오브젝트 함수 추가
static void process_object(PacmanGame *game, int playernum)//#5 Yang : 5.process_object에 playernum 변수 추가
{
	int pelletsEaten = game->pelletHolder.totalNum - game->pelletHolder.numLeft;
	GameObject *o[3];
	for(int i=0;i<3;i++){
		o[i]=&game->gameObject[0][i];
	}

	if (pelletsEaten >= 50 && o[0]->objectMode == NotDisplaying_obj)
	{
		o[0]->objectMode = Displaying_obj;
		regen_object(o[0]);
	}
	else if (pelletsEaten >= 100 && o[1]->objectMode == NotDisplaying_obj)
	{
		o[1]->objectMode = Displaying_obj;
		regen_object(o[1]);
	}
	else if (pelletsEaten >= 150 && o[2]->objectMode == NotDisplaying_obj)
	{
		o[2]->objectMode = Displaying_obj;
		regen_object(o[2]);
	}
	unsigned int odt[3];
	for(int i=0;i<3;i++) odt[i] = ticks_game()-o[i]->startedAt;


	Pacman *pac = &game->pacman[playernum];

	if (o[0]->objectMode == Displaying_obj)
	{
		if (odt[0] > o[0]->displayTime) o[0]->objectMode = Displayed_obj;
	}
	if (o[1]->objectMode == Displaying_obj)
	{
		if (odt[1] > o[1]->displayTime) o[1]->objectMode = Displayed_obj;
	}if (o[2]->objectMode == Displaying_obj)
	{
		if (odt[2] > o[2]->displayTime) o[2]->objectMode = Displayed_obj;
	}

	if (o[0]->objectMode == Displaying_obj && collides_obj(&pac->body, o[0]->x, o[0]->y))
	{
		o[0]->objectMode = Displayed_obj;
		o[0]->eaten = true;
		o[0]->eatenAt = ticks_game();
		game_object_function(o[0],game, playernum);
	}
	if (o[1]->objectMode == Displaying_obj && collides_obj(&pac->body, o[1]->x, o[1]->y))
	{
		o[1]->objectMode = Displayed_obj;
		o[1]->eaten = true;
		o[1]->eatenAt = ticks_game();
		game_object_function(o[1],game,playernum);
	}
	if (o[2]->objectMode == Displaying_obj && collides_obj(&pac->body, o[2]->x, o[2]->y))
	{
		o[2]->objectMode = Displayed_obj;
		o[2]->eaten = true;
		o[2]->eatenAt = ticks_game();
		game_object_function(o[2],game,playernum);
	}
	//#5 Yang : 4. object 기능 구현
	unsigned int oet[3];
	for (int i=0;i<3;i++) oet[i] = ticks_game()-o[i]->eatenAt;

	if (o[0]->eaten)
	{
		if (oet[0] > 5000) {game_object_function_end(o[0],game,playernum);		o[0]->eaten = false;}
	}
	if (o[1]->eaten)
	{
		if (oet[1] > 5000) {game_object_function_end(o[1],game,playernum);		o[1]->eaten = false;}
	}
	if (o[2]->eaten)
	{
		if (oet[2] > 5000) {game_object_function_end(o[2],game,playernum);		o[2]->eaten = false;}
	}
}
static void process_pellets(PacmanGame *game,int player_num)
{//#8 Kim 3. 그냥 배열넣는부부에 player_num 추가해줌으로써 이거 두번호출하고 0, 1 한번씩 호출 하게함.
	int j = 0;
	//if pacman and pellet collide
	//give pacman that many points
	//set pellet to not be active
	//decrease num of alive pellets
	PelletHolder *holder = &game->pelletHolder;

	for (int i = 0; i < holder->totalNum; i++)
	{
		Pellet *p = &holder->pellets[i];

		//skip if we've eaten this one already
		if (p->eaten) continue;

		if (collides_obj(&game->pacman[player_num].body, p->x, p->y))
		{
			holder->numLeft--;

			p->eaten = true;
			game->pacman[player_num].score += pellet_points(p);
			if(pellet_check(p)) {
				//#35 Yang :버그 수정 : 2p모드에서 큰 필렛 먹었을 때 상대방에겐 적용안되던거 수정
				if(game->playMode==Multi_TA){
					game->pacman[player_num].godMode = true;
					game->pacman[player_num].originDt = ticks_game();
					for(j = 0; j< 4; j++) {
						if(game->ghosts[player_num][j].isDead == 2)
							game->ghosts[player_num][j].isDead = 0;

					}
				}else{
					for(int k=0;k<2;k++){
					game->pacman[k].godMode = true;
					game->pacman[k].originDt = ticks_game();
					}
					for(j = 0; j< 4; j++) {
						if(game->ghosts[0][j].isDead == 2)
							game->ghosts[0][j].isDead = 0;
					}
				}
			}
			//play eat sound
			//eating a small pellet makes pacman not move for 1 frame
			//eating a large pellet makes pacman not move for 3 frames
			game->pacman[player_num].missedFrames = pellet_nop_frames(p);
			//can only ever eat 1 pellet in a frame, so return
			return;
		}
	}

	//maybe next time, poor pacman
}

static bool check_pacghost_collision(PacmanGame *game , int player_num)	//#14 Kim : 1. 일단 player 2개로 추가해보도록 함
{

	for (int i = 0; i < 4; i++)
	{
		Ghost *g[2];
			for(int j=0;j<2;j++) {
				g[j]=&game->ghosts[j][i];
			}
		/*
		switch(g->ghostType) {
		case Blinky : printf("red : %d \n", g->isDead); break;
		case Inky: printf("blue : %d \n", g->isDead); break;
		case Clyde: printf("orange : %d \n", g->isDead); break;
		case Pinky: printf("pink : %d \n", g->isDead); break;
		}
		*/
		//#32 Yang: 자기 고스트에만 죽도록
		if(game->playMode==Multi_TA){
			if (collides(&game->pacman[player_num].body, &g[player_num]->body)) {
				if(game->pacman[player_num].godMode == false)
					return true;
				else {
					if(g[player_num]->isDead == 2) {return true;}
					g[player_num]->isDead = 1;
					death_send(g[player_num]);
				}
			}
		}
		else{
			if (collides(&game->pacman[player_num].body, &g[0]->body)) {
				if(game->pacman[player_num].godMode == false)
					return true;
				else {
					if(g[0]->isDead == 2) {return true;}
					g[0]->isDead = 1;
					death_send(g[0]);
				}
			}
		}
	}

	return false;
}

void gamestart_init(PacmanGame *game)
{
	game->currentLevel = 1;
	level_init(game);

	pacman_init(&game->pacman[0],0);
	// #8 Kim : 2. pacman init 부분도 추가
	if(game->playMode==Multi_TA) pacman_init(&game->pacman[1],2);
	else if(game->playMode!=Single)// #13 Kim : 1. play Mode 에 따라서 추가
		pacman_init(&game->pacman[1],1);
	//we need to reset all fruit
	//fuit_init();
	game->highscore = 0; //TODO maybe load this in from a file..?


	//invalidate the state so it doesn't effect the enter_state function
	game->gameState = -1;
	enter_state(game, GameBeginState);
}

void level_init(PacmanGame *game)
{
	//reset pacmans position
	if(Multi_flags() == 1) // # 9 Dong : 2p 맵 연동을 위한 추가
	{
		game_init2(game->currentLevel);
	}
	else
	{
		game_init(game->currentLevel);
	}
	pacman_level_init(&game->pacman[0]);

	//reset pellets
	pellets_init(&game->pelletHolder);

	//reset ghosts
	ghosts_init(game->ghosts[0], game->currentLevel,0);
	ghosts_init(game->ghosts[1],game->currentLevel,1);


	//reset fruit
	for(int i=0;i<5;i++) reset_fruit(&game->gameFruit[0][i], &game->board);

	//#5 Yang : 3.object reset
	for(int i=0;i<3;i++) reset_object(&game->gameObject[0][i], &game->board);
}

void pacdeath_init(PacmanGame *game,int player_num) //#14 Kim : 2. 이 부분도 어떤 팩맨이 죽었는지 추가해줘야할듯 했지만 사실 필요는 없는듯.. 흠..여기서 점수관련한걸 해줘야하나
{
	//pacman_level_init(&game->pacman[player_num]);
	//	if(game->playMode!=Single)// #13 Kim : 1. play Mode 에 따라서 추가
	//		pacman_level_init(&game->pacman[1]); //#8 Kim : 2.level도 흠...
	//따라서 윗 부분은 필요 없게됨.
	// #14 Kim : 2. 그리고 죽어도 계속 진행 되고 있는거니까  그냥 init , reset 부분 지우거
	ghosts_init(game->ghosts[0], game->currentLevel,0);
	ghosts_init(game->ghosts[1], game->currentLevel,1);
	/*reset_fruit(&game->gameFruit1, &game->board);
	reset_fruit(&game->gameFruit2, &game->board);
	reset_fruit(&game->gameFruit3, &game->board);
	reset_fruit(&game->gameFruit4, &game->board);
	reset_fruit(&game->gameFruit5, &game->board);
	reset_object(&game->gameObject1, &game->board);
	reset_object(&game->gameObject2, &game->board);
	reset_object(&game->gameObject3, &game->board);
	 */
}

//TODO: make this method based on a state, not a conditional
//or make the menu system the same. Just make it consistant
bool is_game_over(PacmanGame *game)
{
	unsigned dt = ticks_game() - game->ticksSinceModeChange;

	return dt > 2000 && game->gameState == GameoverState;
}

int int_length(int x)
{
    if (x >= 1000000000) return 10;
    if (x >= 100000000)  return 9;
    if (x >= 10000000)   return 8;
    if (x >= 1000000)    return 7;
    if (x >= 100000)     return 6;
    if (x >= 10000)      return 5;
    if (x >= 1000)       return 4;
    if (x >= 100)        return 3;
    if (x >= 10)         return 2;
    return 1;
}

static bool resolve_telesquare(PhysicsBody *body, int flag)//#
{
	//TODO: chuck this back in the board class somehow

	if(Multi_flags() == 1) // # 9 Dong : 2P 맵 연동을 위한 수정.
	{
		if (body->y != 14) return false;

		if (body->x == -1) { body->x = 27; return true; }
		if (body->x == 28&&flag==0) { body->x = 0; return true; }
		if (body->x == 28&&flag==1) { body->x = 55; return true; }
		if (body->x == 56) { body->x = 29; return true; }

		return false;
	}
	else
	{
		if (body->y != 14) return false;

		if (body->x == -1) { body->x = 27; return true; }
		if (body->x == 28) { body->x =  0; return true; }

		return false;
	}
}

//#5 Yang : 4.각 Object 효과 구현
void game_object_function(GameObject *gameObject, PacmanGame *game, int playernum)//#5 Yang : 5. playernum 추가
{
	switch(gameObject->object)
	{
	case Ghostslow:
		for(int i=0;i<4;i++){
			if(game->playMode==Multi_TA)game->ghosts[playernum][i].body.velocity=50;
			else game->ghosts[0][i].body.velocity=50;
		}
		return;
	//#15 Yang : 1.생명추가 object 추가
	case Life:
		game->pacman[playernum].livesLeft++;
		return;
	//#26 Yang : 1.Godmode
	case God:
		game->pacman[playernum].godMode=true;
		game->pacman[playernum].originDt = ticks_game();
		game->pacman[playernum].body.velocity = 160;
		game->pacman[playernum].boostOn = true;
		return;
	default: return;
	}
}
void game_object_function_end(GameObject *gameObject, PacmanGame *game, int playernum) //#5 Yang : 5. playernum 추가
{
	switch(gameObject->object)
	{
	case Ghostslow:

		for(int i=0;i<4;i++){
			if(game->playMode==Multi_TA)game->ghosts[playernum][i].body.velocity=ghost_speed_normal(game->currentLevel);
			else game->ghosts[0][i].body.velocity= ghost_speed_normal(game->currentLevel);
		}
		return;
	case Life: return;
	case God:
			game->pacman[playernum].godMode=false;
			game->pacman[playernum].body.velocity = 80;
			game->pacman[playernum].boostOn = false;
			return;
	default : return;
	}

}
