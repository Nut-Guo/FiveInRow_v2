#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "five_type.h"
#include "five_global.h"
#include "five_func.h"

extern char cwd[80];
uint8_t player = 0;
uint8_t Round = 0;
uint8_t stop = 0;
Board board;
Pool PieceOnBoard;

extern inline uint8_t check(Point p, uint8_t color, Board *local_board);
extern inline void print_record();
extern inline void init_zobrist();
extern void(*Play[])(Point(*P1)(), Point(*P2)());
extern Point(*Input[])();

/*
	Print the board and the present state.
*/
extern inline void print_board(Board *board)
{
	system("cls");
	for (int i = 0; i <= 14; i++)
	{
		printf("%2d", 15 - i);
		for (int j = 0; j <= 14; j++)
		{
			printf("%s", symbol[(*board).location[i][j]]);
		}
		putchar('\n');
	}
	printf("   A B C D E F G H I J K L M N O \n");
}

/*
	Init the board for the next game.
*/
inline void init_board()
{
	player = 0;
	Round = 0;
	stop = 0;
	board = empty;
	print_board(&board);
	init_zobrist();
}

/*
	Varify the location
*/
extern inline uint8_t verify_location(Point p)
{
	if (p.x >= 0 && p.x <= 14 && p.y >= 0 && p.y <= 14)
		return 1;
	return 0;
}

/*
	Drop the piece at the given place.
*/
extern inline uint8_t drop(Point P)
{
	board.location[P.x][P.y] = player + 2;
	print_board(&board);
	printf("Last piece:%c %d\n", P.y + 'a', 15 - P.x);
	board.location[P.x][P.y] = player;
	PieceOnBoard.record[Round++] = P;
	return check(P,player,&board);
}

/*
	Set the initial state and start the game.
*/
extern inline void new_game(Point(*P1)(Point last), Point(*P2)(Point last))
{
	init_board();
	stop = 0;
	play(*P1, *P2);
}

/*
	Continue the game from a record;
*/
extern inline void continue_game(Point(*P1)(Point last), Point(*P2)(Point last))
{
	stop = 0;
	player = Round % 2;
	play(*P1, *P2);
}

/*
	Control the game state, let the player drop piece in turn 
	and decide if the game should stop.
*/
void play(Point(*P1)(Point last), Point(*P2)(Point last))
{
	clock_t start, finish;
	float duration;
	Point piece = { 7,7 };
	while (Round < 15 * 15 && !stop) {
		printf("Player: %d\n", 1 + player);
		if (!player) {
			start = clock();
			piece = (*P1)(piece);
			finish = clock();
			duration = (float)(finish - start) / CLOCKS_PER_SEC;
			if (stop = drop(piece))
				break;
			printf("Time spent by Player1: %.3fs\n", duration);
		}
		else {
			start = clock();
			piece = (*P2)(piece);
			finish = clock();
			duration = (float)(finish - start) / CLOCKS_PER_SEC;
			if (stop = drop(piece))
				break;
			printf("Time spent by Player2: %.3fs\n", duration);
		}
		player ^= 1;
	}
	if (Round == 225 && !stop)
		printf("Draw!\n");
	else
		printf("The winner is Player%d!\nCongratulations!\n", player + 1);
	printf("Quit(q) \t Record(r)\tMenu(else)\n");
	char c;
	c = _getch();
	switch (c) {
	case 'q':
		exit(0);
		break;
	case 'r':
		print_record();
		break;
	default:
		break;
	}
}