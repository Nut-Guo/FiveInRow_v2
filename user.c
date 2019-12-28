/*
	Ask the user to give a valid input.
*/

#include <stdio.h>
#include "five_global.h"
#include "five_type.h"

extern uint8_t verify_location(Point p);

/*
	Get a valid input.
*/
Point user(Point last)
{
	char x, y, tmpx, tmpn;
	int tmpy;
	printf("Please input the position\n");
	scanf_s("%c%d%c", &tmpx, 1, &tmpy, &tmpn, 1);
	x = 15 - tmpy;
	y = tmpx - 'a';
	Point P = { x,y };
	if (!verify_location(P)||board.location[P.x][P.y]<2) {
		printf("You can't put your piece there!\n");
		printf("Choose another place:\n");
		return user(last);
	}
	return P;
}