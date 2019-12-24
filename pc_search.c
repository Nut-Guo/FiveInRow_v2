#include "five_global.h"
#include "five_type.h"
#include <stdlib.h>
#include <string.h>

#define KILL_DEPTH 5

int16_t hash[4096][4096] = { 0 };
int16_t KILL_hash[4096][4096] = { 0 };
uint8_t MAXDEPTH = 7;
uint8_t WIDTH = 5;
Board local_board;
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

extern uint8_t poolcnt;
extern POOL get_pool(POOL *PieceOnBoard, uint8_t localRound, Board *local_board);
extern int16_t evaluate(uint8_t color, POOL *pool, Board *local_board);
extern uint8_t check_win(disvec* vec, uint8_t color);
extern uint8_t check(Point p, uint8_t color, Board *local_board);
extern disvec getvec(Point p, uint8_t color, Board* local_board);
extern void qsort_pool(uint8_t L, uint8_t R, InfoPOOL* r, uint8_t color, Board* local_board);
extern InfoPOOL get_info(POOL* pool, uint8_t color, Board* local_board);
extern void ssort_pool(InfoPOOL* r, uint8_t color, uint8_t len, uint8_t cnt, Board* local_board);
extern inline int16_t get_value(Point p, uint8_t color, Board* local_board);
extern POOL get_seq_kill(POOL* eva_pool, uint8_t color, uint8_t localRound, Board* local_board);
extern distance id2dis(Point p, uint8_t color, Board* local_board);
extern inline Point move(Point p, uint8_t i, uint8_t j);
extern uint8_t verify_location(Point p);
extern HASH calc_hash(Point p, HASH origin_hash, uint8_t color);
extern uint8_t iskill(Point p, uint8_t color, Board* local_board);
extern void print_board(Board* board);
int16_t search(uint8_t ab, int16_t AB, uint8_t depth, HASH srchash);
Point Preturn = { 7,7 };
POOL local_PieceOnBoard;
uint8_t local_Round;
Board local_board;
uint16_t state_hash = 0;
HASH board_hash = {0,0};
Point KillReturn;
uint8_t SCOPE;
Point BlockReturn = { 7,7 };
typedef struct {
	int16_t value[15][15];
}Value_Table;

Value_Table local_value[2] = { 0 };


inline int16_t get_value(Point p, uint8_t color, Board* local_board) {
	disvec vec = getvec(p, color, local_board);
	int16_t win_value = 0;
	int16_t value = 0;
	if (check_ban(&vec, color))return 0;
	if (win_value = check_win(&vec, color)) {
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
	/*
	value += vec.form[0] < 5 ? 1 << ((5 - vec.form[0])) : 0;
	value += vec.form[1] < 4 ? 1 << ((4 - vec.form[1]) * 2) : 0;
	value += vec.form[2] < 6 ? 1 << ((6 - vec.form[2]) * 2) : 0;
	if (color) {
		value += vec.form[3] < 7 ? 1 << ((7 - vec.form[3]) * 2) : 0;
		value += vec.form[4] < 5 ? 1 << ((5 - vec.form[4]) * 2) : 0;
	}
	*/
	return value;
}

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
	while ((*opinfo_pool).info[i].value > (INT16_MAX >> 1)) {
		tmp_left = Block(*(*opinfo_pool).info[i].point, color, &local_board);
		if (tmp_left > left_max) {
			left_max = tmp_left;
			bestP = BlockReturn;
		}
		i++;
	}
	BlockReturn = bestP;
}

/*
Point block_kill(Point kill, uint8_t color, Board* local_board) {
	distance dis = id2dis(kill, color, local_board);
	uint8_t i = 0;
	while (!(dis.distance[0][i] == 1 || dis.distance[1][i] == 1)) i++;
	uint8_t flag[2] = { 0 };
	Point tmpp[2] = { kill,kill };
	for (uint8_t j = 0; j < 2; j++) {
		while ((*local_board).location[tmpp[j].x][tmpp[j].y] == color) {
			tmpp[j] = move(tmpp[j], i, j);
			if (!verify_location(tmpp[j]))break;
			if ((*local_board).location[tmpp[j].x][tmpp[j].y] == !color)break;
			if ((*local_board).location[tmpp[j].x][tmpp[j].y] > 2)flag[j] = 1;
		}
	}
	if (flag[0] && flag[1]) 
		return local_value[color].value[tmpp[0].x][tmpp[0].y] > local_value[color].value[tmpp[1].x][tmpp[1].y] ? 
		tmpp[0] : tmpp[1];
	else return flag[0] ? tmpp[0] : tmpp[1];
}
*/

int16_t search_seq(uint8_t color, HASH srchash, int8_t depth) {
	//Get the points that makes a kill;
	int16_t local_max = 1;
	POOL eva_pool = get_pool(&local_PieceOnBoard, local_Round, &local_board);
	POOL kill_pool = get_seq_kill(&eva_pool, color, local_Round, &local_board);
	if (!depth) {
		//KillReturn = kill_pool.record[0];
		return INT16_MAX >> 6;
	}
	uint8_t cnt = poolcnt;
	uint16_t flag = 0;
	if (cnt == 0)
		return 0;
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
		if (KILL_hash[newhash.hash[0]][newhash.hash[1]]) {
			local_Round--;
			local_board.location[tmpp.x][tmpp.y] = empty.location[tmpp.x][tmpp.y];
			update_value_table(tmpp);
			return KILL_hash[newhash.hash[0]][newhash.hash[1]] - 1;
		}
		
		/*
		//Go over all the blocks;
		POOL killed_pool = get_pool(&local_PieceOnBoard, local_Round, &local_board);
		InfoPOOL block_info_pool = get_info(&killed_pool, color, &local_board);
		uint8_t blockcnt = poolcnt;
		ssort_pool(&block_info_pool, color, 5, blockcnt, &local_board);
		//Check if the op will win quicker.
		InfoPOOL quicker_info_pool = get_info(&killed_pool, !color, &local_board);
		uint8_t quickercnt = poolcnt;
		ssort_pool(&quicker_info_pool, !color, 1, quickercnt, &local_board);
		if (quicker_info_pool.info[0].value > block_info_pool.info[0].value) {
			local_Round--;
			local_board.location[tmpp.x][tmpp.y] = empty.location[tmpp.x][tmpp.y];
			KILL_hash[newhash.hash[0]][newhash.hash[1]] = 1;
			return 0;
		}
		//Point by Point check.
		uint8_t b = 0;
		uint8_t tmp_max_steps = 0;
		uint8_t steps = 0;
		while (block_info_pool.info[b].value > INT16_MAX >> 1) {
			//get_best_block(&block_info_pool, !color, &local_board);
			Point blockp = *block_info_pool.info[b++].point;
			local_board.location[blockp.x][blockp.y] = !color;
			local_PieceOnBoard.record[local_Round++] = blockp;
			flag = search_seq(color, calc_hash(blockp, newhash, !color), depth - 1);
			local_Round--;
			local_board.location[blockp.x][blockp.y] = empty.location[blockp.x][blockp.y];
			//Check if will be interupted by something else.
			vec = getvec(blockp, !color, &local_board);
			if (check_win(&vec, !color)) {
				flag = 0;
			}
			KILL_hash[newhash.hash[0]][newhash.hash[1]] = max(KILL_hash[newhash.hash[0]][newhash.hash[1]],flag);
			//If there is one way to make the kill not work, then ignore the else.
			if (flag == 0)break;
		}
		*/
		POOL killed_pool = get_pool(&local_PieceOnBoard, local_Round, &local_board);
		InfoPOOL block_info_pool = get_info(&killed_pool, color, &local_board);
		uint8_t blockcnt = poolcnt;
		ssort_pool(&block_info_pool, color, 5, blockcnt, &local_board);
		get_best_block(&block_info_pool, !color, &board);
		Point blockp = BlockReturn;
		vec = getvec(blockp, !color, &local_board);
		if (check_win(&vec, !color)) {
			flag = 0;
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
			KILL_hash[newhash.hash[0]][newhash.hash[1]] = flag + 1;
		}
		local_Round--;
		local_board.location[tmpp.x][tmpp.y] = empty.location[tmpp.x][tmpp.y];
		update_value_table(tmpp);
		if (flag > local_max) {
			local_max = flag;
			KillReturn = tmpp;
		}
	}
	return local_max - 1;
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
		if (hash[newhash.hash[0]][newhash.hash[1]]) {
			local_Round--;
			local_board.location[tmpp.x][tmpp.y] = empty.location[tmpp.x][tmpp.y];
			update_value_table(tmpp);
			return hash[newhash.hash[0]][newhash.hash[1]];
		}
		//Get to an end
		if (0 == depth) {
			POOL value_pool = get_pool(&local_PieceOnBoard, local_Round, &local_board);
			uint8_t value_cnt = poolcnt;
			InfoPOOL my_value_info_pool = get_info(&value_pool, (!ab) ^ player, &local_board);
			ssort_pool(&my_value_info_pool, (!ab) ^ player, 3, cnt, &local_board);
			InfoPOOL op_value_info_pool = get_info(&value_pool, ab ^ player, &local_board);
			ssort_pool(&op_value_info_pool, ab ^ player, 3, cnt, &local_board);
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
				tmp_value /= 2 * poolcnt;
				if (tmp_value < INT16_MAX >> 1) {
					int16_t exist_value = 0;
					for (uint8_t i = 0; i < local_Round; i++) {
						Point tmpp = local_PieceOnBoard.record[i];
						exist_value += local_value[i % 2].value[tmpp.x][tmpp.y] * ((player == i % 2) ? 1 : -1);//get_value(local_PieceOnBoard.record[i], i % 2, &local_board) * (i % 2 == ((!ab) ^ player)) ? 1 : -1;
					}
					tmp_value += exist_value;
				}
			}
			//tmp_value = evaluate(player, &value_pool, &local_board);

			//tmp_value += (*info_pool).info[i].value;
		}
		//Search deeper
		else {
			tmp_value = search(!ab, localAB, depth - 1, newhash);
		}
		//update hash;
		hash[newhash.hash[0]][newhash.hash[1]] = tmp_value;
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

int16_t Precalc(InfoPOOL* myinfo_pool, InfoPOOL* opinfo_pool, uint8_t depth, uint8_t ab, int16_t AB, HASH srchash, uint8_t cnt) {
	if ((*myinfo_pool).info[0].value > (INT16_MAX >> 1)) {
		if ((*myinfo_pool).info[0].value >= (*opinfo_pool).info[0].value) {
			if (depth == MAXDEPTH) {
				Preturn = *(*myinfo_pool).info[0].point;
			}
			return (*myinfo_pool).info[0].value * (ab ? 1 : -1);
		}
		else {
			if (depth == MAXDEPTH) {
				get_best_block(&(*opinfo_pool), (!ab) ^ player, &local_board);
				return (*opinfo_pool).info[0].value * (ab ? 1 : -1);
			}
			int32_t extreme = ab ? INT16_MIN : INT16_MAX;
			return ABsearch(&opinfo_pool, &extreme, ab, AB, depth, srchash, cnt);
		}
	}
	else if ((*opinfo_pool).info[0].value > (INT16_MAX >> 1)) {
		if (depth == MAXDEPTH) {
			get_best_block(&(*opinfo_pool), (!ab) ^ player, &local_board);
			return (*opinfo_pool).info[0].value * (ab ? 1 : -1);
		}
		int32_t extreme = ab ? INT16_MIN : INT16_MAX;
		return ABsearch(&opinfo_pool, &extreme, ab, AB, depth, srchash, cnt);
	}
	return 0;
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
	/*
	if (depth == MAXDEPTH) {
		tmp_value = Precalc(&myinfo_pool, &opinfo_pool, depth, ab, AB, srchash, cnt);
	}
	*/
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
	if (opinfo_pool.info[0].value < myinfo_pool.info[0].value && myinfo_pool.info[3].value > (INT16_MAX >> 10)) {
		if (depth == MAXDEPTH) {
			if (tmp_value = search_seq(player, srchash, KILL_DEPTH)) {
				if (depth == MAXDEPTH) Preturn = KillReturn;
				return tmp_value;
			}
		}
		if (depth == MAXDEPTH - 1) {
			if (tmp_value = search_seq(!player, srchash, KILL_DEPTH))
				return -tmp_value;
		}
	}
	int32_t extreme = ab ? INT16_MIN : INT16_MAX;
	int32_t op_search = ABsearch(&opinfo_pool, &extreme, ab, AB, depth, srchash, cnt);
	int32_t my_search = ABsearch(&myinfo_pool, &extreme, ab, AB, depth, srchash, cnt);
	tmp_value = ab ? max(op_search, my_search) : (min(op_search, my_search));
	//Evaluate the board;
	return tmp_value;
}

Point search_point(Point last) {
	local_board = board;
	local_PieceOnBoard = PieceOnBoard;
	local_Round = Round;
	memset(hash, 0, sizeof(int16_t) * 4096 * 4096);
	memset(KILL_hash, 0, sizeof(int16_t) * 4096 * 4096);
	for (uint8_t i = 0; i < Round; i++) {
		update_value_table(PieceOnBoard.record[i]);
	}
	if (Round == 0) {
		Point center = { 7,7 };
		board_hash = calc_hash(center, board_hash, !player);
		return center;
	}
	board_hash = calc_hash(last, board_hash, !player);
	if (Round < 4)SCOPE = 1;
	else SCOPE = 2;
		MAXDEPTH = 3;
		WIDTH = 20;
	search(1, INT16_MIN, MAXDEPTH, board_hash);
	board_hash = calc_hash(Preturn, board_hash, player);
	local_board.location[Preturn.x][Preturn.y] = player;
	update_value_table(Preturn);
	return Preturn;
}
