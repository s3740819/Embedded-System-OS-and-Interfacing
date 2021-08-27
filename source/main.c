#include "../header/main.h"

int animal_x = 100;
int animal_y = 600;
int animal = 0;
int stage = 0;

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
			move1(animal_x, animal_y);
			animal_x -= 20;
			display_character(animal_x,animal_y, animal);
		}
	}
	else if (c == 'd' || c== 'D'){
		if (animal_x + 20 < 964){
			move1(animal_x, animal_y);
			animal_x += 20;
			display_character(animal_x,animal_y, animal);
		}
	}
	if (c == 'w' || c == 'W'){
		if (animal_y - 20 >= 0) {
			move1(animal_x, animal_y);
			animal_y -= 20;
			display_character(animal_x,animal_y, animal);

		}
	}
	else if (c == 's' || c== 'S'){
		if (animal_y + 20 < 600){
			move1(animal_x, animal_y);
			animal_y += 20;
			display_character(animal_x,animal_y, animal);
		}
	}
	if (animal_y <= 1 && animal_x + 60 >= 995 ){
		if (stage == 1){
			stage = 2;
			animal_x= 100;
			animal_y = 600;
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
	while(1) {
		char c = uart_getc();
		if (stage == 0){
			welcome_screen_handler(c);
		}
		else if (stage == 1 || stage == 2){
			main_game1_handler(c);
		}
		if (c == 27) uart_puts("OUT!");
	}
}

