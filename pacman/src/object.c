#include "object.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL/SDL.h>

#include "fps.h"

int object_points(Object object)
{
	switch (object)
	{
		case Ghostslow:     return 100;
		case Life: 			return 100;
		default : return 0;
	}

	printf("invalid object\naborting\n");
	exit(1);
}
/*
Object object_for_level(int level)
{
	if (level < 1)
	{
		printf("invalid level: %d\n", level);
		printf("aborting\n");
		exit(1);
	}

	switch (level)
	{
		case 1:           return Ghostslow;
		default:          return Ghostslow;
	}
}
*/

//#5 Yang : 6.object 출현 방식 변경
Object random_object()
{
	int random = rand()%NUM_OBJECT;
	switch(random){
	case 0: return Ghostslow;
	case 1: return Life;
	//#26 Yang: 1.godmode
	case 2: return God;
	default : return Ghostslow;
	}
}
void reset_object(GameObject *gameObject, Board *board)
{
	//srand((unsigned)time(NULL));
	int randX = 0;
	int randY = 0;
	if(Multi_flags() == 1) // #9 Dong : 2p 맵 연동을위한 추가
	{
		do {
			randX = rand() % 54;
			randY = rand() % 30;
		} while( (is_valid_square(board, randX, randY) || is_tele_square(randX, randY) ) == false);
	}
	else
	{
		do {
			randX = rand() % 26;
			randY = rand() % 30;
		} while( (is_valid_square(board, randX, randY) || is_tele_square(randX, randY) ) == false);
	}
	gameObject->object = random_object();
	gameObject->objectMode = NotDisplaying_obj;
	gameObject->startedAt = 0;
	gameObject->displayTime = 0;

	gameObject->eaten = false;
	gameObject->eatenAt = 0;

	gameObject->x = randX;
	gameObject->y = randY;
}
//#5 YANG : Object 출현방식 변경
void regen_object(GameObject *gameObject)
{
	gameObject->object = random_object();
	gameObject->displayTime = rand_object_visible_time();
	gameObject->startedAt = ticks_game();
}

int rand_object_visible_time(void)
{
	// chosen by a random number generator
	// guarenteed to be random
	return 9500;
}
