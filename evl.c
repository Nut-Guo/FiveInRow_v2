/*
	Evaluate the score of the point based on the id.
*/

#include "five_type.h"
#include "five_global.h"

Board empty_surroundings = { {0} };
typedef struct {
	uint8_t form[5];
}disvec;

extern disvec getvec(Point p, uint8_t color, Board local_board);
inline Point move(Point p, uint8_t i, uint8_t j);
extern uint8_t verify_location(Point p);

uint8_t poolcnt = 0;

inline Board get_surroundings(POOL PieceOnBoard, uint8_t localRound, Board local_board) {
	Board surroundings = empty_surroundings;
	for (uint8_t k = 0; k < localRound; k++) {
		Point p = PieceOnBoard.record[k];
		Point tmpp;
		for (uint8_t i = 0; i < 4; i++) {
			for (uint8_t j = 0; j < 2; j++) {
				tmpp = move(p, i, j);
				if(verify_location(tmpp) && local_board.location[tmpp.x][tmpp.y] > 2)surroundings.location[tmpp.x][tmpp.y] = 1;
			}
		}
	}
	return surroundings;
}

inline int32_t get_value(Point p, uint8_t color, Board local_board) {
	disvec vec = getvec(p, color, local_board);
	int32_t value = 0;
	value += vec.form[0] < 5 ? 1 << ((5 - vec.form[0]) * 3) : 0;
	value += vec.form[1] < 4 ? 1 << ((4 - vec.form[0]) * 3) : 0;
	return value;
}

POOL get_pool(POOL PieceOnBoard, uint8_t localRound, Board local_board) {
	Board surroundings = get_surroundings(PieceOnBoard,localRound,local_board);
	POOL pool = { {0,0} };
	poolcnt = 0;
	for (uint8_t i = 0; i < 15; i++)
		for (uint8_t j = 0; j < 15; j++) {
			if (surroundings.location[i][j] == 1) {
				Point tmpp = { i,j };
				pool.record[poolcnt++] = tmpp;
			}
		}
	return pool;
}

int32_t evaluate(uint8_t color, POOL pool, Board local_board) {
	int32_t value = 0;
	int32_t mymax = 0;
	int32_t opmax = 0;
	for (uint8_t i = 0; i < poolcnt; i++) {
		int32_t myvalue = get_value(pool.record[i], color, local_board);
		int32_t opvalue = get_value(pool.record[i], !color, local_board);
		value += myvalue;
		value -= opvalue;
	}
		/*
		mymax = myvalue > mymax ? myvalue : mymax;
		opmax = opvalue > opmax ? opvalue : opmax;
		value += myvalue > INT32_MAX - 5 ? 0 : myvalue;
		value -= opvalue > INT32_MAX - 5 ? 0 : opvalue;
	}
	if (mymax > INT32_MAX - 5) {
		if (opmax > INT32_MAX - 5) {
			return mymax > opmax ? mymax : -opmax;
		}
		else mymax;
	}
	else if (opmax > INT32_MAX - 5) {
		return -opmax;
	}
	*/
	return value;
}

uint8_t check_win(Point p, uint8_t color, Board local_board) {
	disvec vec = getvec(p, color, local_board);
	for (uint8_t i = 0; i < 5; i++) {
		if (vec.form[i] == 0) {
			return i + 1;
		}
	}
	return 0;
}
/*
uint16_t evaluate(pieceid id) {
	uint16_t value = 0;
	for (int i = 0; i < 4; i++) {
		if (scoreboard[id.id[i]]) {
			value += scoreboard[id.id[i]];
		}
		else {
			value = 0;
			break;
		}
	}
	return value;
}
*/