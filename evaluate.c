#include "five_type.h"
#include "five_global.h"

enum CASE {
	LONGLINE, FIVE, FOUR2, FOUR1, FOUR_JMP, THREE2, THREE_JMP, THREE1, TWO2, TWO1
};

const uint16_t value_table[] = {};// The values for each case.

typedef uint16_t Value[2];

//The struct used to store the properties of the current line.
typedef struct{
	uint8_t color;//The color of current line.
	uint8_t adjacent;//The number of adjacent pieces.
	uint8_t distance;//The distance to the next line.
	uint8_t length;//The length of the next line.
	uint8_t alive;//The blank around the line.
}Line;

/*Use the position and surroundings to describe a piece.*/
typedef struct {
	Point position;// The position of the piece.
	Line surroundings[4][2];//There are 4 possible lines, and 2 possible directions.
}Piece;

typedef struct {
	uint16_t value[2];
}Value;

/*
	Judge if the location is illegal.
*/
uint8_t varify_location(Point p)
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
	Look up the lines from point p in all the directions.
*/
Piece look_up(Point p) {
	Line surroundings[4][2] = { 0 };
	Piece pie = { p,surroundings };
	for (uint8_t i = 0; i < 4; ++i)
		for (uint8_t j = 0; j < 2; ++j) {
			Point tmpp = move(p, i, j);//look in the specific direction.
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
				tmpp = move(tmpp, i, j);
				if (pie.surroundings[i][j].distance > 1) break;// if the distance is greater than 1, it's meaning less to continue the count.
			}
			if (varify_location(tmpp) && (board[tmpp.x][tmpp.y] == pie.surroundings[i][j].color)|| !pie.surroundings[i][j].adjacent) {
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
				tmpp = move(tmpp, i, j);
				if (pie.surroundings[i][j].alive > 1) break;
			}
		}
	return pie;
}

/*
	Calc the value of point p.
*/
Value evaluate(Point p){
	Piece pie = look_up(p);
	Line s[4][2] = pie.surroundings;
	Value value = { 0,0 };//The value of the point for each side.
	for (uint8_t k = 0; k < 2; k++) {//two colors.
		uint16_t checklist[4];
		for (uint8_t i = 0; i < 4; i++) {//four lines.
			uint8_t conjunction = 0;// wheather to look at the both sides
			if((s[i][0].adjacent && s[i][1].adjacent)&&(s[i][0].color == s[i][1].color))
			/*
			if ((pie.surroundings[i][0].adjacent || pie.surroundings[i][1].length) && \
				(pie.surroundings[i][0].adjacent || pie.surroundings[i][1].length)) {    // There are meaningful things on both sides.
				if (pie.surroundings[i][0].color == pie.surroundings[i][1].color) {
					conjunction = pie.surroundings[i][0].adjacent + pie.surroundings[i][1].adjacent;// Combine the adjacent pieces if of the same player.
					if (conjunction > 4) checklist[LONGLINE] = 1;
					pie.surroundings[i][0].adjacent = conjunction;
					pie.surroundings[i][1].adjacent = conjunction;
				}
			}
			for (uint8_t j = 0; j < 2; j++) {
				Line line = pie.surroundings[i][j];
				if (line.adjacent == 4) checklist[FIVE] = 1;
				if(line.distance == 1 && 

			}
			*/
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