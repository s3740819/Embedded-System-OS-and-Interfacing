#include "../header/video/picture/frame1.h"
#include "../header/video/picture/frame2.h"
#include "../header/video/picture/background.h"
#include "../header/video/video.h"
#include "../header/framebf.h"
#include "../header/uart.h"

/**
 * Wait N microsec (ARM CPU only)
 */
void sleep(unsigned int n)
{
    register unsigned long f, t, r;
    // get the current counter frequency
    asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));
    // read the current counter
    asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
    // calculate expire value for counter
    t+=((f/1000)*n)/1000;
    do{asm volatile ("mrs %0, cntpct_el0" : "=r"(r));}while(r<t);
}

int display_video(){
	for (unsigned int i = 0, x = 177, y =40, frame = 0 ; frame < 80; i++){
		if (frame == 40) i = 0;
		if (frame >= 40) drawPixelARGB32(x, y, frame2[i], 0);
		else drawPixelARGB32(x, y, frame1[i], 0);
		x++;
		if (x == 845){
			y++;
			x = 177;
		}
		if (y == 440){
			y = 40;
			frame ++;
			if (uart_getc() == 27) return 0;
			sleep(50000);
		}
		
	}
	return 1;
}

void display_background(){
	for (unsigned int i = 0, x = 0, y =0 ; i < 786432; i++){
		drawPixelARGB32(x, y, background[i], 0);
		x++;
		if (x == 1024){
			y++;
			x = 0;
		}
	}
}

void execute_video_task(){
	display_background();
	while(display_video());
	
	resetScreen();
}
