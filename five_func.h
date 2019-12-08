/*
	Declare the functions
*/
#pragma once

#include "five_type.h"

uint8_t varify_location(Point p);

//uint16_t evaluate(pieceid id);

//pieceid lookup(Point p, uint8_t color);

Point choose();

Point user();

void play(Point (*P1)(Point last), Point (*P2)(Point last));

void new_game(Point(*P1)(Point last), Point(*P2)(Point last));

void continue_game(Point(*P1)(Point last), Point(*P2)(Point last));

