#include "../header/video/picture/frame1.h"
#include "../header/video/picture/frame2.h"
#include "../header/video/picture/background.h"
#include "../header/video/video.h"
#include "../header/piStuffs/framebf.h"
#include "../header/piStuffs/uart.h"

/**
 * Wait n micro-second
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

/**
 * Display the video frame by frame
 */
int display_video(){
	// For each frame, draw it
	for (unsigned int i = 0, x = 177, y =40, frame = 0 ; frame < 80; i++){
		if (frame == 40) i = 0;
		if (frame >= 40) drawPixelARGB32(x, y, frame2[i], 0);
		else drawPixelARGB32(x, y, frame1[i], 0);
		x++;
		
		// If it reaches the width of the pic -> jump to next row of pixel
		if (x == 845){
			y++;
			x = 177;
		}
		
		// Ready to draw new frame when the current frame is completely drawn 
		if (y == 440){
			y = 40;
			frame ++;
			
			// If there is an esc, terminate the task 
			if (uart_getc() == 27) return 0;
			
			// Wait after drawing a frame
			sleep(50000);
		}
		
	}
	return 1;
}

/**
 * Draw an background
 */
void display_background(){
	for (unsigned int i = 0, x = 0, y =0 ; i < 786432; i++){
		drawPixelARGB32(x, y, background[i], 0);
		x++;
		
		// If it reaches the width of the pic -> jump to next row of pixel
		if (x == 1024){
			y++;
			x = 0;
		}
	}
}


/**
 * Video execution
 */
void execute_video_task(){
	display_background();
	while(display_video());
	
	// Reset screen after terminating
	resetScreen();
}
