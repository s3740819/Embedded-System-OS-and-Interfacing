#include "../header/main.h"

void main(){
	uart_init();
	framebf_init();
	// ------------------ Run ------------------- //
	while(1) {
		display_video();
		display_video2();

	}
}

