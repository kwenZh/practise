#include<stdio.h>
#include<bios.h>
#include<stdlib.h>
#include<dos.h>

main() {
	int key;
	int i = 0;
	int j = 0;

	system("cls");
	for(;;) {
		key = bioskey(0);

		if(key == 0x11b) {
			break;
		}else {
			gotoxy(i,j);
			printf("(%d,%d) 0x%x ",i,j, key);
			++i;
			++j;
		}
	}
}