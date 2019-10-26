#include "five_type.h"
#include "five_global.h"

enum CASE {
	LONGLINE, FIVE, FOUR2, FOUR1, FOUR_JMP, THREE2, THREE_JMP, THREE1, TWO2, TWO1
};

const uint16_t value_table[] = {};

typedef uint16_t Value[2];

typedef struct{
	uint8_t color;
	uint8_t adjacent;
	uint8_t distance;
	uint8_t length;
	uint8_t alive;
}Line;

typedef struct {
	Point position;
	Line surroundings[4][2];//Line,Direction
}Piece;

typedef struct {
	uint16_t value[2];
}Value;
/*
	Judge if the location is illegal.
*/
char varify_location(Point p)
{
	if (p.x >= 0 && p.x <= 14 && p.y >= 0 && p.y <= 14 && board[p.x][p.y] < 2)
		return 1;
	return 0;
}

/*
	Move from the point p in the direction decided by i and j.
*/
inline Point move(Point src, uint8_t i, uint8_t j) {
	Point dst = { src.x + dx[j] * direction[j], src.y + dy[j] * direction[j] };
	return dst;
}

/*
	Look up for the lines from point p in all the directions.
*/
Piece look_up(Point p) {
	Line surroundings[4][2] = { 0 };
	Piece pie = { p,surroundings };
	for (uint8_t i = 0; i < 4; ++i)
		for (uint8_t j = 0; j < 2; ++j) {
			Point tmpp = move(p, i, j);
			if (varify_location(tmpp) && board[tmpp.x][tmpp.y] < 2) {//judge if adjacent
				pie.surroundings[i][j].color = board[tmpp.x][tmpp.y];
				pie.surroundings[i][j].adjacent = 1;//set the color.
				tmpp = move(tmpp, i, j);
			}
			while (varify_location(tmpp) && board[tmpp.x][tmpp.y] == pie.surroundings[i][j].color) {
				pie.surroundings[i][j].adjacent += 1;//update the adjacent.
				tmpp = move(tmpp, i, j);
			}
			while (varify_location(tmpp) && board[tmpp.x][tmpp.y] > 3) {
				pie.surroundings[i][j].distance += 1;//update the distance.
				if (pie.surroundings[i][j].alive > 1) break;
				tmpp = move(tmpp, i, j);
			}
			if (varify_location(tmpp) && board[tmpp.x][tmpp.y] == pie.surroundings[i][j].color) {
				pie.surroundings[i][j].color = board[tmpp.x][tmpp.y];
				pie.surroundings[i][j].length = 1;//init the length.
				tmpp = move(tmpp, i, j);
			}
			while (varify_location(tmpp) && board[tmpp.x][tmpp.y] == pie.surroundings[i][j].color) {
				pie.surroundings[i][j].length += 1;//update the length.
				tmpp = move(tmpp, i, j);
			}
			while (varify_location(tmpp) && board[tmpp.x][tmpp.y] > 3) {
				pie.surroundings[i][j].alive += 1;//update the alive.
				if (pie.surroundings[i][j].alive > 1) break;
				tmpp = move(tmpp, i, j);
			}
		}
	return pie;
}

/*
	Calc the value of point p.
*/
Value evaluate(Point p){
	Piece pie = look_up(p);
	uint16_t tmpvalue[2] = { 0 };
	Value value = { 0,0 };
	for (uint8_t k = 0; k < 2; k++) {
		uint16_t checklist[4];
		for (uint8_t i = 0; i < 4; i++) {
			uint8_t conjunction = 0;// wheather to look at the both sides
			if ((pie.surroundings[i][0].adjacent || pie.surroundings[i][1].length) && \
				(pie.surroundings[i][0].adjacent || pie.surroundings[i][1].length)) {    // There are meaningful thins on both sides.
				if (pie.surroundings[i][0].color == pie.surroundings[i][1].color) {
					conjunction = pie.surroundings[i][0].adjacent + pie.surroundings[i][1].adjacent;// Combine the adjacent pieces if of the same player.
					pie.surroundings[i][0].adjacent = conjunction;
					pie.surroundings[i][1].adjacent = conjunction;
				}
			}
			for (uint8_t j = 0; j < 2; j++) {
				
				if (conjunction > 4) checklist[LONGLINE] = 1;
				if (conjunction == 4) checklist[FIVE] = 1;

			}
		}
	}
			/*
			for (uint8_t j = 0; j < 2; j++) {
			switch (pie.surroundings[i][j].adjacent) {
			case 4:
				tmpvalue[k] += 32;
				flag = 1;
				break;
			case 3:
				if (piece_info.spaces[k][i].small)
				{
					tmpvalue[k] += 16;
					flag = 1;
				}
				else if (piece_info.spaces[k][i].big)
					check_list[k][4] += 1;
				break;
			case 2:
				if (piece_info.spaces[k][i].small > 1)
					check_list[k][7] += 1;
				else if (piece_info.spaces[k][i].small && (piece_info.spaces[k][i].big > 1))
					check_list[k][6] += 1;
				else if (piece_info.spaces[k][i].small && piece_info.spaces[k][i].big)
					check_list[k][5] += 1;
				break;
			case 1:
				if (piece_info.spaces[k][i].small + piece_info.spaces[k][i].big < 3)
					check_list[k][0] += 1;
				else if (piece_info.spaces[k][i].small + piece_info.spaces[k][i].big == 3)
					check_list[k][1] += 1;
				else if (piece_info.spaces[k][i].small + piece_info.spaces[k][i].big == 4)
					check_list[k][2] += 1;
				else check_list[k][3] += 1;
				break;
			case 0:
				tmpvalue[k] += 1;
				break;
			}
			if (piece_info.weight_info[k][i] > 4) {
				tmpvalue[k] += 0;
				if (k == player) {
					tmpvalue[k] = -1;
					return update_value(value, tmpvalue);
				}
			}
		}
		if (check_list[k][4] > 1) {
			tmpvalue[k] += 8;
			ban[k] += 1;
			flag = 1;
			check_cross[piece_info.point.x][piece_info.point.y] = 1;
		}
		else if (check_list[k][4] && (check_list[k][7] + check_list[k][6]))
		{
			tmpvalue[k] += 4;
			flag = 1;
			check_cross[piece_info.point.x][piece_info.point.y] = 1;
		}
		else if (check_list[k][7] > 1)
		{
			ban[k] += 1;
			tmpvalue[k] += 2;
			flag = 1;
			check_cross[piece_info.point.x][piece_info.point.y] = 1;
		}
		if (player == 0 && ban[player]) {
			tmpvalue[player] = -1;
			return update_value(value, tmpvalue);
		}
		tmpvalue[k] += (k == player);
		if (flag) {
			tmpvalue[k] += 63;
		}
		else {
			for (int k = 0; k < 8; k++)
				tmpvalue[k] += k * check_list[k][k] * 4;
		}
	}
	return update_value(value, tmpvalue);
	*/
}