#include "../header/game/display.h"
#include "../header/framebf.h"
#include "../header/game/picture/avatar.h"
#include "../header/game/picture/map1.h"
#include "../header/game/picture/map2.h"
#include"../header/game/picture/map3.h"
#include "../header/game/picture/objects.h"

void displayPicture(int width, int height, const unsigned int picture[]) {
	for (int y = 0, x = 0, i = 0; y < height; i++) {
		drawPixelARGB32(x, y, picture[i], 0);
		x++;
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
				drawPixelARGB32(x, y, dogface[i], 0);
			}
		}
		else if (animal == 1){
			if (foxface[i] > 0xffffff || foxface[i] < 0xfffbef) drawPixelARGB32(x, y, foxface[i], 0);
		}
		else {
			if (bearface[i] > 6075929 || bearface[i] < 901810) drawPixelARGB32(x, y, bearface[i], 0);
		}
		x++;
		if (x == startx + 100){
			y ++;
			x = startx;
		}
	}
}

void display_skull(){
	int x = 362;
	int y = 234;
	int startx = x;
	int starty = y;
	for (int i = 0; y < 300 + starty; i++ ){
		if (dead_ava[i] != 0x00ffffff && dead_ava[i] != 0xfcfcfc && dead_ava[i] != 0xfefefe && dead_ava[i] != 0xfafafa) drawPixelARGB32(x, y, dead_ava[i], 0);
		x++;
		if (x == startx + 300){
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
				drawPixelARGB32(x, y, 0x00FFFF00, 0);
		}
}

void drawAvatar(int x, int y, int animal, int is_goup){
	int startx = x;
	int starty = y;
	for (int i = 0; y < 50+starty; i++ ){
		if (animal == 0){
			if (bear[i] != 0x00000000 && is_goup) drawPixelARGB32(x, y, bear[i], 0);
			else if (bear_back[i] != 0 && !is_goup) drawPixelARGB32(x, y, bear_back[i], 0);
		}
		else if (animal == 1){
			if ((fox[i] > 7914368 || fox[i] < 5202757) && is_goup) drawPixelARGB32(x, y, fox[i], 0);
			else if ((fox_back[i] != 0x7dc183) && !is_goup) drawPixelARGB32(x, y, fox_back[i], 0);
			
		}
		else {
			if (dog[i] < 8066061 && is_goup) drawPixelARGB32(x, y, dog[i], 0);
			else if (dog_back[i] < 8066061 && !is_goup) drawPixelARGB32(x, y, dog_back[i], 0);
		}
		x++;
		if (x == startx + 38){
			y ++;
			x = startx;
		}
	}
}

void drawLog(int x, int y, int is_lose){
	int startx = x;
	int starty = y;
	for (int i = 0; y < 60 + starty; i++ ){
		if (log[i] != 0x00ffffff) drawPixelARGB32(x, y, log[i], is_lose);
		x++;
		if (x == startx + 160){
			y ++;
			x = startx;
		}
	}
}

int isTree(int x, int y, int round){
	int startx = x;
	int starty = y;
	for (int i = (1024*starty) + startx; y < 50+starty; i++ ){
		if((map1_temp[i] == 0x00ffffff && round == 1) || (map2_temp[i] == 0 && round == 2) || (map3_temp[i] == 0 && round == 3)) return 1;
		x++;
		if (x == startx + 38 || x == 1024){
			y ++;
			x = startx;
			i = (1024*y) + x;
		}
	}
	return 0;
}

void avatarMove(int x, int y, int round){
	int startx = x;
	int starty = y;
	for (int i = (1024*starty) + startx; y < 50+starty; i++ ){
		if (round == 1) drawPixelARGB32(x, y, map1[i], 0);
		else if(round ==2) drawPixelARGB32(x, y, map2[i], 0);
		else drawPixelARGB32(x, y, map3[i], 0);
		x++;
		if (x == startx + 38 || x == 1024){
			y ++;
			x = startx;
			i = (1024*y) + x;
		}
	}
	if (x+38 >=965 && y<= 60 && round < 3){
		draw_gate(round);
	}
	else if (x >= 520 && x <= 580 && y+50 >= 690){
		draw_gate(round);
	}
}

void display_map(int round, int is_lose){
	for (int y = 0, x = 0, i = 0; y < 768; i++ ){
		
		if (round == 1)	drawPixelARGB32(x, y, map1[i], is_lose);
		else if (round ==2) drawPixelARGB32(x, y, map2[i], is_lose);
		else drawPixelARGB32(x, y, map3[i], is_lose);

		x++;
		if (x == 1024){
			y ++;
			x = 0;
		}
	}
	if (!is_lose) draw_gate(round);
}



void draw_gate(int round){
	int start_x = 0, start_y = 0;
	if (round < 3) start_x = 965;
	else {
		start_x = 520;
		start_y = 698;
	}
	for (int i = 0, x = start_x, y = start_y; y < start_y + 50; i++){
		if(hole[i] < 12900000) drawPixelARGB32(x, y, hole[i], 0);
		x++;
		if (x == start_x + 50){
			y ++;
			x = start_x;
		}
	}
}

void drawCar(int x, int y, int is_left, int is_lose) { //draw a car of 90x50 pixels at x and y on the screen
	int start_index = 0;
	if (y >380) start_index = 4500;
	int startx = x;
	int starty = y;
	for (int i = start_index; y < 50 + starty; i++ ){
		if (is_left){
			if (car_left[i] < 8066061) drawPixelARGB32(x, y, car_left[i], is_lose);
		}
		else if (car_right[i] != 0x00ffffff){
			if (starty < 380 && car_right[i] >= 8066061) goto skip;
			drawPixelARGB32(x, y, car_right[i], is_lose);
		}
		skip:
		x++;
		if (x == startx + 90){
			y ++;
			x = startx;
		}
	}
}

void carMove(int x, int y, int round, int is_lose) { //draw a 90x50 pixel background where a car was previously drawn
	int startx = x;
	int starty = y;
	for (int i = (1024*starty) + startx; y < 50+starty; i++ ){
		if (round == 1) drawPixelARGB32(x, y, map1[i], is_lose);
		else drawPixelARGB32(x, y, map2[i], is_lose);
		x++;
		if (x == startx + 90){
			y ++;
			x = startx;
			i = (1024*y) + x;
		}
	}
}

void logMove(int x, int y, int is_lose) { //draw a 90x50 pixel background where a car was previously drawn
	int startx = x;
	int starty = y;
	for (int i = (1024*starty) + startx; y < 60+starty; i++ ){
		drawPixelARGB32(x, y, map2[i], is_lose);
		x++;
		if (x == startx + 160){
			y ++;
			x = startx;
			i = (1024*y) + x;
		}
	}
	
}


