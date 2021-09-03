#include "../header/piStuffs/framebf.h"
#include "../header/font/font.h"
#include "../header/piStuffs/uart.h"
#include "../header/font/picture/font_storage.h"

/**
 * Draw a given character
 */ 
void drawChar(unsigned char c, int x1, int y1, unsigned int attr) {
	// For each row
	for(int y = y1, a = 0; y < (y1 + 8); y++, a++) {
		// For each column(pixel)
		for(int x = x1, b = 0; x < (x1 + 8); x++, b++) {
			// If there is value 1 -> draw the color
			if(font[c][a] & (1 << b)) drawPixelARGB32(x, y, attr, 0);
		}
	}
}

/**
 *  Draw a string to the screen
 */
void drawString(char *s, int x, int y, unsigned int attr) {
	// For every character
	while(*s) {
		// Draw it
		drawChar(*s, x, y, attr);
		s++;
		x+=8;
	}
}


/**
 *  Execute the "font" task
 */
void execute_font_task(){
	
	// Draw background first
	for (int i = 0, x=0, y=0; y< 768; i++){
		drawPixelARGB32(x, y, font_bg[i], 0);
		x++;
		if(x == 1024){
			x = 0;
			y++;
		}
	}
	
	// Draw each string (course name and each teammember's name)
	drawString("EEET2490 - Embedded System: OS and Interfacing", 80, 250, 0xf2ef16);
	drawString("Nguyen Le - s3777242", 120, 300, 0x00FF0000);
	drawString("Bao Tran - s3740819", 140, 350, 0x0000FF00);
	drawString("Phuc To - s3758272", 160, 400, 0x16f2ac);
	
	// If the use finish viewing, press esc to exit
	while(uart_getc() != 27);
	
	// Clear the screen
	resetScreen();
}