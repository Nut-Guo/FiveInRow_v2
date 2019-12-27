/*
	Evaluate the score of the point based on the id.
*/

#include "five_type.h"
#include "five_global.h"
#include <stdio.h>

/*
	Vector stores the distance to 5, 4 alive, 3 + 4, 4 + 4, 3 + 3 and loneline.
*/
typedef struct {
	uint8_t form[6];
}Disvec;

/*
	An 2-d array stores the value of every points of current table.
*/
typedef struct {
	int16_t value[15][15];
}Value_Table;

/*
	The pointer of the point and its value.
*/
typedef struct {
	Point* point;
	int16_t value;
}Point_Info;

/*
	An array of all the point to be inspect and its value.
*/
typedef struct {
	Point_Info info[255];
}Info_Pool;

extern inline Disvec getvec(Point p, uint8_t color, Board *local_board);
extern inline void move(Point *p, uint8_t i, uint8_t j);
extern inline uint8_t verify_location(Point p);
extern uint8_t SCOPE;
extern Value_Table local_value[2];
uint8_t poolcnt = 0; //Pass the length of the Info_pool.
Board empty_surroundings = { {0} }; //Board used to identify the empty point around the current pieces.
uint8_t Ban_flag = 0;//Pass the baned infomation.
Board surroundings;//Board used to store the surroundings indicator.

/*
	Check if this step makes a ban.
*/
extern inline uint8_t check_ban(Disvec* vec, uint8_t color) {
	if (!color) {
		for (uint8_t i = 3; i < 6; i++) {
			if (!(*vec).form[i]) {
				Ban_flag = 1;
				return 1;
			}
		}
	}
	else {
		if (!(*vec).form[5]) {
			Ban_flag = 1;
			return 1;
		}
	}
	Ban_flag = 0;
	return 0;
}

/*
	Check if the game should stop
*/
extern inline uint8_t check(Point p, uint8_t color, Board* local_board) {
	for (uint8_t i = 0; i < Round; i++) {
		if (board.location[PieceOnBoard.record[i].x][PieceOnBoard.record[i].y] == color) {
			Disvec tmpvec = getvec(PieceOnBoard.record[i], color, local_board);
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

/*
	Check if this step will win the game.
*/
extern inline uint8_t check_win(Disvec* vec, uint8_t color) {
	//Disvec vec = getvec(p, color, local_board);
	if (check_ban(vec, color)) {
		return 0;
	};
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

/*
	Get the value of the point.
*/
extern inline int16_t get_value(Point p, uint8_t color, Board* local_board) {
	Disvec vec = getvec(p, color, local_board);
	int16_t win_value = 0;
	int16_t value = 0;
	win_value = check_win(&vec, color);
	if (Ban_flag)
		return 0;
	if (win_value) {
		value += (INT16_MAX >> 1) + (INT16_MAX >> (win_value + 1));
		return value;
	}
	value += vec.form[0] < 4 ? INT16_MAX >> ((8 + 2 * vec.form[0])) : 0;
	value += vec.form[1] < 4 ? INT16_MAX >> ((8 + 2 * vec.form[1])) : 0;
	value += vec.form[2] < 4 ? INT16_MAX >> ((8 + 2 * vec.form[2])) : 0;
	if (color) {
		value += vec.form[3] < 4 ? INT16_MAX >> ((8 + 2 * vec.form[3])) : 0;
		value += vec.form[4] < 4 ? INT16_MAX >> ((8 + 2 * vec.form[4])) : 0;
	}
	return value;
}

/*
	Get the surrounding pieces for later use.
*/
extern inline Board get_surroundings(Pool *PieceOnBoard, uint8_t localRound, Board *local_board) {
	Board surroundings = empty_surroundings;
	for (uint8_t k = 0; k < localRound; k++) {
		Point p = (*PieceOnBoard).record[k];
		for (uint8_t i = 0; i < 4; i++) {
			for (uint8_t j = 0; j < 2; j++) {
				Point tmpp = p;
				for (uint8_t l = 0; l < SCOPE; l++) {
					move(&tmpp, i, j);
					if (verify_location(tmpp) && 
						(*local_board).location[tmpp.x][tmpp.y] > 2)
						surroundings.location[tmpp.x][tmpp.y] = 1;
				}
			}
		}
	}
	return surroundings;
}

/*
	Put all the surrounding points to an array.
*/
extern inline Pool get_pool(Pool *PieceOnBoard, uint8_t local_Round, Board *local_board) {
	surroundings = get_surroundings(PieceOnBoard,local_Round,local_board);
	Pool pool = { {0,0} };
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

/*
	Check if this step could threat the enemy.
*/
extern inline uint8_t iskill(Point p, uint8_t color, Board* local_board) {
	Disvec vec = getvec(p, color, local_board);
	if (check_ban(&vec, color))return 0;
	if (vec.form[1] <= 1) return 1;
	if (vec.form[0] <= 1) return 2;
	return 0;
}

extern inline Pool get_seq_kill(Pool* eva_pool, uint8_t color, uint8_t localRound, Board* local_board) {
	uint8_t cnt = 0;
	Pool seq = { {0,0} };
	for (uint8_t i = 0; i < poolcnt; i++) {
		if (iskill((*eva_pool).record[i], color, local_board)) {
			seq.record[cnt++] = (*eva_pool).record[i];
		}
	}
	poolcnt = cnt;
	return seq;
}

extern inline Info_Pool get_info(Pool* pool, uint8_t color, Board * local_board) {
	Info_Pool infop = { 0 };
	for (uint8_t i = 0; i < poolcnt; i++) {
		Point tmpp = (*pool).record[i];
		infop.info[i].point = &(*pool).record[i];
		infop.info[i].value = local_value[color].value[tmpp.x][tmpp.y];
	}
	return infop;
}

extern inline void ssort_pool(Info_Pool* r, uint8_t color, uint8_t len, uint8_t cnt, Board* local_board) {
	uint8_t i, j;
	Point_Info temp;
	for (i = 0; i < len; i++)
	{
		uint8_t max = i;
		for (j = i + 1; j < cnt; j++)
		{
			if ((*r).info[j].value > (*r).info[max].value)
			{
				max = j;
			}
		}
		if (max != i)
		{
			temp = (*r).info[max];
			(*r).info[max] = (*r).info[i];
			(*r).info[i] = temp;
		}
	}
}
