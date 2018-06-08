#pragma once

#include <SDL/SDL.h>
#include <stdbool.h>

#include "direction.h"

#define MAX_KEYS 323

//Call at the end of each round of keypresses. If this isn't called key_pressed() and key_released() will not work.
void keyevents_finished(void);

//Handles when a key is pressed down.
void handle_keydown(int keycode);

//Handles when a key is pressed up.
void handle_keyup(int keycode);

//Returns true if the current direction is currently pressed down.
//A direction can be bound to multiple keypressed, making this function input-method agnostic.
bool dir_key_held(Direction direction,int player_num);// #8 Kim : 2.플레이어에 따라 키 누르는 값이 다름.

//This function sets the input direction to the last direction that was pressed and is still currently pressed.
//If no direction is pressed, the input direction is not assigned to.
//If multiple directions were pressed at the same time-instance, this function prioritises Up, Left, Down, Right.
//It returns true to indicate a direction is currently pressed, and false if no direction is pressed.

bool dir_pressed_now(Direction *direction,int player_num);// #8 Kim : 2.플레이어에 따라 키 누르는 값이 다름.

//Tests if a particular key is pressed. Uses the SDLK keycode constants.
bool key_held(int key);

//Tests if the key was pressed this frame.
bool key_pressed(int key);



typedef struct{
	bool keyHeld[4];
	unsigned int keyPressed[4];
}client_key; // #25 : 클라이언트의 키를 받기 위한 구조체

//Tests if the key was released this frame.
bool key_released(int key);
void get_key(client_key *key);
void insert_key(client_key *key);

bool Client_dir_key_held(Direction direction);
//#25 클라이언트 키 따로 받아서 하기위해 ..
//원래는 로컬에서 할 때는 같은 함수에서 if문으로 player num 떄려박아넣었는데.
//여기서 key를 따로 해줘야 server에서 wasd 로 조정했을때 2p가 움직여버리는 일이 안일어남

