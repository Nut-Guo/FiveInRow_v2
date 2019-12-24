#include "five_type.h"
#include<stdlib.h>

typedef uint16_t Hashtable[2][15][15];

typedef struct {
    uint16_t hash[2];
}HASH;

Hashtable Zobrist;

void init_zobrist() {
    for (int16_t i = 0; i < 15; i++) {
        for (int16_t j = 0; j < 15; j++) {
            Zobrist[0][i][j] = rand() & 0xfff;
            Zobrist[1][i][j] = rand() & 0xfff;
        }
    }
}

HASH calc_hash(Point p, HASH origin_hash, uint8_t color) {
    HASH hash = { (origin_hash.hash[0] ^ Zobrist[color][p.x][p.y]) & 4095, (origin_hash.hash[1] ^ Zobrist[!color][p.x][p.y]) & 4095 };
    return hash;
}