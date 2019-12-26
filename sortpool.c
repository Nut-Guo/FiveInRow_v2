#include "five_type.h"
#include "five_global.h"

extern inline int16_t get_value(Point p, uint8_t color, Board* local_board);
typedef struct {
	Point* point;
	int16_t value;
}Point_info;

typedef struct {
	Point_info info[255];
}InfoPOOL;

void ssort_pool(InfoPOOL* r, uint8_t color, uint8_t len,uint8_t cnt, Board* local_board){
	uint8_t i, j;
	Point_info temp;
	for (i = 0; i < len; i++)
	{
		uint8_t max = i;
		for (j = i + 1; j < cnt; j++)
		{
			if ((*r).info[j].value > (*r).info[max].value)
			{
				max = j;
			}
		}
		if (max != i)
		{
			temp = (*r).info[max];
			(*r).info[max] = (*r).info[i];
			(*r).info[i] = temp;
		}
	}
}