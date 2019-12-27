/*
	Define the consts used in the game.
*/
#include "five_type.h"

const int8_t dx[4] = { 1,0,-1,1 };
const int8_t dy[4] = { 0,1,1,1 };
const int8_t direction[2] = { 1,-1 };

const uint8_t* symbol[] = { "¡ñ","¡ð","¡ø","¡÷","©° ","©Ð ","©´ ","©À ","©à ","©È ","©¸ ","©Ø ","©¼ " };

/*
	Abreviation of the symbols.
*/
enum SYMBOL {
	BP = 0, WP, BT, WT, TL, TC, TR, ML, MC, MR, BL, BC, BR
};

/*
	The empty board.
*/
const Board empty = { {
		{TL, TC, TC, TC, TC, TC, TC, TC, TC, TC, TC, TC, TC, TC, TR}, \
		{ML, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MR}, \
		{ML, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MR}, \
		{ML, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MR}, \
		{ML, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MR}, \
		{ML, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MR}, \
		{ML, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MR}, \
		{ML, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MR}, \
		{ML, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MR}, \
		{ML, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MR}, \
		{ML, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MR}, \
		{ML, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MR}, \
		{ML, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MR}, \
		{ML, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MC, MR}, \
		{BL, BC, BC, BC, BC, BC, BC, BC, BC, BC, BC, BC, BC, BC, BR}}
};