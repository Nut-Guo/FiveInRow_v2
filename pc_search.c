#include "five_global.h"
#include "five_type.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
	uint8_t form[6];
}Disvec;
typedef struct {
	Point* point;
	int16_t value;
}Point_Info;
typedef struct {
	Point_Info info[255];
}Info_Pool;
typedef struct {
	uint8_t distance[2][4];
}distance;
typedef struct {
	uint16_t hash[2];
}HASH;
typedef struct {
	int16_t value[15][15];
}Value_Table;


extern inline Pool get_pool(Pool *PieceOnBoard, uint8_t local_Round, Board *local_board);
extern inline uint8_t check_win(Disvec* vec, uint8_t color);
extern inline Disvec getvec(Point p, uint8_t color, Board* local_board);
extern inline Info_Pool get_info(Pool* pool, uint8_t color, Board* local_board);
extern inline void ssort_pool(Info_Pool* r, uint8_t color, uint8_t len, uint8_t cnt, Board* local_board);
extern inline int16_t get_value(Point p, uint8_t color, Board* local_board);
extern inline Pool get_seq_kill(Pool* eva_pool, uint8_t color, uint8_t localRound, Board* local_board);
extern inline void move(Point* p, uint8_t i, uint8_t j);
extern inline uint8_t verify_location(Point p);
extern inline HASH calc_hash(Point p, HASH origin_hash, uint8_t color);
extern inline uint8_t iskill(Point p, uint8_t color, Board* local_board);
extern inline uint8_t check_ban(Disvec* vec, uint8_t color);
int16_t search(uint8_t ab, int16_t AB, uint8_t depth, HASH srchash);

extern uint8_t poolcnt;
extern Board surroundings;
Value_Table local_value[2] = { 0 };
Point Preturn = { 7,7 };
Point BlockReturn = { 7,7 };
Point KillReturn = { 7,7 };
Pool local_PieceOnBoard;
uint8_t local_Round = 0;
Board local_board;
int16_t hash[4096][4096] = { 0 };
uint16_t check_hash[4096][4096] = { 0 };
uint16_t check_kill_hash[4096][4096] = { 0 };
int16_t KILL_hash[4096][4096] = { 0 };
uint8_t MAXDEPTH = 7;
uint8_t WIDTH = 5;
uint8_t SCOPE = 2;
uint8_t KILL_DEPTH = 8;
uint32_t PC_MAXDEPTH[2] = { 3, 3 };
Pool eva_pool;

//Matain the value table;
void update_value_table(Point p, Board* local_surroundings) {
	local_value[0].value[p.x][p.y] = get_value(p, 0, &local_board);
	local_value[1].value[p.x][p.y] = get_value(p, 1, &local_board);
	for (uint8_t i = 0; i < 4; i++) {
		for (uint8_t j = 0; j < 2; j++) {
			Point tmpp = p;
			for (uint8_t k = 0; k < 4; k++) {
				move(&tmpp, i, j);
				if (!verify_location(tmpp))break; 
				if(!local_surroundings->location[tmpp.x][tmpp.y] && k > 1 && !local_board.location[tmpp.x][tmpp.y]>3) continue;
				local_value[0].value[tmpp.x][tmpp.y] = get_value(tmpp, 0, &local_board);
				local_value[1].value[tmpp.x][tmpp.y] = get_value(tmpp, 1, &local_board);
			}
		}
	}
}

int16_t Block(Point p, uint8_t color, Board* local_board) {
	//Check if after block, there is still a kill; 
	(*local_board).location[p.x][p.y] = color;
	local_PieceOnBoard.record[local_Round++] = p;
	//Get the new pool;
	Pool eva_pool = get_pool(&local_PieceOnBoard, local_Round, local_board);
	uint8_t cnt = poolcnt;
	update_value_table(p, &surroundings);
	Info_Pool opinfo_pool = get_info(&eva_pool, !color, local_board);
	ssort_pool(&opinfo_pool, !color, 5, cnt, local_board);
	local_Round--;
	(*local_board).location[p.x][p.y] = empty.location[p.x][p.y];
	update_value_table(p, &surroundings);
	//Test thenew pool;
	uint8_t left = 0;
	Board local_surroundings = surroundings;
	while (opinfo_pool.info[left].value > (INT16_MAX >> 10)) {
		Point tryp = *opinfo_pool.info[left].point;
		(*local_board).location[tryp.x][tryp.y] = color;
		local_PieceOnBoard.record[local_Round++] = tryp;
		update_value_table(tryp, &local_surroundings);
		// Get the state after the try.
		Pool deeper_pool = get_pool(&local_PieceOnBoard, local_Round, local_board);
		uint8_t deeper_cnt = poolcnt;
		Info_Pool deeper_opinfo_pool = get_info(&deeper_pool, !color, local_board);
		ssort_pool(&deeper_opinfo_pool, !color, min(WIDTH, cnt - 1), deeper_cnt, local_board);
		local_Round--;
		(*local_board).location[tryp.x][tryp.y] = empty.location[p.x][p.y];
		update_value_table(tryp, &local_surroundings);
		// If this block makes a kill itself or it dekill the original one, keep it.
		if (local_value[color].value[tryp.x][tryp.y] > deeper_opinfo_pool.info[0].value 
			|| deeper_opinfo_pool.info[0].value < (INT16_MAX >> 1)) {
			BlockReturn = tryp;
			return local_value[color].value[tryp.x][tryp.y];
		}
		left++;
	}
	BlockReturn = p;
	if (!left) {
		return local_value[color].value[p.x][p.y];
	}
	else {
		return 0;
	}
}

void get_best_block(Info_Pool *opinfo_pool, uint8_t color, Board *board) {
	uint8_t imax = 0;
	uint8_t i = 0;
	int16_t left_max = 1;
	int16_t tmp_left = 0;
	Point bestP = *(*opinfo_pool).info[i].point;
	while ((*opinfo_pool).info[i].value > (INT16_MAX >> 1)) {
		Point tmpp = *(*opinfo_pool).info[i].point;
		tmp_left = Block(tmpp, color, &local_board);
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
	Pool value_pool = get_pool(&local_PieceOnBoard, local_Round, &local_board);
	uint8_t value_cnt = poolcnt;
	Info_Pool my_value_info_pool = get_info(&value_pool,color, &local_board);
	ssort_pool(&my_value_info_pool, color, 3, value_cnt, &local_board);
	Info_Pool op_value_info_pool = get_info(&value_pool, !color, &local_board);
	ssort_pool(&op_value_info_pool, !color, 3, value_cnt, &local_board);
	if (op_value_info_pool.info[0].value > INT16_MAX >> 1 &&
		op_value_info_pool.info[0].value > my_value_info_pool.info[0].value)
		tmp_value =  -op_value_info_pool.info[0].value;
	else if ((my_value_info_pool.info[2].value) > INT16_MAX >> 1) {
		tmp_value = my_value_info_pool.info[2].value;
	}
	else {
		for (uint8_t j = 2; j < poolcnt; j++) {
			tmp_value += my_value_info_pool.info[j].value;
			tmp_value -= op_value_info_pool.info[j].value;
		}
		tmp_value /= poolcnt + 1;
		if (tmp_value < INT16_MAX >> 1) {
			int16_t exist_value = 0;
			for (uint8_t i = 0; i < local_Round; i++) {
				Point tmpp = local_PieceOnBoard.record[i];
				exist_value += local_value[i % 2].value[tmpp.x][tmpp.y] * ((color == i % 2) ? 1 : -1);
			}
			tmp_value += exist_value / local_Round;
		}
	}
	return tmp_value;
}

/*
	Search the sequence kill to win the game.
*/
int16_t search_seq(uint8_t color, HASH srchash, int8_t depth) {
	//Get the points that makes a kill;
	int16_t local_max =0;
	Pool eva_pool = get_pool(&local_PieceOnBoard, local_Round, &local_board);
	Pool kill_pool = get_seq_kill(&eva_pool, color, local_Round, &local_board);
	if (!depth) {
		return evaluate_the_board(color);
	}
	uint8_t cnt = poolcnt;
	int16_t flag = 0;
	if (cnt == 0)
		return evaluate_the_board(color);
	for (uint8_t i = 0; i < cnt; i++) {
		Point tmpp = kill_pool.record[i];
		Disvec vec = getvec(tmpp, color, &local_board);
		if (check_win(&vec, color)) {
			KillReturn = tmpp;
			return local_value[color].value[tmpp.x][tmpp.y];
		}
		local_board.location[tmpp.x][tmpp.y] = color;
		local_PieceOnBoard.record[local_Round++] = tmpp;
		HASH newhash = calc_hash(tmpp, srchash, color);
		Pool killed_pool = get_pool(&local_PieceOnBoard, local_Round, &local_board);
		if (check_kill_hash[newhash.hash[0]][newhash.hash[1]] == (srchash.hash[0] ^ srchash.hash[1])) {
			local_Round--;
			local_board.location[tmpp.x][tmpp.y] = empty.location[tmpp.x][tmpp.y];
			return KILL_hash[newhash.hash[0]][newhash.hash[1]];
		}
		Board local_surroundings = surroundings;
		update_value_table(tmpp, &local_surroundings);
		Info_Pool block_info_pool = get_info(&killed_pool, color, &local_board);
		uint8_t blockcnt = poolcnt;
		ssort_pool(&block_info_pool, color, 5, blockcnt, &local_board);
		get_best_block(&block_info_pool, !color, &local_board);
		Point blockp = BlockReturn;
		vec = getvec(blockp, !color, &local_board);
		if (check_win(&vec, !color)||(iskill(blockp,!color,&local_board))) {
			flag = 0;
			check_kill_hash[newhash.hash[0]][newhash.hash[1]] = srchash.hash[0] ^ srchash.hash[1];
			KILL_hash[newhash.hash[0]][newhash.hash[1]] = -local_value[!color].value[blockp.x][blockp.y];
		}
		else {
			local_board.location[blockp.x][blockp.y] = !color;
			local_PieceOnBoard.record[local_Round++] = blockp;
			update_value_table(blockp, &local_surroundings);
			flag = search_seq(color, calc_hash(blockp, newhash, !color), depth - 1);
			local_Round--;
			local_board.location[blockp.x][blockp.y] = empty.location[blockp.x][blockp.y];
			update_value_table(blockp, &local_surroundings);
			check_kill_hash[newhash.hash[0]][newhash.hash[1]] = srchash.hash[0] ^ srchash.hash[1];
			KILL_hash[newhash.hash[0]][newhash.hash[1]] = flag;
		}
		local_Round--;
		local_board.location[tmpp.x][tmpp.y] = empty.location[tmpp.x][tmpp.y];
		update_value_table(tmpp, &local_surroundings);
		if (flag > local_max) {
			local_max = flag;
			KillReturn = tmpp;
			if (flag > INT16_MAX >> 1)
				return flag;
		}
	}
	return local_max;
}

int16_t ABsearch(Info_Pool* info_pool, int32_t* extreme, uint8_t ab, int16_t AB, uint8_t depth, HASH srchash, uint8_t cnt, Board* local_surroundings) {
	int16_t localAB = ab ? INT16_MIN : INT16_MAX;
	Point tmpp;
	for (uint8_t i = 0; i < min(WIDTH, cnt); i++) {
		if ((*info_pool).info[0].value > INT16_MAX >> 1 && (*info_pool).info[i].value < INT16_MAX >> 1) break;
		int16_t tmp_value = 0;
		tmpp = *(*info_pool).info[i].point;
		/*
		Disvec vec = getvec(tmpp, (!ab) ^ player, &local_board);
		if(check_ban(&vec, (!ab) ^ player)) continue;
		*/
		if (!local_value[(!ab) ^ player].value[tmpp.x][tmpp.y]) continue;
		local_board.location[tmpp.x][tmpp.y] = (!ab) ^ player;
		local_PieceOnBoard.record[local_Round++] = tmpp;
		update_value_table(tmpp, local_surroundings);
		//Chech hash
		HASH newhash = calc_hash(tmpp, srchash, (!ab) ^ player);
		if (check_hash[newhash.hash[0]][newhash.hash[1]] == (srchash.hash[0] ^ srchash.hash[1])){
			local_Round--;
			local_board.location[tmpp.x][tmpp.y] = empty.location[tmpp.x][tmpp.y];
			update_value_table(tmpp, local_surroundings);
			return hash[newhash.hash[0]][newhash.hash[1]];
		}
		//Get to an end
		if (0 == depth) {
			tmp_value = -evaluate_the_board((!ab) ^ player);
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
		update_value_table(tmpp, local_surroundings);
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
	Pool local_eva_pool = get_pool(&local_PieceOnBoard, local_Round, &local_board);
	uint8_t cnt = poolcnt;
	Board local_surroundings = surroundings;
	Info_Pool myinfo_pool = get_info(&local_eva_pool, (!ab) ^ player, &local_board);
	ssort_pool(&myinfo_pool, (!ab) ^ player, min(WIDTH,cnt-1), cnt, &local_board);
	Info_Pool opinfo_pool = get_info(&local_eva_pool, !((!ab) ^ player), &local_board);
	ssort_pool(&opinfo_pool, !((!ab) ^ player), min(WIDTH,cnt-1), cnt, &local_board);
	int16_t tmp_value = 0;
	if (depth == MAXDEPTH) Preturn = *opinfo_pool.info[0].point;
	//Predecide if something trivial;
	if ((myinfo_pool).info[0].value > (INT16_MAX >> 1)) {
		if ((myinfo_pool).info[0].value >= (opinfo_pool).info[0].value) {
			if (depth == MAXDEPTH) {
				Preturn = *myinfo_pool.info[0].point;
			}
			return myinfo_pool.info[0].value * (ab ? 1 : -1);
		}
	}
	int32_t extreme = ab ? INT16_MIN : INT16_MAX;
	//If not threatened, get a tragedy;
	if (opinfo_pool.info[0].value <= myinfo_pool.info[0].value && myinfo_pool.info[7].value > (INT16_MAX >> 10)) {
		if (depth == 1/*MAXDEPTH*/) {
			tmp_value = search_seq(player, srchash, KILL_DEPTH);
			//Preturn = KillReturn;
			extreme = tmp_value;
			if (tmp_value > INT16_MAX >> 1)
				return tmp_value;
		}
		if (depth == 0/*MAXDEPTH - 1*/) {
			tmp_value = search_seq(!player, srchash, KILL_DEPTH);
			extreme = -tmp_value;
			if (tmp_value > INT16_MAX >> 1)
				return -tmp_value;
		}
	}
	if ((opinfo_pool).info[0].value > (INT16_MAX >> 1)) {
		if ((opinfo_pool).info[1].value < (INT16_MAX)) {
			if (depth == MAXDEPTH) {
				get_best_block(&opinfo_pool, (!ab) ^ player, &local_board);
				Preturn = BlockReturn;
				return 1;
			}
		}
		else {
			return ABsearch(&opinfo_pool, &extreme, ab, AB, depth, srchash, cnt, &local_surroundings);
		}
	}
	int32_t op_search = ABsearch(&opinfo_pool, &extreme, ab, AB, depth, srchash, cnt, &local_surroundings);
	int32_t my_search = ABsearch(&myinfo_pool, &extreme, ab, AB, depth, srchash, cnt, &local_surroundings);
	tmp_value = ab ? max(op_search, my_search) : (min(op_search, my_search));
	return tmp_value;
}

Point search_point(Point last) {
	local_board = board;
	local_PieceOnBoard = PieceOnBoard;
	local_Round = Round;
	memset(check_hash, 0, sizeof(uint16_t) * 4096 * 4096);
	memset(check_kill_hash, 0, sizeof(uint16_t) * 4096 * 4096);
	HASH init_hash = { 0,0 };
	get_pool(&local_PieceOnBoard, local_Round, &local_board);
	Board local_surroundings = surroundings;
	for (uint8_t i = 0; i < Round; i++) {
		update_value_table(local_PieceOnBoard.record[i], &local_surroundings);
		init_hash = calc_hash(PieceOnBoard.record[i], init_hash, i % 2);
	}
	if (Round == 0) {
		Point center = { 7,7 };
		return center;
	}
	if (Round < 6) {
		SCOPE = 1;
		MAXDEPTH = 3;
		WIDTH = 20;
	}
	else {
		SCOPE = 2;
		MAXDEPTH = 3;//PC_MAXDEPTH[player];
		WIDTH = 20;
	}
	KILL_DEPTH = 4;
	search(1, INT16_MIN, MAXDEPTH, init_hash);
	local_board.location[Preturn.x][Preturn.y] = player;
	return Preturn;
}
