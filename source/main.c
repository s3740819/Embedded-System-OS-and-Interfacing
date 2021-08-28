#include "../header/main.h"
#include "../header/picture/welcomepic.h"
#include "../header/picture/endpic.h"
#include "../header/uart.h"
#include "../header/framebf.h"


enum gameState {welcome, round1, round2, ending};
enum gameState state = welcome;
int animal = 0;
int animal_x = 100;
int animal_y = 710;
int car_x[] = {0,933,600,500,0,300,800,150};//starting x position of each car
int car_y[] = {102,164,226,288,350,464,526,588}; //lanes
int log_x[] = {0, 860};
int log_y[] = {90, 165};
int log_contain[] = {0,0};
void main_game_handler(char c, int round) {
	if (c == 'a' || c == 'A'){
		if (animal_x - 20 >= 0) {
			if ((round == 2 && animal_y != 170 && animal_y != 90) || round == 1){
				avatarMove(animal_x, animal_y, round);
				animal_x -= 20;
				drawAvatar(animal_x,animal_y, animal);
			}
		}
	}
	else if (c == 'd' || c== 'D'){
		if (animal_x + 20 <= 986){
			if ((round == 2 && animal_y != 170 && animal_y != 90) || round == 1){
				avatarMove(animal_x, animal_y, round);
				animal_x += 20;
				drawAvatar(animal_x,animal_y, animal);
			}
		}
	}
	if (c == 'w' || c == 'W'){
		if (animal_y - 20 >= 0) {
			avatarMove(animal_x, animal_y, round);
			if(animal_y == 230 && round ==2){
				if(animal_x > log_x[1] && animal_x < log_x[1] + 160){
					animal_y = 170;
					animal_x = log_x[1] + 60;
					log_contain[1] = 1;
				}
				else uart_puts("Outt!!");
			}
			else if(animal_y == 170 && round ==2){
				if(animal_x > log_x[0] && animal_x < log_x[0] + 160){
					drawLog(log_x[1], log_y[1]);
					animal_y = 90;
					animal_x = log_x[0] + 60;
					log_contain[0] = 1;
					log_contain[1] = 0;
				}
				else uart_puts("Outt!!");
			}
			else if(animal_y == 90 && round == 2){
				drawLog(log_x[0], log_y[0]);
				animal_y = 30;
				log_contain[0] = 0;
			}
			else{
				animal_y -= 20;
			}
			drawAvatar(animal_x,animal_y, animal);
		}
	}
	else if (c == 's' || c== 'S'){
		if (animal_y + 20 <= 710){
			avatarMove(animal_x, animal_y, round);
			if(animal_y == 30 && round ==2){
				if(animal_x > log_x[0] && animal_x < log_x[0] + 160){
					animal_y = 90;
					animal_x = log_x[0] + 60;
					log_contain[0] = 1;
				}
				else uart_puts("Outt!!");
			}
			else if(animal_y == 90 && round ==2){
				if(animal_x > log_x[1] && animal_x < log_x[1] + 160){
					drawLog(log_x[0], log_y[0]);
					animal_y = 170;
					animal_x = log_x[1] + 60;
					log_contain[1] = 1;
					log_contain[0] = 0;
				}
				else uart_puts("Outt!!");
			}
			else if(animal_y == 170 && round == 2){
				drawLog(log_x[1], log_y[1]);
				animal_y = 230;
				log_contain[1] = 0;
			}
			else{
				animal_y += 20;
			}
			drawAvatar(animal_x,animal_y, animal);
		}
	}
	if (animal_y <= 10 && animal_x + 60 >= 995 ) {
			if (round == 1){
				state = round2;
				
				car_y[0] = 292;
				car_y[1] = 354;
				car_y[2] = 416;
				car_y[3] = 480;
				car_y[4] = 540;
				car_y[5] = 602;

				animal_x = 100;
				animal_y = 710;
				display_map2();

				drawAvatar(animal_x,animal_y, animal);
			}
			else{
				displayPicture(1024, 768, endscr);
				state = ending;
			}
			
	}
}

void main(){
	uart_init();
	framebf_init();

	while(1) {
		char c = 0;
		while(c != '\n') c = uart_getc();
		StateMachine();
	}
}


void StateMachine() {
	char c = 0;
	displayPicture(1024, 768, welcomescr);
	display_avatars(280,110, 0);
	display_avatars(490,110, 1);
	display_avatars(700,110, 2);
	state = welcome;
	animal = 0;
	select(animal);

	register unsigned long f1, t1, r1, f2, t2, r2, f3, t3, r3;
	register unsigned long f4, t4, r4, f5, t5, r5;

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
	
	// calculate expire value for counter
	t1+=((f1/1000)*90000)/1000;
	t2+=((f2/1000)*50000)/1000;
	t3+=((f3/1000)*40000)/1000; //unused (yet)
	t4+=((f4/1000)*45000)/1000; //unused (yet)
	t5+=((f5/1000)*80000)/1000; //unused (yet)
	
	while(c != 27) {
		c = uart_getc();
		
		asm volatile ("mrs %0, cntpct_el0" : "=r"(r1));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(r2));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(r3));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(r4));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(r5));
		
		switch(state) {
		case welcome:
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
				display_map1();
				drawAvatar(animal_x,animal_y, animal);
				state = round1;
			}
			break;

		case round1:
			if (r1 >= t1 && c != 0) {
				main_game_handler(c, 1);

				// get the current counter frequency
				asm volatile ("mrs %0, cntfrq_el0" : "=r"(f1));
				// read the current counter
				asm volatile ("mrs %0, cntpct_el0" : "=r"(t1));
				// calculate expire value for counter
				t1+=((f1/1000)*90000)/1000;
			}

			// car run
			if (r2 >= t2) {
				for (int i = 0; i < 8; i++) { // 8 cars on street lanes
					carMove(car_x[i], car_y[i],1); //clear the previous drawing
					if (i == 1 || i == 2 || i == 7) { // lane 2,3,8 from top
						car_x[i] -= 20; // redraw a consecutive image horizontally
						if (car_x[i] <= 0) car_x[i] = 933;
						drawCar(car_x[i], car_y[i], 0); // car heading right
					}
					else {
						car_x[i] += 20;
						if (car_x[i] >= 1024) car_x[i] = 0;
						drawCar(car_x[i], car_y[i], 1);
					}
				}
				// get the current counter frequency
				asm volatile ("mrs %0, cntfrq_el0" : "=r"(f2));
				// read the current counter
				asm volatile ("mrs %0, cntpct_el0" : "=r"(t2));
				// calculate expire value for counter
				t2+=((f2/1000)*50000)/1000;
			}

			break;
			
			
		case round2:
			if (r1 >= t1 && c != 0) {
				main_game_handler(c, 2);
	
				// get the current counter frequency
				asm volatile ("mrs %0, cntfrq_el0" : "=r"(f1));
				// read the current counter
				asm volatile ("mrs %0, cntpct_el0" : "=r"(t1));
				// calculate expire value for counter
				t1+=((f1/1000)*90000)/1000;
			}
			
			// car run
			if (r3 >= t3) {
				for (int i = 0; i < 6; i++) { // 8 cars on street lanes
					carMove(car_x[i], car_y[i],2); //clear the previous drawing
					if (i == 1 || i == 2 || i == 7) { // lane 2,3,8 from top
						car_x[i] -= 20; // redraw a consecutive image horizontally
						if (car_x[i] <= 0) car_x[i] = 933;
						drawCar(car_x[i], car_y[i], 0); // car heading right
					}
					else {
						car_x[i] += 20;
						if (car_x[i] >= 1024) car_x[i] = 0;
						drawCar(car_x[i], car_y[i], 1);
					}
				}
				// get the current counter frequency
				asm volatile ("mrs %0, cntfrq_el0" : "=r"(f3));
				// read the current counter
				asm volatile ("mrs %0, cntpct_el0" : "=r"(t3));
				// calculate expire value for counter
				t3+=((f3/1000)*40000)/1000;
			}
			
			if(r4 >= t4){
				
				logMove(log_x[0], log_y[0]); //clear the previous drawing
				log_x[0] += 20; // redraw a consecutive image horizontally
				if (log_x[0] > 924) {
					if (log_contain[0] == 1) uart_puts("Out!");
					log_x[0] = 0;
				}
				drawLog(log_x[0], log_y[0]); // car heading right
				
				if(log_contain[0] == 1){
					animal_x = log_x[0] + 60;
					drawAvatar(animal_x,animal_y, animal);
				}

				// get the current counter frequency
				asm volatile ("mrs %0, cntfrq_el0" : "=r"(f4));
				// read the current counter
				asm volatile ("mrs %0, cntpct_el0" : "=r"(t4));
				// calculate expire value for counter
				t4+=((f4/1000)*45000)/1000;
			}
			
			if(r5 >= t5){
				logMove(log_x[1], log_y[1]); //clear the previous drawing
				log_x[1] -= 20; // redraw a consecutive image horizontally
				if (log_x[1] < 0){
					if (log_contain[1] == 1) uart_puts("Out!");
					log_x[1] = 863;
				}
				drawLog(log_x[1], log_y[1]); // car heading right
				
				if(log_contain[1] == 1){
					animal_x = log_x[1] + 60;
					drawAvatar(animal_x,animal_y, animal);
				}
				
				// get the current counter frequency
				asm volatile ("mrs %0, cntfrq_el0" : "=r"(f5));
				// read the current counter
				asm volatile ("mrs %0, cntpct_el0" : "=r"(t5));
				// calculate expire value for counter
				t5+=((f5/1000)*80000)/1000;
			}
	
			break;

		case ending:
			if (c == '\n') {
				displayPicture(1024, 768, welcomescr);
				display_avatars(280,110, 0);
				display_avatars(490,110, 1);
				display_avatars(700,110, 2);
				animal = 0;
				select(0);
				state = welcome;
			}
			break;
		}
	}
	uart_puts("OUT!");
	drawRectARGB32(0,0,1024,768,0x00000000,1); //Clear screen
}
