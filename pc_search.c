#include "five_global.h"
#include "five_type.h"
#include "five_search.h"
#include <stdlib.h>
#include <string.h>
#include <windows.h>
int16_t search(uint8_t ab, int16_t AB, uint8_t depth, HASH srchash);

/*
	Useful intermediate variables.
*/
extern uint8_t poolcnt;
extern Board surroundings;

/*
	The value table of black and white.
*/
Value_Table local_value[2] = { 0 };

/*
	Pass the return point of search.
*/
Point Preturn = { 7,7 };

/*
	Pass the return point of block.
*/
uint8_t blocked = 0;

/*
	Pass the return point of kill.
*/
Point KillReturn = { 7,7 };
uint8_t KILLED_DEPTH;

/*
	Copies of global variables.
*/
Pool local_PieceOnBoard;
uint8_t local_Round = 0;
Board local_board;

/*
	Hash table and their checks.
*/
int16_t hash[4096][4096] = { 0 };
uint16_t check_hash[4096][4096] = { 0 };
int16_t KILL_hash[4096][4096] = { 0 };
uint16_t check_kill_hash[4096][4096] = { 0 };

/*
	Basic configuration of search.
*/
uint8_t MAXDEPTH = 7;
uint8_t WIDTH = 5;
uint8_t SCOPE = 2;
uint8_t KILL_DEPTH = 3;
uint32_t PC_MAXDEPTH[2] = { 3, 3 };
extern int32_t global_value[15][15];

/*
	Update the value table.
*/
inline void update_value_table(Point p, Board* local_surroundings) {
	local_value[0].value[p.x][p.y] = get_value(p, 0, &local_board);
	local_value[1].value[p.x][p.y] = get_value(p, 1, &local_board);
	for (uint8_t i = 0; i < 4; i++) {
		for (uint8_t j = 0; j < 2; j++) {
			Point tmpp = p;
			for (uint8_t k = 0; k < 4; k++) {
				move(&tmpp, i, j);
				if (!verify_location(tmpp))break; 
				if(!local_surroundings->location[tmpp.x][tmpp.y]//Is not surroundings
					&& k > 1 //Beyond scope
					&& local_board.location[tmpp.x][tmpp.y]>3//Is not occupied.
					) continue;
				if (local_board.location[tmpp.x][tmpp.y] > 3) {
					local_value[0].value[tmpp.x][tmpp.y] = get_value(tmpp, 0, &local_board);
					local_value[1].value[tmpp.x][tmpp.y] = get_value(tmpp, 1, &local_board);
				}
				else {
					get_on_board_value(tmpp, local_board.location[tmpp.x][tmpp.y], &local_board);
				}
			}
		}
	}
}

/*
	Evaluate the board.
*/
int16_t evaluate_the_board(uint8_t color) {
	int16_t tmp_value = 0;
	Pool value_pool = get_pool(&local_PieceOnBoard, local_Round, &local_board);
	uint8_t value_cnt = poolcnt;
	uint8_t counter = 0;
	int16_t mymax = INT16_MIN;
	int16_t opmax = INT16_MIN;
	for (uint8_t i = 0; i < value_cnt; i++) {
		Point tmpp = value_pool.record[i];
		if (local_value[!color].value[tmpp.x][tmpp.y] > INT16_MAX >> 1) {
			opmax = opmax < local_value[!color].value[tmpp.x][tmpp.y] ? local_value[!color].value[tmpp.x][tmpp.y] : opmax;
		}
		if ((local_value[color].value[tmpp.x][tmpp.y] > INT16_MAX >> 1)) {
			counter++;
			mymax = mymax < local_value[color].value[tmpp.x][tmpp.y] ? local_value[color].value[tmpp.x][tmpp.y] : mymax;
			tmp_value -= local_value[!color].value[tmpp.x][tmpp.y];
			continue;
		}
		tmp_value += local_value[color].value[tmpp.x][tmpp.y];
		tmp_value -= local_value[!color].value[tmpp.x][tmpp.y];
	}
	if (opmax >= mymax && opmax > INT16_MAX >> 1) 
		return -opmax;
	if (counter > 1) {
		return mymax;
	}
	if (tmp_value < INT16_MAX >> 1) {
		int16_t exist_value = 0;
		for (uint8_t i = 0; i < local_Round; i++) {
			Point tmpp = local_PieceOnBoard.record[i];
			exist_value += global_value[tmpp.x][tmpp.y] * ((color == i % 2) ? 1 : -1);//local_value[i % 2].value[tmpp.x][tmpp.y] * ((color == i % 2) ? 1 : -1);
		}
		tmp_value += exist_value;
	}
	return tmp_value;
}
extern void print_board(Board* board);
/*
	Get the best block as next step.
*/
Point get_best_block(Board* local_surroundings, Info_Pool* opinfo_pool, uint8_t blockcnt, uint8_t color, Board* board) {
	uint8_t i = 0;
	int16_t tmp_max = INT16_MIN;
	int16_t tmp_left = 0;
	Point bestP = *(*opinfo_pool).info[i].point;
	uint8_t kill_exist = 0;
	while ((*opinfo_pool).info[i++].value > (INT16_MAX >> 10) && i < blockcnt) {
		Point tmpp = *(*opinfo_pool).info[i-1].point;
		local_board.location[tmpp.x][tmpp.y] = color;
		local_PieceOnBoard.record[local_Round++] = tmpp;
		//Get the new pool;
		int32_t tmp_value = 0;
		update_value_table(tmpp, &local_surroundings);
		tmp_value = evaluate_the_board(color);
		local_Round--;
		local_board.location[tmpp.x][tmpp.y] = empty.location[tmpp.x][tmpp.y];
		update_value_table(tmpp, &local_surroundings);
		uint8_t tmp_kill = iskill(tmpp, color, &local_board);
		if (tmp_kill && tmp_value > -(INT16_MAX >> 1)) {
			if (!kill_exist){
				kill_exist = tmp_kill;
				bestP = tmpp;
			}
			else if(kill_exist < tmp_kill){
				continue;
			}
		}
		else if (kill_exist) {
			continue;
		}
		else if (tmp_value > tmp_max) {
			tmp_max = tmp_value;
			bestP = tmpp;
		}
	}
	return bestP;
}

/*
	Search the sequence kill to win the game.
*/
int16_t search_seq(uint8_t color, HASH srchash, int8_t depth) {
	//Get the points that makes a kill;
	if (depth < KILLED_DEPTH) return 0;
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
			KILLED_DEPTH = depth;
			return local_value[color].value[tmpp.x][tmpp.y];
		}
	}
	for (uint8_t i = 0; i < cnt; i++) {
		Point tmpp = kill_pool.record[i];
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
		ssort_pool(&block_info_pool, color, WIDTH, blockcnt, &local_board);
		Point blockp = get_best_block(&local_surroundings, &block_info_pool, WIDTH, !color, &local_board);
		Disvec vec = getvec(blockp, !color, &local_board);
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
		if (flag >= local_max) {
			local_max = flag;
			if(flag > INT16_MAX >> 1)
				KillReturn = tmpp;
		}
	}
	return local_max;
}

/*
	Search with alpha-beta cutoffs.
*/
int16_t ABsearch(Info_Pool* info_pool, int32_t* extreme, uint8_t ab, int16_t AB, uint8_t depth, HASH srchash, uint8_t cnt, Board* local_surroundings) {
	int16_t localAB = ab ? INT16_MIN : INT16_MAX;
	Point tmpp;
	for (uint8_t i = 0; i < min(WIDTH, cnt); i++) {
		if ((*info_pool).info[0].value > INT16_MAX >> 1 && (*info_pool).info[i].value < INT16_MAX >> 1) break;
		int16_t tmp_value = 0;
		tmpp = *(*info_pool).info[i].point;
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

/*
	Coordinate multiple search strategies.
*/
int16_t search(uint8_t ab, int16_t AB, uint8_t depth, HASH srchash) {
	Pool local_eva_pool = get_pool(&local_PieceOnBoard, local_Round, &local_board);
	uint8_t cnt = poolcnt;
	Board local_surroundings = surroundings;
	Info_Pool myinfo_pool = get_info(&local_eva_pool, (!ab) ^ player, &local_board);
	ssort_pool(&myinfo_pool, (!ab) ^ player, min(WIDTH,cnt-1), cnt, &local_board);
	Info_Pool opinfo_pool = get_info(&local_eva_pool, ab ^ player, &local_board);
	ssort_pool(&opinfo_pool,ab ^ player, min(WIDTH,cnt-1), cnt, &local_board);
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
	if (opinfo_pool.info[0].value <= myinfo_pool.info[0].value && myinfo_pool.info[5].value > (INT16_MAX >> 10)) {
		if (depth == MAXDEPTH) {
			KILLED_DEPTH = 0;
			tmp_value = search_seq(player, srchash, 8);
			if (tmp_value > INT16_MAX >> 1) {
				Preturn = KillReturn;
				return tmp_value;
			}
		}
		if (depth == 1) {
			KILLED_DEPTH = 0;
			tmp_value = search_seq(player, srchash, 6);
			extreme = tmp_value;
			if (tmp_value > INT16_MAX >> 1)
				return tmp_value;
		}
		if (depth == 0) {
			KILLED_DEPTH = 0;
			tmp_value = search_seq(!player, srchash, 3);
			extreme = -tmp_value;
			if (tmp_value > INT16_MAX >> 1)
				return -tmp_value;
		}
	}
	if ((opinfo_pool).info[0].value > (INT16_MAX >> 1)) {
		if ((opinfo_pool).info[1].value < (INT16_MAX)) {
			if (depth == MAXDEPTH) {
				Preturn = get_best_block(&local_surroundings, &opinfo_pool, WIDTH, (!ab) ^ player, &local_board);
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

/*
	Prepare for the search.
*/
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
		WIDTH = 10;
	}
	else {
		SCOPE = 2;
		MAXDEPTH = 3;//PC_MAXDEPTH[player];
		WIDTH = 8;
	}
	//KILL_DEPTH = 6;
	int16_t result = search(1, INT16_MIN, MAXDEPTH, init_hash);
	if (result <= -(INT16_MAX >> 1)) system("color 4");
	else if (result >= (INT16_MAX >> 1)) system("color 2");
	else system("color 0");
	local_board.location[Preturn.x][Preturn.y] = player;
	return Preturn;
}
