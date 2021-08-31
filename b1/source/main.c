#include "../header/main.h"
#include "../header/displayPicture.h"

void main(){
	//set up serial console
	uart_init();

	// Test
	// uart_puts("\n\nHello World!\n");

	// Initialize frame buffer
	framebf_init();
	
	// Display picture
	displayPicture(500, 500, picture);
}

