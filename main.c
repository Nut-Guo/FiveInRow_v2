#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <direct.h>
#include "five_func.h"

char cwd[80];
Point(*Input[])(Point last) = { user, search_point };
void(*Play[])(Point(*P1)(Point last), Point(*P2)(Point last)) = { new_game, continue_game };

/*Start the game, choose the player and the mode.*/
void game(char mode)
{
	printf("USER(0)\tPC(1)\n");
	printf("Please select Player 1: ");
	char player1 = _getch();
	while (player1 > '1' || player1 < '0')
		player1 = _getch();
	printf("\n");
	printf("Please select Player 2: ");
	char player2 = _getch();
	while (player2 > '1' || player2 < '0')
		player2 = _getch();
	printf("\n");
	(*Play[mode])(Input[player1 - '0'], Input[player2 - '0']);
}

/*The entry for a new game or the archive.*/
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