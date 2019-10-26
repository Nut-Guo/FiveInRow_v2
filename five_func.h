#pragma once

char pc();

char user();

char(*Input[])() = { user, pc};

void play(char (*P1)(), char (*P2)());

void new_game(char(*P1)(), char(*P2)());

void continue_game(char(*P1)(), char(*P2)());

void(*Play[])(char (*P1)(), char (*P2)()) = { new_game, continue_game };