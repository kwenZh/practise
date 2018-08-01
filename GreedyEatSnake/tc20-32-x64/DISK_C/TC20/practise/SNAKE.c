#include<stdio.h>  
#include<stdlib.h>  
#include<dos.h>  
#include<bios.h>  
#include<conio.h>  
#include<time.h>  
  
#define ARROW_UP    0x4800  
#define ARROW_DOWN  0x5000  
#define ARROW_LEFT  0x4B00  
#define ARROW_RIGHT 0x4D00  
#define ESC         0x011B  
  
#define DIRECT_UP       0  
#define DIRECT_LEFT     1  
#define DIRECT_DOWN     2  
#define DIRECT_RIGHT    3  
#define DIRECT_ERROR    -1  
  
#define MAX_ROW     15  
#define MAX_COL     20  
  
#define MAX_LENGTH  50  
  
#define DEFAULT_COUNT   5  
#define DEFAULT_LEVEL   2  
#define DEFAULT_DIRECT  DIRECT_RIGHT  
  
char HOLE_headShow[4] = {'^', '<', 'V', '>'};  
char HOLE_bodyShow[2] = {'-', '|'};  
  
int HOLE_direction[4][2] = {  
    0, -1,      /*UP*/  
    -1, 0,      /*LEFT*/  
    0, 1,       /*DOWN*/  
    1, 0        /*RIGHT*/  
};  
  
int HOLE_keyName[4] = {  
    ARROW_UP, ARROW_LEFT, ARROW_DOWN, ARROW_RIGHT,  
};  
  
int const HOLE_delayTime[5] = {  
    1000, 700, 300, 200, 100,  
};  
  
#define DEFAULT_LENGTH  4  
  
#define TRUE    1  
#define FALSE   0  
  
typedef struct POINT {  
    int x;  
    int y;  
    int direct;  
}POINT;  
  
typedef struct SNAKE {  
    POINT snake[MAX_LENGTH];  
    int isAlive;  
    int headIndex;  
    int tailIndex;  
    int snakeLength;  
    int level;  
    POINT food;  
}SNAKE;  
  
void initSnake(SNAKE *snake);  
void showBodyPoint(POINT point);  
void showHeadPoint(POINT point);  
void showFoodPoint(POINT food);  
void moveSnake(SNAKE *snake);  
void deletePoint(POINT point);  
int getDirectByKeyName(int keyName);  
void addSnakeLength(SNAKE *snake, int addLength);  
POINT getFood(SNAKE *snake);  
int isPartOfSnake(SNAKE snake, POINT point);  
void moveOnceOnDirect(SNAKE *snake, int direct);  
  
void moveOnceOnDirect(SNAKE *snake, int direct) {  
    POINT newPoint;  
  
    newPoint.x = snake->snake[snake->headIndex].x + HOLE_direction[direct][0];  
    newPoint.y = snake->snake[snake->headIndex].y + HOLE_direction[direct][1];  
  
    if (isPartOfSnake(*snake, newPoint)) {  
        snake->isAlive = FALSE;  
    }  
    showBodyPoint(snake->snake[snake->headIndex]);  
  
    snake->headIndex = (snake->headIndex + 1) % MAX_LENGTH;  
    snake->snake[snake->headIndex] = newPoint;  
    snake->snake[snake->headIndex].direct = direct;  
  
    showHeadPoint(snake->snake[snake->headIndex]);  
  
    deletePoint(snake->snake[snake->tailIndex]);  
    snake->tailIndex = (snake->tailIndex + 1) % MAX_LENGTH;  
}  
  
int isPartOfSnake(SNAKE snake, POINT point) {  
    int i;  
  
    for(i = snake.headIndex; i !=  snake.tailIndex; i = (MAX_LENGTH + i - 1) % MAX_LENGTH) {  
        if(point.x == snake.snake[i].x && point.y == snake.snake[i].y) {  
            return TRUE;  
        }  
    }  
    if(point.x == snake.snake[i].x && point.y == snake.snake[i].y) {  
        return TRUE;  
    }  
    return FALSE;  
}  
  
POINT getFood(SNAKE *snake) {  
    char point[MAX_ROW * MAX_COL] = {0};  
    int pointIndex[MAX_ROW *MAX_COL] = {0};  
    int i;  
    int j = 0;  
    int foodIndex;  
    POINT food;  
  
    for(i = snake->headIndex; i != snake->tailIndex; i = (i - 1 + MAX_LENGTH) % MAX_LENGTH) {  
        point[snake->snake[i].x + (snake->snake[i].y - 1) * MAX_COL - 1] = 1;  
    }  
  
    point[snake->snake[i].x + (snake->snake[i].y - 1) *MAX_COL - 1] = 1;  
  
    for(i = 0; i < MAX_ROW *MAX_COL; i++) {  
        if(0 == point[i]) {  
            pointIndex[j++] = i;  
        }  
    }  
    srand(time(0));  
    foodIndex = pointIndex[rand() % (MAX_ROW * MAX_COL - snake->snakeLength)];  
    food.x = foodIndex % MAX_COL + 1;  
    food.y = foodIndex / MAX_COL + 1;  
  
    showFoodPoint(food);  
  
    return food;  
}  
  
void addSnakeLength(SNAKE *snake, int addLength) {  
    int i;  
    POINT tailIndexData;  
  
    tailIndexData = snake->snake[snake->tailIndex];  
  
    for(i = 1; i <= addLength; i++) {  
        snake->snake[(snake->tailIndex - i + MAX_LENGTH) % MAX_LENGTH] = tailIndexData;  
    }  
    snake->tailIndex = (MAX_LENGTH + snake->tailIndex - addLength) % MAX_LENGTH;  
    snake->snakeLength += addLength;  
}  
  
int getDirectByKeyName(int keyName) {  
    int i;  
  
    for(i = 0; i < 4; i++) {  
        if(keyName == HOLE_keyName[i]) {  
            return i;  
        }  
    }  
  
    return DIRECT_ERROR;  
}  
  
void deletePoint(POINT point) {  
    gotoxy(point.x, point.y);  
  
    printf(" ");  
}  
  
void moveSnake(SNAKE *snake) {  
    int keyName = ARROW_RIGHT;  
    int direct = DIRECT_RIGHT;  
    int isEating = FALSE;  
  
    while(keyName != ESC && snake->isAlive && snake->snakeLength < MAX_LENGTH) {  
        moveOnceOnDirect(snake, direct);  
  
        if (snake->snake[snake->headIndex].x == snake->food.x && snake->snake[snake->headIndex].y == snake->food.y) {  
            isEating = TRUE;  
        }  
  
        if(TRUE == isEating) {  
            snake->food = getFood(snake);  
            addSnakeLength(snake, 3);  
            isEating = FALSE;  
        }  
  
        delay(HOLE_delayTime[snake->level]);  
  
        if(bioskey(1)) {  
            keyName = bioskey(0);  
  
            if (DIRECT_ERROR != getDirectByKeyName(keyName)) {  
                direct = getDirectByKeyName(keyName);  
            }  
        }  
    }  
    printf("isAlive:%d, snakeLength:%d, ", snake->isAlive, snake->snakeLength);  
}  
  
void showFoodPoint(POINT food) {  
    gotoxy(food.x, food.y);  
  
    printf("@");  
}  
  
void showHeadPoint(POINT point) {  
    gotoxy(point.x, point.y);  
  
    printf("%c", HOLE_headShow[point.direct]);  
}  
  
void showBodyPoint(POINT point) {  
    gotoxy(point.x, point.y);  
  
    printf("%c", HOLE_bodyShow[(point.direct + 1) % 2]);  
}  
  
void initSnake(SNAKE *snake) {  
    int i;  
  
    snake->isAlive = TRUE;  
    snake->headIndex = DEFAULT_LENGTH - 1;  
    snake->tailIndex = 0;  
    snake->snakeLength = DEFAULT_LENGTH;  
    snake->level = DEFAULT_LEVEL;  
  
    for(i = 0; i < snake->snakeLength; i++) {  
        snake->snake[i].x = i + 1;  
        snake->snake[i].y = 1;  
        snake->snake[i].direct = DEFAULT_DIRECT;  
        DEFAULT_LENGTH - i - 1 == 0 ? showHeadPoint(snake->snake[i]) :  
            showBodyPoint(snake->snake[i]);  
    }  
    snake->food = getFood(snake);  
    getchar();  
}  
  
void main(void) {  
    SNAKE snake = {0};  
  
    system("cls");  
    initSnake(&snake);  
    moveSnake(&snake);  
    getchar();  
}  