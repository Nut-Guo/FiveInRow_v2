#include "five_type.h"
#include<stdlib.h>

typedef uint16_t Hashtable[2][15][15];

Hashtable Zobrist;

void init_zobrist() {
    for (int32_t i = 0; i < 15; i++) {
        for (int32_t j = 0; j < 15; j++) {
            Zobrist[0][i][j] = rand();
            Zobrist[1][i][j] = rand();
        }
    }
}

uint16_t calc_hash(Point p, uint16_t origin_hash, uint8_t color) {
    return origin_hash ^ Zobrist[color][p.x][p.y];
}