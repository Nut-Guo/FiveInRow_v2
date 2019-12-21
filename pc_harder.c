/*
	stupid version that tries to find the most valuable position.
*/
/*
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

vecpool hardervec;
uint8_t harderpsp = 0;
POOL harderpool;
extern void add2pool(Point last, POOL pool, uint8_t* psp, vecpool disvecpool);
extern Point calcboard(uint8_t color, POOL pool, uint8_t* psp, vecpool disvecpool);
extern uint8_t check(Point p, uint8_t color);

typedef struct {
	uint8_t distance[4][2];
}distance;

typedef struct {
	Point root;
	Board state;
	POOL pool;
	uint8_t color;
	uint8_t psp;
}Search_Unit;
*/