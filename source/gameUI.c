#include "../header/game/gameUI.h"
#include "../header/piStuffs/framebf.h"
#include "../header/piStuffs/uart.h"
#include "../header/game/picture/avatar.h"
#include "../header/game/picture/map1.h"
#include "../header/game/picture/map2.h"
#include"../header/game/picture/map3.h"
#include "../header/game/picture/objects.h"
#include "../header/game/picture/instructions.h"
#include "../header/game/picture/welcomepic.h"

/**
 * Display the background
 */
void displayBackground(int width, int height, const unsigned int picture[]) {
	for (int y = 0, x = 0, i = 0; y < height; i++) {
		drawPixelARGB32(x, y, picture[i], 0);
		x++;
		if (x == width){
			y++;
			x = 0;
		}
	}
}


/**
 * Draw all components for welcome screen
 */
void displayWelcomeScreen(int character){
	displayBackground(1024, 768, welcomescr);
	drawCharacterAva(280,110, 0);
	drawCharacterAva(490,110, 1);
	drawCharacterAva(700,110, 2);
	select(character);
}


/**
 * Draw skull for game over state
 */
void displaySkull(){
	int x = 362;
	int y = 234;
	int start_x = x;
	int start_y = y;
	for (int i = 0; y < 300 + start_y; i++){
		if (dead_ava[i] != 0x00ffffff && dead_ava[i] != 0xfcfcfc && dead_ava[i] != 0xfefefe && dead_ava[i] != 0xfafafa) drawPixelARGB32(x, y, dead_ava[i], 0);
		x++;
		if (x == start_x + 300){
			y ++;
			x = start_x;
		}
	}
}


/**
 * Display instruction for every round
 */
int displayInstruction(int round){
	// Draw map with dark color first
	for (int y = 0, x = 0, i = 0; y < 768; i++){
		if (round == 1)	drawDarkPixel(x, y, map1[i]);
		else if (round ==2) drawDarkPixel(x, y, map2[i]);
		else drawDarkPixel(x, y, map3[i]);
		x++;
		if (x == 1024){
			y ++;
			x = 0;
		}
	}
	
	// Then, draw instruction on it
	for (int i = 75261*(round-1), x = 356, y = 262; y < 504; i++){
		if (instructions[i] < 13574923) drawPixelARGB32(x, y, instructions[i], 0);
		x++;
		if (x == 667){
			x = 356;
			y++;
		}
	}
	
	// Wait until there is an enter or esc
	char c = 0;
	while(c != '\n' && c != 27) c = uart_getc();
	
	// If there is esc -> terminate the game
	if (c == '\n') return 1;
	return 0;
}


/**
 * Draw the select box outside the chosen animal
 */
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
			if((x == x1 || x == x2) || (y == y1 || y == y2)) drawPixelARGB32(x, y, 0x00FFFF00, 0);
		}
}


/**
 * Draw character faces
 */
void drawCharacterAva(int x, int y, int animal){
	int start_x = x;
	int start_y = y;
	for (int i = 0; y < 100+start_y; i++){
		
		// Draw avatar for given animal
		if (animal == 2){
			if (dogface[i] > 13891866 || dogface[i] < 11313592) drawPixelARGB32(x, y, dogface[i], 0);
		}
		else if (animal == 1){
			if (foxface[i] > 0xffffff || foxface[i] < 0xfffbef) drawPixelARGB32(x, y, foxface[i], 0);
		}
		else {
			if (bearface[i] > 6075929 || bearface[i] < 901810) drawPixelARGB32(x, y, bearface[i], 0);
		}
		x++;
		if (x == start_x + 100){
			y ++;
			x = start_x;
		}
	}
}


/**
 * Draw character after moving
 */
void drawCharacter(int x, int y, int animal, int is_goup){
	int start_x = x;
	int start_y = y;
	for (int i = 0; y < 50+start_y; i++){
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
		if (x == start_x + 38){
			y ++;
			x = start_x;
		}
	}
}


/**
 * Draw map for given round
 */
void drawMap(int round, int is_lose){
	for (int y = 0, x = 0, i = 0; y < 768; i++){
		if (round == 1)	drawPixelARGB32(x, y, map1[i], is_lose);
		else if (round ==2) drawPixelARGB32(x, y, map2[i], is_lose);
		else drawPixelARGB32(x, y, map3[i], is_lose);
		x++;
		if (x == 1024){
			y ++;
			x = 0;
		}
	}	
	
	// If the user is lose, dont need to draw gate
	if (!is_lose) drawGate(round);
}


/**
 * Draw the train for round 3
 */
void drawTrain(int x, int is_lose){
	// Clear the previous position of train
	int start_x = x, starti = 0;
	if (x < 0){
		start_x = 0;
		starti = start_x - x;
		x = 0;
	}
	for (int i = starti, y = 9; y < 70; i++){
		if(train[i] != 0xffffff) drawPixelARGB32(x, y, train[i], is_lose);
		x++;
		if (x == start_x - starti + 606 || x == 1024){ 
			y ++;
			x = start_x;
			i = ((606)*(y-10))+ starti;
		}
	}
	
	// Draw the new position
	x = start_x -starti + 606;
	if (starti == 600) x = 0;
	for (int i = (1024*9) + x, y =9; y < 70; i++){
		if (x>= 0 && x < 1024) drawPixelARGB32(x, y, map3[i], is_lose);
		x++;
		if (x == start_x-starti + 616 || x == 1024){
			x = start_x -starti + 606;
			if (starti == 600) x = 0; 
			y++;
			i = (1024*y) + x;
		}
	}
}


/**
 * Draw the gate 
 */
void drawGate(int round){
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


/**
 * Draw cars for every round
 */
void drawCar(int x, int y, int is_left, int is_lose) { 
	int start_index = 0;
	if (y >380) start_index = 4500;
	int start_x = x;
	int start_y = y;
	for (int i = start_index; y < 50 + start_y; i++){
		
		// If there is car from left, draw car from left
		if (is_left){
			if (car_left[i] < 8066061) drawPixelARGB32(x, y, car_left[i], is_lose);
		}
		
		// Else, draw car from right
		else if (car_right[i] != 0x00ffffff){
			if (start_y <= 380  && car_right[i] >= 8066061) goto skip;
			drawPixelARGB32(x, y, car_right[i], is_lose);
		}
		
		skip:
		x++;
		if (x == start_x + 90){
			y ++;
			x = start_x;
		}
	}
}


/**
 * Draw the log for round 2
 */
void drawLog(int x, int y, int is_lose){
	int start_x = x;
	int start_y = y;
	for (int i = 0; y < 60 + start_y; i++){
		if (log[i] != 0x00ffffff) drawPixelARGB32(x, y, log[i], is_lose);
		x++;
		if (x == start_x + 160){
			y ++;
			x = start_x;
		}
	}
}


/**
 * Clear the character before moving
 */
void eraseCharacter(int x, int y, int round){
	int start_x = x;
	int start_y = y;
	for (int i = (1024*start_y) + start_x; y < 50+start_y; i++){
		if (round == 1) drawPixelARGB32(x, y, map1[i], 0);
		else if(round ==2) drawPixelARGB32(x, y, map2[i], 0);
		else drawPixelARGB32(x, y, map3[i], 0);
		x++;
		if (x == start_x + 38 || x == 1024){
			y ++;
			x = start_x;
			i = (1024*y) + x;
		}
	}
	
	// Redraw the gate if it is overlap
	if (x + 40 >=965 && y<= 90 && round < 3) drawGate(round);
	else if (x >= 520 && x <= 580 && y+50 >= 690) drawGate(round);
	
}


/**
 * Clear the previous position of car
 */
void eraseCar(int x, int y, int round, int is_lose) { 
	int start_x = x;
	int start_y = y;
	for (int i = (1024*start_y) + start_x; y < 50+start_y; i++){
		if (round == 1) drawPixelARGB32(x, y, map1[i], is_lose);
		else drawPixelARGB32(x, y, map2[i], is_lose);
		x++;
		if (x == start_x + 90){
			y ++;
			x = start_x;
			i = (1024*y) + x;
		}
	}
}


/**
 * Clear the log 
 */
void eraseLog(int x, int y, int is_lose) { 
	int start_x = x;
	int start_y = y;
	for (int i = (1024*start_y) + start_x; y < 60+start_y; i++){
		drawPixelARGB32(x, y, map2[i], is_lose);
		x++;
		if (x == start_x + 160){
			y ++;
			x = start_x;
			i = (1024*y) + x;
		}
	}
	
}


/**
 * Check whether there is a tree or not
 */
int isTree(int x, int y, int round){
	int start_x = x;
	int start_y = y;
	for (int i = (1024*start_y) + start_x; y < 50+start_y; i++){
		if((map1_temp[i] == 0x00ffffff && round == 1) || (map2_temp[i] == 0 && round == 2) || (map3_temp[i] == 0 && round == 3)) return 1;
		x++;
		if (x == start_x + 38 || x == 1024){
			y ++;
			x = start_x;
			i = (1024*y) + x;
		}
	}
	return 0;
}


