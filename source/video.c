#include "../header/picture/frame1.h"
#include "../header/picture/frame2.h"
#include "../header/picture/background.h"
#include "../header/video.h"
#include "../header/framebf.h"

/**
 * Wait N microsec (ARM CPU only)
 */
void wait_msec(unsigned int n)
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

void display_video(){
	
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
			wait_msec(50000);
		}
	}

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

void execute_video(){
	display_background();
	char c = 0;
	while(c != '\n'){
		display_video();
	}
}