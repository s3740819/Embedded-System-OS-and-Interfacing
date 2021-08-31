#include "../header/main.h"
#include "../header/displayPicture.h"
#include "../header/wallStreet.h"

void main(){
	//set up serial console
	uart_init();

	// Initialize frame buffer
	framebf_init();

	// Display 2400 x 1350 wall street picture
	displayPicture(2400, 1350, wallStreet);
	uart_puts("Hello World!\n");

	int current_y_pos = 0;
	while(1) {
		char c = uart_getc();
		if (c == 's') {
			//Scroll down
			uart_sendc(c);

			// Moving 32px each time user press
			current_y_pos -= 32;

			// Maximum depth reach


			// Display the current setup
			displayPictureScrollUp(2400, 1350, current_y_pos);
		}

		else if (c == 'w') {
			//Scroll up
			uart_sendc(c);

			// Moving 32px each time user press
			current_y_pos += 32;

			// Maximum height reach
			if (current_y_pos > 0){
				current_y_pos = 0;
			}

			displayPictureScrollDown(2400, 1350, current_y_pos);
		}

	}
}

