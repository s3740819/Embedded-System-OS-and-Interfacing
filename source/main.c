#include "../header/main.h"
#include "../header/display.h"
#include "../header/picture.h"
#include "../header/framebf.h"
#include "../header/uart.h"

void main() {
	uart_init();
	framebf_init();

	while(1) {
		char c = 0;
		while(c != '\n') c = uart_getc();
		StateMachine();
		drawRectARGB32(0,0,1024,768,0x00000000,1); //Clear screen
	}
}

void StateMachine() {
	enum gameState {welcome, round1, ending};
	enum gameState state = welcome;
	int animal = 0;
	char c = 0;
	displayPicture(1024, 768, welcomescr);
	display_character(280,110, 0);
	display_character(490,110, 1);
	display_character(700,110, 2);
	select(0);
	while(c != 27) {
		switch(state) {
		case welcome:
			c = uart_getc();
			if (c == 'a' || c == 'A') {
				if (animal > 0) {
					displayPicture(1024, 768, welcomescr);
					display_character(280,110, 0);
					display_character(490,110, 1);
					display_character(700,110, 2);
					animal--;
					select(animal);
				}
			}
			else if (c == 'd' || c== 'D') {
				if (animal < 2) {
					displayPicture(1024, 768, welcomescr);
					display_character(280,110, 0);
					display_character(490,110, 1);
					display_character(700,110, 2);
					animal ++;
					select(animal);
				}
			}
			else if (c == '\n') {
				uart_puts("Play!");
				displayPicture(1024, 768, endscr);
				state = ending;
			}
			break;
		case ending:
			c = uart_getc();
			if (c == '\n') {
				uart_puts("Play!");
				displayPicture(1024, 768, welcomescr);
				display_character(280,110, 0);
				display_character(490,110, 1);
				display_character(700,110, 2);
				select(0);
				state = welcome;
			}
			break;
		}
	}
	uart_puts("OUT!");

}
