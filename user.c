#include <stdio.h>
#include "five_global.h"

extern void print_board();

char user()
{
	char x, y, tmpx, tmpn;
	int tmpy;
	printf("Please input the position\n");
	scanf_s("%c%d%c", &tmpx, 1, &tmpy, &tmpn, 1);
	x = 15 - tmpy;
	y = tmpx - 'a';

	if (!varify_location(x, y)) {
		printf("Do you want to place your piece out of the board?\n");
		printf("Don't do this please\n");
		return user();
	}
	if (board[x][y] <= 1) {
		printf("This position has been taken, you can't put you piece there.\n");
		printf("Choose another place.\n");
		return user();
	}
	return drop(x, y);
}