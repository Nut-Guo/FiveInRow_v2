/*
	Get the pieceid which consists of 4 numbers representing 
	the state in the 4 directions.
*/

#include "five_type.h"
#include "five_global.h"
#include <stdio.h>

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
	uint8_t distance[2][4];
}distance;

typedef struct {
	uint8_t form[6];
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
	   10, 10, 10, 10, 10, 10, 10, 10, 20, 10, 10,  3, 10,  3,  3,  2, 10,
		3,  3,  2, 10,  2,  2,  1, 10,  3,  3,  2, 10,  2,  2,  1, 10,  2,
	   10,  1, 10,  1,  1,  0, 10,  3,  3, 10, 10,  2,  2, 10, 10,  2,  2,
	   10, 10,  1,  1, 10, 10,  2,  2, 10, 10,  1,  1, 10, 10,  1,  1, 10,
	   10,  0,  0, 10, 20, 10, 10,  3, 10,  3, 10,  2, 10,  3, 10,  2, 10,
	   10, 10,  1, 10,  3, 10,  2, 10,  2, 10,  1, 10,  2, 10,  1, 10,  1,
	   10,  0, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	   10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 20,
	   20, 10, 10,  3, 10,  3,  3,  2, 10,  3,  3,  2,  3,  2,  2,  1, 10,
		3,  3,  2,  3,  2,  2,  1, 10,  2,  2,  1,  2,  1,  1,  0, 10,  3,
		3,  3,  3,  2,  2, 10, 10,  2,  2,  2,  2,  1,  1, 10, 10,  2,  2,
	   10, 10,  1,  1, 10, 10,  1,  1,  1,  1,  0,  0, 10, 20,  3,  3,  3,
		3,  3, 10,  2, 10,  2,  2,  2,  2, 10, 10,  1, 10,  2,  2,  2,  2,
		2, 10,  1, 10,  1,  1,  1,  1,  1, 10,  0, 10,  2,  2,  2,  2, 10,
	   10, 10, 10,  1,  1,  1,  1, 10, 10, 10, 10,  1,  1,  1,  1, 10, 10,
	   10, 10,  0,  0,  0,  0, 10, 10, 20, 20, 10, 10,  3, 10,  3,  3,  2,
	   10,  3,  3,  2, 10,  2,  2,  1, 10,  3,  3,  2, 10,  2,  2,  1, 10,
		2, 10,  1, 10,  1,  1,  0, 10,  3,  3, 10, 10,  2,  2, 10, 10,  2,
		2, 10, 10,  1,  1, 10, 10,  2,  2, 10, 10,  1,  1, 10, 10,  1,  1,
	   10, 10,  0,  0, 10, 20, 10, 10,  3, 10,  3, 10,  2, 10,  3, 10,  2,
	   10, 10, 10,  1, 10,  3, 10,  2, 10,  2, 10,  1, 10,  2, 10,  1, 10,
		1, 10,  0, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	   10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 20, 20,
	   20, 20,
	   10, 10,  3, 10,  3,  3,  2, 10,  3,  3,  2,  3,  2,  2,  1, 10,  3,
		3,  2,  3,  2,  2,  1,  3,  2,  2,  1,  2,  1,  1,  0, 10,  3,  3,
		3,  3,  2,  2,  3,  3,  2,  2,  2,  2,  1,  1, 10, 10,  2,  2,  2,
		2,  1,  1,  2,  2,  1,  1,  1,  1,  0,  0, 10, 20,  3,  3,  3,  3,
		3,  3,  2,  3,  2,  2,  2,  2, 10, 10,  1, 10,  2,  2,  2,  2,  2,
		2,  1,  2,  1,  1,  1,  1,  1, 10,  0, 10,  2,  2,  2,  2, 10, 10,
	   10, 10,  1,  1,  1,  1, 10, 10, 10, 10,  1,  1,  1,  1,  1,  1,  1,
		1,  0,  0,  0,  0, 10, 10, 20, 20,  3,  3,  3,  3,  3,  3,  2,  3,
		3,  3,  2, 10,  2,  2,  1, 10,  2,  2,  2,  2,  2,  2,  1,  2,  2,
	   10,  1, 10,  1,  1,  0, 10,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
	   10, 10,  1,  1, 10, 10,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 10,
	   10,  0,  0, 10, 20,  2,  2,  2,  2,  2,  2,  2,  2,  3, 10,  2, 10,
	   10, 10,  1, 10,  1,  1,  1,  1,  1,  1,  1,  1,  2, 10,  1, 10,  1,
	   10,  0, 10,  1,  1,  1,  1,  1,  1,  1,  1, 10, 10, 10, 10, 10, 10,
	   10, 10,  0,  0,  0,  0,  0,  0,  0,  0, 10, 10, 10, 10, 20, 20, 20,
	   20, 10, 10,  3, 10,  3,  3,  2, 10,  3,  3,  2,  3,  2,  2,  1, 10,
		3,  3,  2,  3,  2,  2,  1, 10,  2,  2,  1,  2,  1,  1,  0, 10,  3,
		3,  3,  3,  2,  2, 10, 10,  2,  2,  2,  2,  1,  1, 10, 10,  2,  2,
	   10, 10,  1,  1, 10, 10,  1,  1,  1,  1,  0,  0, 10, 20,  3,  3,  3,
		3,  3, 10,  2, 10,  2,  2,  2,  2, 10, 10,  1, 10,  2,  2,  2,  2,
		2, 10,  1, 10,  1,  1,  1,  1,  1, 10,  0, 10,  2,  2,  2,  2, 10,
	   10, 10, 10,  1,  1,  1,  1, 10, 10, 10, 10,  1,  1,  1,  1, 10, 10,
	   10, 10,  0,  0,  0,  0, 10, 10, 20, 20, 10, 10,  3, 10,  3,  3,  2,
	   10,  3,  3,  2, 10,  2,  2,  1, 10,  3,  3,  2, 10,  2,  2,  1, 10,
		2, 10,  1, 10,  1,  1,  0, 10,  3,  3, 10, 10,  2,  2, 10, 10,  2,
		2, 10, 10,  1,  1, 10, 10,  2,  2, 10, 10,  1,  1, 10, 10,  1,  1,
	   10, 10,  0,  0, 10, 20, 10, 10,  3, 10,  3, 10,  2, 10,  3, 10,  2,
	   10, 10, 10,  1, 10,  3, 10,  2, 10,  2, 10,  1, 10,  2, 10,  1, 10,
		1, 10,  0, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	   10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 20, 20, 20, 20, 20, 20,
	   20, 20
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
		1,  2,  2,  1,  1,  1,  1,  0, 20, 10,  4,  4,  3,  4,  3,  3,  2,
		4,  3,  3,  2,  3,  2,  2,  1,  4,  3,  3,  2,  3,  2,  2,  1,  3,
		2,  2,  1,  2,  1,  1,  0,  4,  4,  3,  3,  3,  3,  2,  2,  3,  3,
		2,  2,  2,  2,  1,  1,  3,  3,  2,  2,  2,  2,  1,  1,  2,  2,  1,
		1,  1,  1,  0, 20,  4,  4,  4,  3,  3,  3,  3,  2,  3,  3,  3,  2,
		2,  2,  2,  1,  3,  3,  3,  2,  2,  2,  2,  1,  2,  2,  2,  1,  1,
		1,  1,  0,  3,  3,  3,  3,  2,  2,  2,  2,  2,  2,  2,  2,  1,  1,
		1,  1,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0, 20,
	   20, 10,  4,  4,  3,  4,  3,  3,  2,  4,  3,  3,  2,  3,  2,  2,  1,
		4,  3,  3,  2,  3,  2,  2,  1,  3,  2,  2,  1,  2,  1,  1,  0,  4,
		4,  3,  3,  3,  3,  2,  2,  3,  3,  2,  2,  2,  2,  1,  1,  3,  3,
		2,  2,  2,  2,  1,  1,  2,  2,  1,  1,  1,  1,  0, 20,  4,  4,  4,
		3,  3,  3,  3,  2,  3,  3,  3,  2,  2,  2,  2,  1,  3,  3,  3,  2,
		2,  2,  2,  1,  2,  2,  2,  1,  1,  1,  1,  0,  3,  3,  3,  3,  2,
		2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  2,  2,  2,  2,  1,  1,
		1,  1,  1,  1,  1,  1,  0,  0, 20, 20,  4,  4,  4,  3,  4,  3,  3,
		2,  3,  3,  3,  2,  3,  2,  2,  1,  3,  3,  3,  2,  3,  2,  2,  1,
		2,  2,  2,  1,  2,  1,  1,  0,  3,  3,  3,  3,  3,  3,  2,  2,  2,
		2,  2,  2,  2,  2,  1,  1,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,
		1,  1,  1,  1,  0, 20,  3,  3,  3,  3,  3,  3,  3,  2,  2,  2,  2,
		2,  2,  2,  2,  1,  2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,
		1,  1,  1,  0,  2,  2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,
		1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0, 20, 20,
	   20, 20,
	   10,  4,  4,  3,  4,  3,  3,  2,  4,  3,  3,  2,  3,  2,  2,  1,  4,
		3,  3,  2,  3,  2,  2,  1,  3,  2,  2,  1,  2,  1,  1,  0,  4,  4,
		3,  3,  3,  3,  2,  2,  3,  3,  2,  2,  2,  2,  1,  1,  3,  3,  2,
		2,  2,  2,  1,  1,  2,  2,  1,  1,  1,  1,  0, 20,  4,  4,  4,  3,
		3,  3,  3,  2,  3,  3,  3,  2,  2,  2,  2,  1,  3,  3,  3,  2,  2,
		2,  2,  1,  2,  2,  2,  1,  1,  1,  1,  0,  3,  3,  3,  3,  2,  2,
		2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  2,  2,  2,  2,  1,  1,  1,
		1,  1,  1,  1,  1,  0,  0, 20, 20,  4,  4,  4,  3,  4,  3,  3,  2,
		3,  3,  3,  2,  3,  2,  2,  1,  3,  3,  3,  2,  3,  2,  2,  1,  2,
		2,  2,  1,  2,  1,  1,  0,  3,  3,  3,  3,  3,  3,  2,  2,  2,  2,
		2,  2,  2,  2,  1,  1,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,
		1,  1,  1,  0, 20,  3,  3,  3,  3,  3,  3,  3,  2,  2,  2,  2,  2,
		2,  2,  2,  1,  2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,
		1,  1,  0,  2,  2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,
		1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0, 20, 20, 20,
	   20,  4,  4,  4,  3,  4,  3,  3,  2,  4,  3,  3,  2,  3,  2,  2,  1,
		3,  3,  3,  2,  3,  2,  2,  1,  3,  2,  2,  1,  2,  1,  1,  0,  3,
		3,  3,  3,  3,  3,  2,  2,  3,  3,  2,  2,  2,  2,  1,  1,  2,  2,
		2,  2,  2,  2,  1,  1,  2,  2,  1,  1,  1,  1,  0, 20,  3,  3,  3,
		3,  3,  3,  3,  2,  3,  3,  3,  2,  2,  2,  2,  1,  2,  2,  2,  2,
		2,  2,  2,  1,  2,  2,  2,  1,  1,  1,  1,  0,  2,  2,  2,  2,  2,
		2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
		1,  1,  1,  1,  1,  1,  0,  0, 20, 20,  3,  3,  3,  3,  3,  3,  3,
		2,  3,  3,  3,  2,  3,  2,  2,  1,  2,  2,  2,  2,  2,  2,  2,  1,
		2,  2,  2,  1,  2,  1,  1,  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
		1,  1,  1,  1,  0, 20,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
		2,  2,  2,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
		1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
		1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0, 20, 20, 20, 20, 20, 20,
	   20, 20
};

extern uint8_t verify_location(Point p);
// move one step in the specific direction
inline Point move(Point p, uint8_t i, uint8_t j) {
	Point tmpp = { p.x + dx[i] * direction[j], p.y + dy[i] * direction[j] };
	return tmpp;
}

// look up the end in one direction.
Point get_end(Point p, uint8_t i, uint8_t j, uint8_t color, Board *local_board) {// look up the end in one direction.
	Point end = p;
	Point tmpp = move(p, i, j);
	uint8_t len = 1;
	while (verify_location(tmpp) && ((*local_board).location[tmpp.x][tmpp.y] == color || (*local_board).location[tmpp.x][tmpp.y] > 3) && len < 5) {
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
uint16_t parse_line(Point start, Point end, uint8_t d, uint8_t color, Board *local_board) {
	Point tmpp = start;
	uint16_t id = 1;
	while (!comaprepoint(tmpp, end)) {
		id = (id << 1) + ((*local_board).location[tmpp.x][tmpp.y] == color);
		tmpp = move(tmpp, d, 1);
	}
	id = (id << 1) + ((*local_board).location[tmpp.x][tmpp.y] == color);
	return id;
}

//The function that calls the former ones that to get the id.
pieceid lookup(Point p, uint8_t color, Board *local_board) {
	uint8_t save = (*local_board).location[p.x][p.y];
	(*local_board).location[p.x][p.y] = color;
	pieceid id = {0};
	for (uint8_t i = 0; i < 4; i++) {// look up in the four directions;
		Point end[2] = { get_end(p, i, 0, color, local_board), get_end(p, i, 1, color,local_board) };
		id.id[i] = parse_line(end[0], end[1], i, color,local_board);
	}
	(*local_board).location[p.x][p.y] = save;
	return id;
}

//Translate id 2 distance
distance id2dis(Point p, uint8_t color, Board *local_board) {
	pieceid id = lookup(p, color,local_board);
	distance dis;
	for (uint8_t i = 0; i < 4; i++) {
		dis.distance[0][i] = dis2four[id.id[i]];
		dis.distance[1][i] = dis2five[id.id[i]];
	}
	return dis;
}

inline uint8_t min(uint8_t x, uint8_t y) {
	return x < y ? x : y;
}

uint8_t mini[2];//store the min direction of 4 and 5;
void selection_sort(uint8_t a[], uint8_t len, uint8_t id)
{
	uint8_t i, j, temp;
	for (i = 0; i < 2; i++)
	{
		uint8_t min = i;
		for (j = i + 1; j < len; j++)
		{
			if (a[j] < a[min])
			{
				min = j;
			}
		}
		if (min != i)
		{
			temp = a[min];
			a[min] = a[i];
			a[i] = temp;
		}
		if (i == 0) mini[id] = min;
	}
}

disvec getvec(Point p, uint8_t color, Board *local_board) {
	distance dis = id2dis(p,color,local_board);
	//sort the dis;
	uint8_t mindis40 = 10;
	uint8_t mindis41 = 10;
	uint8_t mindis50 = 10;
	uint8_t mindis51 = 10;
	uint8_t i40 = 4;
	uint8_t i50 = 4;
	uint8_t longline = 20;
	for (uint8_t i = 0; i < 4; i++) {
		if (dis.distance[0][i] == 20) {
			longline = 0;
			break;
		}
	}
	selection_sort(dis.distance[0], 4, 0);
	selection_sort(dis.distance[1], 4, 1);
	mindis40 = dis.distance[0][0];
	mindis41 = dis.distance[0][1];
	mindis50 = dis.distance[1][0];
	mindis51 = dis.distance[1][1];
	i40 = mini[0];
	i50 = mini[1];
	//calc double 3
	uint8_t mindisd3 = (mindis40 ? mindis40 - 1 : 0) + (mindis41 ? mindis41 - 1 : 0);
	//calc 4 and 3
	uint8_t mindis34 = i40 == i50 ? min((mindis40 ? mindis40 - 1 : 0) + (mindis51 ? mindis51 - 1 : 0), (mindis41 ? mindis41 - 1 : 0) + (mindis50 ? mindis50 - 1 : 0)) : ((mindis40 ? mindis40 - 1 : 0) + (mindis50 ? mindis50 - 1 : 0));
	//calc double 4
	uint8_t mindisd4 = (mindis50 ? mindis50 - 1 : 0) + (mindis51 ? mindis51 - 1 : 0);
	if (!mindis50) {
		mindisd4 = 10;
		mindis34 = 10;
		mindisd3 = 10;
	}
	if (!mindisd4) {
		mindisd3 = 10;
		mindis34 = 10;
	}
	if (!mindis34) {
		mindisd3 = 10;
	}
	disvec vec = { { mindis50,mindis40,mindis34,mindisd4,mindisd3,longline} };
	return vec;
}