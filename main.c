#include "uart.h"
#include "mbox.h"
#include "framebf.h"
void main()
{
	 // set up serial console
	uart_init();
	// say hello
	uart_puts("\n\nHello World!\n");
	// Initialize frame buffer
	framebf_init();
	// Draw something on the screen
	drawString("Nguyen Le - s3777242", 100, 100, 0x00FF00);
	drawString("Bao Tran - s3740819", 100, 116, 0x00FF00);
	drawString("Phuc To - s3758272", 100, 132, 0x00FF00);

	// echo everything back
	while(1) {
		//read each char
		char c = uart_getc();
		//send back twice
		uart_sendc(c);
		uart_sendc(c);
	}
}
