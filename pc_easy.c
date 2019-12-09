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

vecpool easyvec;
uint8_t easypsp = 0;
POOL easypool;

extern void updatevec(POOL pool, uint8_t index, vecpool disvecpool);
extern void add2pool(Point last,POOL pool,uint8_t *psp, vecpool disvecpool);
extern Point calcboard(uint8_t color,POOL pool, uint8_t* psp, vecpool disvecpool);
Point choose(Point last) {
	if (Round == 0||Round == 1) {
		easypool[0] = last;
		updatevec(easypool, 0, easyvec);
		easypsp = 1;
		add2pool(last,easypool,&easypsp, easyvec);
		if(!Round) return last;
	}
	else add2pool(last, easypool, &easypsp, easyvec);
	Point goodP = calcboard(player, easypool, &easypsp, easyvec);
	add2pool(goodP, easypool, &easypsp, easyvec);
	return goodP;
}