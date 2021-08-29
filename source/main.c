#include "../header/main.h"
#include "../header/game.h"
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
	}
}
