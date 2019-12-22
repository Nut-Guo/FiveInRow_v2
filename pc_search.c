#include "five_global.h"
#include "five_type.h"
#include <stdlib.h>
#include <string.h>

int32_t hash[4096][4096] = { 0 };
uint8_t MAXDEPTH = 7;
uint8_t WIDTH = 5;
Board local_board;
typedef struct {
	uint8_t form[6];
}disvec;
typedef struct {
	Point* point;
	int32_t value;
}Point_info;

typedef struct {
	Point_info info[255];
}InfoPOOL;
typedef struct {
	uint8_t distance[2][4];
}distance;
typedef struct {
	uint16_t hash[2];
}HASH;

extern uint8_t poolcnt;
extern POOL get_pool(POOL *PieceOnBoard, uint8_t localRound, Board *local_board);
extern int32_t evaluate(uint8_t color, POOL *pool, Board *local_board);
extern uint8_t check_win(disvec* vec, uint8_t color);
extern uint8_t check(Point p, uint8_t color, Board *local_board);
extern disvec getvec(Point p, uint8_t color, Board* local_board);
extern void qsort_pool(uint8_t L, uint8_t R, InfoPOOL* r, uint8_t color, Board* local_board);
extern InfoPOOL get_info(POOL* pool, uint8_t color, Board* local_board);
extern void ssort_pool(InfoPOOL* r, uint8_t color, uint8_t len, uint8_t cnt, Board* local_board);
extern inline int32_t get_value(Point p, uint8_t color, Board* local_board);
extern POOL get_seq_kill(POOL* eva_pool, uint8_t color, uint8_t localRound, Board* local_board);
extern distance id2dis(Point p, uint8_t color, Board* local_board);
extern inline Point move(Point p, uint8_t i, uint8_t j);
extern uint8_t verify_location(Point p);
extern HASH calc_hash(Point p, HASH origin_hash, uint8_t color);
Point Preturn = { 7,7 };
POOL local_PieceOnBoard;
uint8_t local_Round;
Board local_board;
uint16_t state_hash = 0;
HASH board_hash = {0,0};

inline int32_t Block(Point p, uint8_t color, Board* local_board) {
	(*local_board).location[p.x][p.y] = color;
	local_PieceOnBoard.record[local_Round++] = p;
	POOL eva_pool = get_pool(&local_PieceOnBoard, local_Round, local_board);
	uint8_t cnt = poolcnt;
	InfoPOOL opinfo_pool = get_info(&eva_pool, !color, local_board);
	ssort_pool(&opinfo_pool, !color, min(WIDTH, cnt - 1), cnt, local_board);
	local_Round--;
	(*local_board).location[p.x][p.y] = empty.location[p.x][p.y];
	uint8_t left = 0;
	while (opinfo_pool.info[left].value > (INT32_MAX >> 1)) {
		Point tryp = *opinfo_pool.info[left].point;
		(*local_board).location[tryp.x][tryp.y] = color;
		local_PieceOnBoard.record[local_Round++] = tryp;
		POOL deeper_pool = get_pool(&local_PieceOnBoard, local_Round, local_board);
		uint8_t deeper_cnt = poolcnt;
		InfoPOOL deeper_opinfo_pool = get_info(&eva_pool, !color, local_board);
		ssort_pool(&deeper_opinfo_pool, !color, min(WIDTH, cnt - 1), deeper_cnt, local_board);
		local_Round--;
		(*local_board).location[p.x][p.y] = empty.location[p.x][p.y];
		if (deeper_opinfo_pool.info[0].value < (INT32_MAX >> 1)) {
			Preturn = tryp;
			return get_value(tryp, color, local_board);
		}
		left++;
	}
	Preturn = p;
	if (!left) {
		return get_value(p, color, local_board);
	}
	else {
		return 0;
	}
}
inline void get_best_block(InfoPOOL *opinfo_pool, uint8_t color, Board *board) {
	uint8_t imax = 0;
	uint8_t i = 0;
	int32_t left_max = INT32_MIN;
	int32_t tmp_left = 0;
	Point bestP = *(*opinfo_pool).info[i].point;
	while ((*opinfo_pool).info[i].value > (INT32_MAX >> 1)) {
		if (Block(*(*opinfo_pool).info[i].point, color, &local_board) > left_max) {
			left_max = tmp_left;
			bestP = Preturn;
		}
		i++;
	}
	Preturn = bestP;
}
Point block_kill(Point kill, uint8_t color, Board* local_board) {
	distance dis = id2dis(kill, color, local_board);
	uint8_t i = 0;
	while (!(dis.distance[0][i] == 1 || dis.distance[1][i] == 1)) i++;
	uint8_t flag[2] = { 0 };
	Point tmpp[2] = { kill,kill };
	for (uint8_t j = 0; j < 2; j++) {
		while ((*local_board).location[tmpp[j].x][tmpp[j].y] == color) {
			tmpp[j] = move(tmpp[j], i, j);
			if (verify_location(tmpp[j]))break;
			if ((*local_board).location[tmpp[j].x][tmpp[j].y] == !color)break;
			if ((*local_board).location[tmpp[j].x][tmpp[j].y] > 2)flag[j] = 1;
		}
	}
	if (flag[0] && flag[1]) return get_value(tmpp[0], !color, local_board) > get_value(tmpp[1], !color, local_board) ? tmpp[0] : tmpp[1];
	else return flag[0] ? tmpp[0] : tmpp[1];
}
uint8_t search_seq(uint8_t color) {
	POOL eva_pool = get_pool(&local_PieceOnBoard, local_Round, &local_board);
	POOL kill_pool = get_seq_kill(&eva_pool, color, local_Round, &local_board);
	uint8_t cnt = poolcnt;
	uint8_t flag = 0;
	if (cnt == 0)return 0;
	for (uint8_t i = 0; i < cnt; i++) {
		Point tmpp = kill_pool.record[i];
		disvec vec = getvec(tmpp, color, &local_board);
		if (check_win(&vec, color)) {
			Preturn = tmpp;
			return 1;
		}
		local_board.location[tmpp.x][tmpp.y] = color;
		local_PieceOnBoard.record[local_Round++] = tmpp;
		Point blockp = block_kill(tmpp, color, &local_board);
		local_board.location[blockp.x][blockp.y] = !color;
		local_PieceOnBoard.record[local_Round++] = blockp;
		flag = search_seq(color);
		local_Round--;
		local_board.location[blockp.x][blockp.y] = empty.location[blockp.x][blockp.y];
		local_Round--;
		local_board.location[tmpp.x][tmpp.y] = empty.location[tmpp.x][tmpp.y];
		if (flag) {
			Preturn = tmpp;
			break;
		}
	}
	return flag;
}
int32_t search(uint8_t ab, int32_t AB, uint8_t depth, HASH srchash) {
	int32_t max = INT32_MIN;
	int32_t min = INT32_MAX;
	POOL eva_pool = get_pool(&local_PieceOnBoard, local_Round, &local_board);
	uint8_t cnt = poolcnt;
	InfoPOOL myinfo_pool = get_info(&eva_pool, (!ab) ^ player, &local_board);
	ssort_pool(&myinfo_pool, (!ab) ^ player, min(WIDTH,cnt-1), cnt, &local_board);
	InfoPOOL opinfo_pool = get_info(&eva_pool, !((!ab) ^ player), &local_board);
	ssort_pool(&opinfo_pool, !((!ab) ^ player), min(WIDTH,cnt-1), cnt, &local_board);
	if (myinfo_pool.info[0].value > (INT32_MAX >> 1)) {
		if (myinfo_pool.info[0].value >= opinfo_pool.info[0].value) {
			if (depth == MAXDEPTH) {
				Preturn = *myinfo_pool.info[0].point;
			}
			return myinfo_pool.info[0].value * ab ? 1 : -1;
		}
		else {
			if (depth == MAXDEPTH) {
				get_best_block(&opinfo_pool, (!ab) ^ player, &local_board);
			}
			return opinfo_pool.info[0].value * ab ? 1 : -1;
		}
	}
	else if (opinfo_pool.info[0].value > (INT32_MAX >> 1)) {
		if (depth == MAXDEPTH) {
			get_best_block(&opinfo_pool, (!ab) ^ player, &local_board);
		}
		return opinfo_pool.info[0].value * ab ? 1 : -1;
	}
	if (depth == MAXDEPTH) {
		if (search_seq(player))
			return 1;
	}
	int32_t localAB = ab ? INT32_MIN : INT32_MAX;
	for (uint8_t i = 0; i < min(WIDTH,cnt); i++) {
		for (uint8_t j = 0; j < 2; j++) {
			Point tmpp = j ? *myinfo_pool.info[i].point : *opinfo_pool.info[0].point;
			int32_t tmp_value = 0;
			local_board.location[tmpp.x][tmpp.y] = (!ab) ^ player;
			local_PieceOnBoard.record[local_Round++] = tmpp;
			HASH newhash = calc_hash(tmpp, srchash, (!ab) ^ player);
			if (hash[newhash.hash[0]][newhash.hash[1]]) {
				local_Round--;
				local_board.location[tmpp.x][tmpp.y] = empty.location[tmpp.x][tmpp.y];
				return hash[newhash.hash[0]][newhash.hash[1]];
			}
			if (0 == depth) {
				POOL value_pool = get_pool(&local_PieceOnBoard, local_Round, &local_board);
				tmp_value = evaluate(player, &value_pool, &local_board);
			}
			else {
				tmp_value = search(!ab, localAB, depth - 1,newhash);
			}
			hash[newhash.hash[0]][newhash.hash[1]] = tmp_value;
			local_Round--;
			local_board.location[tmpp.x][tmpp.y] = empty.location[tmpp.x][tmpp.y];
			if (ab) {
				if (max < tmp_value) {
					max = tmp_value;
					if (depth == MAXDEPTH) {
						Preturn = tmpp;
					}
					else if (max > AB)
						break;
				}
			}
			else {
				min = min > tmp_value ? tmp_value : min;
				if (min < AB)
					break;
			}
			localAB = ab ? (localAB < max ? max : localAB) : (localAB > min ? min : localAB);
		}
	}
	return ab ? max : min;
}

Point search_point(Point last) {
	local_board = board;
	local_PieceOnBoard = PieceOnBoard;
	local_Round = Round;
	board_hash = calc_hash(last, board_hash, !player);
	if (Round == 0) {
		Point center = { 7,7 };
		return center;
	}
	if (Round < 5) {
		MAXDEPTH = 11;
		WIDTH = 20;
	}
	else if(Round < 20){
		MAXDEPTH = 21;
		WIDTH = 10;
	}
	else {
		MAXDEPTH = 21;
		WIDTH = 5;
	}
	search(1, INT32_MIN, MAXDEPTH, board_hash);
	return Preturn;
}
