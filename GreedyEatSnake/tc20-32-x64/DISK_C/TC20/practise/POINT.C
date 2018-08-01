#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<bios.h>

#define ESC 		 0x011b

#define key_error    -1

#define key_up		 0x4800
#define key_down	 0x5000
#define key_left	 0x4b00
#define key_right    0x4d00

#define delay_time   10000
int const key[] = {key_up,key_down,key_left,key_right};

void showPoint(int oldRow, int oldCol,int row, int col, char point);
int getPointDirectByKey(int keyInput);
void movePointByKey(int direct, int *row, int *col);
void delay(unsigned int xms);

void delay(unsigned int xms) {
	unsigned int x, y;

	for(x = xms; x > 0; x--)
		for(y = 110; y > 0; y--)
			;
}

void movePointByKey(int direct, int *row, int *col) {
	int addData[4][2]={-1,0,1,0,0,-1,0,1};

	*col += addData[direct][0];
	*row += addData[direct][1];
}

void showPoint(int oldRow, int oldCol,int row, int col, char point) {
	gotoxy(oldRow, oldCol);
	printf(" ");

	gotoxy(row, col);
	printf("%c",point);
}

int getPointDirectByKey(int keyInput) {
	int i;

	for(i = 0; i < 4; i++) {
		if(key[i] == keyInput) {
			return i;
		}
	}
	return key_error;
}

int main(void) {
	char snakePoint = '0';
	int beginRow=15;
	int beginCol=16;
	int oldRow = beginRow;
	int oldCol = beginCol;
	int getDirectByKey = 3 ;
	int keyInput = 0;
	int beginspeed = 1400;

	system("cls");
	showPoint(beginRow, beginCol, beginRow, beginCol, snakePoint);

	while(ESC != keyInput) {
		if(bioskey(1)) {
			keyInput = bioskey(0);
			getDirectByKey = getPointDirectByKey(keyInput);
		}
		while(!bioskey(1)) {
			oldRow = beginRow;
			oldCol = beginCol;
			if(getDirectByKey != -1) {
				movePointByKey(getDirectByKey, &beginRow, &beginCol);
				delay(beginspeed);
				showPoint(oldRow, oldCol, beginRow, beginCol, snakePoint);
			}
		}
		showPoint(oldRow, oldCol, beginRow, beginCol, snakePoint);
		keyInput = bioskey(0);
		getDirectByKey = getPointDirectByKey(keyInput);
	}
	return 0;
}
