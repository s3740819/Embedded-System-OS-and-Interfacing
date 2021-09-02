#include "../header/game/game.h"
#include "../header/uart.h"
#include "../header/framebf.h"
#include "../header/game/picture/endpic.h"

enum gameState {welcome, round1, round2, round3, winning};
enum gameState state = welcome;
int character = 0;
int character_x = 100;
int character_y = 710;
int car_x[] = {0,933,600,500,0,300,800,150,0,0,0,0};//starting x position of each car
int car_y[] = {102,164,226,288,350,464,526,588,0,0,0,0}; //lanes
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


void gameInit(){
	// Display welcome screen
	displayWelcomeScreen(character);
	
	// reset value 
	state = welcome;
	character = 0;
	is_esc = 0;

	
	// Set timers 
	// get the current counter frequency
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(debouncing_f));
	// read the current counter
	asm volatile ("mrs %0, cntpct_el0" : "=r"(debouncing_t));
	// get the current counter frequency
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(car_f));
	// read the current counter
	asm volatile ("mrs %0, cntpct_el0" : "=r"(car_t));
	// get the current counter frequency
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(log1_f));
	// read the current counter
	asm volatile ("mrs %0, cntpct_el0" : "=r"(log1_t));
	// get the current counter frequency
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(log2_f));
	// read the current counter
	asm volatile ("mrs %0, cntpct_el0" : "=r"(log2_t));	
	// get the current counter frequency
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(railroad_f));
	// read the current counter
	asm volatile ("mrs %0, cntpct_el0" : "=r"(railroad_t));
	// get the current counter frequency
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(train_f));
	// read the current counter
	asm volatile ("mrs %0, cntpct_el0" : "=r"(train_t));
	
	// calculate expire value for counter
	debouncing_t+=((debouncing_f/1000)*90000)/1000;
	car_t+=((car_f/1000)*50000)/1000;
	log1_t+=((log1_f/1000)*45000)/1000; 
	log2_t+=((log2_f/1000)*80000)/1000; 
	railroad_t+=((railroad_f/1000)*5000000)/1000; 
	train_t+=((train_f/1000)*40000)/1000; 
}

void gameOverHandler(char c, int round){
	if (is_lose == 1) {
		displayMap(round, is_lose);
		displaySkull();
	}
	is_lose++;
	if (c == '\n') reset(0);
}

void executeGame() {
	char c = 0;
	gameInit();
	
	while(c != 27 && !is_esc) {
		
		c = uart_getc();
		
		asm volatile ("mrs %0, cntpct_el0" : "=r"(deboucing_r));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(car_r));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(log1_r));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(log2_r));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(railroad_r));

		switch(state) {
		case welcome:
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
			else if (c == '\n') {
				reset(0);
				if(!displayInstruction(1)) is_esc = 1;
				displayMap(1,is_lose);
				drawCharacterAvatar(character_x,character_y, character, is_moveUp);
				state = round1;
			}
			break;

		case round1:	
			
			// car run
			carRun(1);
			
			if(is_lose) gameOverHandler(c,1);
			else buttonHandler(c, 1);

			break;
			
			
		case round2:		
			
			// car run
			carRun(2);
			
			logRun();
			
			if(is_lose) gameOverHandler(c,2);
			else buttonHandler(c, 2);
	
			break;
		case round3:
			
			trainRun();
			
			// car run
			carRun(3);
			
			if(is_lose){
				gameOverHandler(c,3);
			}
			else buttonHandler(c, 3);
	
			break;
			
		case winning:
			
			if (c == '\n') reset(0);
			break;
		}
	}
	resetScreen();
}

void buttonHandler(char c, int round){
	if (deboucing_r >= debouncing_t && c != 0) {
		controlCharacter(c, round);

		// get the current counter frequency
		asm volatile ("mrs %0, cntfrq_el0" : "=r"(debouncing_f));
		// read the current counter
		asm volatile ("mrs %0, cntpct_el0" : "=r"(debouncing_t));
		// calculate expire value for counter
		debouncing_t+=((debouncing_f/1000)*90000)/1000;
	}
}

void controlCharacter(char c, int round) {
	if (c == 'a' || c == 'A'){
		if (character_x - 20 >= 0 && !isTree(character_x - 20, character_y, round)) {
			if ((round == 2 && character_y != 170 && character_y != 90) || round == 1 || (round == 3 && (character_y >= 230 || character_y <= 30))){
				characterMove(character_x, character_y, round);
				character_x -= 20;
				drawCharacterAvatar(character_x,character_y, character, is_moveUp);
			}
		}
	}
	else if (c == 'd' || c== 'D'){
		if (character_x + 20 <= 986 && !isTree(character_x + 20, character_y, round)){
			if ((round == 2 && character_y != 170 && character_y != 90) || round == 1 || (round == 3 && (character_y >= 230 || character_y <= 30))){
				characterMove(character_x, character_y, round);
				character_x += 20;
				drawCharacterAvatar(character_x,character_y, character, is_moveUp);
			}
		}
	}
	if (c == 'w' || c == 'W'){
		if (character_y - 20 >= 0 && !isTree(character_x, character_y - 20, round)) {
			characterMove(character_x, character_y, round);
			if(character_y == 230 && round ==2) { //on first log
				if(character_x > log_x[1] && character_x < log_x[1] + 160) { //avatar still on the log?
					character_y = 170;
					character_x = log_x[1] + 60;
					log_contain[1] = 1;
				}
				else is_lose = 1;
			}
			else if(character_y == 170 && round ==2){ //on second log
				if(character_x > log_x[0] && character_x < log_x[0] + 160){
					drawLog(log_x[1], log_y[1], is_lose);
					character_y = 90;
					character_x = log_x[0] + 60;
					log_contain[0] = 1;
					log_contain[1] = 0;
				}
				else is_lose = 1;
			}
			else if(character_y == 90 && round == 2){
				drawLog(log_x[0], log_y[0], is_lose);
				character_y = 30;
				log_contain[0] = 0;
			}
			else if (character_y == 230 && round == 3 && !((character_x >= 80 && character_x <= 140) || (character_x >= 860 && character_x <= 900))){
				is_lose = 1;
			}
			else{
				character_y -= 20;
			}
			is_moveUp = 1;
			drawCharacterAvatar(character_x,character_y, character, is_moveUp);
		}
	}
	else if (c == 's' || c== 'S'){
		if (character_y + 20 <= 710 && !isTree(character_x, character_y + 20, round)){
			characterMove(character_x, character_y, round);
			if(character_y == 30 && round ==2){
				if(character_x > log_x[0] && character_x < log_x[0] + 160){
					character_y = 90;
					character_x = log_x[0] + 60;
					log_contain[0] = 1;
				}
				else is_lose = 1;
			}
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
			else if(character_y == 170 && round == 2){
				drawLog(log_x[1], log_y[1], is_lose);
				character_y = 230;
				log_contain[1] = 0;
			}
			else if (character_y == 30 && round == 3 && !((character_x >= 80 && character_x <= 140) || (character_x >= 860 && character_x <= 900))){
				is_lose = 1;
			}
			else{
				character_y += 20;
			}
			is_moveUp = 0;
			drawCharacterAvatar(character_x,character_y, character, is_moveUp);
		}
	}
	
	if ((character_y <= 10 && character_x + 60 >= 995 && round < 3)  || (character_y >= 670 && character_x +50 >= 520 && character_x <= 550 && round == 3)) {
		reset(round);
	}	
}

void trainRun(){
	if (railroad_r >= railroad_t){
		asm volatile ("mrs %0, cntpct_el0" : "=r"(train_r));
		if (train_r >= train_t){
			trainMove(train_x, is_lose);
			train_x-=10;
			if (train_x + 606 < 0){
				train_x = 1020;
				// get the current counter frequency
				asm volatile ("mrs %0, cntfrq_el0" : "=r"(railroad_f));
				// read the current counter
				asm volatile ("mrs %0, cntpct_el0" : "=r"(railroad_t));
				// calculate expire value for counter
				railroad_t+=((railroad_f/1000)*5000000)/1000;
			}
			// get the current counter frequency
			asm volatile ("mrs %0, cntfrq_el0" : "=r"(train_f));
			// read the current counter
			asm volatile ("mrs %0, cntpct_el0" : "=r"(train_t));
			// calculate expire value for counter
			train_t+=((train_f/1000)*40000)/1000;
			
			if (is_lose == 0 && character_y < 70 && character_x >= train_x && character_x <= train_x + 606){
				is_lose = 1;
			}
		}
	}
}

void carRun(int round){
	if (car_r >= car_t){
		if (round == 1){
			for (int i = 0; i < 8; i++) { // 8 cars on street lanes
				if(is_lose != 2) carMove(car_x[i], car_y[i], round, is_lose); //clear the previous drawing
				if (i == 1 || i == 2 || i == 7) { // lane 2,3,8 from top
					car_x[i] -= 20; // redraw a consecutive image horizontally
					
					if (car_x[i] <= 0) car_x[i] = 933;
					if (!(is_lose && car_x[i] >= 362 && car_x[i] <= 662)) drawCar(car_x[i], car_y[i], 0, is_lose); // car heading right
					else car_x[i] = 310;
				}
				
				else {
					car_x[i] += 20;
					
					if (car_x[i] >= 1024) car_x[i] = 0;
					if (!(is_lose && car_x[i]+45 >= 362 && car_x[i]+45 <= 662)) drawCar(car_x[i], car_y[i], 1, is_lose);
					else car_x[i] = 664;
				}
			}
			
			// get the current counter frequency
			asm volatile ("mrs %0, cntfrq_el0" : "=r"(car_f));
			// read the current counter
			asm volatile ("mrs %0, cntpct_el0" : "=r"(car_t));
			// calculate expire value for counter
			car_t+=((car_f/1000)*50000)/1000;
		}
		else if (round == 2) {
			for (int i = 0; i < 6; i++) { // 8 cars on street lanes
				if(is_lose != 2) carMove(car_x[i], car_y[i], round, is_lose); //clear the previous drawing
				if (i == 1 || i == 3) { // lane 2,3,8 from top
					car_x[i] -= 20; // redraw a consecutive image horizontally
					if (car_x[i] <= 0) car_x[i] = 933;
					if (!(is_lose && car_x[i] >= 362 && car_x[i] <= 662)) drawCar(car_x[i], car_y[i], 0, is_lose); // car heading right
					else car_x[i] = 310;
				}
				else {
					car_x[i] += 20;
					if (car_x[i] >= 1024) car_x[i] = 0;
					if (!(is_lose && car_x[i]+45 >= 362 && car_x[i]+45 <= 662)) drawCar(car_x[i], car_y[i], 1, is_lose); // car heading left
					else car_x[i] = 664;
				}
			}
			// get the current counter frequency
			asm volatile ("mrs %0, cntfrq_el0" : "=r"(car_f));
			// read the current counter
			asm volatile ("mrs %0, cntpct_el0" : "=r"(car_t));
			// calculate expire value for counter
			car_t+=((car_f/1000)*40000)/1000;
		}
		else {
			for (int i = 0; i < 12; i++) { // 8 cars on street lanes
				if(is_lose != 2) carMove(car_x[i], car_y[i], round, is_lose); //clear the previous drawing
				if (i == 1 || i == 3 || i == 4|| i == 6 || i == 7 || i == 9 || i == 11) { // lane 2,3,8 from top
					car_x[i] -= 20; // redraw a consecutive image horizontally
					
					if (car_x[i] <= 0 && i < 6) car_x[i] = 400;
					else if (car_x[i] <= 515 && i >= 6) car_x[i] = 933;
					
					if (!(is_lose && car_x[i] >= 362 && car_x[i] <= 662)) drawCar(car_x[i], car_y[i], 0, is_lose); // car heading right
					else car_x[i] = 310;
				}
				else {
					car_x[i] += 20;
					
					if (car_x[i] > 933 && i >= 6) car_x[i] = 512;
					else if (car_x[i] > 400 && i < 6) car_x[i] = 0;
	
					if (!(is_lose && car_x[i]+45 >= 362 && car_x[i]+45 <= 662)) drawCar(car_x[i], car_y[i], 1, is_lose); // car heading left
					else car_x[i] = 664;
				}
			}
			// get the current counter frequency
			asm volatile ("mrs %0, cntfrq_el0" : "=r"(car_f));
			// read the current counter
			asm volatile ("mrs %0, cntpct_el0" : "=r"(car_t));
			// calculate expire value for counter
			car_t+=((car_f/1000)*110000)/1000;
			
		}
	}
	
	if (is_lose == 0){
		if(isHit(round)) is_lose = 1;
	}
	
}

void logRun(){
	if (is_lose){
		log_contain[0] = 0;
		log_contain[1] = 0;
	}
	if(log1_r >= log1_t){// second log moving
		logMove(log_x[0], log_y[0], is_lose); //clear the previous drawing
		log_x[0] += 20; // redraw a consecutive image horizontally
		if (log_x[0] > 924) { // if the log is out of sight but avatar still on it
			if (log_contain[0] == 1) is_lose = 1; //losing
			log_x[0] = 0;
		}
		drawLog(log_x[0], log_y[0], is_lose);
		
		if(log_contain[0] == 1){ //containing the avatar
			character_x = log_x[0] + 60;
			drawCharacterAvatar(character_x,character_y, character, is_moveUp);
		}

		// get the current counter frequency
		asm volatile ("mrs %0, cntfrq_el0" : "=r"(log1_f));
		// read the current counter
		asm volatile ("mrs %0, cntpct_el0" : "=r"(log1_t));
		// calculate expire value for counter
		log1_t+=((log1_f/1000)*45000)/1000;
	}
	
	if(log2_r >= log2_t){ // first log moving
		logMove(log_x[1], log_y[1], is_lose); //clear the previous drawing
		log_x[1] -= 20; // redraw a consecutive image horizontally
		if (log_x[1] < 0){
			if (log_contain[1] == 1) is_lose = 1;
			log_x[1] = 863;
		}
		drawLog(log_x[1], log_y[1], is_lose);
		
		if(log_contain[1] == 1){
			character_x = log_x[1] + 60;
			drawCharacterAvatar(character_x,character_y, character, is_moveUp);
		}
		
		// get the current counter frequency
		asm volatile ("mrs %0, cntfrq_el0" : "=r"(log2_f));
		// read the current counter
		asm volatile ("mrs %0, cntpct_el0" : "=r"(log2_t));
		// calculate expire value for counter
		log2_t+=((log2_f/1000)*80000)/1000;
	}
}

int isHit(int round){
	int num = 0;
	if (round == 1) num = 8;
	else if (round == 2) num = 6;
	else num = 12;
	for (int ii = 0; ii < num ; ii++ ){
		if ((character_x >= car_x[ii] && character_x < car_x[ii] + 80) || ((character_x+ 37) >= car_x[ii] && (character_x +37)< car_x[ii] + 80)){
			if ((character_y >= car_y[ii] && character_y < car_y[ii] + 50) || ((character_y+ 50) >= car_y[ii] && (character_y +50)< car_y[ii] + 50)){
				return 1;
			}
		}

	}
	return 0;
}

void reset(int round){
	character_x = 100;
	character_y = 710;
	is_moveUp = 1;
	if (round == 1){
		state = round2;
		
		car_y[0] = 292;
		car_y[1] = 354;
		car_y[2] = 416;
		car_y[3] = 478;
		car_y[4] = 538;
		car_y[5] = 600;
		if(!displayInstruction(2)) is_esc = 1;
		displayMap(2, is_lose);
		drawCharacterAvatar(character_x,character_y, character, is_moveUp);
	}
	else if (round == 2){
		state = round3;
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
		if(!displayInstruction(3)) is_esc = 1;
		displayMap(3, is_lose);
		drawCharacterAvatar(character_x,character_y, character, is_moveUp);
		
	}
	else{
		car_y[7] = 588;
		car_y[6] = 526;
		car_y[5] = 464;
		car_y[4] = 350;
		car_y[3] = 288;
		car_y[2] = 226;
		car_y[1] = 164;
		car_y[0] = 102;
		
		car_x[7] = 0;
		car_x[6] = 933;
		car_x[5] = 600;
		car_x[4] = 500;
		car_x[3] = 0;
		car_x[2] = 300;
		car_x[1] = 800;
		car_x[0] = 150;
		
		if (round == 3){
			displayPicture(1024, 768, endscr);
			state = winning;
		}
		else {
			displayWelcomeScreen(character);
			log_contain[0] = 0;
			log_contain[1] = 0;
			is_lose = 0;
			state = welcome;
		}
	}
}