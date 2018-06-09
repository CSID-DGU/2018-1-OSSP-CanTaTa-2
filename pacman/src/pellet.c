#include "pellet.h"

void pellets_init(PelletHolder *pelletHolder)
{
	if(Multi_flags() == 1) // # 35 Dong : 버그 수정, 펠렛을 다먹어도 진행이 안되던 사항.
	{
	for (int i = 0; i < NUM_PELLETS; i++)
	{
		pelletHolder->pellets[i].eaten = false;
	}

	pelletHolder->numLeft = NUM_PELLETS;
	pelletHolder->totalNum = NUM_PELLETS;
	}
	else
	{
		for (int i = 0; i < 244; i++)
		{
			pelletHolder->pellets[i].eaten = false;
		}

		pelletHolder->numLeft = 244;
		pelletHolder->totalNum = 244;
	}
}

int pellet_points(Pellet *pellet)
{
	switch (pellet->type)
	{
		case SmallPellet: return 10;
		case LargePellet: return 50;
	}

	printf("error\n");
	exit(1);
}

int pellet_nop_frames(Pellet *pellet)
{
	switch (pellet->type)
	{
		case SmallPellet: return 1;
		case LargePellet: return 3;
	}

	printf("error\n");
	exit(1);
}

bool pellet_check(Pellet *pellet) {
	if(pellet->type == SmallPellet) {
		return false;
	} else {
		return true;
	}
}
