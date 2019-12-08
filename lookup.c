/*
	Get the pieceid which consists of 4 numbers representing 
	the state in the 4 directions.
*/

#include "five_type.h"
#include "five_global.h"
#include "five_func.h"

/*
	2 kinds of id: from start to end/from end to start;
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

Point pool[225];
uint8_t psp = 0;

// move one step in the specific direction
inline Point move(Point p, uint8_t i, uint8_t j) {
	Point tmpp = { p.x + dx[i] * direction[j], p.y + dy[i] * direction[j] };
	return tmpp;
}

// look up the end in one direction.
Point get_end(Point p, uint8_t i, uint8_t j, uint8_t color) {// look up the end in one direction.
	/*
	Point end = p;
	Point tmpp = move(p, i, j);
	uint8_t blank = 0;
	while (varify_location(tmpp) && (board[tmpp.x][tmpp.y] == color || (board[tmpp.x][tmpp.y] > 3 && blank < 2))) {
		end = tmpp;
		if (board[tmpp.x][tmpp.y] > 3) blank += 1;
		if (board[tmpp.x][tmpp.y] == color) blank = 0;
		tmpp = move(tmpp, i, j);
	}
	return end;
	*/
	Point end = p;
	Point tmpp = move(p, i, j);
	uint8_t len = 1;
	while (varify_location(tmpp) && (board[tmpp.x][tmpp.y] == color || board[tmpp.x][tmpp.y] > 3) && len < 5) {
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
uint16_t parse_line(Point start, Point end, uint8_t d, uint8_t color) {
	
	/*
	Point tmpp = start;
	uint16_t id = (board[tmpp.x][tmpp.y] == color);
	while (!comaprepoint(tmpp,end)) {
		tmpp = move(tmpp, d, 1);
		id = (id << 1) + (board[tmpp.x][tmpp.y] == color);
	}
	pid[0] = id;
	id = !(board[tmpp.x][tmpp.y] ^ color);
	while (!comaprepoint(tmpp,start)) {
		tmpp = move(tmpp, d, 0);
		id = (id << 1) + (board[tmpp.x][tmpp.y] == color);
	}
	pid[1] = id;
	*/
	Point tmpp = start;
	uint16_t id = 1;
	while (!comaprepoint(tmpp, end)) {
		id = (id << 1) + (board[tmpp.x][tmpp.y] == color);
		tmpp = move(tmpp, d, 1);
	}
	id = (id << 1) + (board[tmpp.x][tmpp.y] == color);
	return id;
}

//The function that calls the former ones that to get the id.
pieceid lookup(Point p, uint8_t color) {
	uint8_t save = board[p.x][p.y];
	board[p.x][p.y] = color;
	pieceid id = {0};
	for (uint8_t i = 0; i < 4; i++) {// look up in the four directions;
		Point end[2] = { get_end(p, i, 0, color), get_end(p, i, 1, color) };
		id.id[i] = parse_line(end[0], end[1], i, color);
	}
	board[p.x][p.y] = save;
	return id;
}

//Translate id 2 distance
distance id2dis(pieceid id) {
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

void insertion_sort(uint8_t arr[], uint8_t len) {
	uint8_t i, j, temp;
	for (i = 1; i < len; i++) {
		temp = arr[i];
		for (j = i; j > 0 && arr[j - 1] > temp; j--)
			arr[j] = arr[j - 1];
		arr[j] = temp;
	}
}

disvec getvec(distance dis) {
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
	uint8_t mindis = 10;
	uint8_t form[5] = { mindis50,mindis40,mindisd4,mindis34,mindisd3 };
	/*
	for (uint8_t j = 0; j < 5; j++) {
		if (form[j] == 0) {
			for (uint8_t i = 0; i < 5; i++) {
				vec.form[i] = form[0];
			}
			return vec;
		}
	}
	*/
	/*
	for (int8_t i = 0; i <5 ; i++) {
		mindis = min(mindis, form[i]);
		if (mindis == 0) {
			i = i;
		}
	}
	*/
	for (uint8_t i = 0; i < 5; i++) {
		vec.form[i] = form[i];
	}
	return vec;
}


/*Check if the game should stop*/
uint8_t check(Point p, uint8_t color) {
	disvec mydis = getvec(id2dis(lookup(p, color)));
	if (mydis.form[0] == 0) {
		return 1;
	}
	return 0;
}

uint8_t smaller(disvec A, disvec B) {
	//insertion_sort(A.form, 5);
	//insertion_sort(B.form, 5);
	if (A.form[0] == 0) return 1;
	if (B.form[0] == 0) return 0;
	for (uint8_t i = 1; i < 5; i++) {
		if (A.form[i] == 0)
			return 1;
		if (B.form[i] == 0)
			return 0;
		if (A.form[i] == B.form[i]) {
			continue;
		}
		else return A.form[i] < B.form[i];
	}
	return 1;
}

uint8_t cross_compare(disvec mydis, disvec opdis) {
	if (mydis.form[0] == 0) return 1;
	else if (opdis.form[0] == 0) return 0;
	else if (mydis.form[1] == 0 || mydis.form[2] == 0 || mydis.form[3] == 0) return 1;
	else if (opdis.form[1] == 0 || opdis.form[2] == 0 || opdis.form[3] == 0) return 0;
	else if (mydis.form[4] == 0) return 1;
	else if (opdis.form[4] == 0) return 0;
	for (uint8_t i = 1; i < 5; i++) {
		if (mydis.form[i] == opdis.form[i]) {
			continue;
		}
		else return mydis.form[i] < opdis.form[i];
	}
	return 1;
}

Point checknull( Point last ) {
	if (board[last.x][last.y] > 3) {
		pool[++psp] = last;
	}
}

Point add2pool(Point last) {
	/*
	if (board[last.x][last.y] > 3) {
		pool[++psp] = last;
	}
	*/
	for (uint8_t i = 0; i < psp + 1; i++) {
		if (pool[i].x == last.x && pool[i].y == last.y) {
			pool[i] = pool[psp--];
		}
	}

	for (uint8_t i = 0; i < 4; i++) {
		for (uint8_t j = 0; j < 2; j++) {
			Point tmpp = last;
			for (uint8_t k = 0; k < 1; k++) {
				tmpp = move(tmpp, i, j);
				if (varify_location(tmpp) && board[tmpp.x][tmpp.y] > 3) {
					uint8_t l = 0;
					while (!(pool[l].x == tmpp.x && pool[l].y == tmpp.y) && l < psp + 1) {
						l++;
					}
					if (l == psp + 1) {
						pool[l] = tmpp;
						psp ++;
					}
				}
			}
		}
	}
}

Point calcboard(uint8_t color) {
	Point minmyP = { 7,7 };
	disvec minmydis;
	Point minopP = { 7,7 };
	disvec minopdis;
	Point resP;
	for (uint8_t i = 0; i < 5; i++) {
		minmydis.form[i] = 10;
		minopdis.form[i] = 10;
	}
	disvec mydis;
	disvec opdis;
	/*
	for (uint8_t i = 0; i < 15; i++) {
		for (uint8_t j = 0; j < 15; j++) {
			if (board[i][j] < 2) {
				for (uint8_t k = 0; k < 4; k++) {

				}
			}
			Point P = { i,j };
			if (board[i][j] > 3) {
				mydis = getvec(id2dis(lookup(P, color)));
				if (smaller(mydis, minmydis)) {
					minmyP = P;
					minmydis = mydis;
					//if (mindis.form[0] == 0) return minP;
				}
				opdis = getvec(id2dis(lookup(P, !color)));
				if (smaller(opdis, minopdis)) {
					minopP = P;
					minopdis = opdis;
					//if (mindis.form[0] == 0) return minP;
				}
			}
		}
	}
	*/
	for (uint8_t i = 0; i < psp + 1; i++) {
		Point P = pool[i];
		mydis = getvec(id2dis(lookup(P, color)));
		if (smaller(mydis, minmydis)) {
			minmyP = P;
			minmydis = mydis;
			//if (mindis.form[0] == 0) return minP;
		}
		opdis = getvec(id2dis(lookup(P, !color)));
		if (smaller(opdis, minopdis)) {
			minopP = P;
			minopdis = opdis;
			//if (mindis.form[0] == 0) return minP;
		}
	}
	/*
	for (uint8_t i = 0; i < 15; i++) {
		for (uint8_t j = 0; j < 15; j++) {
			Point P = { i,j };
			if (board[i][j] > 3) {
				if (i == 6 && j == 7) {
					i = i;
				}
				opdis = getvec(id2dis(lookup(P, !color)));
				if (smaller(opdis, mindis)) {
					minP = P;
					mindis = opdis;
					//if (mindis.form[0] == 0) return minP;
				}
			}
		}
	}
	*/
	resP = cross_compare(minmydis, minopdis) ? minmyP : minopP;
	add2pool(resP);
	/*
	for (uint8_t i = 0; i < psp + 1; i++) {
		if (pool[i].x == resP.x && pool[i].y == resP.y) {
			pool[i] = pool[psp--];
			break;
		}
	}
	*/
	return resP;
}