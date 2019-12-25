/*
	Declare the functions
*/
#pragma once

#include "five_type.h"

Point user(Point last);

Point search_point(Point last);

void play(Point (*P1)(Point last), Point (*P2)(Point last));

void new_game(Point(*P1)(Point last), Point(*P2)(Point last));

void continue_game(Point(*P1)(Point last), Point(*P2)(Point last));

void get_the_record();

