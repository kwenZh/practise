#include <stdio.h>
#include <bios.h>
#include <conio.h>

#define	HIGH	1
#define LOW		0

typedef union SCAN_CODE {
	int keyName;
	unsigned char byte[2];
}SCAN_CODE;

#define ESC			0x011B

#define ARROW_UP	0x4800
#define ARROW_DOWN	0x5000
#define ARROW_LEFT	0x4B00
#define ARROW_RIGHT	0x4D00

#define HOME		0x4700
#define END			0x4F00
#define ENTER		0x1C0D

#define SPACE		0x3920

void main(void) {
	SCAN_CODE key = { 0 };
	char circleStick[4] = { '-', '\\', '|', '/' };
	int circleStickIndex = 0;

	while(ESC != key.keyName) {
		if(bioskey(1)) {
			key.keyName = bioskey(0);
			gotoxy(1, 1);
			printf("%d(%02X %02X)\n", key.keyName,
				key.byte[LOW], key.byte[HIGH]);
		}
		gotoxy(40, 12);
		printf("%c", circleStick[circleStickIndex++]);
		circleStickIndex %= 4;
	}

	gotoxy(1, 1);
	printf("%d(%02X %02X)\n", key.keyName,
		key.byte[LOW], key.byte[HIGH]);
}
