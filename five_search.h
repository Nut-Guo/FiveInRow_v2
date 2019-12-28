#pragma once
/*
	Types and external functions to use in the pc_search.c
*/
#include "five_type.h"
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

extern inline Pool get_pool(Pool* PieceOnBoard, uint8_t local_Round, Board* local_board);

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