#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <direct.h>
#include "five_func.h"

char cwd[80];
Point(*Input[])(Point last) = { user, search_point };
void(*Play[])(Point(*P1)(Point last), Point(*P2)(Point last)) = { new_game, continue_game };
extern uint32_t PC_MAXDEPTH[2];

/*
	Choose a search depth of PC, Optional, but a depth of 4 seems to be the best.
*/
void choose_depth(uint8_t i) {
	printf("Please input the max selection depth between 1 and 5: ");
	while (1) {
		scanf_s("%d", &PC_MAXDEPTH[i]);
		PC_MAXDEPTH[i] -= 1;
		if (PC_MAXDEPTH[i] < 1 || PC_MAXDEPTH[i] >5) {
			printf("Invalid value, try again!\n");
		}
		else return;
	}
}

/*
	Start the game, choose the player and the mode.
*/
void game(char mode)
{
	printf("USER(0)\tPC(1)\n");
	printf("Please select Player 1:\n");
	char player1;
	do {
		player1 = _getch();
		/*if (player1 == '1') {
			choose_depth(0);
		}*/
	} while (player1 > '1' || player1 < '0');
	printf("Please select Player 2:\n");
	char player2;
	 do {
		player2 = _getch();
		/*if (player2 == '1') {
			choose_depth(1);
		}*/
	 } while (player2 > '1' || player2 < '0');
	printf("\n");
	(*Play[mode])(Input[player1 - '0'], Input[player2 - '0']);
}

/*
	The entry for a new game or the archive.
*/
int main(int argc, char* argv[])
{
	char* receive = _getcwd(cwd, 80);
	while (1) {
		system("cls");
		printf("Welcome to the FiveInRow game!\n");
		printf("\tVersion: 1.0\n\tQin Siliang\n");
		printf("New game(n)\tLoad archive(a)\n");
		switch (_getch()) {
		case 'n':
			game(0);
			break;
		case 'a':
			get_the_record();
			break;
		}
	}
	return 0;
}