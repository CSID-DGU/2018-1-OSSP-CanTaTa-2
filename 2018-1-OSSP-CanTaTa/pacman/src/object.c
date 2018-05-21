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
	}

	printf("invalid object\naborting\n");
	exit(1);
}

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

void reset_object(GameObject *gameObject, Board *board)
{
	//srand((unsigned)time(NULL));
	int randX = 0;
	int randY = 0;
	do {
			randX = rand() % 26;
			randY = rand() % 30;
	} while( (is_valid_square(board, randX, randY) || is_tele_square(randX, randY) ) == false);

	gameObject->object = Ghostslow;
	gameObject->objectMode = NotDisplaying_obj;
	gameObject->startedAt = 0;
	gameObject->displayTime = 0;

	gameObject->eaten = false;
	gameObject->eatenAt = 0;

	gameObject->x = randX;
	gameObject->y = randY;
}

void regen_object(GameObject *gameObject, int level)
{
	gameObject->object = object_for_level(level);
	gameObject->displayTime = rand_object_visible_time();
	gameObject->startedAt = ticks_game();
}

int rand_object_visible_time(void)
{
	// chosen by a random number generator
	// guarenteed to be random
	return 9500;
}
