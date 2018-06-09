#pragma once

#include <stdbool.h>
#include "board.h"
#include "main.h"

#define NUM_OBJECT 3

typedef enum
{
	Ghostslow,
	Life,
	God//#15 Yang : 1.생명 증가 object 추가
}Object;

//Different modes a fruit can be in while in-game
//Either not-displayed (haven't eaten enough pellets yet)
//Displaying
//Displayed (either we timed-out or the player ate the fruit)
typedef enum
{
	NotDisplaying_obj,
	Displaying_obj,
	Displayed_obj
} ObjectMode;

//Defines a fruit for use in Pacman.
typedef struct
{
	Object object;              // the fruit we are displaying
	ObjectMode objectMode;      // the mode the fruit is currently in
	unsigned int startedAt;   // the time we started displaying this fruit at (in SDL_Ticks())
	unsigned int displayTime; // the random time between 9000-10000 ms this fruit displays for
	int x;                    // x location onscreen
	int y;                    // y location onscreen
	bool eaten;               // if the fruit was eaten while it displayed
	unsigned int eatenAt;     // the time it was eaten (used for displaying the points gained)
} GameObject;

//Returns the number of points the given fruit is worth.
int objcet_points(Object object);

//Returns the fruit that should be displayed for the given level.
//Object object_for_level(int level);

//Generates a random time between 9000-10000 ms, for which the fruit should be displayed.
int rand_object_visible_time(void);

//Resets the given fruit back to its default state.
void reset_object(GameObject *gameObject, Board *board);

//#5 Yang : 6.object 출현 방식 변경
Object random_object(void);
void regen_object(GameObject *gameObject);
