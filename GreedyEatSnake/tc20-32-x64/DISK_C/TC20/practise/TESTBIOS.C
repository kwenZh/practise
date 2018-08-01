#include<stdio.h>
#include<bios.h>
#include<stdlib.h>

#define ESC 0x11b

/*
void delay(unsigned int xms);
void delay(unsigned int xms) {
	unsigned int x,y;
	for(x = xms; x > 0; x--)
		for(y = 110; y > 0; y--)
			;
}*/

int main(void) {
	int keyIn = 0;
	int beginspeed = 0;
	int delay_time = 5000;

	system("cls");
	while(ESC != keyIn) {
		if(bioskey(1)) {
			keyIn = bioskey(0);
		}
		
		++beginspeed;
		if(beginspeed == delay_time) {
			printf("0x%x", keyIn);
			beginspeed = 0;
		}
		
		if(ESC == keyIn) {
			puts("Input stop\n");
		}
	}
	puts("press any key return\n");
	getch();
	
}
