#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<bios.h>

#define ESC 		 0x011b

#define KEY_ERROR    -1

#define KEY_UP		 0x4800
#define KEY_DOWN	 0x5000
#define KEY_LEFT	 0x4b00
#define KEY_RIGHT    0x4d00

#define DIRECT_UP	 0
#define DIRECT_DOWN  1
#define DIRECT_LEFT  2
#define DIRECT_RIGHT 3

#define WALL_WIDTH	 80
#define WALL_HEIGHT  25

#define INDEX_X 	 0
#define INDEX_Y      1

#define LEVE_COUNT	 5

#define LEVEL_1      0
#define LEVEL_2		 1
#define LEVEL_3		 2
#define LEVEL_4 	 3
#define LEVEL_5      4

#define LEVEL_1_DELAYTIME     	 8000
#define LEVEL_2_DELAYTIME		 7000
#define LEVEL_3_DELAYTIME		 6000
#define LEVEL_4_DELAYTIME 	 	 5000
#define LEVEL_5_DELAYTIME    	 3000

const char run_direct[] = {'^','v','<','>'};
const char snake_head[] = {'-','|'};
const int key[] = {KEY_UP,KEY_DOWN,KEY_LEFT,KEY_RIGHT};
const int GAME_LEVEL[LEVE_COUNT] = {LEVEL_1_DELAYTIME, LEVEL_2_DELAYTIME, LEVEL_3_DELAYTIME, LEVEL_4_DELAYTIME, LEVEL_5_DELAYTIME};

typedef struct POINT {
	int x;
	int y;
	int direct;
}POINT;

typedef struct SNAKE {
	int alive;
}SNAKE;

void showPoint(POINT headPoint);
void clearPoint(POINT headPoint);
int getPointDirectByKey(int keyInput);
void movePointByKey(int direct, int *row, int *col);
void beiginRun(POINT headPoint);

void movePointByKey(int direct, int *row, int *col) {
	int addData[4][2]={0,-1,0,1,-1,0,1,0};

	if(*col > 0 && *col < WALL_HEIGHT) {
		*col += addData[direct][INDEX_Y];
	}

	if(*row > 0 && *row < WALL_WIDTH) {
		*row += addData[direct][INDEX_X];
	}
}

void showPoint(POINT headPoint) {
	gotoxy(headPoint.x, headPoint.y);
	putchar(run_direct[headPoint.direct]);
}

void clearPoint(POINT headPoint) {

	gotoxy(headPoint.x, headPoint.y);
	putchar(' ');
}

int getPointDirectByKey(int keyInput) {
	int directIndex;

	for(directIndex = 0; directIndex < 4; directIndex++) {
		if(key[directIndex] == keyInput) {
			return directIndex;
		}
	}
	return KEY_ERROR;
}

void beiginRun(POINT headPoint) {
	int keyInput = 0;
	int beginspeed = 0;
	int delay_time = GAME_LEVEL[LEVEL_1];  /*The game difficulty*/

	system("cls");
	while(ESC != keyInput) {
		if(bioskey(1)) {
			keyInput = bioskey(0);
			headPoint.direct = (KEY_ERROR != getPointDirectByKey(keyInput)) ? getPointDirectByKey(keyInput) : headPoint.direct;
		}
		
		++beginspeed;
		if(beginspeed == delay_time) {
			clearPoint(headPoint);
			movePointByKey(headPoint.direct, &headPoint.x, &headPoint.y);
			showPoint(headPoint);
			beginspeed = 0;
		}
		if(ESC == keyInput) {
			puts("Remove stop\n");
		}
	}
	puts("press any key return\n");
	getch();
}

int main(void) {
	POINT headPoint = {0};
	headPoint.x = 15;
	headPoint.y = 16;
	headPoint.direct = DIRECT_RIGHT;

	beiginRun(headPoint);

	return 0;
}
