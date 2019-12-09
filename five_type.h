/*
	Define main types.
*/

#pragma once

#include<stdint.h>

typedef struct {
	uint8_t x;
	uint8_t y;
}Point;

typedef uint8_t Board[15][15];

typedef Point POOL[225];

/*
typedef struct Piece {
	char x;
	char y;
	char player;
	char* len[4];
	struct Chunk_Info* chunk_in[4];
}Pieces[15][15];

typedef struct Piece* Used_Pieces[15 * 15];
*/