/*
	Get the pieceid which consists of 4 numbers representing 
	the state in the 4 directions.
*/

#include "five_type.h"
#include "five_global.h"

/*
	4 directions.
*/
typedef struct {
	uint16_t id[4];
}pieceid;

/*
	2 kinds of distance: to 4 and to 5;
	4 directions.
*/
typedef struct {
	uint8_t distance[4][2];
}distance;

typedef struct {
	uint8_t form[5];
}disvec;

typedef struct {
	disvec color[2];
}vecpare;

typedef vecpare vecpool[255];

const uint8_t dis2four[1024] = 
{		
	   10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	   10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	   10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	   10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,  3, 10,
		3, 10,  2, 10,  3, 10,  2, 10, 10, 10,  1, 10,  3, 10,  2, 10,  2,
	   10,  1, 10,  2, 10,  1, 10,  1, 10,  0, 10, 10, 10, 10, 10, 10, 10,
	   10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	   10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,  3, 10,  3,  3,  2, 10,
		3,  3,  2, 10,  2,  2,  1, 10,  3,  3,  2, 10,  2,  2,  1, 10,  2,
	   10,  1, 10,  1,  1,  0, 10,  3,  3, 10, 10,  2,  2, 10, 10,  2,  2,
	   10, 10,  1,  1, 10, 10,  2,  2, 10, 10,  1,  1, 10, 10,  1,  1, 10,
	   10,  0,  0, 10, 10, 10, 10,  3, 10,  3, 10,  2, 10,  3, 10,  2, 10,
	   10, 10,  1, 10,  3, 10,  2, 10,  2, 10,  1, 10,  2, 10,  1, 10,  1,
	   10,  0, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	   10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	   10, 10, 10,  3, 10,  3,  3,  2, 10,  3,  3,  2,  3,  2,  2,  1, 10,
		3,  3,  2,  3,  2,  2,  1, 10,  2,  2,  1,  2,  1,  1,  0, 10,  3,
		3,  3,  3,  2,  2, 10, 10,  2,  2,  2,  2,  1,  1, 10, 10,  2,  2,
	   10, 10,  1,  1, 10, 10,  1,  1,  1,  1,  0,  0, 10, 10,  3,  3,  3,
		3,  3, 10,  2, 10,  2,  2,  2,  2, 10, 10,  1, 10,  2,  2,  2,  2,
		2, 10,  1, 10,  1,  1,  1,  1,  1, 10,  0, 10,  2,  2,  2,  2, 10,
	   10, 10, 10,  1,  1,  1,  1, 10, 10, 10, 10,  1,  1,  1,  1, 10, 10,
	   10, 10,  0,  0,  0,  0, 10, 10, 10, 10, 10, 10,  3, 10,  3,  3,  2,
	   10,  3,  3,  2, 10,  2,  2,  1, 10,  3,  3,  2, 10,  2,  2,  1, 10,
		2, 10,  1, 10,  1,  1,  0, 10,  3,  3, 10, 10,  2,  2, 10, 10,  2,
		2, 10, 10,  1,  1, 10, 10,  2,  2, 10, 10,  1,  1, 10, 10,  1,  1,
	   10, 10,  0,  0, 10, 10, 10, 10,  3, 10,  3, 10,  2, 10,  3, 10,  2,
	   10, 10, 10,  1, 10,  3, 10,  2, 10,  2, 10,  1, 10,  2, 10,  1, 10,
		1, 10,  0, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	   10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	   10, 10,
	   10, 10,  3, 10,  3,  3,  2, 10,  3,  3,  2,  3,  2,  2,  1, 10,  3,
		3,  2,  3,  2,  2,  1,  3,  2,  2,  1,  2,  1,  1,  0, 10,  3,  3,
		3,  3,  2,  2,  3,  3,  2,  2,  2,  2,  1,  1, 10, 10,  2,  2,  2,
		2,  1,  1,  2,  2,  1,  1,  1,  1,  0,  0, 10, 10,  3,  3,  3,  3,
		3,  3,  2,  3,  2,  2,  2,  2, 10, 10,  1, 10,  2,  2,  2,  2,  2,
		2,  1,  2,  1,  1,  1,  1,  1, 10,  0, 10,  2,  2,  2,  2, 10, 10,
	   10, 10,  1,  1,  1,  1, 10, 10, 10, 10,  1,  1,  1,  1,  1,  1,  1,
		1,  0,  0,  0,  0, 10, 10, 10, 10,  3,  3,  3,  3,  3,  3,  2,  3,
		3,  3,  2, 10,  2,  2,  1, 10,  2,  2,  2,  2,  2,  2,  1,  2,  2,
	   10,  1, 10,  1,  1,  0, 10,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
	   10, 10,  1,  1, 10, 10,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 10,
	   10,  0,  0, 10, 10,  2,  2,  2,  2,  2,  2,  2,  2,  3, 10,  2, 10,
	   10, 10,  1, 10,  1,  1,  1,  1,  1,  1,  1,  1,  2, 10,  1, 10,  1,
	   10,  0, 10,  1,  1,  1,  1,  1,  1,  1,  1, 10, 10, 10, 10, 10, 10,
	   10, 10,  0,  0,  0,  0,  0,  0,  0,  0, 10, 10, 10, 10, 10, 10, 10,
	   10, 10, 10,  3, 10,  3,  3,  2, 10,  3,  3,  2,  3,  2,  2,  1, 10,
		3,  3,  2,  3,  2,  2,  1, 10,  2,  2,  1,  2,  1,  1,  0, 10,  3,
		3,  3,  3,  2,  2, 10, 10,  2,  2,  2,  2,  1,  1, 10, 10,  2,  2,
	   10, 10,  1,  1, 10, 10,  1,  1,  1,  1,  0,  0, 10, 10,  3,  3,  3,
		3,  3, 10,  2, 10,  2,  2,  2,  2, 10, 10,  1, 10,  2,  2,  2,  2,
		2, 10,  1, 10,  1,  1,  1,  1,  1, 10,  0, 10,  2,  2,  2,  2, 10,
	   10, 10, 10,  1,  1,  1,  1, 10, 10, 10, 10,  1,  1,  1,  1, 10, 10,
	   10, 10,  0,  0,  0,  0, 10, 10, 10, 10, 10, 10,  3, 10,  3,  3,  2,
	   10,  3,  3,  2, 10,  2,  2,  1, 10,  3,  3,  2, 10,  2,  2,  1, 10,
		2, 10,  1, 10,  1,  1,  0, 10,  3,  3, 10, 10,  2,  2, 10, 10,  2,
		2, 10, 10,  1,  1, 10, 10,  2,  2, 10, 10,  1,  1, 10, 10,  1,  1,
	   10, 10,  0,  0, 10, 10, 10, 10,  3, 10,  3, 10,  2, 10,  3, 10,  2,
	   10, 10, 10,  1, 10,  3, 10,  2, 10,  2, 10,  1, 10,  2, 10,  1, 10,
		1, 10,  0, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	   10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	   10, 10
};

const uint8_t dis2five[1024] =
{
		10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	   10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,  4,
		4,  3,  4,  3,  3,  2,  4,  3,  3,  2,  3,  2,  2,  1,  4,  3,  3,
		2,  3,  2,  2,  1,  3,  2,  2,  1,  2,  1,  1,  0, 10,  4,  4,  3,
		4,  3,  3,  2,  4,  3,  3,  2,  3,  2,  2,  1,  4,  3,  3,  2,  3,
		2,  2,  1,  3,  2,  2,  1,  2,  1,  1,  0,  4,  4,  3,  3,  3,  3,
		2,  2,  3,  3,  2,  2,  2,  2,  1,  1,  3,  3,  2,  2,  2,  2,  1,
		1,  2,  2,  1,  1,  1,  1,  0, 10, 10,  4,  4,  3,  4,  3,  3,  2,
		4,  3,  3,  2,  3,  2,  2,  1,  4,  3,  3,  2,  3,  2,  2,  1,  3,
		2,  2,  1,  2,  1,  1,  0,  4,  4,  3,  3,  3,  3,  2,  2,  3,  3,
		2,  2,  2,  2,  1,  1,  3,  3,  2,  2,  2,  2,  1,  1,  2,  2,  1,
		1,  1,  1,  0, 10,  4,  4,  4,  3,  3,  3,  3,  2,  3,  3,  3,  2,
		2,  2,  2,  1,  3,  3,  3,  2,  2,  2,  2,  1,  2,  2,  2,  1,  1,
		1,  1,  0,  3,  3,  3,  3,  2,  2,  2,  2,  2,  2,  2,  2,  1,  1,
		1,  1,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0, 10,
	   10, 10,  4,  4,  3,  4,  3,  3,  2,  4,  3,  3,  2,  3,  2,  2,  1,
		4,  3,  3,  2,  3,  2,  2,  1,  3,  2,  2,  1,  2,  1,  1,  0,  4,
		4,  3,  3,  3,  3,  2,  2,  3,  3,  2,  2,  2,  2,  1,  1,  3,  3,
		2,  2,  2,  2,  1,  1,  2,  2,  1,  1,  1,  1,  0, 10,  4,  4,  4,
		3,  3,  3,  3,  2,  3,  3,  3,  2,  2,  2,  2,  1,  3,  3,  3,  2,
		2,  2,  2,  1,  2,  2,  2,  1,  1,  1,  1,  0,  3,  3,  3,  3,  2,
		2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  2,  2,  2,  2,  1,  1,
		1,  1,  1,  1,  1,  1,  0,  0, 10, 10,  4,  4,  4,  3,  4,  3,  3,
		2,  3,  3,  3,  2,  3,  2,  2,  1,  3,  3,  3,  2,  3,  2,  2,  1,
		2,  2,  2,  1,  2,  1,  1,  0,  3,  3,  3,  3,  3,  3,  2,  2,  2,
		2,  2,  2,  2,  2,  1,  1,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,
		1,  1,  1,  1,  0, 10,  3,  3,  3,  3,  3,  3,  3,  2,  2,  2,  2,
		2,  2,  2,  2,  1,  2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,
		1,  1,  1,  0,  2,  2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,
		1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0, 10, 10,
	   10, 10,
	   10,  4,  4,  3,  4,  3,  3,  2,  4,  3,  3,  2,  3,  2,  2,  1,  4,
		3,  3,  2,  3,  2,  2,  1,  3,  2,  2,  1,  2,  1,  1,  0,  4,  4,
		3,  3,  3,  3,  2,  2,  3,  3,  2,  2,  2,  2,  1,  1,  3,  3,  2,
		2,  2,  2,  1,  1,  2,  2,  1,  1,  1,  1,  0, 10,  4,  4,  4,  3,
		3,  3,  3,  2,  3,  3,  3,  2,  2,  2,  2,  1,  3,  3,  3,  2,  2,
		2,  2,  1,  2,  2,  2,  1,  1,  1,  1,  0,  3,  3,  3,  3,  2,  2,
		2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  2,  2,  2,  2,  1,  1,  1,
		1,  1,  1,  1,  1,  0,  0, 10, 10,  4,  4,  4,  3,  4,  3,  3,  2,
		3,  3,  3,  2,  3,  2,  2,  1,  3,  3,  3,  2,  3,  2,  2,  1,  2,
		2,  2,  1,  2,  1,  1,  0,  3,  3,  3,  3,  3,  3,  2,  2,  2,  2,
		2,  2,  2,  2,  1,  1,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,
		1,  1,  1,  0, 10,  3,  3,  3,  3,  3,  3,  3,  2,  2,  2,  2,  2,
		2,  2,  2,  1,  2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,
		1,  1,  0,  2,  2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,
		1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0, 10, 10, 10,
	   10,  4,  4,  4,  3,  4,  3,  3,  2,  4,  3,  3,  2,  3,  2,  2,  1,
		3,  3,  3,  2,  3,  2,  2,  1,  3,  2,  2,  1,  2,  1,  1,  0,  3,
		3,  3,  3,  3,  3,  2,  2,  3,  3,  2,  2,  2,  2,  1,  1,  2,  2,
		2,  2,  2,  2,  1,  1,  2,  2,  1,  1,  1,  1,  0, 10,  3,  3,  3,
		3,  3,  3,  3,  2,  3,  3,  3,  2,  2,  2,  2,  1,  2,  2,  2,  2,
		2,  2,  2,  1,  2,  2,  2,  1,  1,  1,  1,  0,  2,  2,  2,  2,  2,
		2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
		1,  1,  1,  1,  1,  1,  0,  0, 10, 10,  3,  3,  3,  3,  3,  3,  3,
		2,  3,  3,  3,  2,  3,  2,  2,  1,  2,  2,  2,  2,  2,  2,  2,  1,
		2,  2,  2,  1,  2,  1,  1,  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
		1,  1,  1,  1,  0, 10,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		2,  2,  2,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
		1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
		1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0, 10, 10, 10, 10, 10, 10,
	   10, 10
};

extern uint8_t verify_location(Point p);
// move one step in the specific direction
inline Point move(Point p, uint8_t i, uint8_t j) {
	Point tmpp = { p.x + dx[i] * direction[j], p.y + dy[i] * direction[j] };
	return tmpp;
}

// look up the end in one direction.
Point get_end(Point p, uint8_t i, uint8_t j, uint8_t color, Board local_board) {// look up the end in one direction.
	Point end = p;
	Point tmpp = move(p, i, j);
	uint8_t len = 1;
	while (verify_location(tmpp) && (local_board.location[tmpp.x][tmpp.y] == color || local_board.location[tmpp.x][tmpp.y] > 3) && len < 5) {
		end = tmpp;
		++len;
		tmpp = move(tmpp, i, j);
	}
	return end;
}

inline uint8_t comaprepoint(Point A, Point B) {
	return (A.x == B.x) && (A.y == B.y);
}

// parse the line with the specified ends
uint16_t parse_line(Point start, Point end, uint8_t d, uint8_t color, Board local_board) {
	Point tmpp = start;
	uint16_t id = 1;
	while (!comaprepoint(tmpp, end)) {
		id = (id << 1) + (local_board.location[tmpp.x][tmpp.y] == color);
		tmpp = move(tmpp, d, 1);
	}
	id = (id << 1) + (local_board.location[tmpp.x][tmpp.y] == color);
	return id;
}

//The function that calls the former ones that to get the id.
pieceid lookup(Point p, uint8_t color, Board local_board) {
	uint8_t save = local_board.location[p.x][p.y];
	local_board.location[p.x][p.y] = color;
	pieceid id = {0};
	for (uint8_t i = 0; i < 4; i++) {// look up in the four directions;
		Point end[2] = { get_end(p, i, 0, color, local_board), get_end(p, i, 1, color,local_board) };
		id.id[i] = parse_line(end[0], end[1], i, color,local_board);
	}
	local_board.location[p.x][p.y] = save;
	return id;
}

//Translate id 2 distance
distance id2dis(Point p, uint8_t color, Board local_board) {
	pieceid id = lookup(p, color,local_board);
	distance dis;
	for (uint8_t i = 0; i < 4; i++) {
		dis.distance[i][0] = dis2four[id.id[i]];
		dis.distance[i][1] = dis2five[id.id[i]];
	}
	return dis;
}

inline uint8_t min(uint8_t x, uint8_t y) {
	return x < y ? x : y;
}

disvec getvec(Point p, uint8_t color, Board local_board) {
	distance dis = id2dis(p,color,local_board);
	disvec vec;
	//sort the dis;
	uint8_t mindis40 = 10;
	uint8_t mindis41 = 10;
	uint8_t mindis50 = 10;
	uint8_t mindis51 = 10;
	uint8_t i40 = 4;
	uint8_t i50 = 4;
	for (uint8_t i = 0; i < 4; i++) {
		if (mindis40 >= dis.distance[i][0]) {
			mindis41 = mindis40;
			mindis40 = dis.distance[i][0];
			i40 = i;
		}
		if (mindis50 >= dis.distance[i][1]) {
			mindis51 = mindis50;
			mindis50 = dis.distance[i][1];
			i50 = i;
		}
	}
	//calc double 3
	uint8_t mindisd3 = mindis40 + mindis41 - 2;
	//calc 4 and 3
	uint8_t mindis34 = i40 == i50 ? min(mindis40 + mindis51, mindis41 + mindis50) : mindis40 + mindis50;
	mindis34 = mindis34 - 2;
	//calc double 4
	uint8_t mindisd4 = mindis50 + mindis51 - 2;
	uint8_t form[5] = { mindis50,mindis40,mindisd4,mindis34,mindisd3 };
	for (uint8_t i = 0; i < 5; i++) {
		vec.form[i] = form[i];
	}
	return vec;
}

/*Check if the game should stop*/
uint8_t check(Point p, uint8_t color,Board local_board) {
	disvec mydis = getvec(p, color,local_board);
	if (mydis.form[0] == 0) {
		return 1;
	}
	return 0;
}