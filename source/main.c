#include "../header/main.h"
#include "../header/display.h"
#include "../header/welcomepic.h"
#include "../header/endpic.h"
#include "../header/framebf.h"
#include "../header/uart.h"

enum gameState {welcome, round1, ending};
enum gameState state = welcome;
int animal = 0;
int animal_x = 100;
int animal_y = 710;
int car_x[] = {0,933,600,500,0,300,800,150};//starting x position of each car
int car_y[] = {102,164,226,288,350,464,526,588}; //lanes

void main_game_handler(char c) {
	if (c == 'a' || c == 'A'){
		if (animal_x - 20 >= 0) {
			avatarMove(animal_x, animal_y);
			animal_x -= 20;
			drawAvatar(animal_x,animal_y, animal);
		}
	}
	else if (c == 'd' || c== 'D'){
		if (animal_x + 20 <= 986){
			avatarMove(animal_x, animal_y);
			animal_x += 20;
			drawAvatar(animal_x,animal_y, animal);
		}
	}
	if (c == 'w' || c == 'W'){
		if (animal_y - 20 >= 0) {
			avatarMove(animal_x, animal_y);
			animal_y -= 20;
			drawAvatar(animal_x,animal_y, animal);

		}
	}
	else if (c == 's' || c== 'S'){
		if (animal_y + 20 <= 710){
			avatarMove(animal_x, animal_y);
			animal_y += 20;
			drawAvatar(animal_x,animal_y, animal);
		}
	}
	if (animal_y <= 10 && animal_x + 60 >= 995 ) {
			displayPicture(1024, 768, endscr);
			animal_x = 100;
			animal_y = 710;
			state = ending;
			//animal_x= 100;
			//animal_y = 590;
			//display_map2();
			//drawAvatar(animal_x,animal_y, animal);
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
	// calculate expire value for counter
	t1+=((f1/1000)*90000)/1000;
	t2+=((f2/1000)*50000)/1000;
	t3+=((f3/1000)*50000)/1000; //unused (yet)

	while(c != 27) {
		c = uart_getc();
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
			asm volatile ("mrs %0, cntpct_el0" : "=r"(r1));
			asm volatile ("mrs %0, cntpct_el0" : "=r"(r2));
			asm volatile ("mrs %0, cntpct_el0" : "=r"(r3));

			if (r1 >= t1 && c != 0) {
				main_game_handler(c);

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
					carMove(car_x[i], car_y[i]); //clear the previous drawing
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
