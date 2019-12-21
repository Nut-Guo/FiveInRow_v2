/*
#include "five_type.h"
#include "five_global.h"

typedef struct {
	uint8_t form[5];
}disvec;

inline Point move(Point p, uint8_t i, uint8_t j) {
	Point tmpp = { p.x + dx[i] * direction[j], p.y + dy[i] * direction[j] };
	return tmpp;
}

extern disvec getvec(Point P, uint8_t color);

POOL pool;

uint8_t get_pool() {
	uint8_t i = 0;
	Point tmpp;
	Board inpool = { 0 };
	while (i < Round) {
		tmpp = PieceOnBoard.record[i];
	}
}
*/
/*
typedef struct {
	uint16_t id[4];
}pieceid;

typedef struct {
	uint8_t distance[4][2];
}distance;

typedef struct {
	uint8_t form[5];
}disvec;

typedef struct {
	disvec color[2];
}vecpare;

typedef vecpare vecpool[255];

typedef Point POOL[225];

void insertion_sort(uint8_t arr[], uint8_t len) {
	uint8_t i, j, temp;
	for (i = 1; i < len; i++) {
		temp = arr[i];
		for (j = i; j > 0 && arr[j - 1] > temp; j--)
			arr[j] = arr[j - 1];
		arr[j] = temp;
	}
}

uint8_t smaller(disvec A, disvec B) {
	//insertion_sort(A.form, 5);
	//insertion_sort(B.form, 5);
	if (A.form[0] == 0) return 1;
	if (B.form[0] == 0) return 0;
	for (uint8_t i = 1; i < 5; i++) {
		if (A.form[i] == 0)
			return 1;
		if (B.form[i] == 0)
			return 0;
		if (A.form[i] == B.form[i]) {
			continue;
		}
		else return A.form[i] < B.form[i];
	}
	return A.form[0] < B.form[0];
}

uint8_t get_surroundings(Point p, uint8_t color) {
	Point tmpp;
	uint8_t count = 0;
	for (uint8_t i = 0; i < 4; i++) {
		for (uint8_t j = 0; j < 2; j++) {
			tmpp = move(p, i, j);
			if (board[tmpp.x][tmpp.y] == color)
				count++;
		}
	}
	return count;
}

inline uint8_t mindis(disvec vec) {
	uint8_t mindis = 10;
	for (uint8_t i = 0; i < 5; i++) {
		if (vec.form[i] < mindis)mindis = vec.form[i];
	}
	return mindis;
}

uint8_t equal(disvec A, disvec B) {
	return mindis(A) == mindis(B);
}

uint8_t cross_compare(disvec mydis, disvec opdis) {
	if (mydis.form[0] == 0) return 1;
	else if (opdis.form[0] == 0) return 0;
	else if (mydis.form[1] == 0 || mydis.form[2] == 0 || mydis.form[3] == 0) return 1;
	else if (opdis.form[1] == 0 || opdis.form[2] == 0 || opdis.form[3] == 0) return 0;
	else if (mydis.form[4] == 0) return 1;
	else if (opdis.form[4] == 0) return 0;
	for (uint8_t i = 0; i < 5; i++) {
		if (minform(opdis) < 1) return 0;
	}
	return 1;
}

void sortvecpool(vecpool disvecpool, POOL pool, uint8_t psp, uint8_t color) {
	uint8_t i, j;
	vecpare tempvec;
	Point tmpp;
	for (i = 0; i < psp; i++) {
		tempvec = disvecpool[i];
		tmpp = pool[i];
		for (j = i; j > 0 && smaller(tempvec.color[color], disvecpool[j - 1].color[color]); j--) {
			disvecpool[j] = disvecpool[j - 1];
			pool[j] = pool[j - 1];
		}
		disvecpool[j] = tempvec;
		pool[j] = tmpp;
	}
}

void updatevec(POOL pool, uint8_t index, vecpool disvecpool) {
	disvecpool[index].color[0] = getvec(id2dis(lookup(pool[index], 0)));
	disvecpool[index].color[1] = getvec(id2dis(lookup(pool[index], 1)));
}

void add2pool(Point last, POOL pool, uint8_t* psp, vecpool disvecpool) {

	for (uint8_t i = 0; i < (*psp) + 1; i++) {
		//updatevec(pool, i, disvecpool);
		if (pool[i].x == last.x && pool[i].y == last.y) {
			pool[i] = pool[--(*psp)];
			//updatevec(pool, i, disvecpool);
			break;
		}
	}
	for (uint8_t i = 0; i < 4; i++) {
		for (uint8_t j = 0; j < 2; j++) {
			Point tmpp = last;
			for (uint8_t k = 0; k < 4; k++) {
				tmpp = move(tmpp, i, j);
				if (verify_location(tmpp) && board[tmpp.x][tmpp.y] > 3) {
					uint8_t l = 0;
					while (!(pool[l].x == tmpp.x && pool[l].y == tmpp.y) && l < (*psp)) {
						l++;
					}
					if (l == (*psp)) {
						(*psp)++;
					}
					pool[l] = tmpp;
					//updatevec(pool, l, disvecpool);
				}
				else break;
			}
		}
	}
}

Point calcboard(uint8_t color, POOL pool, uint8_t* psp, vecpool disvecpool) {
	Point minmyP = { 7,7 };
	disvec minmydis;
	Point minopP = { 7,7 };
	disvec minopdis;
	disvec minoppare;
	disvec minmypare;
	Point resP;
	for (uint8_t i = 0; i < 5; i++) {
		minmydis.form[i] = 10;
		minopdis.form[i] = 10;
		minoppare.form[i] = 10;
		minmypare.form[i] = 10;
	}
	disvec mydis;
	disvec opdis;
	for (uint8_t i = 0; i < (*psp) + 1; i++) {
		updatevec(pool, i, disvecpool);
	}
	sortvecpool(disvecpool, pool, *psp, !color);
	sortvecpool(disvecpool + 10, pool + 10, (*psp) - 10, color);
	for (uint8_t i = 0; i < (*psp); i++) {
		Point P = pool[i];
		mydis = disvecpool[i].color[color];
		opdis = disvecpool[i].color[!color];
		if (equal(mydis, minmydis)) {
			if (get_surroundings(P, color) > get_surroundings(minmyP, color)) {
				minmyP = P;
				minmydis = mydis;
				minmypare = opdis;
			}
		}
		if (smaller(mydis, minmydis)) {
			minmyP = P;
			minmydis = mydis;
			minmypare = opdis;
		}
		if (equal(opdis, minopdis)) {
			if (get_surroundings(P, !color) > get_surroundings(minopP, !color)) {
				minopP = P;
				minopdis = opdis;
				minoppare = mydis;
			}
		}
		if (smaller(opdis, minopdis)) {
			minopP = P;
			minopdis = opdis;
			minoppare = mydis;
		}
	}
	resP = cross_compare(minmydis, minopdis) ? minmyP : minopP;
	add2pool(resP, pool, psp, disvecpool);
	return resP;
}
*/