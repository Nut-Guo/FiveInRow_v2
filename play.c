#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "five_type.h"
#include "five_global.h"

uint8_t player = 0;
uint8_t Round = 0;
uint8_t stop = 0;
Board board;
Point PieceOnBoard[225];

/*
Chunks stored_chunk;
Used_Pieces PieceOnBoard;
Pieces pieces_exist;
extern int ghost_num;
*/

extern char check(char x, char y);

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


void init_board()
{
	player = 0;
	Round = 0;
	memcpy(board, empty, sizeof(empty));
	print_board();
}

/*
char drop(char x, char y)
{
	board[x][y] = player + 2;
	print_board();
	printf("Last piece:%c %d\n", y + 'a', 15 - x);
	Value value = evaluate(get_info(x, y));
	if (value.value[player] == -1) {
		printf("The ban is triggered, sorry!\n");
		player ^= 1;
		return 1;
	}
	return check(x, y);
}
*/

void init()
{
	init_board();
	stop = 0;
}

void play(char(*P1)(), char(*P2)())
{
	while (Round < 15 * 15 && !stop) {
		printf("Player: %d\n", 1 + player);
		if (!player) {
			if (stop = (*P1)())
				break;
		}
		else {
			if (stop = (*P2)())
				break;
		}
		player ^= 1;
	}
	if (Round == 225 && !stop)
		printf("Draw!\n");
	else
		printf("The winner is Player%d!\nCongratulations!\n", player + 1);
	clean();
	printf("Press (q) to quit.Press (r) to get the record or anything else to play again.\n");
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

void new_game(char(*P1)(), char(*P2)())
{
	init();
	play(*P1, *P2);
}

void continue_game(char(*P1)(), char(*P2)())
{
	play(*P1, *P2);
}