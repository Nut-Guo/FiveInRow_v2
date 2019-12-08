/*
	stupid version that tries to find the most valuable position.
*/

#include "five_func.h"
#include "five_global.h"
#include "five_type.h"

extern Point add2pool(Point last);
extern Point calcboard(uint8_t color);
extern Point checknull(Point last);
Point choose(Point last) {
	if (Round == 0) {
		checknull(last);
	}
	add2pool(last);
	/*
	Point maxP = { 7,7 };
	int16_t maxV = 0;
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			if (board[i][j] > 3) {
				Point tmpp = { i,j };
				int16_t tmpval = evaluate(lookup(tmpp, player)) + evaluate(lookup(tmpp,!player));
				if ( tmpval > maxV) {
					maxP = tmpp;
					maxV = tmpval;
				}
				//maxP = tmpval > maxV ? tmpp : maxP;
				//maxV = tmpval > maxV ? tmpval : maxV;
			}
		}
	}
	return maxP;
	*/
	return calcboard(player);
}
