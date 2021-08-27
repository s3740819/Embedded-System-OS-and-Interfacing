#include "../header/display.h"
#include "../header/framebf.h"
#include "../header/avatar.h"

void displayPicture(int width, int height, unsigned int *picture) {
	for (int y = 0, x = 0, i = 0; y < height; i++) {
		drawPixelARGB32(x, y, *picture);
		x++;
		*picture++;
		if (x == width){
			y++;
			x = 0;
		}
	}
}

void display_character(int x, int y, int animal){
	int startx = x;
	int starty = y;
	for (int i = 0; y < 79+starty; i++ ){
		if (animal == 0){
			if (bear[i] != 0x00000000) {
				drawPixelARGB32(x, y, bear[i]);
			}
		}
		else if (animal == 1){
			if (rabit[i] > 16777215 || rabit[i] < 14521772) drawPixelARGB32(x, y, rabit[i]);
		}
		else {
			if (dog[i] > 16777215 || dog[i] < 5066061) drawPixelARGB32(x, y, dog[i]);
		}
		x++;
		if (x == startx + 60){
			y ++;
			x = startx;
		}
	}
}

void select(int animal){
	int x1;
	int y1 = 110;
	if (animal == 0) x1 = 280;
	else if (animal == 1) x1 = 490;
	else x1 = 700;

	int y2 = y1 + 78;
	int x2 = x1 + 59;
	for(int y = y1; y <= y2; y++ )
		for(int x = x1; x <= x2; x++) {
			if((x == x1 || x == x2) || (y == y1 || y == y2))
				drawPixelARGB32(x, y, 0x00FFFF00);
		}
}






