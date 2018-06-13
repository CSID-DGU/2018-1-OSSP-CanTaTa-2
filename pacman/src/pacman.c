#include "pacman.h"

void pacman_init(Pacman *pacman, int flag)
{
	pacman_location_init(pacman, flag);

	pacman->score = 0;
	pacman->livesLeft = 3;

	pacman->missedFrames = 0;
	pacman->godMode = false;
	pacman->boostOn = false;
}

void pacman_level_init(Pacman *pacman)
{
	pacman_location_init(pacman,0);
}

void pacman_location_init(Pacman *pacman, int flag)
{
	switch(flag){
	case 0:
		pacman->body = (PhysicsBody) {14, 23, -8, 0, Left, Left, 1, 0, 0}; break;
	case 1:
		pacman->body = (PhysicsBody) {14, 5, -8, 0, Left, Left, 1, 0, 0}; break;
	case 2:
		pacman->body = (PhysicsBody) {43, 23, -8, 0, Left, Left, 1, 0, 0}; break;
	default: break;
	}

	pacman->body.velocity = 80;
	pacman->movementType = Unstuck;
	pacman->lastAttemptedMoveDirection = Left;
}

int pacman_speed_normal(int level)
{
	if (level == 1)  return 80;
	if (level <= 4)  return 90;
	if (level <= 20) return 100;
	return 90;
}

int pacman_speed_fright(int level)
{
	if (level == 1)  return 90;
	if (level <= 4)  return 95;
	if (level <= 20) return 100;
	return 100;
}
