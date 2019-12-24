/*
	Evaluate the score of the point based on the id.
*/

#include "five_type.h"
#include "five_global.h"

Board empty_surroundings = { {0} };
typedef struct {
	uint8_t form[6];
}disvec;

typedef struct {
	int16_t value[15][15];
}Value_Table;

extern Value_Table local_value[2];

extern disvec getvec(Point p, uint8_t color, Board *local_board);
extern inline Point move(Point p, uint8_t i, uint8_t j);
extern uint8_t verify_location(Point p);
uint8_t poolcnt = 0;
extern uint8_t SCOPE;
typedef struct {
	Point* point;
	int16_t value;
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
				for (uint8_t k = 0; k < SCOPE; k++) {
					tmpp = move(tmpp, i, j);
					if (verify_location(tmpp) && (*local_board).location[tmpp.x][tmpp.y] > 2)surroundings.location[tmpp.x][tmpp.y] = 1;
				}
			}
		}
	}
	return surroundings;
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

uint8_t iskill(Point p, uint8_t color, Board* local_board) {
	disvec vec = getvec(p, color, local_board);
	if (vec.form[0] <= 1) return 2;
	if (vec.form[1] <= 1) return 1;
	return 0;
}

POOL get_seq_kill(POOL* eva_pool, uint8_t color, uint8_t localRound, Board* local_board) {
	uint8_t cnt = 0;
	POOL seq = { {0,0} };
	for (uint8_t i = 0; i < poolcnt; i++) {
		if (iskill((*eva_pool).record[i], color, local_board)) {
			seq.record[cnt++] = (*eva_pool).record[i];
		}
	}
	poolcnt = cnt;
	return seq;
}

InfoPOOL get_info(POOL* pool, uint8_t color, Board * local_board) {
	InfoPOOL infop = { 0 };
	for (uint8_t i = 0; i < poolcnt; i++) {
		Point tmpp = (*pool).record[i];
		//int16_t value = get_value(tmpp, color, local_board);
		infop.info[i].point = &(*pool).record[i];
		infop.info[i].value = local_value[color].value[tmpp.x][tmpp.y];
	}
	return infop;
}

int16_t evaluate(uint8_t color, POOL *pool, Board *local_board) {
	int16_t value = 0;
	int16_t mymax = 0;
	int16_t opmax = 0;
	for (uint8_t i = 0; i < poolcnt; i++) {
		int16_t myvalue = local_value[color].value[(*pool).record[i].x][(*pool).record[i].y];//get_value((*pool).record[i], color, local_board);
		int16_t opvalue = local_value[!color].value[(*pool).record[i].x][(*pool).record[i].y];//get_value((*pool).record[i], !color, local_board);
		value += myvalue;
		value -= opvalue;
		mymax = myvalue > mymax ? myvalue : mymax;
		opmax = opvalue > opmax ? opvalue : opmax;
	}
	
	if (mymax > INT16_MAX >> 1) {
		if (opmax > INT16_MAX >> 1) {
			return mymax >= opmax ? mymax : -opmax;
		}
		else return mymax;
	}
	else if (opmax > INT16_MAX >> 1) {
		return -opmax;
	}
	return value;
}
