/*
	Evaluate the score of the point based on the id.
*/

#include "five_type.h"
#include "five_global.h"

Board empty_surroundings = { {0} };
typedef struct {
	uint8_t form[6];
}disvec;

extern disvec getvec(Point p, uint8_t color, Board *local_board);
extern inline Point move(Point p, uint8_t i, uint8_t j);
extern uint8_t verify_location(Point p);
uint8_t poolcnt = 0;

typedef struct {
	Point* point;
	int32_t value;
}Point_info;

typedef struct {
	Point_info info[255];
}InfoPOOL;

inline uint8_t check_ban(disvec* vec, uint8_t color) {
	if (!color) {
		for (uint8_t i = 3; i < 6; i++) {
			if (!(*vec).form[i]) 
				return 1;
		}
	}
	else {
		if (!(*vec).form[5]) return 1;
	}
	return 0;
}

/*Check if the game should stop*/
uint8_t check(Point p, uint8_t color, Board* local_board) {
	for (uint8_t i = 0; i < Round; i++) {
		if (board.location[PieceOnBoard.record[i].x][PieceOnBoard.record[i].y] == color) {
			disvec tmpvec = getvec(PieceOnBoard.record[i], color, local_board);
			if (check_ban(&tmpvec, color)) {
				printf("Ban triggered!\n");
				player = !player;
				return 1;
			}
			if (tmpvec.form[0] == 0) return 1;
		}
	};
	return 0;
}

uint8_t check_win(disvec* vec, uint8_t color) {
	/*
	disvec vec = getvec(p, color, local_board);
	if (check_ban(vec, color)) {
		return 10;
	};
	*/
	for (uint8_t i = 0; i < 3; i++) {
		if ((*vec).form[i] == 0) {
			return i + 1;
		}
	}
	if (color)
	{
		if ((*vec).form[3] == 0) return 4;
		if ((*vec).form[4] == 0) return 5;
	}
	return 0;
}

inline Board get_surroundings(POOL *PieceOnBoard, uint8_t localRound, Board *local_board) {
	Board surroundings = empty_surroundings;
	for (uint8_t k = 0; k < localRound; k++) {
		Point p = (*PieceOnBoard).record[k];
		for (uint8_t i = 0; i < 4; i++) {
			for (uint8_t j = 0; j < 2; j++) {
				Point tmpp = p;
				for (uint8_t k = 0; k < 2; k++) {
					tmpp = move(tmpp, i, j);
					if (verify_location(tmpp) && (*local_board).location[tmpp.x][tmpp.y] > 2)surroundings.location[tmpp.x][tmpp.y] = 1;
				}
			}
		}
	}
	return surroundings;
}

inline int32_t get_value(Point p, uint8_t color, Board *local_board) {
	disvec vec = getvec(p, color, local_board);
	int32_t win_value = 0;
	int32_t value = 0;
	if (check_ban(&vec, color))return 0;
	if (win_value = check_win(&vec, color)) value += (INT32_MAX >> 1) + (INT32_MAX >> (win_value + 1));
	//value += vec.form[0] < 5 ? 1 << ((5 - vec.form[0]) * 3) : 0;
	value += vec.form[1] < 4 ? 1 << ((4 - vec.form[1])) : 0;
	value += vec.form[2] < 6 ? 1 << ((6 - vec.form[2]) * 2) : 0;
	if (color) {
		value += vec.form[3] < 7 ? 1 << ((7 - vec.form[3])) : 0;
		value += vec.form[3] < 5 ? 1 << ((5 - vec.form[3])) : 0;
	}
	return value;
}

POOL get_pool(POOL *PieceOnBoard, uint8_t localRound, Board *local_board) {
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

InfoPOOL get_info(POOL* pool, uint8_t color, Board * local_board) {
	InfoPOOL infop = { 0 };
	for (uint8_t i = 0; i < poolcnt; i++) {
		Point tmpp = (*pool).record[i];
		int32_t value = get_value(tmpp, color, local_board);
		infop.info[i].point = &(*pool).record[i];
		infop.info[i].value = value;
	}
	return infop;
}

int32_t evaluate(uint8_t color, POOL *pool, Board *local_board) {
	int32_t value = 0;
	int32_t mymax = 0;
	int32_t opmax = 0;
	for (uint8_t i = 0; i < poolcnt; i++) {
		int32_t myvalue = get_value((*pool).record[i], color, local_board);
		int32_t opvalue = get_value((*pool).record[i], !color, local_board);
		value += myvalue;
		value -= opvalue;
		mymax = myvalue > mymax ? myvalue : mymax;
		opmax = opvalue > opmax ? opvalue : opmax;
	}
	if (mymax > INT32_MAX >> 1) {
		if (opmax > INT32_MAX >> 1) {
			return mymax > opmax ? mymax : -opmax;
		}
		else mymax;
	}
	else if (opmax > INT32_MAX >> 1) {
		return -opmax;
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
