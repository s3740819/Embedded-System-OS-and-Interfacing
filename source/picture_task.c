#include "../header/picture_tasks/picture_task.h"
#include "../header/picture_tasks/picture/other_pictures.h"
#include "../header/uart.h"
#include "../header/framebf.h"


void display_pic(){
	resetScreen();

	for (int x = 261, y= 133, i = 0; y < 633; i++){
		drawPixelARGB32(x, y, picture[i], 0);
		x++;
		if (x == 761){
			x = 261;
			y++;
		}
	}
	while(uart_getc() != 27);
	resetScreen();

}

void display_scrollable_pic(int x, int y){
	int start_x = x;
	int start_y = y;
	for (int i = (2351*start_y)+ start_x; y < start_y + 768; i++){
		drawPixelARGB32(x-start_x, y-start_y, scrollable_picture[i], 0);
		x++;
		if (x == start_x + 1024){
			x = start_x;
			y++;
			i = (2351*y)+ x;
		}
	}
	display_scroll_icon();
}

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

void execute_scrollable_pic(){
	char c = 0; 
	int x = 0, y = 0;
	display_scrollable_pic(x, y);
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
	resetScreen();
}
