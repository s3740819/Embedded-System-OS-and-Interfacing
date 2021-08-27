#include "../header/main.h"

void main(){
	uart_init();
	framebf_init();
	display_background(); 
	display_character(280,110, 0);
	display_character(490,110, 1);
	display_character(700,110, 2);
	select(0);
	int animal = 0;
	while(1) {
		char c = uart_getc();
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
		else if (c == '\n') uart_puts("Play!");
		else if (c == 27) uart_puts("OUT!");
	}
}

