#include "five_global.h"
#include "five_type.h"
#include <stdlib.h>
#include <string.h>

#define MAXDEPTH 1
Board local_board;
extern uint8_t poolcnt;
extern POOL get_pool(POOL *PieceOnBoard, uint8_t localRound, Board *local_board);
extern int32_t evaluate(uint8_t color, POOL *pool, Board *local_board);
extern uint8_t check_win(Point p, uint8_t color, Board *local_board);
extern uint8_t check(Point p, uint8_t color, Board *local_board);
extern void qsortp(uint8_t L, uint8_t R, POOL* r, uint8_t color, Board* board);
extern void init_zobrist();

uint16_t calc_hash(Point p, uint16_t origin_hash, uint8_t color);

int32_t hash[UINT16_MAX];
Point Preturn = { 7,7 };
POOL local_PieceOnBoard;
uint8_t local_Round;
Board local_board;
uint16_t state_hash = 0;


int32_t search(uint8_t ab, int32_t AB, uint8_t depth, uint16_t src_hash, int32_t *hash) {
	int32_t max = INT32_MIN;
	int32_t min = INT32_MAX;
	POOL eva_pool = get_pool(&local_PieceOnBoard, local_Round, &local_board);
	//qsortp(0, poolcnt - 1, &eva_pool, player, &local_board);
	uint8_t cnt = poolcnt;
	for (uint8_t i = 0; i < cnt; i++) {
		Point tmpp = eva_pool.record[i];
		int32_t tmp_value = 0, check_value = 0;
		int32_t localAB = ab ? INT32_MIN : INT32_MAX;
		if (check_value = check_win(tmpp, (!ab) ^ player, &local_board)) {
			if (check_value == 1 && depth == MAXDEPTH) { Preturn = tmpp; return INT32_MAX; }
			check_value = ab ? INT32_MAX - check_value : INT32_MIN + check_value;
		}
		uint16_t dst_hash = calc_hash(tmpp, src_hash, (!ab) ^ player);
		if (hash[dst_hash]) tmp_value = hash[dst_hash];
		else {
			local_board.location[tmpp.x][tmpp.y] = (!ab) ^ player;
			local_PieceOnBoard.record[local_Round++] = tmpp;
			if (0 == depth) {
				POOL value_pool = get_pool(&local_PieceOnBoard, local_Round, &local_board);
				tmp_value = evaluate(player, &value_pool, &local_board);
			}
			else {
				tmp_value = search(!ab, localAB, depth - 1, dst_hash, hash);
			}
			local_Round--;
			local_board.location[tmpp.x][tmpp.y] = empty.location[tmpp.x][tmpp.y];
			hash[dst_hash] = tmp_value;
		}
		if (abs(check_value) > abs(tmp_value))tmp_value = check_value;
		if (ab) {
			if (max < tmp_value) {
				max = tmp_value;
				if (depth == MAXDEPTH) {
					Preturn = tmpp;
				}
				else if (max > AB) break;
			}
		}
		else {
			min = min > tmp_value ? tmp_value : min;
			if (min < AB) break;
		}
		localAB = ab ? (localAB < max ? max : localAB) : (localAB > min ? min : localAB);
	}
	return ab ? max : min;
}

Point search_point(Point last) {

	local_board = board;
	local_PieceOnBoard = PieceOnBoard;
	local_Round = Round;
	memset(hash, 0, sizeof(int32_t) * UINT16_MAX);
	state_hash = calc_hash(last, state_hash, !player);
	search(1,INT32_MIN, MAXDEPTH, state_hash, hash);
	state_hash = calc_hash(Preturn, state_hash, player);
	return Preturn;
}
