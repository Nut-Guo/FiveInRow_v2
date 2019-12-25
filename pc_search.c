#include "five_global.h"
#include "five_type.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
	uint8_t form[6];
}disvec;
typedef struct {
	Point* point;
	int16_t value;
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
typedef struct {
	int16_t value[15][15];
}Value_Table;

extern uint8_t poolcnt;
extern POOL get_pool(POOL *PieceOnBoard, uint8_t localRound, Board *local_board);
extern uint8_t check_win(disvec* vec, uint8_t color);
extern disvec getvec(Point p, uint8_t color, Board* local_board);
extern InfoPOOL get_info(POOL* pool, uint8_t color, Board* local_board);
extern void ssort_pool(InfoPOOL* r, uint8_t color, uint8_t len, uint8_t cnt, Board* local_board);
extern int16_t get_value(Point p, uint8_t color, Board* local_board);
extern POOL get_seq_kill(POOL* eva_pool, uint8_t color, uint8_t localRound, Board* local_board);
extern inline Point move(Point p, uint8_t i, uint8_t j);
extern inline uint8_t verify_location(Point p);
extern HASH calc_hash(Point p, HASH origin_hash, uint8_t color);
extern uint8_t iskill(Point p, uint8_t color, Board* local_board);

int16_t search(uint8_t ab, int16_t AB, uint8_t depth, HASH srchash);

Value_Table local_value[2] = { 0 };
Point Preturn = { 7,7 };
Point BlockReturn = { 7,7 };
Point KillReturn = { 7,7 };
POOL local_PieceOnBoard;
uint8_t local_Round = 0;
Board local_board;
int16_t hash[4096][4096] = { 0 };
uint16_t check_hash[4096][4096] = { 0 };
int16_t KILL_hash[4096][4096] = { 0 };
uint8_t MAXDEPTH = 7;
uint8_t WIDTH = 5;
uint8_t SCOPE = 2;
uint8_t KILL_DEPTH = 8;

//Matain the value table;
void update_value_table(Point p) {
	local_value[0].value[p.x][p.y] = get_value(p, 0, &local_board);
	local_value[1].value[p.x][p.y] = get_value(p, 1, &local_board);
	for (uint8_t i = 0; i < 4; i++) {
		for (uint8_t j = 0; j < 2; j++) {
			Point tmpp = p;
			for (uint8_t k = 0; k < 4; k++) {
				tmpp = move(tmpp, i, j);
				if (!verify_location(tmpp)) break;
				local_value[0].value[tmpp.x][tmpp.y] = get_value(tmpp, 0, &local_board);
				local_value[1].value[tmpp.x][tmpp.y] = get_value(tmpp, 1, &local_board);
			}
		}
	}
}

inline int16_t Block(Point p, uint8_t color, Board* local_board) {
	//Check if after block, there is still a kill; 
	(*local_board).location[p.x][p.y] = color;
	local_PieceOnBoard.record[local_Round++] = p;
	update_value_table(p);
	//Get the new pool;
	POOL eva_pool = get_pool(&local_PieceOnBoard, local_Round, local_board);
	uint8_t cnt = poolcnt;
	InfoPOOL opinfo_pool = get_info(&eva_pool, !color, local_board);
	ssort_pool(&opinfo_pool, !color, 5, cnt, local_board);
	local_Round--;
	(*local_board).location[p.x][p.y] = empty.location[p.x][p.y];
	update_value_table(p);
	//Test thenew pool;
	uint8_t left = 0;
	while (opinfo_pool.info[left].value > (INT16_MAX >> 1)) {
		Point tryp = *opinfo_pool.info[left].point;
		(*local_board).location[tryp.x][tryp.y] = color;
		local_PieceOnBoard.record[local_Round++] = tryp;
		update_value_table(tryp);
		// Get the state after the try.
		POOL deeper_pool = get_pool(&local_PieceOnBoard, local_Round, local_board);
		uint8_t deeper_cnt = poolcnt;
		InfoPOOL deeper_opinfo_pool = get_info(&deeper_pool, !color, local_board);
		ssort_pool(&deeper_opinfo_pool, !color, min(WIDTH, cnt - 1), deeper_cnt, local_board);
		local_Round--;
		(*local_board).location[tryp.x][tryp.y] = empty.location[p.x][p.y];
		update_value_table(tryp);
		// If this block makes a kill itself or it dekill the original one, keep it.
		if (/*get_value(tryp, color, local_board)*/local_value[color].value[tryp.x][tryp.y] > deeper_opinfo_pool.info[0].value 
			|| deeper_opinfo_pool.info[0].value < (INT16_MAX >> 1)) {
			BlockReturn = tryp;
			return local_value[color].value[tryp.x][tryp.y];//get_value(tryp, color, local_board);
		}
		left++;
	}
	BlockReturn = p;
	if (!left) {
		return local_value[color].value[p.x][p.y];//get_value(p, color, local_board);
	}
	else {
		return 0;
	}
}

inline void get_best_block(InfoPOOL *opinfo_pool, uint8_t color, Board *board) {
	uint8_t imax = 0;
	uint8_t i = 0;
	int16_t left_max = 1;
	int16_t tmp_left = 0;
	Point bestP = *(*opinfo_pool).info[i].point;
	while ((*opinfo_pool).info[i].value > (INT16_MAX >> 10)) {
		tmp_left = Block(*(*opinfo_pool).info[i].point, color, &local_board);
		if (tmp_left > left_max) {
			left_max = tmp_left;
			bestP = BlockReturn;
		}
		i++;
	}
	BlockReturn = bestP;
}

int16_t evaluate_the_board(uint8_t color) {
	int16_t tmp_value = 0;
	POOL value_pool = get_pool(&local_PieceOnBoard, local_Round, &local_board);
	uint8_t value_cnt = poolcnt;
	InfoPOOL my_value_info_pool = get_info(&value_pool,color, &local_board);
	ssort_pool(&my_value_info_pool, color, 3, value_cnt, &local_board);
	InfoPOOL op_value_info_pool = get_info(&value_pool, !color, &local_board);
	ssort_pool(&op_value_info_pool, !color, 3, value_cnt, &local_board);
	if (op_value_info_pool.info[0].value > INT16_MAX >> 1 &&
		op_value_info_pool.info[0].value > my_value_info_pool.info[0].value)
		tmp_value = op_value_info_pool.info[0].value;
	else if ((tmp_value = my_value_info_pool.info[2].value) > INT16_MAX >> 1)
		tmp_value = -my_value_info_pool.info[2].value;
	else {
		for (uint8_t j = 2; j < poolcnt; j++) {
			tmp_value += op_value_info_pool.info[j].value;
			tmp_value -= my_value_info_pool.info[j].value;
		}
		tmp_value /= poolcnt + 1;
		if (tmp_value < INT16_MAX >> 1) {
			int16_t exist_value = 0;
			for (uint8_t i = 0; i < local_Round; i++) {
				Point tmpp = local_PieceOnBoard.record[i];
				exist_value += local_value[i % 2].value[tmpp.x][tmpp.y] * ((player == i % 2) ? 1 : -1);//get_value(local_PieceOnBoard.record[i], i % 2, &local_board) * (i % 2 == ((!ab) ^ player)) ? 1 : -1;
			}
			tmp_value += exist_value / local_Round;
		}
	}
	return tmp_value;
}

int16_t search_seq(uint8_t color, HASH srchash, int8_t depth) {
	//Get the points that makes a kill;
	int16_t local_max = INT16_MIN;
	POOL eva_pool = get_pool(&local_PieceOnBoard, local_Round, &local_board);
	POOL kill_pool = get_seq_kill(&eva_pool, color, local_Round, &local_board);
	if (!depth) {
		//KillReturn = kill_pool.record[0];
		//return INT16_MAX >> 6;
		return evaluate_the_board(color);
	}
	uint8_t cnt = poolcnt;
	int16_t flag = 0;
	if (cnt == 0)
		return evaluate_the_board(color);
	//Iterate through the points
	for (uint8_t i = 0; i < cnt; i++) {
		Point tmpp = kill_pool.record[i];
		disvec vec = getvec(tmpp, color, &local_board);
		if (check_win(&vec, color)) {
			KillReturn = tmpp;
			return local_value[color].value[tmpp.x][tmpp.y];// get_value(tmpp, color, &local_board);
		}
		//Set the point.
		local_board.location[tmpp.x][tmpp.y] = color;
		local_PieceOnBoard.record[local_Round++] = tmpp;
		update_value_table(tmpp);
		HASH newhash = calc_hash(tmpp, srchash, color);
		if (check_hash[newhash.hash[0]][newhash.hash[1]] == (srchash.hash[0] ^ srchash.hash[1])) {
			local_Round--;
			local_board.location[tmpp.x][tmpp.y] = empty.location[tmpp.x][tmpp.y];
			update_value_table(tmpp);
			return KILL_hash[newhash.hash[0]][newhash.hash[1]];
		}
		POOL killed_pool = get_pool(&local_PieceOnBoard, local_Round, &local_board);
		InfoPOOL block_info_pool = get_info(&killed_pool, color, &local_board);
		uint8_t blockcnt = poolcnt;
		ssort_pool(&block_info_pool, color, 5, blockcnt, &local_board);
		get_best_block(&block_info_pool, !color, &local_board);
		Point blockp = BlockReturn;
		vec = getvec(blockp, !color, &local_board);
		if (check_win(&vec, !color)||(iskill(blockp,!color,&local_board))) {
			flag = evaluate_the_board(color);
			check_hash[newhash.hash[0]][newhash.hash[1]] = srchash.hash[0] ^ srchash.hash[1];
			KILL_hash[newhash.hash[0]][newhash.hash[1]] = local_value[!color].value[blockp.x][blockp.y];
		}
		else {
			local_board.location[blockp.x][blockp.y] = !color;
			local_PieceOnBoard.record[local_Round++] = blockp;
			update_value_table(blockp);
			flag = search_seq(color, calc_hash(blockp, newhash, !color), depth - 1);
			local_Round--;
			local_board.location[blockp.x][blockp.y] = empty.location[blockp.x][blockp.y];
			update_value_table(blockp);
			check_hash[newhash.hash[0]][newhash.hash[1]] = srchash.hash[0] ^ srchash.hash[1];
			KILL_hash[newhash.hash[0]][newhash.hash[1]] = flag;
		}
		local_Round--;
		local_board.location[tmpp.x][tmpp.y] = empty.location[tmpp.x][tmpp.y];
		update_value_table(tmpp);
		if (flag > local_max) {
			local_max = flag;
			KillReturn = tmpp;
		}
	}
	return local_max;
}

int16_t ABsearch(InfoPOOL* info_pool, int32_t* extreme, uint8_t ab, int16_t AB, uint8_t depth, HASH srchash, uint8_t cnt) {
	int16_t localAB = ab ? INT16_MIN : INT16_MAX;
	Point tmpp;
	for (uint8_t i = 0; i < min(WIDTH, cnt); i++) {
		int16_t tmp_value = 0;
		tmpp = *(*info_pool).info[i].point;
		local_board.location[tmpp.x][tmpp.y] = (!ab) ^ player;
		local_PieceOnBoard.record[local_Round++] = tmpp;
		update_value_table(tmpp);
		//Chech hash
		HASH newhash = calc_hash(tmpp, srchash, (!ab) ^ player);
		if (check_hash[newhash.hash[0]][newhash.hash[1]] == (srchash.hash[0] ^ srchash.hash[1])){
			local_Round--;
			local_board.location[tmpp.x][tmpp.y] = empty.location[tmpp.x][tmpp.y];
			update_value_table(tmpp);
			return hash[newhash.hash[0]][newhash.hash[1]];
		}
		//Get to an end
		if (0 == depth) {
			tmp_value = evaluate_the_board((!ab) ^ player);
		}
		//Search deeper
		else {
			tmp_value = search(!ab, localAB, depth - 1, newhash);
		}
		//update hash;
		hash[newhash.hash[0]][newhash.hash[1]] = tmp_value;
		check_hash[newhash.hash[0]][newhash.hash[1]] = srchash.hash[0] ^ srchash.hash[1];
		//get back the piece;
		local_Round--;
		local_board.location[tmpp.x][tmpp.y] = empty.location[tmpp.x][tmpp.y];
		update_value_table(tmpp);
		//update min max;
		if (ab) {
			if ((*extreme) < tmp_value) {
				(*extreme) = tmp_value;
				if (depth == MAXDEPTH) {
					Preturn = tmpp;
				}
				else if ((*extreme) > AB)
					break;
			}
		}
		else {
			(*extreme) = (*extreme) > tmp_value ? tmp_value : (*extreme);
			if ((*extreme) < AB)
				break;
		}
		localAB = (*extreme);
	}
	return (*extreme);
}

int16_t search(uint8_t ab, int16_t AB, uint8_t depth, HASH srchash) {
	POOL eva_pool = get_pool(&local_PieceOnBoard, local_Round, &local_board);
	uint8_t cnt = poolcnt;
	InfoPOOL myinfo_pool = get_info(&eva_pool, (!ab) ^ player, &local_board);
	ssort_pool(&myinfo_pool, (!ab) ^ player, min(WIDTH,cnt-1), cnt, &local_board);
	InfoPOOL opinfo_pool = get_info(&eva_pool, !((!ab) ^ player), &local_board);
	ssort_pool(&opinfo_pool, !((!ab) ^ player), min(WIDTH,cnt-1), cnt, &local_board);
	int16_t tmp_value = 0;
	//Predecide if something trivial;
	if ((myinfo_pool).info[0].value > (INT16_MAX >> 1)) {
		if ((myinfo_pool).info[0].value >= (opinfo_pool).info[0].value) {
			if (depth == MAXDEPTH) {
				Preturn = *myinfo_pool.info[0].point;
			}
			return myinfo_pool.info[0].value * (ab ? 1 : -1);
		}
	}
	//if (tmp_value) return tmp_value;
	//If not threatened, get a tragedy;
	int32_t extreme = ab ? INT16_MIN : INT16_MAX;
	if (opinfo_pool.info[0].value <= myinfo_pool.info[0].value && myinfo_pool.info[5].value > (INT16_MAX >> 10)) {
		if (depth == MAXDEPTH) {
			KILL_DEPTH = 5;
			tmp_value = search_seq(player, srchash, KILL_DEPTH);
			Preturn = KillReturn;
			extreme = max(tmp_value, extreme);
			if ( tmp_value> (INT16_MAX >> 1))
				return tmp_value;
		}
		if (depth == MAXDEPTH - 1) {
			KILL_DEPTH = 3;
			tmp_value = search_seq(!player, srchash, KILL_DEPTH);
			extreme = min(extreme, -tmp_value);
			if(tmp_value > (INT16_MAX >> 1))
				return -tmp_value;
		}
	}
	int32_t op_search = ABsearch(&opinfo_pool, &extreme, ab, AB, depth, srchash, cnt);
	int32_t my_search = ABsearch(&myinfo_pool, &extreme, ab, AB, depth, srchash, cnt);
	tmp_value = ab ? max(op_search, my_search) : (min(op_search, my_search));
	return tmp_value;
}

Point search_point(Point last) {
	local_board = board;
	local_PieceOnBoard = PieceOnBoard;
	local_Round = Round;
	memset(check_hash, 0, sizeof(uint16_t) * 4096 * 4096);
	HASH init_hash = { 0,0 };
	for (uint8_t i = 0; i < Round; i++) {
		update_value_table(PieceOnBoard.record[i]);
		init_hash = calc_hash(PieceOnBoard.record[i], init_hash, i % 2);
	}
	if (Round == 0) {
		Point center = { 7,7 };
		return center;
	}
	if (Round < 4) {
		SCOPE = 1;
		MAXDEPTH = 3;
	}
	else {
		SCOPE = 2;
		MAXDEPTH = 3;
	}
	WIDTH = 10;

	search(1, INT16_MIN, MAXDEPTH, init_hash);
	local_board.location[Preturn.x][Preturn.y] = player;
	update_value_table(Preturn);
	return Preturn;
}
