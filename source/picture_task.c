#include "../header/picture_tasks/picture_task.h"
#include "../header/picture_tasks/picture/pictures.h"
#include "../header/piStuffs/uart.h"
#include "../header/piStuffs/framebf.h"

/**
 * Display a picture
 */
void display_pic(){
	// Reset the screen first
	resetScreen();

	// Draw the picture to pixel one by one
	for (int x = 261, y= 133, i = 0; y < 633; i++){
		drawPixelARGB32(x, y, picture[i], 0);
		x++;
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
void display_scrollable_pic(int x, int y){
	int start_x = x;
	int start_y = y;
	
	// Draw the picture 
	for (int i = (2351*start_y)+ start_x; y < start_y + 768; i++){
		drawPixelARGB32(x-start_x, y-start_y, scrollable_picture[i], 0);
		x++;
		if (x == start_x + 1024){
			x = start_x;
			y++;
			i = (2351*y)+ x;
		}
	}
	
	// Display scroll icon on the pic
	display_scroll_icon();
}

/**
 * Display the scroll icon
 */
void display_scroll_icon(){
	for (int x = 461, y = 567, i = 0; y < 667; i++){
		if (scroll_icon[i] < 0x202020) drawPixelARGB32(x, y, scroll_icon[i], 0);
		x++;
		if (x == 561){
			x = 461;
			y++;
		}
	}
}

/**
 * scroll pic handler
 */
void execute_scrollable_pic(){
	char c = 0; 
	int x = 0, y = 0;
	display_scrollable_pic(x, y);
	
	// If there is not esc
	while(c!=27){
		c = uart_getc();
		if (c == 'w' || c == 'W'){
			if (y - 50 >= 0){
				y -= 50;
				display_scrollable_pic(x,y);
			}
		}
		else if (c == 's' || c == 'S'){
			if (y + 50 + 768 < 1500){
				y += 50;
				display_scrollable_pic(x,y);
			}
		}
		else if (c == 'd' || c == 'D'){
			if (x + 50 + 1024 < 2351){
				x += 50;
				display_scrollable_pic(x,y);
			}
		}
		else if (c == 'a' || c == 'A'){
			if (x - 50 >= 0){
				x -= 50;
				display_scrollable_pic(x,y);
			}
		}
	}
	
	// After terminating, clear the qemu screen
	resetScreen();
}
