#include "../header/main.h"

int animal_x = 100;
int animal_y = 710;
int animal = 0;
int stage = 0;
int car_x[] = {0,933,600,500,0,300, 800, 150};
int car_y[] = {102,164,226,288,350,464,526,588};

void welcome_screen_handler(char c){
	if (c == 'a' || c == 'A'){
		if (animal > 0) {
			display_background(); 
			display_character(280,110, 0);
			display_character(490,110, 1);
			display_character(700,110, 2);
			animal--;
			select(animal);
		}
	}
	else if (c == 'd' || c== 'D'){
		if (animal < 2) {
			display_background(); 
			display_character(280,110, 0);
			display_character(490,110, 1);
			display_character(700,110, 2);
			animal ++;
			select(animal);
		}
	}
	else if (c == '\n'){
		stage = 1;
		display_map1();
		display_character(animal_x,animal_y, animal);
	}

}

void main_game_handler(char c){
	if (c == 'a' || c == 'A'){
		if (animal_x - 20 >= 0) {
			character_move(animal_x, animal_y);
			animal_x -= 20;
			display_character(animal_x,animal_y, animal);
		}
	}
	else if (c == 'd' || c== 'D'){
		if (animal_x + 20 <= 986){
			character_move(animal_x, animal_y);
			animal_x += 20;
			display_character(animal_x,animal_y, animal);
		}
	}
	if (c == 'w' || c == 'W'){
		if (animal_y - 20 >= 0) {
			character_move(animal_x, animal_y);
			animal_y -= 20;
			display_character(animal_x,animal_y, animal);

		}
	}
	else if (c == 's' || c== 'S'){
		if (animal_y + 20 <= 710){
			character_move(animal_x, animal_y);
			animal_y += 20;
			display_character(animal_x,animal_y, animal);
		}
	}
	if (animal_y <= 10 && animal_x + 60 >= 995 ){
		if (stage == 1){
			stage = 2;
			animal_x= 100;
			animal_y = 590;
			display_map2();
			display_character(animal_x,animal_y, animal);
		}
		else uart_puts("End Game Stage!!");
	}
}



void main(){
	uart_init();
	framebf_init();
	display_background(); 
	display_character(280,110, 0);
	display_character(490,110, 1);
	display_character(700,110, 2);
	select(animal);
	
	
	register unsigned long f1, t1, r1, f2, t2, r2;
	// get the current counter frequency
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(f1));
	// read the current counter
	asm volatile ("mrs %0, cntpct_el0" : "=r"(t1));
	// get the current counter frequency
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(f2));
	// read the current counter
	asm volatile ("mrs %0, cntpct_el0" : "=r"(t2));
	// calculate expire value for counter
	t1+=((f1/1000)*90000)/1000;
	t2+=((f2/1000)*50000)/1000;

	while(1) {
		asm volatile ("mrs %0, cntpct_el0" : "=r"(r1));
		asm volatile ("mrs %0, cntpct_el0" : "=r"(r2));

		char c = uart_getc();
		
		// Control
		if (r1>= t1 && c!=0){
			if (stage == 0){
				welcome_screen_handler(c);
			}
			else if (stage == 1 || stage == 2){
				main_game_handler(c);
			}
			// get the current counter frequency
			asm volatile ("mrs %0, cntfrq_el0" : "=r"(f1));
			// read the current counter
			asm volatile ("mrs %0, cntpct_el0" : "=r"(t1));
			// calculate expire value for counter
			t1+=((f1/1000)*90000)/1000;
		}
		
		if (stage == 1){
			// car run
			if (r2 >= t2){
				for (int i = 0; i < 8; i++){
					move(car_x[i], car_y[i]);
					if (i == 1 || i == 2 || i == 7){
						car_x[i] -= 20;
						if (car_x[i] <= 0) car_x[i] = 933;
						drawcar(car_x[i], car_y[i], 0);
					}
					else {
						car_x[i] += 20;
						if (car_x[i] >= 1023) car_x[i] = 0;
						drawcar(car_x[i], car_y[i], 1);
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
		
		
		// Out
		if (c == 27) uart_puts("OUT!");
	}
}

