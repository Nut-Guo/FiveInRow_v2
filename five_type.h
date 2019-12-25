/*
	Define main types.
*/

#pragma once

#include<stdint.h>

typedef struct {
	uint8_t x;
	uint8_t y;
}Point;

typedef struct {
	uint8_t location[15][15];
}Board;

typedef struct {
	Point record[225];
}POOL;