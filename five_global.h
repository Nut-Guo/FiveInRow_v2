/*
	Define the global virables and consts
*/

#pragma once

#include "five_type.h"

extern uint8_t player;
extern uint8_t Round;
extern Board board;
extern POOL PieceOnBoard;

extern const uint8_t dx[4];
extern const uint8_t dy[4];
extern const uint8_t direction[2];
extern const uint32_t scoreboard[512];

extern const uint8_t* symbol[];
enum SYMBOL {
	BP = 0, WP, BT, WT, TL, TC, TR, ML, MC, MR, BL, BC, BR
};
extern const Board empty;

extern void(*Play[])(Point(*P1)(), Point(*P2)());
extern Point(*Input[])();