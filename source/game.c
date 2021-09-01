#include "../header/game/game.h"
#include "../header/uart.h"
#include "../header/framebf.h"
#include "../header/game/picture/welcomepic.h"
#include "../header/game/picture/endpic.h"

enum gameState {welcome, round1, round2, round3, winning};
enum gameState state = welcome;
int animal = 0;
int animal_x = 100;
int animal_y = 710;
int car_x[] = {0,933,600,500,0,300,800,150,0,0,0,0};//starting x position of each car
int car_y[] = {102,164,226,288,350,464,526,588,0,0,0,0}; //lanes
int log_x[] = {0, 860};
int log_y[] = {90, 165};
int log_contain[] = {0,0};
unsigned long f1, t1, r1, f2, t2, r2, f3, t3, r3;
unsigned long f4, t4, r4, f5, t5, r5, f6, t6, r6;
int is_lose = 0;
int is_goup = 1;

void game_init(){
	displayPicture(1024, 768, welcomescr);
	display_avatars(280,110, 0);
	display_avatars(490,110, 1);
	display_avatars(700,110, 2);
	state = welcome;
	animal = 0;
	select(animal);

	// get the current counter frequency
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(f1));
	// read the current counter
	asm volatile ("mrs %0, cntpct_el0" : "=r"(t1));
	// get the current counter frequency
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(f2));
	// read the current counter
	asm volatile ("mrs %0, cntpct_el0" : "=r"(t2));
	// get the current counter frequency
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(f3));
	// read the current counter
	asm volatile ("mrs %0, cntpct_el0" : "=r"(t3));
	// get the current counter frequency
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(f4));
	// read the current counter
	asm volatile ("mrs %0, cntpct_el0" : "=r"(t4));
	// get the current counter frequency
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(f5));
	// read the current counter
	asm volatile ("mrs %0, cntpct_el0" : "=r"(t5));
	// get the current counter frequency
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(f6));
	// read the current counter
	asm volatile ("mrs %0, cntpct_el0" : "=r"(t6));
	
	// calculate expire value for counter
	t1+=((f1/1000)*90000)/1000;
	t2+=((f2/1000)*50000)/1000;
	t3+=((f3/1000)*40000)/1000; 
	t4+=((f4/1000)*45000)/1000; 
	t5+=((f5/1000)*80000)/1000; 
	t6+=((f6/1000)*85000)/1000; 

}

void executeGame() {
	char c = 0;
	game_init();
	
	while(c != 27) {
		
		c = uart_getc();
		
		asm volatile ("mrs %0, cntpct_el0" : "=r"(r1));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(r2));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(r3));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(r4));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(r5));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(r6));

		switch(state) {
		case welcome:
			is_lose = 0;
			if (c == 'a' || c == 'A') {
				if (animal > 0) {
					displayPicture(1024, 768, welcomescr);
					display_avatars(280,110, 0);
					display_avatars(490,110, 1);
					display_avatars(700,110, 2);
					animal--;
					select(animal);
				}
			}
			else if (c == 'd' || c== 'D') {
				if (animal < 2) {
					displayPicture(1024, 768, welcomescr);
					display_avatars(280,110, 0);
					display_avatars(490,110, 1);
					display_avatars(700,110, 2);
					animal ++;
					select(animal);
				}
			}
			else if (c == '\n') {
				reset(0);
				display_instruction(1);
				display_map(1,is_lose);
				drawAvatar(animal_x,animal_y, animal, is_goup);
				state = round1;
			}
			break;

		case round1:			
			// car run
			carRun(1);
			
			if(is_lose){
				if (is_lose == 1 ) {
					display_map(1, is_lose);
					display_skull();
				}
				is_lose++;
				if (c == '\n') reset(0);
			}
			else control_button_handler(c, 1);

			break;
			
			
		case round2:			
			// car run
			carRun(2);
			
			logRun();
			
			if(is_lose){
				if(is_lose == 1){
					display_map(2, is_lose);
					display_skull();
				}
				is_lose++;
				if (c == '\n') reset(0);
			}
			else control_button_handler(c, 2);
	
			break;
		case round3:			
			// car run
			carRun(3);
			
			if(is_lose){
				if(is_lose == 1){
					display_map(3, is_lose);
					display_skull();
				}
				is_lose++;
				if (c == '\n') reset(0);
			}
			else control_button_handler(c, 3);
	
			break;
			
		case winning:
			if (c == '\n') {
				 reset(0);
			}
			break;
		}
	}
	resetScreen();
}

void control_button_handler(char c, int round){
	if (r1 >= t1 && c != 0) {
		main_game_handler(c, round);

		// get the current counter frequency
		asm volatile ("mrs %0, cntfrq_el0" : "=r"(f1));
		// read the current counter
		asm volatile ("mrs %0, cntpct_el0" : "=r"(t1));
		// calculate expire value for counter
		t1+=((f1/1000)*90000)/1000;
	}
}

void main_game_handler(char c, int round) {
	if (c == 'a' || c == 'A'){
		if (animal_x - 20 >= 0 && !isTree(animal_x - 20, animal_y, round)) {
			if ((round == 2 && animal_y != 170 && animal_y != 90) || round == 1 || (round == 3 && (animal_y >= 230 || animal_y <= 30))){
				avatarMove(animal_x, animal_y, round);
				animal_x -= 20;
				drawAvatar(animal_x,animal_y, animal, is_goup);
			}
		}
	}
	else if (c == 'd' || c== 'D'){
		if (animal_x + 20 <= 986 && !isTree(animal_x + 20, animal_y, round)){
			if ((round == 2 && animal_y != 170 && animal_y != 90) || round == 1 || (round == 3 && (animal_y >= 230 || animal_y <= 30))){
				avatarMove(animal_x, animal_y, round);
				animal_x += 20;
				drawAvatar(animal_x,animal_y, animal, is_goup);
			}
		}
	}
	if (c == 'w' || c == 'W'){
		if (animal_y - 20 >= 0 && !isTree(animal_x, animal_y - 20, round)) {
			avatarMove(animal_x, animal_y, round);
			if(animal_y == 230 && round ==2) { //on first log
				if(animal_x > log_x[1] && animal_x < log_x[1] + 160) { //avatar still on the log?
					animal_y = 170;
					animal_x = log_x[1] + 60;
					log_contain[1] = 1;
				}
				else is_lose = 1;
			}
			else if(animal_y == 170 && round ==2){ //on second log
				if(animal_x > log_x[0] && animal_x < log_x[0] + 160){
					drawLog(log_x[1], log_y[1], is_lose);
					animal_y = 90;
					animal_x = log_x[0] + 60;
					log_contain[0] = 1;
					log_contain[1] = 0;
				}
				else is_lose = 1;
			}
			else if(animal_y == 90 && round == 2){
				drawLog(log_x[0], log_y[0], is_lose);
				animal_y = 30;
				log_contain[0] = 0;
			}
			else if (animal_y == 230 && round == 3 && !((animal_x >= 80 && animal_x <= 140) || (animal_x >= 860 && animal_x <= 900))){
				is_lose = 1;
			}
			else{
				animal_y -= 20;
			}
			is_goup = 1;
			drawAvatar(animal_x,animal_y, animal, is_goup);
		}
	}
	else if (c == 's' || c== 'S'){
		if (animal_y + 20 <= 710 && !isTree(animal_x, animal_y + 20, round)){
			avatarMove(animal_x, animal_y, round);
			if(animal_y == 30 && round ==2){
				if(animal_x > log_x[0] && animal_x < log_x[0] + 160){
					animal_y = 90;
					animal_x = log_x[0] + 60;
					log_contain[0] = 1;
				}
				else is_lose = 1;
			}
			else if(animal_y == 90 && round ==2){
				if(animal_x > log_x[1] && animal_x < log_x[1] + 160){
					drawLog(log_x[0], log_y[0], is_lose);
					animal_y = 170;
					animal_x = log_x[1] + 60;
					log_contain[1] = 1;
					log_contain[0] = 0;
				}
				else is_lose = 1;
			}
			else if(animal_y == 170 && round == 2){
				drawLog(log_x[1], log_y[1], is_lose);
				animal_y = 230;
				log_contain[1] = 0;
			}
			else if (animal_y == 30 && round == 3 && !((animal_x >= 80 && animal_x <= 140) || (animal_x >= 860 && animal_x <= 900))){
				is_lose = 1;
			}
			else{
				animal_y += 20;
			}
			is_goup = 0;
			drawAvatar(animal_x,animal_y, animal, is_goup);
		}
	}
	if ((animal_y <= 10 && animal_x + 60 >= 995 && round < 3)  || (animal_y >= 670 && animal_x +50 >= 520 && animal_x <= 550 && round == 3)) {
		reset(round);
	}	
}

void carRun(int round){
	if (round == 1){
		if (r2 >= t2) {
			for (int i = 0; i < 8; i++) { // 8 cars on street lanes
				if(is_lose != 2) carMove(car_x[i], car_y[i],1, is_lose); //clear the previous drawing
				if (i == 1 || i == 2 || i == 7) { // lane 2,3,8 from top
					car_x[i] -= 20; // redraw a consecutive image horizontally
					if (car_x[i] <= 0) car_x[i] = 933;
					if (!(is_lose && car_x[i] >= 362 && car_x[i] <= 662))
						drawCar(car_x[i], car_y[i], 0, is_lose); // car heading right
					else car_x[i] = 310;
				}
				else {
					car_x[i] += 20;
					if (car_x[i] >= 1024) car_x[i] = 0;
					if (!(is_lose && car_x[i]+45 >= 362 && car_x[i]+45 <= 662)) 
						drawCar(car_x[i], car_y[i], 1, is_lose);
					else car_x[i] = 664;
				}
			}
			// get the current counter frequency
			asm volatile ("mrs %0, cntfrq_el0" : "=r"(f2));
			// read the current counter
			asm volatile ("mrs %0, cntpct_el0" : "=r"(t2));
			// calculate expire value for counter
			t2+=((f2/1000)*50000)/1000;
		}
	}
	else if (round == 2) {
		if (r3 >= t3) {
			for (int i = 0; i < 6; i++) { // 8 cars on street lanes
				if(is_lose != 2) carMove(car_x[i], car_y[i],2, is_lose); //clear the previous drawing
				if (i == 1 || i == 3) { // lane 2,3,8 from top
					car_x[i] -= 20; // redraw a consecutive image horizontally
					if (car_x[i] <= 0) car_x[i] = 933;
					if (!(is_lose && car_x[i] >= 362 && car_x[i] <= 662))
						drawCar(car_x[i], car_y[i], 0, is_lose); // car heading right
					else car_x[i] = 310;
				}
				else {
					car_x[i] += 20;
					if (car_x[i] >= 1024) car_x[i] = 0;
					if (!(is_lose && car_x[i]+45 >= 362 && car_x[i]+45 <= 662))
						drawCar(car_x[i], car_y[i], 1, is_lose); // car heading left
					else car_x[i] = 664;
				}
			}
			// get the current counter frequency
			asm volatile ("mrs %0, cntfrq_el0" : "=r"(f3));
			// read the current counter
			asm volatile ("mrs %0, cntpct_el0" : "=r"(t3));
			// calculate expire value for counter
			t3+=((f3/1000)*40000)/1000;
		}
	}
	else {
		if (r6 >= t6) {
			for (int i = 0; i < 12; i++) { // 8 cars on street lanes
				if(is_lose != 2) carMove(car_x[i], car_y[i],2, is_lose); //clear the previous drawing
				if (i == 1 || i == 3 || i == 4 || i == 10 || i == 8) { // lane 2,3,8 from top
					car_x[i] -= 20; // redraw a consecutive image horizontally
					
					if (car_x[i] <= 0 && i < 6) car_x[i] = 400;
					else if (car_x[i] <= 515 && i >= 6) car_x[i] = 933;
					
					if (!(is_lose && car_x[i] >= 362 && car_x[i] <= 662))
						drawCar(car_x[i], car_y[i], 0, is_lose); // car heading right
					else car_x[i] = 310;
				}
				else {
					car_x[i] += 20;
					
					if (car_x[i] > 933 && i >= 6) car_x[i] = 512;
					else if (car_x[i] > 400 && i < 6) car_x[i] = 0;

					if (!(is_lose && car_x[i]+45 >= 362 && car_x[i]+45 <= 662))
						drawCar(car_x[i], car_y[i], 1, is_lose); // car heading left
					else car_x[i] = 664;
				}
			}
			// get the current counter frequency
			asm volatile ("mrs %0, cntfrq_el0" : "=r"(f6));
			// read the current counter
			asm volatile ("mrs %0, cntpct_el0" : "=r"(t6));
			// calculate expire value for counter
			t6+=((f6/1000)*85000)/1000;
		}
	}
	if (is_lose == 0){
		if(is_hit(round)) is_lose = 1;
	}
}

void logRun(){
	if(r4 >= t4){// second log moving
		logMove(log_x[0], log_y[0], is_lose); //clear the previous drawing
		log_x[0] += 20; // redraw a consecutive image horizontally
		if (log_x[0] > 924) { // if the log is out of sight but avatar still on it
			if (log_contain[0] == 1) is_lose = 1; //losing
			log_x[0] = 0;
		}
		drawLog(log_x[0], log_y[0], is_lose);
		
		if(log_contain[0] == 1){ //containing the avatar
			animal_x = log_x[0] + 60;
			drawAvatar(animal_x,animal_y, animal, is_goup);
		}

		// get the current counter frequency
		asm volatile ("mrs %0, cntfrq_el0" : "=r"(f4));
		// read the current counter
		asm volatile ("mrs %0, cntpct_el0" : "=r"(t4));
		// calculate expire value for counter
		t4+=((f4/1000)*45000)/1000;
	}
	
	if(r5 >= t5){ // first log moving
		logMove(log_x[1], log_y[1], is_lose); //clear the previous drawing
		log_x[1] -= 20; // redraw a consecutive image horizontally
		if (log_x[1] < 0){
			if (log_contain[1] == 1) is_lose = 1;
			log_x[1] = 863;
		}
		drawLog(log_x[1], log_y[1], is_lose);
		
		if(log_contain[1] == 1){
			animal_x = log_x[1] + 60;
			drawAvatar(animal_x,animal_y, animal, is_goup);
		}
		
		// get the current counter frequency
		asm volatile ("mrs %0, cntfrq_el0" : "=r"(f5));
		// read the current counter
		asm volatile ("mrs %0, cntpct_el0" : "=r"(t5));
		// calculate expire value for counter
		t5+=((f5/1000)*80000)/1000;
	}
}

int is_hit(int round){
	int num = 0;
	if (round == 1) num = 8;
	else if (round == 2) num = 6;
	else num = 12;
	for (int ii = 0; ii < num ; ii++ ){
		if ((animal_x >= car_x[ii] && animal_x < car_x[ii] + 45) || ((animal_x+ 37) >= car_x[ii] && (animal_x +37)< car_x[ii] + 45)){
			if ((animal_y >= car_y[ii] && animal_y < car_y[ii] + 50) || ((animal_y+ 50) >= car_y[ii] && (animal_y +50)< car_y[ii] + 50)){
				return 1;
			}
		}

	}
	return 0;
}

void reset(int round){
	animal_x = 100;
	animal_y = 710;
	is_goup = 1;
	if (round == 1){
		state = round2;
		
		car_y[0] = 292;
		car_y[1] = 354;
		car_y[2] = 416;
		car_y[3] = 478;
		car_y[4] = 538;
		car_y[5] = 600;
		display_instruction(2);
		display_map(2, is_lose);
		drawAvatar(animal_x,animal_y, animal, is_goup);
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
		car_x[6] = 615;
		car_x[7] = 950;
		car_x[8] = 880;
		car_x[9] = 515;
		car_x[10] = 780;
		car_x[11] = 800;
		display_instruction(3);
		display_map(3, is_lose);
		drawAvatar(animal_x,animal_y, animal, is_goup);
		
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
			displayPicture(1024, 768, welcomescr);
			display_avatars(280,110, 0);
			display_avatars(490,110, 1);
			display_avatars(700,110, 2);
			select(animal);
			log_contain[0] = 0;
			log_contain[1] = 0;
			state = welcome;
		}
	}
}