#include "../header/game/game.h"
#include "../header/piStuffs/uart.h"
#include "../header/piStuffs/framebf.h"
#include "../header/game/gameUI.h"
#include "../header/game/picture/endpic.h"

//---- Global variables declaration -------//

enum gameState {welcome, round1, round2, round3, winning};
enum gameState state = welcome;
int character = 0;
int character_x = 100;
int character_y = 710;
int car_x[] = {0,933,600,500,0,300,800,150,0,0,0,0};	
int car_y[] = {102,164,226,288,350,464,526,588,0,0,0,0};
int log_x[] = {0, 860};
int log_y[] = {90, 165};
int log_contain[] = {0,0};
int train_x = 1020;
unsigned long debouncing_f, debouncing_t, deboucing_r, car_f, car_t, car_r;
unsigned long log1_f, log1_t, log1_r, log2_f, log2_t, log2_r;
unsigned long railroad_f, railroad_t, railroad_r, train_f, train_t, train_r;
int is_esc = 0;
int is_lose = 0;
int is_moveUp = 1;

//--------------------------------------------//

/**
 * Initialize the setting for the game
 */
void gameInit(){
	// reset value 
	state = welcome;
	character = 0;
	is_esc = 0;

	// Display welcome screen
	displayWelcomeScreen(character);
	
	// Set frequencies for timers 
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(debouncing_f));
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(car_f));
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(log1_f));
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(log2_f));
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(railroad_f));
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(train_f));
	
	// Set current counter for timer
	asm volatile ("mrs %0, cntpct_el0" : "=r"(debouncing_t));
	asm volatile ("mrs %0, cntpct_el0" : "=r"(car_t));
	asm volatile ("mrs %0, cntpct_el0" : "=r"(log1_t));
	asm volatile ("mrs %0, cntpct_el0" : "=r"(log2_t));
	asm volatile ("mrs %0, cntpct_el0" : "=r"(railroad_t));
	asm volatile ("mrs %0, cntpct_el0" : "=r"(train_t));
	
	// set the expire value for counters
	debouncing_t+=((debouncing_f/1000)*80000)/1000;
	car_t+=((car_f/1000)*50000)/1000;
	log1_t+=((log1_f/1000)*45000)/1000; 
	log2_t+=((log2_f/1000)*80000)/1000; 
	railroad_t+=((railroad_f/1000)*4750000)/1000; 
	train_t+=((train_f/1000)*40000)/1000; 
}



/**
 *  Execute the game task
 */
void executeGame() {
	// Initialization 
	char c = 0;
	gameInit();
	
	// If there is not the esc button 
	while(c != 27 && !is_esc) {
		
		// Store the input of the user continously 
		c = uart_getc();
		
		// Get the current time for the counter 
		asm volatile ("mrs %0, cntpct_el0" : "=r"(deboucing_r));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(car_r));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(log1_r));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(log2_r));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(railroad_r));

		// State machine
		switch(state) {
		
		case welcome:
			// For character selection part 
			if (c == 'a' || c == 'A') {
				if (character > 0) {
					character--;
					displayWelcomeScreen(character);
				}
			}
			else if (c == 'd' || c== 'D') {
				if (character < 2) {
					character ++;
					displayWelcomeScreen(character);
				}
			}
			
			// After selecting the character, enter to start the game
			else if (c == '\n') {
				
				// Reset all the components
				reset(0);
				
				// Display the round 1 components
				if(!displayInstruction(1)) is_esc = 1;
				drawMap(1,is_lose);
				drawCharacter(character_x,character_y, character, is_moveUp);
				state = round1;
			}
			break;

		case round1:	
			
			// Run the cars in round 1
			carRun(1);
			
			// Game handler for round 1
			if(is_lose) gameOverHandler(c,1);
			else buttonHandler(c, 1);
			break;
			
		case round2:		
			
			// Run the cars in round 2
			carRun(2);
			
			// Run the logs as wekk
			logRun();
			
			// Game handler for round 2
			if(is_lose) gameOverHandler(c,2);
			else buttonHandler(c, 2);
			break;
			
		case round3:
			
			// Run the train in round 3
			trainRun();
			
			// car run
			carRun(3);
			
			// Game handler for round 3
			if(is_lose) gameOverHandler(c,3);
			else buttonHandler(c, 3);
			break;
			
		case winning:
			// If there is an enter, then reset == play again
			if (c == '\n') reset(0);
			break;
		}
	}
	
	// Terminate the game task
	resetScreen();
}


/**
 * Game over stage (when the user is hit)
 */
void gameOverHandler(char c, int round){
	if (is_lose == 1) {
		drawMap(round, is_lose);
		displaySkull();
	}
	is_lose++;
	if (c == '\n') reset(0);
}


/**
 * Button click handler
 */
void buttonHandler(char c, int round){
	
	// If there is not the debouncing time
	if (deboucing_r >= debouncing_t && c != 0) {
		
		// Execute the button event handler
		controlCharacter(c, round);

		// Reset the debouncing timer
		asm volatile ("mrs %0, cntfrq_el0" : "=r"(debouncing_f));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(debouncing_t));
		debouncing_t+=((debouncing_f/1000)*80000)/1000;
	}
}


/**
 * Button event handler to control the character 
 */
void controlCharacter(char c, int round) {
	
	// Request move left
	if (c == 'a' || c == 'A'){
		// If it is not out of the screen or is not blocked by a tree
		if (character_x - 20 >= 0 && !isTree(character_x - 20, character_y, round)) {
			// Condition for each round
			if ((round == 2 && character_y != 170 && character_y != 90) || round == 1 || (round == 3 && (character_y >= 230 || character_y <= 30))){
				// Erase the previous position and draw a new one
				eraseCharacter(character_x, character_y, round);
				character_x -= 20;
				drawCharacter(character_x,character_y, character, is_moveUp);
			}
		}
	}
	
	// Request move right
	else if (c == 'd' || c== 'D'){
		if (character_x + 20 <= 986 && !isTree(character_x + 20, character_y, round)){
			if ((round == 2 && character_y != 170 && character_y != 90) || round == 1 || (round == 3 && (character_y >= 230 || character_y <= 30))){
				eraseCharacter(character_x, character_y, round);
				character_x += 20;
				drawCharacter(character_x,character_y, character, is_moveUp);
			}
		}
	}
	
	// Request move up
	if (c == 'w' || c == 'W'){
		// If is is not out of screen and is not blocked by a tree
		if (character_y - 20 >= 0 && !isTree(character_x, character_y - 20, round)) {
			// Erase the previous position
			eraseCharacter(character_x, character_y, round);
			
			// If there is round 2 and there is the first log in the front
			if(character_y == 230 && round ==2) { 
				// If the use jump right on the log
				if(character_x > log_x[1] && character_x < log_x[1] + 160) {
					// set the character on the log
					character_y = 170;
					character_x = log_x[1] + 60;
					log_contain[1] = 1;
				}
				// Else, fall to the rive -> dead
				else is_lose = 1;
			}
			
			// If there is the second log in the front, same concept with the previous log
			else if(character_y == 170 && round ==2){ 
				if(character_x > log_x[0] && character_x < log_x[0] + 160){
					drawLog(log_x[1], log_y[1], is_lose);
					character_y = 90;
					character_x = log_x[0] + 60;
					log_contain[0] = 1;
					log_contain[1] = 0;
				}
				else is_lose = 1;
			}
			
			// If the user in on the last log, jump to the land
			else if(character_y == 90 && round == 2){
				drawLog(log_x[0], log_y[0], is_lose);
				character_y = 30;
				log_contain[0] = 0;
			}
			
			// If there is round 3 and the user does not move to the bridge -> fall to the river 
			else if (character_y == 230 && round == 3 && !((character_x >= 80 && character_x <= 140) || (character_x >= 860 && character_x <= 900))) is_lose = 1;
			else character_y -= 20;
		
			// Draw the character in the new position with the up direction
			is_moveUp = 1;
			drawCharacter(character_x,character_y, character, is_moveUp);
		}
	}
	
	// Request move backward
	else if (c == 's' || c== 'S'){
		// If it is not out of screen and is not blocked by a tree
		if (character_y + 20 <= 710 && !isTree(character_x, character_y + 20, round)){
			// Erase previous position
			eraseCharacter(character_x, character_y, round);
			
			// Work with the logs and work the same with the move up handler
			if(character_y == 30 && round ==2){
				if(character_x > log_x[0] && character_x < log_x[0] + 160){
					character_y = 90;
					character_x = log_x[0] + 60;
					log_contain[0] = 1;
				}
				else is_lose = 1;
			}
			
			// Work with the logs and the work same with the move up handler
			else if(character_y == 90 && round ==2){
				if(character_x > log_x[1] && character_x < log_x[1] + 160){
					drawLog(log_x[0], log_y[0], is_lose);
					character_y = 170;
					character_x = log_x[1] + 60;
					log_contain[1] = 1;
					log_contain[0] = 0;
				}
				else is_lose = 1;
			}
			
			// Work with the last log and work the same with the move up handler 
			else if(character_y == 170 && round == 2){
				drawLog(log_x[1], log_y[1], is_lose);
				character_y = 230;
				log_contain[1] = 0;
			}
			
			// Work with the bridge and work the same with the move up handler
			else if (character_y == 30 && round == 3 && !((character_x >= 80 && character_x <= 140) || (character_x >= 860 && character_x <= 900))) is_lose = 1;
			else character_y += 20;
			
			// Draw character in the new position with the down direction 
			is_moveUp = 0;
			drawCharacter(character_x,character_y, character, is_moveUp);
		}
	}
	
	// If pass the gate, move to new round/win
	if ((character_y <= 10 && character_x + 60 >= 995 && round < 3)  || (character_y >= 670 && character_x +50 >= 520 && character_x <= 550 && round == 3)) {
		reset(round);
	}	
}


/**
 * Run the train for round 3
 */ 
void trainRun(){
	
	// For every 5s waitting timer
	if (railroad_r >= railroad_t){
		// count the timer for running the train
		asm volatile ("mrs %0, cntpct_el0" : "=r"(train_r));
		
		// If reach the expired train timer, move train 1 step
		if (train_r >= train_t){
			// Draw train
			drawTrain(train_x, is_lose);
			train_x-=10;
			
			// If the whole train is passed away, reset the train
			if (train_x + 606 < 0){
				train_x = 1020;
				
				// Reset train timer
				asm volatile ("mrs %0, cntfrq_el0" : "=r"(railroad_f));
				asm volatile ("mrs %0, cntpct_el0" : "=r"(railroad_t));
				railroad_t+=((railroad_f/1000)*4750000)/1000;
			}
			
			// Reset timer for train move next step
			asm volatile ("mrs %0, cntfrq_el0" : "=r"(train_f));
			asm volatile ("mrs %0, cntpct_el0" : "=r"(train_t));
			train_t+=((train_f/1000)*40000)/1000;
			
			// If the train hit the character -> lose
			if (is_lose == 0 && character_y < 70 && character_x >= train_x && character_x <= train_x + 606){
				is_lose = 1;
			}
		}
	}
}


/**
 * Run the cars based on the given round
 */
void carRun(int round){
	// time interval for car move a step
	if (car_r >= car_t){
		if (round == 1){
			// For every car
			for (int i = 0; i < 8; i++) {
				// Clear the previous position
				if(is_lose != 2) eraseCar(car_x[i], car_y[i], round, is_lose); 
				
				// Indicate the car that move from right
				if (i == 1 || i == 2 || i == 7) { 
					
					// Set new position
					car_x[i] -= 20;
					if (car_x[i] <= 0) car_x[i] = 933;
					
					if (!(is_lose && car_x[i] >= 362 && car_x[i] <= 662)) drawCar(car_x[i], car_y[i], 0, is_lose); 
					else car_x[i] = 310;
				}
				
				// The rest will move from left
				else {
					
					// Set new position
					car_x[i] += 20;
					if (car_x[i] >= 1024) car_x[i] = 0;
					
					if (!(is_lose && car_x[i]+45 >= 362 && car_x[i]+45 <= 662)) drawCar(car_x[i], car_y[i], 1, is_lose);
					else car_x[i] = 664;
				}
			}
			
			// Reset the timer for moving cars
			asm volatile ("mrs %0, cntfrq_el0" : "=r"(car_f));
			asm volatile ("mrs %0, cntpct_el0" : "=r"(car_t));
			car_t+=((car_f/1000)*50000)/1000;
		}
		else if (round == 2) {
			// For every car
			for (int i = 0; i < 6; i++) { 
				
				// Clear the previous position
				if(is_lose != 2) eraseCar(car_x[i], car_y[i], round, is_lose);
				
				// Indicate the car that move from right
				if (i == 1 || i == 3) { 
					
					// Set the new position
					car_x[i] -= 20; 
					if (car_x[i] <= 0) car_x[i] = 933;
					
					if (!(is_lose && car_x[i] >= 362 && car_x[i] <= 662)) drawCar(car_x[i], car_y[i], 0, is_lose); // car heading right
					else car_x[i] = 310;
				}
				
				// The rest will move from left
				else {
					
					// Set the new position
					car_x[i] += 20;
					if (car_x[i] >= 1024) car_x[i] = 0;
					
					if (!(is_lose && car_x[i]+45 >= 362 && car_x[i]+45 <= 662)) drawCar(car_x[i], car_y[i], 1, is_lose); // car heading left
					else car_x[i] = 664;
				}
			}
		
			// Reset the timer for moving cars
			asm volatile ("mrs %0, cntfrq_el0" : "=r"(car_f));
			asm volatile ("mrs %0, cntpct_el0" : "=r"(car_t));
			car_t+=((car_f/1000)*40000)/1000;
		}
		else {
			// For every car
			for (int i = 0; i < 12; i++) { 
				
				// Clear the previous position
				if(is_lose != 2) eraseCar(car_x[i], car_y[i], round, is_lose); 
				
				// Indicate the cars that move from right
				if (i == 1 || i == 3 || i == 4|| i == 6 || i == 7 || i == 9 || i == 11) { 
					
					// Set new position
					car_x[i] -= 20; 
					
					if (car_x[i] <= 0 && i < 6) car_x[i] = 400;
					else if (car_x[i] <= 515 && i >= 6) car_x[i] = 933;
					
					if (!(is_lose && car_x[i] >= 362 && car_x[i] <= 662)) drawCar(car_x[i], car_y[i], 0, is_lose);
					else car_x[i] = 310;
				}
				
				// The rest will move from left
				else {
					
					// Set new position
					car_x[i] += 20;
	
					if (car_x[i] > 933 && i >= 6) car_x[i] = 512;
					else if (car_x[i] > 400 && i < 6) car_x[i] = 0;
	
					if (!(is_lose && car_x[i]+45 >= 362 && car_x[i]+45 <= 662)) drawCar(car_x[i], car_y[i], 1, is_lose);
					else car_x[i] = 664;
				}
			}
			
			// Reset the timer for moving cars
			asm volatile ("mrs %0, cntfrq_el0" : "=r"(car_f));
			asm volatile ("mrs %0, cntpct_el0" : "=r"(car_t));
			car_t+=((car_f/1000)*110000)/1000;
		}
	}
	
	// If the car hit the character -> lose
	if (is_lose == 0){
		if(isHit(round)) is_lose = 1;
	}
	
}


/**
 * Move the logs for round 2
 */
void logRun(){
	
	// If is lose -> reset the log
	if (is_lose){
		log_contain[0] = 0;
		log_contain[1] = 0;
	}
	
	// First log timer is expired 
	if(log1_r >= log1_t){
		
		// Erase the previous position
		eraseLog(log_x[0], log_y[0], is_lose);
		log_x[0] += 20; 
		
		// If the log is run out of the screen
		if (log_x[0] > 924) {
			
			// If the character is still on it -> lose
			if (log_contain[0] == 1) is_lose = 1;
			
			// Else run again from the start position
			log_x[0] = 0;
		}
		
		// Draw log with new position
		drawLog(log_x[0], log_y[0], is_lose);
		
		// Redraw character with the log
		if(log_contain[0] == 1){
			character_x = log_x[0] + 60;
			drawCharacter(character_x,character_y, character, is_moveUp);
		}

		// Reset timer for first log
		asm volatile ("mrs %0, cntfrq_el0" : "=r"(log1_f));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(log1_t));
		log1_t+=((log1_f/1000)*45000)/1000;
	}
	
	// Second log timer is expired
	if(log2_r >= log2_t){ 
		
		//clear the previous drawing
		eraseLog(log_x[1], log_y[1], is_lose); 
		log_x[1] -= 20; 
		
		// If the log run out of screen
		if (log_x[1] < 0){
			
			// If the character is still on the log -> lose
			if (log_contain[1] == 1) is_lose = 1;
			
			// Else run from start point
			log_x[1] = 863;
		}
		
		// Draw the log with new position
		drawLog(log_x[1], log_y[1], is_lose);
		
		// If the character is on it, draw the character on it too
		if(log_contain[1] == 1){
			character_x = log_x[1] + 60;
			drawCharacter(character_x,character_y, character, is_moveUp);
		}
		
		// Reset timer for the second log
		asm volatile ("mrs %0, cntfrq_el0" : "=r"(log2_f));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(log2_t));
		log2_t+=((log2_f/1000)*80000)/1000;
	}
}


/**
 * Check if the character is hit by any car
 */
int isHit(int round){
	// Get the number of cars bases on given round
	int num = 0;
	if (round == 1) num = 8;
	else if (round == 2) num = 6;
	else num = 12;
	
	// For each car
	for (int ii = 0; ii < num ; ii++ ){
		// If the car overlap the character -> lose
		if ((character_x >= car_x[ii] && character_x < car_x[ii] + 80) || ((character_x+ 37) >= car_x[ii] && (character_x +37)< car_x[ii] + 80))
			if ((character_y >= car_y[ii] && character_y < car_y[ii] + 50) || ((character_y+ 50) >= car_y[ii] && (character_y +50)< car_y[ii] + 50)) 
				return 1;
	}
	return 0;
}


/**
 * Reset for new round or play again
 */
void reset(int round){
	
	// Set the character back to the start position
	character_x = 100;
	character_y = 710;
	is_moveUp = 1;
	
	// If the round 1 is finished -> move to round 2
	if (round == 1){
		state = round2;
		
		// Set position for each car in round 2
		car_y[0] = 292;
		car_y[1] = 354;
		car_y[2] = 416;
		car_y[3] = 478;
		car_y[4] = 538;
		car_y[5] = 600;
		
		// Display instruction
		if(!displayInstruction(2)) is_esc = 1;
		
		// Draw components for round 2
		drawMap(2, is_lose);
		drawCharacter(character_x,character_y, character, is_moveUp);
	}
	
	// If the round 2 is finished -> move to round 3
	else if (round == 2){
		state = round3;
		
		// Set y position for each car in round 3
		car_y[0] = 292;
		car_y[1] = 354;
		car_y[2] = 416;
		car_y[3] = 478;
		car_y[4] = 538;
		car_y[5] = 600;
		car_y[6] = 292;
		car_y[7] = 354;
		car_y[8] = 416;
		car_y[9] = 478;
		car_y[10] = 538;
		car_y[11] = 600;
		
		// Set x position for each car in round 3
		car_x[0] = 150;
		car_x[1] = 0;
		car_x[2] = 300;
		car_x[3] = 100;
		car_x[4] = 400;
		car_x[5] = 220;
		car_x[6] = 950;
		car_x[7] = 615;
		car_x[8] = 680;
		car_x[9] = 515;
		car_x[10] = 800;
		car_x[11] = 515;
		
		// Display the instruction first
		if(!displayInstruction(3)) is_esc = 1;
		
		// Then, display round3's components
		drawMap(3, is_lose);
		drawCharacter(character_x,character_y, character, is_moveUp);
	}
	else{
		
		// Reset the y position of cars to the default one
		car_y[7] = 588;
		car_y[6] = 526;
		car_y[5] = 464;
		car_y[4] = 350;
		car_y[3] = 288;
		car_y[2] = 226;
		car_y[1] = 164;
		car_y[0] = 102;
		
		// Reset the x position of cars to the default one
		car_x[7] = 0;
		car_x[6] = 933;
		car_x[5] = 600;
		car_x[4] = 500;
		car_x[3] = 0;
		car_x[2] = 300;
		car_x[1] = 800;
		car_x[0] = 150;
		
		// If the user finish round 3 -> win 
		if (round == 3){
			displayBackground(1024, 768, endscr);
			state = winning;
		}
		
		// If the user is lose and want to play again
		else {
			
			// Move back to welcome screen
			displayWelcomeScreen(character);
			log_contain[0] = 0;
			log_contain[1] = 0;
			is_lose = 0;
			state = welcome;
		}
	}
}