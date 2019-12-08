#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "five_type.h"
#include "five_global.h"
#include "five_func.h"

uint8_t player = 0;
uint8_t Round = 0;
uint8_t stop = 0;
Board board;
Point PieceOnBoard[225];

extern uint8_t check(Point p, uint8_t color);
/*Print the board and the present state.*/
void print_board()
{
	system("cls");
	for (int i = 0; i <= 14; i++)
	{
		printf("%2d", 15 - i);
		for (int j = 0; j <= 14; j++)
		{
			printf("%s", symbol[board[i][j]]);
		}
		putchar('\n');
	}
	printf("   A B C D E F G H I J K L M N O \n");
}

/*Print the record after the game.*/
/*
void print_record() {
	record();
	printf("Round Player1 Player2\n");
	for (int i = 0; i < Round; i++) {
		if (!(i % 2))
			printf("%4d ", i / 2 + 1);
		printf("%4c%-4d", (PieceOnBoard[i]).y + 'a', 15 - (PieceOnBoard[i]).x);
		if (i % 2)
			putchar('\n');
	}
	printf("\nPress (q) to quit or anything else to play again.\n");
	if (_getch() == 'q')
		exit(0);
}
*/

/*Init the board for the next game.*/
void init_board()
{
	player = 0;
	Round = 0;
	memcpy(board, empty, sizeof(empty));
	print_board();
}

/*Varify the location*/
uint8_t varify_location(Point p)
{
	if (p.x >= 0 && p.x <= 14 && p.y >= 0 && p.y <= 14)
		return 1;
	return 0;
}

/*Drop the piece at the given place.*/
uint8_t drop(Point P)
{
	board[P.x][P.y] = player + 2;
	print_board();
	printf("Last piece:%c %d\n", P.y + 'a', 15 - P.x);
	board[P.x][P.y] = player;
	return check(P,player);
}

/*Set the initial state and start the game.*/
void new_game(Point(*P1)(Point last), Point(*P2)(Point last))
{
	init_board();
	stop = 0;
	play(*P1, *P2);
}

void continue_game(Point(*P1)(Point last), Point(*P2)(Point last))
{
	play(*P1, *P2);
}

/*Control the game state, let the player drop piece in turn and decide if the game should stop.*/
void play(Point(*P1)(Point last), Point(*P2)(Point last))
{
	Point piece = { 7,7 };
	while (Round < 15 * 15 && !stop) {
		printf("Player: %d\n", 1 + player);
		if (!player) {
			piece = (*P1)(piece);
			if (stop = drop(piece))
				break;
		}
		else {
			piece = (*P2)(piece);
			if (stop = drop(piece))
				break;
		}
		player ^= 1;
	}
	if (Round == 225 && !stop)
		printf("Draw!\n");
	else
		printf("The winner is Player%d!\nCongratulations!\n", player + 1);
	printf("Press (q) to quit.Press (r) to get the record or anything else to play again.\n");
	char c;
	c = _getch();
	switch (c) {
	case 'q':
		exit(0);
		break;
	/*
	case 'r':
		print_record();
		break;
	*/
	default:
		break;
	}
}