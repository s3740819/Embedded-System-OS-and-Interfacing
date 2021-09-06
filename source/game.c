#include "../header/game/game.h"

#include "../header/game/game_ui.h"
#include "../header/piStuffs/uart.h"
#include "../header/piStuffs/framebf.h"
#include "../header/game/picture/endpic.h"

//---- Global variables declaration -------//

enum gameState {welcome, round1, round2, round3, winning};
enum gameState state = welcome;
int character = 0;
int characterX = 100;
int characterY = 710;
int carX[] = {0,933,600,500,0,300,800,150,0,0,0,0};	
int carY[] = {102,164,226,288,350,464,526,588,0,0,0,0};
int logX[] = {0, 860};
int logY[] = {90, 165};
int logContain[] = {0,0};
int trainX = 1020;
unsigned long debouncingFreg, debouncingExpected, deboucingTimer, carFreg, carExpected, carTimer;
unsigned long log1Freg, log1Expected, log1Timer, log2Freg, log2Expected, log2Timer;
unsigned long railroadFreg, railroadExpected, railroadTimer, trainFreg, trainExpected, trainTimer;
int isEsc = 0;
int isLose = 0;
int isMoveUp = 1;

//--------------------------------------------//

/**
 * Initialize the setting for the game
 */
void gameInit(){
	// reset value 
	state = welcome;
	character = 0;
	isEsc = 0;

	// Display welcome screen
	displayWelcomeScreen(character);
	
	// Set frequencies for timers 
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(debouncingFreg));
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(carFreg));
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(log1Freg));
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(log2Freg));
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(railroadFreg));
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(trainFreg));
	
	// Set current counter for timer
	asm volatile ("mrs %0, cntpct_el0" : "=r"(debouncingExpected));
	asm volatile ("mrs %0, cntpct_el0" : "=r"(carExpected));
	asm volatile ("mrs %0, cntpct_el0" : "=r"(log1Expected));
	asm volatile ("mrs %0, cntpct_el0" : "=r"(log2Expected));
	asm volatile ("mrs %0, cntpct_el0" : "=r"(railroadExpected));
	asm volatile ("mrs %0, cntpct_el0" : "=r"(trainExpected));
	
	// set the expire value for counters
	debouncingExpected+=((debouncingFreg/1000)*80000)/1000;
	carExpected+=((carFreg/1000)*50000)/1000;
	log1Expected+=((log1Freg/1000)*45000)/1000; 
	log2Expected+=((log2Freg/1000)*80000)/1000; 
	railroadExpected+=((railroadFreg/1000)*4750000)/1000; 
	trainExpected+=((trainFreg/1000)*40000)/1000; 
}



/**
 *  Execute the game task
 */
void executeGame() {
	// Initialization 
	char c = 0;
	gameInit();
	
	// If there is not the esc button 
	while(c != 27 && !isEsc) {
		
		// Store the input of the user continously 
		c = uart_getc();
		
		// Get the current time for the counter 
		asm volatile ("mrs %0, cntpct_el0" : "=r"(deboucingTimer));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(carTimer));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(log1Timer));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(log2Timer));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(railroadTimer));

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
				if(!displayInstruction(1)) isEsc = 1;
				drawMap(1,isLose);
				drawCharacter(characterX,characterY, character, isMoveUp);
				state = round1;
			}
			break;

		case round1:	
			
			// Run the cars in round 1
			carRun(1);
			
			// Game handler for round 1
			if(isLose) gameOverHandler(c,1);
			else buttonHandler(c, 1);
			break;
			
		case round2:		
			
			// Run the cars in round 2
			carRun(2);
			
			// Run the logs as wekk
			logRun();
			
			// Game handler for round 2
			if(isLose) gameOverHandler(c,2);
			else buttonHandler(c, 2);
			break;
			
		case round3:
			
			// Run the train in round 3
			trainRun();
			
			// car run
			carRun(3);
			
			// Game handler for round 3
			if(isLose) gameOverHandler(c,3);
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
	if (isLose == 1) {
		drawMap(round, isLose);
		displaySkull();
	}
	isLose++;
	if (c == '\n') reset(0);
}


/**
 * Button click handler
 */
void buttonHandler(char c, int round){
	
	// If there is not the debouncing time
	if (deboucingTimer >= debouncingExpected && c != 0) {
		
		// Execute the button event handler
		controlCharacter(c, round);

		// Reset the debouncing timer
		asm volatile ("mrs %0, cntfrq_el0" : "=r"(debouncingFreg));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(debouncingExpected));
		debouncingExpected+=((debouncingFreg/1000)*80000)/1000;
	}
}


/**
 * Button event handler to control the character 
 */
void controlCharacter(char c, int round) {
	
	// Request move left
	if (c == 'a' || c == 'A'){
		// If it is not out of the screen or is not blocked by a tree
		if (characterX - 20 >= 0 && !isTree(characterX - 20, characterY, round)) {
			// Condition for each round
			if ((round == 2 && characterY != 170 && characterY != 90) || round == 1 || (round == 3 && (characterY >= 230 || characterY <= 30))){
				// Erase the previous position and draw a new one
				eraseCharacter(characterX, characterY, round);
				characterX -= 20;
				drawCharacter(characterX,characterY, character, isMoveUp);
			}
		}
	}
	
	// Request move right
	else if (c == 'd' || c== 'D'){
		if (characterX + 20 <= 986 && !isTree(characterX + 20, characterY, round)){
			if ((round == 2 && characterY != 170 && characterY != 90) || round == 1 || (round == 3 && (characterY >= 230 || characterY <= 30))){
				eraseCharacter(characterX, characterY, round);
				characterX += 20;
				drawCharacter(characterX,characterY, character, isMoveUp);
			}
		}
	}
	
	// Request move up
	if (c == 'w' || c == 'W'){
		// If is is not out of screen and is not blocked by a tree
		if (characterY - 20 >= 0 && !isTree(characterX, characterY - 20, round)) {
			// Erase the previous position
			eraseCharacter(characterX, characterY, round);
			
			// If there is round 2 and there is the first log in the front
			if(characterY == 230 && round ==2) { 
				// If the use jump right on the log
				if(characterX > logX[1] && characterX < logX[1] + 160) {
					// set the character on the log
					characterY = 170;
					characterX = logX[1] + 60;
					logContain[1] = 1;
				}
				// Else, fall to the rive -> dead
				else isLose = 1;
			}
			
			// If there is the second log in the front, same concept with the previous log
			else if(characterY == 170 && round ==2){ 
				if(characterX > logX[0] && characterX < logX[0] + 160){
					drawLog(logX[1], logY[1], isLose);
					characterY = 90;
					characterX = logX[0] + 60;
					logContain[0] = 1;
					logContain[1] = 0;
				}
				else isLose = 1;
			}
			
			// If the user in on the last log, jump to the land
			else if(characterY == 90 && round == 2){
				drawLog(logX[0], logY[0], isLose);
				characterY = 30;
				logContain[0] = 0;
			}
			
			// If there is round 3 and the user does not move to the bridge -> fall to the river 
			else if (characterY == 230 && round == 3 && !((characterX >= 80 && characterX <= 140) || (characterX >= 860 && characterX <= 900))) isLose = 1;
			else characterY -= 20;
		
			// Draw the character in the new position with the up direction
			isMoveUp = 1;
			drawCharacter(characterX,characterY, character, isMoveUp);
		}
	}
	
	// Request move backward
	else if (c == 's' || c== 'S'){
		// If it is not out of screen and is not blocked by a tree
		if (characterY + 20 <= 710 && !isTree(characterX, characterY + 20, round)){
			// Erase previous position
			eraseCharacter(characterX, characterY, round);
			
			// Work with the logs and work the same with the move up handler
			if(characterY == 30 && round ==2){
				if(characterX > logX[0] && characterX < logX[0] + 160){
					characterY = 90;
					characterX = logX[0] + 60;
					logContain[0] = 1;
				}
				else isLose = 1;
			}
			
			// Work with the logs and the work same with the move up handler
			else if(characterY == 90 && round ==2){
				if(characterX > logX[1] && characterX < logX[1] + 160){
					drawLog(logX[0], logY[0], isLose);
					characterY = 170;
					characterX = logX[1] + 60;
					logContain[1] = 1;
					logContain[0] = 0;
				}
				else isLose = 1;
			}
			
			// Work with the last log and work the same with the move up handler 
			else if(characterY == 170 && round == 2){
				drawLog(logX[1], logY[1], isLose);
				characterY = 230;
				logContain[1] = 0;
			}
			
			// Work with the bridge and work the same with the move up handler
			else if (characterY == 30 && round == 3 && !((characterX >= 80 && characterX <= 140) || (characterX >= 860 && characterX <= 900))) isLose = 1;
			else characterY += 20;
			
			// Draw character in the new position with the down direction 
			isMoveUp = 0;
			drawCharacter(characterX,characterY, character, isMoveUp);
		}
	}
	
	// If pass the gate, move to new round/win
	if ((characterY <= 10 && characterX + 60 >= 995 && round < 3)  || (characterY >= 670 && characterX +50 >= 520 && characterX <= 550 && round == 3)) {
		reset(round);
	}	
}


/**
 * Run the train for round 3
 */ 
void trainRun(){
	
	// For every 5s waitting timer
	if (railroadTimer >= railroadExpected){
		// count the timer for running the train
		asm volatile ("mrs %0, cntpct_el0" : "=r"(trainTimer));
		
		// If reach the expired train timer, move train 1 step
		if (trainTimer >= trainExpected){
			// Draw train
			drawTrain(trainX, isLose);
			trainX-=10;
			
			// If the whole train is passed away, reset the train
			if (trainX + 606 < 0){
				trainX = 1020;
				
				// Reset train timer
				asm volatile ("mrs %0, cntfrq_el0" : "=r"(railroadFreg));
				asm volatile ("mrs %0, cntpct_el0" : "=r"(railroadExpected));
				railroadExpected+=((railroadFreg/1000)*4750000)/1000;
			}
			
			// Reset timer for train move next step
			asm volatile ("mrs %0, cntfrq_el0" : "=r"(trainFreg));
			asm volatile ("mrs %0, cntpct_el0" : "=r"(trainExpected));
			trainExpected+=((trainFreg/1000)*40000)/1000;
			
			// If the train hit the character -> lose
			if (isLose == 0 && characterY < 70 && characterX >= trainX && characterX <= trainX + 606){
				isLose = 1;
			}
		}
	}
}


/**
 * Run the cars based on the given round
 */
void carRun(int round){
	// time interval for car move a step
	if (carTimer >= carExpected){
		if (round == 1){
			// For every car
			for (int i = 0; i < 8; i++) {
				// Clear the previous position
				if(isLose != 2) eraseCar(carX[i], carY[i], round, isLose); 
				
				// Indicate the car that move from right
				if (i == 1 || i == 2 || i == 7) { 
					
					// Set new position
					carX[i] -= 20;
					if (carX[i] <= 0) carX[i] = 933;
					
					if (!(isLose && carX[i] >= 362 && carX[i] <= 662)) drawCar(carX[i], carY[i], 0, isLose); 
					else carX[i] = 310;
				}
				
				// The rest will move from left
				else {
					
					// Set new position
					carX[i] += 20;
					if (carX[i] >= 1024) carX[i] = 0;
					
					if (!(isLose && carX[i]+45 >= 362 && carX[i]+45 <= 662)) drawCar(carX[i], carY[i], 1, isLose);
					else carX[i] = 664;
				}
			}
			
			// Reset the timer for moving cars
			asm volatile ("mrs %0, cntfrq_el0" : "=r"(carFreg));
			asm volatile ("mrs %0, cntpct_el0" : "=r"(carExpected));
			carExpected+=((carFreg/1000)*50000)/1000;
		}
		else if (round == 2) {
			// For every car
			for (int i = 0; i < 6; i++) { 
				
				// Clear the previous position
				if(isLose != 2) eraseCar(carX[i], carY[i], round, isLose);
				
				// Indicate the car that move from right
				if (i == 1 || i == 3) { 
					
					// Set the new position
					carX[i] -= 20; 
					if (carX[i] <= 0) carX[i] = 933;
					
					if (!(isLose && carX[i] >= 362 && carX[i] <= 662)) drawCar(carX[i], carY[i], 0, isLose); // car heading right
					else carX[i] = 310;
				}
				
				// The rest will move from left
				else {
					
					// Set the new position
					carX[i] += 20;
					if (carX[i] >= 1024) carX[i] = 0;
					
					if (!(isLose && carX[i]+45 >= 362 && carX[i]+45 <= 662)) drawCar(carX[i], carY[i], 1, isLose); // car heading left
					else carX[i] = 664;
				}
			}
		
			// Reset the timer for moving cars
			asm volatile ("mrs %0, cntfrq_el0" : "=r"(carFreg));
			asm volatile ("mrs %0, cntpct_el0" : "=r"(carExpected));
			carExpected+=((carFreg/1000)*40000)/1000;
		}
		else {
			// For every car
			for (int i = 0; i < 12; i++) { 
				
				// Clear the previous position
				if(isLose != 2) eraseCar(carX[i], carY[i], round, isLose); 
				
				// Indicate the cars that move from right
				if (i == 1 || i == 3 || i == 4|| i == 6 || i == 7 || i == 9 || i == 11) { 
					
					// Set new position
					carX[i] -= 20; 
					
					if (carX[i] <= 0 && i < 6) carX[i] = 400;
					else if (carX[i] <= 515 && i >= 6) carX[i] = 933;
					
					if (!(isLose && carX[i] >= 362 && carX[i] <= 662)) drawCar(carX[i], carY[i], 0, isLose);
					else carX[i] = 310;
				}
				
				// The rest will move from left
				else {
					
					// Set new position
					carX[i] += 20;
	
					if (carX[i] > 933 && i >= 6) carX[i] = 512;
					else if (carX[i] > 400 && i < 6) carX[i] = 0;
	
					if (!(isLose && carX[i]+45 >= 362 && carX[i]+45 <= 662)) drawCar(carX[i], carY[i], 1, isLose);
					else carX[i] = 664;
				}
			}
			
			// Reset the timer for moving cars
			asm volatile ("mrs %0, cntfrq_el0" : "=r"(carFreg));
			asm volatile ("mrs %0, cntpct_el0" : "=r"(carExpected));
			carExpected+=((carFreg/1000)*110000)/1000;
		}
	}
	
	// If the car hit the character -> lose
	if (isLose == 0){
		if(isHit(round)) isLose = 1;
	}
	
}


/**
 * Move the logs for round 2
 */
void logRun(){
	
	// If is lose -> reset the log
	if (isLose){
		logContain[0] = 0;
		logContain[1] = 0;
	}
	
	// First log timer is expired 
	if(log1Timer >= log1Expected){
		
		// Erase the previous position
		eraseLog(logX[0], logY[0], isLose);
		logX[0] += 20; 
		
		// If the log is run out of the screen
		if (logX[0] > 924) {
			
			// If the character is still on it -> lose
			if (logContain[0] == 1) isLose = 1;
			
			// Else run again from the start position
			logX[0] = 0;
		}
		
		// Draw log with new position
		drawLog(logX[0], logY[0], isLose);
		
		// Redraw character with the log
		if(logContain[0] == 1){
			characterX = logX[0] + 60;
			drawCharacter(characterX,characterY, character, isMoveUp);
		}

		// Reset timer for first log
		asm volatile ("mrs %0, cntfrq_el0" : "=r"(log1Freg));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(log1Expected));
		log1Expected+=((log1Freg/1000)*45000)/1000;
	}
	
	// Second log timer is expired
	if(log2Timer >= log2Expected){ 
		
		//clear the previous drawing
		eraseLog(logX[1], logY[1], isLose); 
		logX[1] -= 20; 
		
		// If the log run out of screen
		if (logX[1] < 0){
			
			// If the character is still on the log -> lose
			if (logContain[1] == 1) isLose = 1;
			
			// Else run from start point
			logX[1] = 863;
		}
		
		// Draw the log with new position
		drawLog(logX[1], logY[1], isLose);
		
		// If the character is on it, draw the character on it too
		if(logContain[1] == 1){
			characterX = logX[1] + 60;
			drawCharacter(characterX,characterY, character, isMoveUp);
		}
		
		// Reset timer for the second log
		asm volatile ("mrs %0, cntfrq_el0" : "=r"(log2Freg));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(log2Expected));
		log2Expected+=((log2Freg/1000)*80000)/1000;
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
		if ((characterX >= carX[ii] && characterX < carX[ii] + 80) || ((characterX+ 37) >= carX[ii] && (characterX +37)< carX[ii] + 80))
			if ((characterY >= carY[ii] && characterY < carY[ii] + 50) || ((characterY+ 50) >= carY[ii] && (characterY +50)< carY[ii] + 50)) 
				return 1;
	}
	return 0;
}


/**
 * Reset for new round or play again
 */
void reset(int round){
	
	// Set the character back to the start position
	characterX = 100;
	characterY = 710;
	isMoveUp = 1;
	
	// If the round 1 is finished -> move to round 2
	if (round == 1){
		state = round2;
		
		// Set position for each car in round 2
		carY[0] = 292;
		carY[1] = 354;
		carY[2] = 416;
		carY[3] = 478;
		carY[4] = 538;
		carY[5] = 600;
		
		// Display instruction
		if(!displayInstruction(2)) isEsc = 1;
		
		// Draw components for round 2
		drawMap(2, isLose);
		drawCharacter(characterX,characterY, character, isMoveUp);
	}
	
	// If the round 2 is finished -> move to round 3
	else if (round == 2){
		state = round3;
		
		// Set y position for each car in round 3
		carY[0] = 292;
		carY[1] = 354;
		carY[2] = 416;
		carY[3] = 478;
		carY[4] = 538;
		carY[5] = 600;
		carY[6] = 292;
		carY[7] = 354;
		carY[8] = 416;
		carY[9] = 478;
		carY[10] = 538;
		carY[11] = 600;
		
		// Set x position for each car in round 3
		carX[0] = 150;
		carX[1] = 0;
		carX[2] = 300;
		carX[3] = 100;
		carX[4] = 400;
		carX[5] = 220;
		carX[6] = 950;
		carX[7] = 615;
		carX[8] = 680;
		carX[9] = 515;
		carX[10] = 800;
		carX[11] = 515;
		
		// Display the instruction first
		if(!displayInstruction(3)) isEsc = 1;
		
		// Then, display round3's components
		drawMap(3, isLose);
		drawCharacter(characterX,characterY, character, isMoveUp);
	}
	else{
		
		// Reset the y position of cars to the default one
		carY[7] = 588;
		carY[6] = 526;
		carY[5] = 464;
		carY[4] = 350;
		carY[3] = 288;
		carY[2] = 226;
		carY[1] = 164;
		carY[0] = 102;
		
		// Reset the x position of cars to the default one
		carX[7] = 0;
		carX[6] = 933;
		carX[5] = 600;
		carX[4] = 500;
		carX[3] = 0;
		carX[2] = 300;
		carX[1] = 800;
		carX[0] = 150;
		
		// If the user finish round 3 -> win 
		if (round == 3){
			displayBackground(1024, 768, endscr);
			state = winning;
		}
		
		// If the user is lose and want to play again
		else {
			
			// Move back to welcome screen
			displayWelcomeScreen(character);
			logContain[0] = 0;
			logContain[1] = 0;
			isLose = 0;
			state = welcome;
		}
	}
}