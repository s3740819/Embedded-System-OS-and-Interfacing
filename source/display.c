#include "../header/display.h"
#include "../header/framebf.h"
#include "../header/avatar.h"
#include "../header/map1.h"
#include "../header/objects.h"

void displayPicture(int width, int height, unsigned int *picture) {
	for (int y = 0, x = 0, i = 0; y < height; i++) {
		drawPixelARGB32(x, y, *picture);
		x++;
		picture++;
		if (x == width){
			y++;
			x = 0;
		}
	}
}

void display_avatars(int x, int y, int animal){
	int startx = x;
	int starty = y;
	for (int i = 0; y < 100+starty; i++ ){
		if (animal == 2){
			if (dogface[i] > 13891866 || dogface[i] < 11313592) {
				drawPixelARGB32(x, y, dogface[i]);
			}
		}
		else if (animal == 1){
			if (foxface[i] > 0xffffff || foxface[i] < 0xfffbef) drawPixelARGB32(x, y, foxface[i]);
		}
		else {
			if (bearface[i] > 6075929 || bearface[i] < 901810) drawPixelARGB32(x, y, bearface[i]);
		}
		x++;
		if (x == startx + 100){
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

	int y2 = y1 + 100;
	int x2 = x1 + 100;
	for(int y = y1; y <= y2; y++ )
		for(int x = x1; x <= x2; x++) {
			if((x == x1 || x == x2) || (y == y1 || y == y2))
				drawPixelARGB32(x, y, 0x00FFFF00);
		}
}

void drawAvatar(int x, int y, int animal){
	int startx = x;
	int starty = y;
	for (int i = 0; y < 50+starty; i++ ){
		if (animal == 0){
			if (bear[i] != 0x00000000) {
				drawPixelARGB32(x, y, bear[i]);
			}
		}
		else if (animal == 1){
			if (fox[i] > 7914368 || fox[i] < 5202757) drawPixelARGB32(x, y, fox[i]);
		}
		else {
			if (dog[i] < 8066061) drawPixelARGB32(x, y, dog[i]);
		}
		x++;
		if (x == startx + 38){
			y ++;
			x = startx;
		}
	}
}

void avatarMove(int x, int y){
	int startx = x;
	int starty = y;
	for (int i = (1024*starty) + startx; y < 79+starty; i++ ){
		drawPixelARGB32(x, y, map1[i]);
		x++;
		if (x == startx + 60 || x == 1024){
			y ++;
			x = startx;
			i = (1024*y) + x;
		}
	}
	if (x+60 >=973 && y<= 60){
		draw_gate1();
	}
}

void display_map1(){
	for (int y = 0, x = 0, i = 0; y < 768; i++ ){
		drawPixelARGB32(x, y, map1[i]);
		x++;
		if (x == 1024){
			y ++;
			x = 0;
		}
	}
	draw_gate1();

}

void draw_gate1(){
	for (int i = 0, x = 965, y = 0; y < 50; i++){
		//if(hole[i] > 16777215 || hole[i] < 12900000) drawPixelARGB32(x, y, hole[i]);
		if(hole[i] < 12900000) drawPixelARGB32(x, y, hole[i]);
		x++;
		if (x == 1015){
			y ++;
			x = 965;
		}
	}
}

void drawCar(int x, int y, int is_left) { //draw a car of 90x50 pixels at x and y on the screen
	int startx = x;
	int starty = y;
	for (int i = 0; y < 50 + starty; i++ ){
		if (is_left){
			if (car_left[i] < 8066061) drawPixelARGB32(x, y, car_left[i]);
		}
		else if (car_right[i] < 8066061) drawPixelARGB32(x, y, car_right[i]);
		x++;
		if (x == startx + 90){
			y ++;
			x = startx;
		}
	}
}

void carMove(int x, int y) { //draw a 90x50 pixel background where a car was previously drawn
	int startx = x;
	int starty = y;
	for (int i = (1024*starty) + startx; y < 50+starty; i++ ){
		drawPixelARGB32(x, y, map1[i]);
		x++;
		if (x == startx + 90){
			y ++;
			x = startx;
			i = (1024*y) + x;
		}
	}
}


