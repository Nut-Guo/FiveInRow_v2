#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <conio.h>
#include <string.h>
#include "five_global.h"

extern char stop;
extern char cwd[80];

extern void game(char mode);
extern char drop(Point p);
extern Point(*Input[])(Point last);
extern void init_board();

void record() {
	printf("Please specify the filename:");
	char filename[20];
	scanf_s("%s", filename, 20);
	putchar('\n');
	char filepath[120];
	sprintf_s(filepath, 120, "%s%s%s", cwd, "\\History\\", filename);
	_Post_ _Notnull_ FILE* fp;
	errno_t err = fopen_s(&fp, filepath, "w");
	if (err)
		printf_s("The file %s was not opened\n", filename);
	else
	{
		printf("%s\n", filepath);
		if (fp) {
			fprintf(fp, "%d\n", Round);
			for (int i = 0; i < Round; i++) {
				fprintf(fp, "%c%d", (PieceOnBoard.record[i]).y + 'a', 15 - (PieceOnBoard.record[i]).x);
				if (i % 2)
					fprintf(fp, "\n");
				else fprintf(fp, " ");
			}
			fclose(fp);
		}
	}
}

void print_record() {
	record();
	printf("Round Player1 Player2\n");
	for (int i = 0; i < Round; i++) {
		if (!(i % 2))
			printf("%4d ", i / 2 + 1);
		printf("%4c%-4d", (PieceOnBoard.record[i]).y + 'a', 15 - (PieceOnBoard.record[i]).x);
		if (i % 2)
			putchar('\n');
	}
	printf("\nPress (q) to quit or anything else to play again.\n");
	if (_getch() == 'q')
		exit(0);
}

void load_record(char* name) {
	char filepath[120] = { 0 };
	sprintf_s(filepath, 120, "%s%s%s", cwd, "\\History\\", name);
	FILE* archive;
	errno_t err = fopen_s(&archive, filepath, "r");
	if (err)
		printf_s("The file %s was not opened\n", name);
	else
	{
		int Round = 0;
		char tmpn;
		if (archive) {
			fseek(archive, 0L, SEEK_SET);
			fscanf_s(archive, "%d%c", &Round, &tmpn, 1);
		}
		int counter = 0;
		init_board();
		if (archive) {
			while (counter++ < Round && !stop) {
				char x, y, tmpx;
				int tmpy;
				fscanf_s(archive, "%c%d%c", &tmpx, 1, &tmpy, &tmpn, 1);
				x = 15 - tmpy;
				y = tmpx - 'a';
				Point p = { x,y };
				stop = drop(p);
				if (!stop)
					player ^= 1;
			}
			fclose(archive);
		}
		printf("Continue(c)\tQuit(q) or anything else to go back to menu.\n");
		switch (_getch()) {
		case 'c':
			game(1);
			break;
		case 'q':
			exit(0);
		default:
			break;
		}
	}
}

void get_the_record() {
	char num = 0;
	char name[20][20] = { 0 };
	char dir[120];
	sprintf_s(dir, 120, "%s%s", cwd, "\\History\\*");
	long long lfDir;
	struct _finddata_t fileDir;
	if ((lfDir = _findfirst(dir, &fileDir)) == -1l)
		printf("No record is found\n");
	else {
		printf("record list:\n");
		do {
			if ((strcmp(fileDir.name, ".") != 0) && (strcmp(fileDir.name, "..") != 0)) {
				strcpy_s(name[num], 20, fileDir.name);
				printf("%2d: %s\n", ++num, fileDir.name);
			}
		} while (!(_findnext(lfDir, &fileDir)));
	}
	_findclose(lfDir);
	printf("Input the archive number to load the archive or 0 to the menu.\n");
	int c = 0;
	char tmpn;
	scanf_s("%d%c", &c, &tmpn, 1);
	if (c <= num && c) {
		load_record(name[c - 1]);
	}
}