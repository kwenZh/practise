#include <stdio.h>
#include <bios.h>
#include <conio.h>
#include <stdlib.h>

#define DIRECT_UP		0
#define DIRECT_DOWN		1
#define DIRECT_LEFT		2
#define DIRECT_RIGHT	3

#define	KEY_ERROR		-1

#define ESC				0x011B

#define ARROW_UP		0x4800
#define ARROW_DOWN		0x5000
#define ARROW_LEFT		0x4B00
#define ARROW_RIGHT		0x4D00

#define HOME			0x4700
#define END				0x4F00
#define ENTER			0x1C0D

#define SPACE			0x3920

#define DELAY_TIME		10000

int const key[] = {
	ARROW_UP,
	ARROW_DOWN,
	ARROW_LEFT,
	ARROW_RIGHT,
};

void showPointAt(int originRow, int originCol, int targetRow, int targetCol,
		char point);
void moveOnceAtDirect(int direct, int *row, int *col);
int getDirectByInputKey(int keyName);
/*
void delay(long delayTime);

void delay(long delayTime) {
	long i;

	for(i = 0; i < delayTime; i++)
		;
}
*/
int getDirectByInputKey(int keyName) {
	int i;
	int directCount = sizeof(key) / sizeof(int);

	for(i = 0; i < directCount; i++) {
		if(key[i] == keyName) {
			return i;
		}
	}

	return KEY_ERROR;
}

void moveOnceAtDirect(int direct, int *row, int *col) {
	int addDelta[][2] = {
		-1, 0,
		1, 0,
		0, -1,
		0, 1,
	};

	if(*row > 1 && *row < 24) {
		*row += addDelta[direct][0];
	}
	if(*col > 1 && *col < 79) {
		*col += addDelta[direct][1];
	}
}

void showPointAt(int originRow, int originCol, int targetRow, int targetCol,
		char point) {
	gotoxy(originCol, originRow);
	printf(" ");

	gotoxy(targetCol, targetRow);
	printf("%c", point);
}

void main(void) {
	char point = '*';
	int row = 12;
	int col = 40;
	int rowSave = row;
	int colSave = col;
	int direct = DIRECT_RIGHT;
	int keyIn = 0;
	long delayTime = 0;

	system("cls");
	showPointAt(row, col, row, col, point);

	while(ESC != keyIn) {
		if(bioskey(1)) {
			keyIn = bioskey(0);
			direct = getDirectByInputKey(keyIn);
			delayTime = DELAY_TIME - 1;
		}
		if(++delayTime >= DELAY_TIME) {
			rowSave = row;
			colSave = col;
			moveOnceAtDirect(direct, &row, &col);
			showPointAt(rowSave, colSave, row, col, point);
			delayTime = 0;
		}
	}
}
