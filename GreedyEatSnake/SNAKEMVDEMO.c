#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<bios.h>
#include<math.h>

#define ESC 		 		0x011b
#define ENTER				0x1c0d
#define SPACE 				0x3920

#define KEY_ERROR    		-1

#define KEY_UP_VALUE		0x4800
#define KEY_LEFT_VALUE	 	0x4b00
#define KEY_DOWN_VALUE	 	0x5000
#define KEY_RIGHT_VALUE    	0x4d00

#define DIRECT_UP			0
#define DIRECT_LEFT  		1
#define DIRECT_DOWN  		2
#define DIRECT_RIGHT 		3

#define MAX_X	 			80
#define MAX_Y		  		25

#define MIN_X				1
#define MIN_Y				5

#define INDEX_X 	 		0
#define INDEX_Y      		1

#define LEVEL_1      		0
#define LEVEL_2		 		1
#define LEVEL_3		 		2  
#define LEVEL_4 	 		3
#define LEVEL_5      		4

#define LEVEL_1_DELAYTIME	8000
#define LEVEL_2_DELAYTIME	6000
#define LEVEL_3_DELAYTIME	4000
#define LEVEL_4_DELAYTIME 	3000
#define LEVEL_5_DELAYTIME   2000

#define MAX_LENGTH			1024
#define BEIGIN_LENGTH 		2

#define TRUE				1
#define FALSE				0

const char snakeHead_Global[] = {'^','<','v','>'};
const char snakeBody_Global[] = {'|','-'};
const int directKeyValue_Global[] = {
	KEY_UP_VALUE,
 	KEY_LEFT_VALUE,
 	KEY_DOWN_VALUE,
 	KEY_RIGHT_VALUE};
const int GAME_LEVEL_Global[] = {LEVEL_1_DELAYTIME, LEVEL_2_DELAYTIME, LEVEL_3_DELAYTIME, LEVEL_4_DELAYTIME, LEVEL_5_DELAYTIME};

typedef unsigned char boolean;

typedef struct POINT {
	int x;
	int y;
}POINT;

typedef struct SNAKE_NODE {
	POINT node;
	int direct;
}SNAKE_NODE;

typedef struct SNAKE {
	SNAKE_NODE snake[MAX_LENGTH];
	POINT food;
	int headIndex;
	int tailIndex;
	int isAlive;
	int snake_length;
	int game_level;
	boolean snake_is_run;
}SNAKE;

void showHead(SNAKE_NODE snakeHead);
void showBody(SNAKE_NODE snakeBody);
void showRandomFood(SNAKE *mySnake);
void showOver(int score);
void showMapHead(SNAKE *mySnake);
void clearTail(SNAKE_NODE snakeBody);
void initSnake(SNAKE *mySnake);
int getPointDirectByKey(int keyInput);
void snakeMoveOnceByDirect(SNAKE *mySnake, int direct);
void judgeAlive(SNAKE *mySnake);
void beginMoveSnake(SNAKE *mySnake);
void snakeEatFood(SNAKE *mySnake, int addLength);
void addSnakeLength(SNAKE *mySnake, int length);
boolean dealSpaceKey(boolean snake_is_run, int keyIn);

boolean dealSpaceKey(boolean snake_is_run, int keyIn) {
	if(snake_is_run == TRUE) {
		gotoxy(70, 3);
		puts("PAUSE!");
	}else{
		gotoxy(70, 3);
		puts("      ");
	}

	return (keyIn == SPACE) ? !snake_is_run : snake_is_run;
}

void showMapHead(SNAKE *mySnake) {
	gotoxy(5,3);
	printf("LENGTH-%d\t\tScore-%d\t\tLEVEL:%d ms",
		mySnake->snake_length, (mySnake->snake_length-2)*5, mySnake->game_level);
}

void addSnakeLength(SNAKE *mySnake, int length) {
	int i;

	for(i = 0; i < length; i++) {
		SNAKE_NODE oldTailNode = mySnake->snake[mySnake->tailIndex];
		mySnake->tailIndex = (mySnake->tailIndex - 1 + MAX_LENGTH) % MAX_LENGTH;
		mySnake->snake[mySnake->tailIndex] = oldTailNode;
		mySnake->snake_length++;
	}
	showMapHead(mySnake);
}

void snakeEatFood(SNAKE *mySnake, int addLength) {
	if((mySnake->food.x == mySnake->snake[mySnake->headIndex].node.x) && 
		(mySnake->food.y == mySnake->snake[mySnake->headIndex].node.y)) {
		addSnakeLength(mySnake, addLength);
		showRandomFood(mySnake);
	}
}


void showRandomFood(SNAKE *mySnake) {
	int allPoint[MAX_X * (MAX_Y - MIN_Y)] = {0};
	char snake[MAX_X * (MAX_Y - MIN_Y)] = {FALSE};
	int i;
	int index = 0;
	int random_index;

	for(i = mySnake->tailIndex; i != (mySnake->headIndex + 1) % MAX_LENGTH; i = (i + 1) % MAX_LENGTH) {
		snake[(mySnake->snake[i].node.y - MIN_Y) * MAX_X + mySnake->snake[i].node.x - 1] = TRUE;
	}

	for(i = 0; i < MAX_X * (MAX_Y - MIN_Y); i++) {
		if(snake[i] == FALSE) {
			allPoint[index++] = i + (MIN_Y * MAX_X) + 1;
		}
	}

	srand(time(NULL));
	random_index = (int)(rand() % index);
	mySnake->food.x = allPoint[random_index ] % MAX_X;
	mySnake->food.y = allPoint[random_index ] / MAX_X;

	gotoxy(mySnake->food.x, mySnake->food.y);
	putchar('*');
}

void showOver(int score) {
	gotoxy(30, 12);
	printf("\n\t\tGAME GG! YOUR SCORE = %d", score);
}

void judgeAlive(SNAKE *mySnake) {
	int i;

	for(i = mySnake->tailIndex; i != mySnake->headIndex ; i = (i + 1) % MAX_LENGTH) {
		if((mySnake->snake[mySnake->headIndex].node.x == mySnake->snake[i].node.x) && 
		   (mySnake->snake[mySnake->headIndex].node.y == mySnake->snake[i].node.y)) {
			mySnake->isAlive = FALSE;
		}
	}

	if(mySnake->snake[mySnake->headIndex].direct % 2 == 1) {
		if(mySnake->snake[mySnake->headIndex].node.x > MAX_X || mySnake->snake[mySnake->headIndex].node.x < MIN_X) {		
			mySnake->isAlive = FALSE;		
		}
	} else {
		if(mySnake->snake[mySnake->headIndex].node.y > MAX_Y || mySnake->snake[mySnake->headIndex].node.y < MIN_Y) {			
			mySnake->isAlive = FALSE;		
		}
	}
}

void snakeMoveOnceByDirect(SNAKE *mySnake, int direct){
	int addData[4][2]={0,-1,-1,0,0,1,1,0};
	int temphead = mySnake->headIndex;

	showBody(mySnake->snake[mySnake->headIndex]);
	clearTail(mySnake->snake[mySnake->tailIndex]);

	mySnake->headIndex = (mySnake->headIndex + 1) % MAX_LENGTH;
	mySnake->tailIndex = (mySnake->tailIndex + 1) % MAX_LENGTH;

	mySnake->snake[mySnake->headIndex].node.x = mySnake->snake[temphead].node.x + addData[direct][INDEX_X];
	mySnake->snake[mySnake->headIndex].node.y = mySnake->snake[temphead].node.y + addData[direct][INDEX_Y];
	mySnake->snake[mySnake->headIndex].direct = direct;

	judgeAlive(mySnake);
	showHead(mySnake->snake[mySnake->headIndex]);
	if(!mySnake->isAlive) {
		showOver((mySnake->snake_length - BEIGIN_LENGTH) * 5);
	}
}

void beginMoveSnake(SNAKE *mySnake) {
	int keyIn = 0;
	int speed = 0;
	int addLengthOnce = 3;
	int direct = mySnake->snake[mySnake->headIndex].direct;

	while(keyIn != ESC && mySnake->isAlive && mySnake->snake_length <= MAX_LENGTH ) {
		if(bioskey(1)) {
			keyIn = bioskey(0);
			mySnake->snake_is_run = (keyIn == SPACE) ? dealSpaceKey(mySnake->snake_is_run, keyIn) : mySnake->snake_is_run;
			direct = ((abs(getPointDirectByKey(keyIn) - direct) == 2) || getPointDirectByKey(keyIn) == KEY_ERROR || !mySnake->snake_is_run) 
				? direct : getPointDirectByKey(keyIn);
		}

		if(++speed >= mySnake->game_level && mySnake->snake_is_run) {
			snakeMoveOnceByDirect(mySnake, direct);		
			speed = 0;
		}
		snakeEatFood(mySnake, addLengthOnce);
	}
	
	printf("\n\n\t\tPress any key...");	
	getch();	
}

void initSnake(SNAKE *mySnake) {
	int i;
	int start_x = MIN_X;
	int start_direct = DIRECT_RIGHT;

	mySnake->snake_length = BEIGIN_LENGTH;                              
	mySnake->game_level = GAME_LEVEL_Global[LEVEL_3];	/*可以从键盘输入 1 2 3 4 5*/
	mySnake->tailIndex = 0;
	mySnake->headIndex = mySnake->snake_length - 1;
	mySnake->isAlive = TRUE;
	mySnake->snake_is_run = TRUE;
	
	for(i = 0; i < mySnake->snake_length; i++) {
		mySnake->snake[i].node.x = start_x;
		mySnake->snake[i].node.y = MIN_Y;
		mySnake->snake[i].direct = start_direct;
		start_x++;
	}

	system("cls");
	
	puts("--------------------------------------------------------------------------------\n\n--------------------------Press Spacebar Pause-----------------------------------");

	for(i = mySnake->tailIndex; i != mySnake->headIndex; i = (i + 1) % MAX_LENGTH) {
		showBody(mySnake->snake[i]);
	}
	showHead(mySnake->snake[mySnake->headIndex]);
	showRandomFood(mySnake);	
	showMapHead(mySnake);
}
void showHead(SNAKE_NODE snakeHead) {
	gotoxy(snakeHead.node.x, snakeHead.node.y);
	putchar(snakeHead_Global[snakeHead.direct]);
}

void showBody(SNAKE_NODE snakeBody) {
	gotoxy(snakeBody.node.x, snakeBody.node.y);
	putchar(snakeBody_Global[(snakeBody.direct) % 2]);
}

void clearTail(SNAKE_NODE snakeBody) {
	gotoxy(snakeBody.node.x, snakeBody.node.y);
	putchar(' ');
}

int getPointDirectByKey(int keyInput) {
	int directIndex;

	for(directIndex = 0; directIndex < 4; directIndex++) {
		if(directKeyValue_Global[directIndex] == keyInput) {
			return directIndex;
		}
	}
	return KEY_ERROR;
}

int main(void) {
	SNAKE *mySnake = NULL;

	mySnake = (SNAKE *)malloc(sizeof(SNAKE));
	initSnake(mySnake);
	beginMoveSnake(mySnake);

	free(mySnake);

	return 0;
}