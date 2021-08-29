#include "../header/main.h"
#include "../header/displayPicture.h"

void main(){
	//set up serial console
	uart_init();

	// Test
	// uart_puts("\n\nHello World!\n");

	// Initialize frame buffer
	framebf_init();
	// ------------------ Run ------------------- //
	// display_background();
	
	displayPicture(500, 500, picture);
	// Draw something on the screen

	while(1) {
		//display_video();
		//display_video2();
		//read each char
		char c = uart_getc();
		//send back twice
		uart_sendc(c);
		uart_sendc(c);
	}
}

