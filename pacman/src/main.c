#include "main.h"
#include <stdbool.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "board.h"
#include "boardloader.h"
#include "fps.h"
#include "game.h"
#include "input.h"
#include "intermission.h"
#include "imageloader.h"
#include "menu.h"
#include "sound.h"
#include "text.h"
#include "window.h"
#include "server.h"
#include "client.h"

//Initializes all resources.
static void resource_init(void);

//Initialize all the internal entities needed for the game at startup.
static void game_init(void);

//Called when a game is about to begin (player hits enter from main menu).
static void startgame_init(void);

//Frees all resources.
static void clean_up(void);

//Performs a loop, updating and rendering at 60hz.
static void main_loop(void);

//Defers to appropriate tick, based on current state.
static void internal_tick(void);

//Defers to appropriate render, based on current state.
static void internal_render(void);

//Processess and deals with all SDL events.
static void process_events(void);

//Performs specific actions on various keypresses. Used for testing.
//static void key_down_hacks(int keycode); #38 : ip 칠때 0 이 같이 적용되어 주석처리 hack은 사실 필요없자나?

static void cp_pacman(PacmanGame *pac);

static ProgramState state;
static MenuSystem menuSystem;
static PacmanGame pacmanGame;
static PacmanGame* recvPac;
static bool gameRunning = true;
static int numCredits = 0;

int main(void)
{
	resource_init();
	game_init();

	main_loop();

	clean_up();

	return 0;
}

static void main_loop(void)
{
	while (gameRunning && !key_held(SDLK_ESCAPE))
	{
		process_events();

		internal_tick();
		internal_render();

		fps_sleep();
	}
}

static void internal_tick(void)
{
	int up_down = 0;
	switch (state)
	{
	case Menu:// #8 Kim : 2. menu_tick 에서 키 입력에 따라 up_down 값을 바꿔줌

		up_down = menu_tick(&menuSystem);

		if(pacmanGame.playMode==Single && up_down ==-1)//Single 이면서 up_down 이 -1 ( 즉 맨 위인데 다시 위로가기면 그대로 유지)
			pacmanGame.playMode=Single;
		else if (pacmanGame.playMode==Online&&up_down==1)//Online 이면서 up_down 이 1 (즉 맨 아래인데 다시 아래로가면 그대로 유지)
			pacmanGame.playMode = Online;
		else
			pacmanGame.playMode+=up_down;//그 외는 그안에서 왔다갔다 하도록 함.

		menuSystem.playMode=pacmanGame.playMode; //#13 Kim : 이 부분에서 메뉴시스템한테 playMOde 전달~

		if (menuSystem.action == GoToGame)
		{
			state = Game;
			startgame_init();
		}
		else if(menuSystem.action == GoToJoin)
		{
			state = Join;
		}

		break;
	case Game:


		if(menuSystem.playMode==Online_Server)
		{
			client_key key;

			recv(c_socket_fd, (char*)&key, sizeof(key), MSG_WAITALL);
			get_key(&key);

			game_tick(&pacmanGame);
			send(c_socket_fd, (char*)&pacmanGame, sizeof(PacmanGame),0);

		}
		else if(menuSystem.playMode==Online_Client)
		{
			client_key key;
			insert_key(&key);

			send(clientSocket, (char*)&key, sizeof(key),0);

			recvPac = (PacmanGame*)malloc(sizeof(PacmanGame));
			recv(clientSocket, (char*)recvPac, sizeof(PacmanGame), MSG_WAITALL);

			cp_pacman(recvPac);
		}
		else
		{
			game_tick(&pacmanGame);
		}
		if (is_game_over(&pacmanGame))
		{
			menu_init(&menuSystem);
			state = Menu;
			pacmanGame.playMode = Single;
		}

		break;
	case Intermission:
		intermission_tick();
		break;
	case Join:

		break;
	}
}

static void internal_render(void)
{
	clear_screen(0, 0, 0, 0);

	switch (state)
	{
	case Menu:
		menu_render(&menuSystem);
		break;
	case Game:
		game_render(&pacmanGame);
		break;
	case Intermission:
		intermission_render();
		break;
	case Join:

		switch(online_mode_render(&menuSystem))// #20 Kim : 1. 만약 접속 되었으면 state를 Menu로 바꿔줌
		{
		case JoinServer:case WaitClient:
			pacmanGame.playMode= menuSystem.playMode;
			state=Menu;
			break;
		default:
			break;
		}

		break;
	}

	flip_screen();
}

static void game_init(void)
{
	//Load the board here. We only need to do it once
	load_board(&pacmanGame.board, &pacmanGame.pelletHolder, "maps/encodedboard");

	//set to be in menu
	state = Menu;

	//init the framerate manager
	fps_init(60);

	menu_init(&menuSystem);
}

static void startgame_init(void)
{
	gamestart_init(&pacmanGame);
}

static void resource_init(void)
{
	init_window(SCREEN_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);
	load_images();
	load_sounds();
	load_text();

	//TODO: ensure all the resources loaded properly with some nice function calls
}

static void clean_up(void)
{
	dispose_window();
	dispose_images();
	dispose_sounds();
	dispose_text();

	SDL_Quit();
}

static void process_events(void)
{
	static SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			gameRunning = false;

			break;
		case SDL_KEYDOWN:
			handle_keydown(event.key.keysym.sym);
			//key_down_hacks(event.key.keysym.sym); #38 : ip 칠때 0 이 같이 적용되어 주석처리 hack은 사실 필요없자나?

			break;
		case SDL_KEYUP:
			handle_keyup(event.key.keysym.sym);

			break;
		}
	}

	keyevents_finished();
}

/*static void key_down_hacks(int keycode) #38 : ip 칠때 0 이 같이 적용되어 주석처리 hack은 사실 필요없자나?
{// #8 Kim : 1.
	if (keycode == SDLK_RETURN) pacmanGame.currentLevel++;
	if (keycode == SDLK_BACKSPACE) menuSystem.ticksSinceModeChange = SDL_GetTicks();

	static bool rateSwitch = false;

	//TODO: remove this hack and try make it work with the physics body
	if (keycode == SDLK_SPACE) fps_sethz(60);//(rateSwitch = !rateSwitch) ? 200 : 60);// 버그 : 일단 ..뭐지흠.

	if (keycode == SDLK_b) {
		if(!pacmanGame.pacman[0].boostOn) {
			pacmanGame.pacman[0].body.velocity = 100;
			pacmanGame.pacman[0].boostOn = true;
		} else {
			pacmanGame.pacman[0].body.velocity = 80;
			pacmanGame.pacman[0].boostOn = false;
		}
	}

	//TODO: move logic into the tick method of the menu
	if (state == Menu && keycode == SDLK_5 && numCredits < 99)
	{
		numCredits++;
	}

	if (keycode == SDLK_9)
	{
		printf("plus\n");
		for (int i = 0; i < 4; i++) pacmanGame.ghosts[i].body.velocity += 5;

		printf("ghost speed: %d\n", pacmanGame.ghosts[0].body.velocity);
	}
	else if (keycode == SDLK_0)
	{
		printf("minus\n");
		for (int i = 0; i < 4; i++) pacmanGame.ghosts[i].body.velocity -= 5;

		printf("ghost speed: %d\n", pacmanGame.ghosts[0].body.velocity);
	}
}
 */
int num_credits(void)
{
	return numCredits;
}


static void cp_pacman(PacmanGame* pac)
{


	pacmanGame.gameState = pac->gameState;
	pacmanGame.ticksSinceModeChange = pac->ticksSinceModeChange;
	pacmanGame.highscore = pac->highscore;
	pacmanGame.currentLevel = pac->currentLevel;
	pacmanGame.pacman[0] = pac->pacman[0];
	pacmanGame.pacman[1] = pac->pacman[1];
	for(int i=0; i<4; i++) {
		pacmanGame.ghosts[i] = pac->ghosts[i];
	}



}
