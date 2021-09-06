#include "../header/game/game_ui.h"

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
		
		// If it reaches the width of the pic -> jump to next row of pixel
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
	int startX = x;
	int startY = y;
	for (int i = 0; y < 300 + startY; i++){
		if (deadAva[i] != 0x00ffffff && deadAva[i] != 0xfcfcfc && deadAva[i] != 0xfefefe && deadAva[i] != 0xfafafa) drawPixelARGB32(x, y, deadAva[i], 0);
		x++;
		
		// If it reaches the width of the pic -> jump to next row of pixel
		if (x == startX + 300){
			y ++;
			x = startX;
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
		
		// If it reaches the width of the pic -> jump to next row of pixel
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
 * Draw the select box outside the chosen character
 */
void select(int character){
	int x1;
	int y1 = 110;
	if (character == 0) x1 = 280;
	else if (character == 1) x1 = 490;
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
void drawCharacterAva(int x, int y, int character){
	int startX = x;
	int startY = y;
	for (int i = 0; y < 100+startY; i++){
		
		// Draw avatar for given character
		if (character == 2){
			if (dogface[i] > 13891866 || dogface[i] < 11313592) drawPixelARGB32(x, y, dogface[i], 0);
		}
		else if (character == 1){
			if (foxface[i] > 0xffffff || foxface[i] < 0xfffbef) drawPixelARGB32(x, y, foxface[i], 0);
		}
		else {
			if (bearface[i] > 6075929 || bearface[i] < 901810) drawPixelARGB32(x, y, bearface[i], 0);
		}
		x++;
		
		// If it reaches the width of the pic -> jump to next row of pixel
		if (x == startX + 100){
			y ++;
			x = startX;
		}
	}
}


/**
 * Draw character after moving
 */
void drawCharacter(int x, int y, int character, int isMoveUp){
	int startX = x;
	int startY = y;
	for (int i = 0; y < 50+startY; i++){
		if (character == 0){
			if (bear[i] != 0x00000000 && isMoveUp) drawPixelARGB32(x, y, bear[i], 0);
			else if (bearBack[i] != 0 && !isMoveUp) drawPixelARGB32(x, y, bearBack[i], 0);
		}
		else if (character == 1){
			if ((fox[i] > 7914368 || fox[i] < 5202757) && isMoveUp) drawPixelARGB32(x, y, fox[i], 0);
			else if ((foxBack[i] != 0x7dc183) && !isMoveUp) drawPixelARGB32(x, y, foxBack[i], 0);
			
		}
		else {
			if (dog[i] < 8066061 && isMoveUp) drawPixelARGB32(x, y, dog[i], 0);
			else if (dogBack[i] < 8066061 && !isMoveUp) drawPixelARGB32(x, y, dogBack[i], 0);
		}
		x++;
		
		// If it reaches the width of the pic -> jump to next row of pixel
		if (x == startX + 38){
			y ++;
			x = startX;
		}
	}
}


/**
 * Draw map for given round
 */
void drawMap(int round, int isLose){
	for (int y = 0, x = 0, i = 0; y < 768; i++){
		if (round == 1)	drawPixelARGB32(x, y, map1[i], isLose);
		else if (round ==2) drawPixelARGB32(x, y, map2[i], isLose);
		else drawPixelARGB32(x, y, map3[i], isLose);
		x++;
		
		// If it reaches the width of the pic -> jump to next row of pixel
		if (x == 1024){
			y ++;
			x = 0;
		}
	}	
	
	// If the user is lose, dont need to draw gate
	if (!isLose) drawGate(round);
}


/**
 * Draw the train for round 3
 */
void drawTrain(int x, int isLose){
	// Clear the previous position of train
	int startX = x, starti = 0;
	if (x < 0){
		startX = 0;
		starti = startX - x;
		x = 0;
	}
	for (int i = starti, y = 9; y < 70; i++){
		if(train[i] != 0xffffff) drawPixelARGB32(x, y, train[i], isLose);
		x++;
		
		// If it reaches the width of the pic -> jump to next row of pixel
		if (x == startX - starti + 606 || x == 1024){ 
			y ++;
			x = startX;
			i = ((606)*(y-10))+ starti;
		}
	}
	
	// Draw the new position
	x = startX -starti + 606;
	if (starti == 600) x = 0;
	for (int i = (1024*9) + x, y =9; y < 70; i++){
		if (x>= 0 && x < 1024) drawPixelARGB32(x, y, map3[i], isLose);
		x++;
		
		// If it reaches the width of the pic -> jump to next row of pixel
		if (x == startX-starti + 616 || x == 1024){
			x = startX -starti + 606;
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
	int startX = 0, startY = 0;
	
	if (round < 3) startX = 965;
	else {
		startX = 520;
		startY = 698;
	}
	for (int i = 0, x = startX, y = startY; y < startY + 50; i++){
		if(hole[i] < 12900000) drawPixelARGB32(x, y, hole[i], 0);
		x++;
		
		// If it reaches the width of the pic -> jump to next row of pixel
		if (x == startX + 50){
			y ++;
			x = startX;
		}
	}
}


/**
 * Draw cars for every round
 */
void drawCar(int x, int y, int isLeft, int isLose) { 
	int startIndex = 0;
	if (y >380) startIndex = 4500;
	int startX = x;
	int startY = y;
	for (int i = startIndex; y < 50 + startY; i++){
		
		// If there is car from left, draw car from left
		if (isLeft){
			if (carLeft[i] < 8066061) drawPixelARGB32(x, y, carLeft[i], isLose);
		}
		
		// Else, draw car from right
		else if (carRight[i] != 0x00ffffff){
			if (startY <= 380  && carRight[i] >= 8066061) goto skip;
			drawPixelARGB32(x, y, carRight[i], isLose);
		}
		
		skip:
		x++;
		
		// If it reaches the width of the pic -> jump to next row of pixel
		if (x == startX + 90){
			y ++;
			x = startX;
		}
	}
}


/**
 * Draw the log for round 2
 */
void drawLog(int x, int y, int isLose){
	int startX = x;
	int startY = y;
	for (int i = 0; y < 60 + startY; i++){
		if (log[i] != 0x00ffffff) drawPixelARGB32(x, y, log[i], isLose);
		x++;
		
		// If it reaches the width of the pic -> jump to next row of pixel
		if (x == startX + 160){
			y ++;
			x = startX;
		}
	}
}


/**
 * Clear the character before moving
 */
void eraseCharacter(int x, int y, int round){
	int startX = x;
	int startY = y;
	for (int i = (1024*startY) + startX; y < 50+startY; i++){
		if (round == 1) drawPixelARGB32(x, y, map1[i], 0);
		else if(round ==2) drawPixelARGB32(x, y, map2[i], 0);
		else drawPixelARGB32(x, y, map3[i], 0);
		x++;
		
		// If it reaches the width of the pic -> jump to next row of pixel
		if (x == startX + 38 || x == 1024){
			y ++;
			x = startX;
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
void eraseCar(int x, int y, int round, int isLose) { 
	int startX = x;
	int startY = y;
	for (int i = (1024*startY) + startX; y < 50+startY; i++){
		if (round == 1) drawPixelARGB32(x, y, map1[i], isLose);
		else drawPixelARGB32(x, y, map2[i], isLose);
		x++;
		
		// If it reaches the width of the pic -> jump to next row of pixel
		if (x == startX + 90){
			y ++;
			x = startX;
			i = (1024*y) + x;
		}
	}
}


/**
 * Clear the log 
 */
void eraseLog(int x, int y, int isLose) { 
	int startX = x;
	int startY = y;
	for (int i = (1024*startY) + startX; y < 60+startY; i++){
		drawPixelARGB32(x, y, map2[i], isLose);
		x++;
		
		// If it reaches the width of the pic -> jump to next row of pixel
		if (x == startX + 160){
			y ++;
			x = startX;
			i = (1024*y) + x;
		}
	}
	
}


/**
 * Check whether there is a tree or not
 */
int isTree(int x, int y, int round){
	int startX = x;
	int startY = y;
	for (int i = (1024*startY) + startX; y < 50+startY; i++){
		if((map1Temp[i] == 0x00ffffff && round == 1) || (map2Temp[i] == 0 && round == 2) || (map3Temp[i] == 0 && round == 3)) return 1;
		x++;
		
		// If it reaches the width of the pic -> jump to next row of pixel
		if (x == startX + 38 || x == 1024){
			y ++;
			x = startX;
			i = (1024*y) + x;
		}
	}
	return 0;
}


