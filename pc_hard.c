/*
	stupid version that tries to find the most valuable position.
*/

#include "five_func.h"
#include "five_global.h"
#include "five_type.h"

typedef struct {
	uint8_t form[5];
}disvec;

typedef struct {
	disvec color[2];
}vecpare;

typedef vecpare vecpool[255];

uint8_t hardpsp = 0;
POOL hardpool;
vecpool hardvec;
extern void add2pool(Point last, POOL pool, uint8_t* psp, vecpool disvecpool);
extern Point calcboard(uint8_t color, POOL pool, uint8_t* psp, vecpool disvecpool);
extern uint8_t check(Point p, uint8_t color);

typedef struct {
	Point root;
	Board state;
	POOL pool;
	uint8_t color;
	uint8_t psp;
}Search_Unit;

Point minP;
uint8_t getmin(uint8_t array[],uint8_t length,POOL pool) {
	uint8_t min = 255;
	for (uint8_t i = 0; i < length; i++) {
		min = min > array[i] ? array[i] : min;
		minP = min > array[i] ? pool[i] : minP;
	}
	return min;
}

uint8_t search(uint8_t depth, Point root,Board state,POOL pool, uint8_t color, uint8_t psp, vecpool disvecpool) {
	uint8_t step[255];
	if (depth > 3)return 255;
	for (uint8_t i = 0; i < 100; i++) {
		step[i] = 255;
	}
	for (uint8_t i = 0; i < 20 && i < psp; i++) {
		Board tmpstate;
		for(uint8_t x = 0; x < 15; x++)
			for (uint8_t y = 0; y < 15; y++) {
				tmpstate[x][y] = state[x][y];
			}
		POOL tmppool;
		vecpool tmpvecpool;
		for (uint8_t sp = 0; sp < psp; sp++) {
			tmppool[sp] = pool[sp];
			tmpvecpool[sp] = disvecpool[sp];
		}
		uint8_t tmppsp = psp;
		tmpstate[tmppool[i].x][tmppool[i].y] = color;
		add2pool(tmppool[i],tmppool, &tmppsp, hardvec);
		if (check(tmppool[i],color)) {
			if (color == player) {
				return depth;
			}
			return 255;
		}
		else {
			Point orig_root = depth == 1 ? pool[i] : root;
			step[i] = search(depth + 1, orig_root, tmpstate,tmppool,!color,tmppsp, tmpvecpool);
		}
	}
	return getmin(step, psp,pool);
}

Point choose_indepth(Point last) {
	if (Round == 0 || Round == 1) {
		hardpool[0] = last;
		hardpsp = 1;
		add2pool(last, hardpool, &hardpsp, hardvec);
		if (!Round) return last;
	}
	else add2pool(last, hardpool, &hardpsp, hardvec);
	minP = calcboard(player, hardpool, &hardpsp, hardvec);
	search(1, minP, board, hardpool, player, hardpsp, hardvec);
	add2pool(minP, hardpool, &hardpsp, hardvec);
	return minP;
}
