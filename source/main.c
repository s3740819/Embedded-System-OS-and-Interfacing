#include "../header/main.h"
#include "../header/game.h"
#include "../header/display.h"
#include "../header/uart.h"
#include "../header/framebf.h"
#include "../header/video.h"

void main(){
	uart_init();
	framebf_init();

	while(1) {
		char c =  uart_getc();
		if (c == 'g'){
			StateMachine();
		}
		else if (c == 'v'){
			execute_video();
		}
		else if(c == 'f') {
			drawString("Nguyen Le - s3777242", 100, 100, 0x00FF0000);
			drawString("Bao Tran - s3740819", 100, 116, 0x0000FF00);
			drawString("Phuc To - s3758272", 100, 132, 0x00FFFFFF);
		} else if (c == 'p') {
			display_pic();
		}
		else if (c == 's') {
			execute_scrollable_pic();
		}
	}
}
