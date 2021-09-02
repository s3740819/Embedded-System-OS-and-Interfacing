#include "../header/framebf.h"
#include "../header/font/font.h"
#include "../header/uart.h"
#include "../header/font/picture/font_storage.h"

void drawChar(unsigned char c, int x1, int y1, unsigned int attr) {
	for(int y = y1, a = 0; y < (y1 + 8); y++, a++) {
		for(int x = x1, b = 0; x < (x1 + 8); x++, b++) {
			if(font[c][a] & (1 << b)) drawPixelARGB32(x, y, attr, 0);
		}
	}
}

void drawString(char *s, int x, int y, unsigned int attr) {
	while(*s) {
		drawChar(*s, x, y, attr);
		s++;
		x+=8;
	}
}

void execute_font_task(){
	for (int i = 0, x=0, y=0; y< 768; i++){
		drawPixelARGB32(x, y, font_bg[i], 0);
		x++;
		if(x == 1024){
			x = 0;
			y++;
		}
	}
	drawString("EEET2490 - Embedded System: OS and Interfacing", 80, 250, 0xf2ef16);
	drawString("Nguyen Le - s3777242", 120, 300, 0x00FF0000);
	drawString("Bao Tran - s3740819", 140, 350, 0x0000FF00);
	drawString("Phuc To - s3758272", 160, 400, 0x16f2ac);
	while(uart_getc() != 27);
	resetScreen();
}