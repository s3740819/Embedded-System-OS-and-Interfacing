#include "../header/picture_tasks/picture_task.h"
#include "../header/picture_tasks/picture/pictures.h"
#include "../header/piStuffs/uart.h"
#include "../header/piStuffs/framebf.h"

/**
 * Display a picture
 */
void displayPic(){
	// Reset the screen first
	resetScreen();

	// Draw the picture to pixel one by one
	for (int x = 261, y= 133, i = 0; y < 633; i++){
		drawPixelARGB32(x, y, picture[i], 0);
		x++;
		
		// If it reaches the width of the pic -> jump to next row of pixel
		if (x == 761){
			x = 261;
			y++;
		}
	}
	
	// Until there is an esc
	while(uart_getc() != 27);
	
	// Terminate the task and reset the screen
	resetScreen();

}


/**
 *  Display the scrollable picture
 */
void diplayScrollPic(int x, int y){
	int start_x = x;
	int start_y = y;
	
	// Draw the picture 
	for (int i = (2351*start_y)+ start_x; y < start_y + 768; i++){
		drawPixelARGB32(x-start_x, y-start_y, scrollablePicture[i], 0);
		x++;
		
		// If it reaches the width of the pic -> jump to next row of pixel
		if (x == start_x + 1024){
			x = start_x;
			y++;
			i = (2351*y)+ x;
		}
	}
	
	// Display scroll icon on the pic
	displayScrollIcon();
}

/**
 * Display the scroll icon
 */
void displayScrollIcon(){
	for (int x = 461, y = 567, i = 0; y < 667; i++){
		if (scrollIcon[i] < 0x202020) drawPixelARGB32(x, y, scrollIcon[i], 0);
		x++;
		
		// If it reaches the width of the pic -> jump to next row of pixel
		if (x == 561){
			x = 461;
			y++;
		}
	}
}

/**
 * scroll pic handler
 */
void executeScrollablePic(){
	char c = 0; 
	int x = 0, y = 0;
	diplayScrollPic(x, y);
	
	// If there is not esc
	while(c!=27){
		c = uart_getc();
		if (c == 'w' || c == 'W'){
			if (y - 50 >= 0){
				y -= 50;
				diplayScrollPic(x,y);
			}
		}
		else if (c == 's' || c == 'S'){
			if (y + 50 + 768 < 1500){
				y += 50;
				diplayScrollPic(x,y);
			}
		}
		else if (c == 'd' || c == 'D'){
			if (x + 50 + 1024 < 2351){
				x += 50;
				diplayScrollPic(x,y);
			}
		}
		else if (c == 'a' || c == 'A'){
			if (x - 50 >= 0){
				x -= 50;
				diplayScrollPic(x,y);
			}
		}
	}
	
	// After terminating, clear the qemu screen
	resetScreen();
}
