#include <bios.h>
#include <conio.h>

#define KEY_ESC		 0X11B
#define KEY_NO_PRESS 0

void main(void) {
	int keyvalue = 0;

	while(keyvalue != KEY_ESC) {
		if(bioskey(1) != KEY_NO_PRESS) {
			keyvalue = bioskey(0);
		}
		printf("you press key: 0x%X\n", keyvalue);
		if(keyvalue == KEY_ESC) {
			printf("stop!\n");
		}
	}

	getch();
}