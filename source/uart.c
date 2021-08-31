#include "../header/uart.h"
#include "../header/gpio.h"

/**
 * Set baud rate and characteristics (115200 8N1) and map to GPIO
 */
void uart_init()
{
    register unsigned int r;

    /* initialize UART */
    *AUX_ENABLE |= 1;       //enable UART1, AUX mini uart (AUXENB)
    *AUX_MU_CNTL = 0;		//stop transmitter and receiver
    *AUX_MU_LCR  = 3;       //8-bit mode
    *AUX_MU_MCR  = 0;		//RTS (request to send)
    *AUX_MU_IER  = 0;		//disable interrupts
    *AUX_MU_IIR  = 0xc6;    //clear FIFOs
    *AUX_MU_BAUD = 270;    	// 115200 baud

    /* map UART1 to GPIO pins */
    r = *GPFSEL3;
    r &= ~( (7 << 9)); //Clear bits 9 (gpio33)
    r |=    (2 << 12);   //Set value 2 (select ALT5: UART1)
    *GPFSEL3 = r;
    r = *GPFSEL4;
    r &= ~( (7 << 0)); //Clear bits 0 (gpio40)
    r |=    (2 << 0);   //Set value 2 (select ALT5: UART1)
    *GPFSEL4 = r;
    /* enable GPIO 14, 15 */
    *GPPUD = 0;            //No pull up/down control
    r = 150; while(r--) { asm volatile("nop"); } //waiting 150 cycles
    *GPPUDCLK1 = (1 << 1)|(1 << 8); //enable clock for GPIO 33, 40
    r = 150; while(r--) { asm volatile("nop"); } //waiting 150 cycles
    *GPPUDCLK1 = 0;        // flush GPIO setup

    *AUX_MU_CNTL = 3;      //Enable transmitter and receiver (Tx, Rx)
}

/**
 * Send a character
 */
void uart_sendc(unsigned char c) {
    /* wait until transmitter is empty */
    do {
    	asm volatile("nop");
    } while ( !(*AUX_MU_LSR & 0x20) );

    /* write the character to the buffer */
    *AUX_MU_IO = c;
}

/**
 * Receive a character
 */
char uart_getc() {
    char c;

    /* wait until data is ready (one symbol) */
    if(*AUX_MU_LSR & 0x01) {

		/* read it and return */
		c = (char)(*AUX_MU_IO);

		/* convert carriage return to newline */
		return (c == '\r' ? '\n' : c);
    }
    return 0;
}

/**
 * Display a string
 */
void uart_puts(char *s) {
    while (*s) {
        /* convert newline to carriage return + newline */
        if (*s == '\n')
            uart_sendc('\r');
        uart_sendc(*s++);
    }
}

void uart_hex(unsigned int d) {
	unsigned int n;
	int c;
	uart_puts("0x");
	for(c = 28; c >= 0; c = c -4) {// Get highest 4-bit nibble
		n = (d >> c) & 0xF;// 0-9 => '0'-'9', 10-15 => 'A'-'F'
		n += (n > 9) ? (-10 + 'A') : '0';
		uart_sendc(n);
	}
}

void uart_dec(int num){
	char str[33] = "";
	int i, rem, len = 0, n;
	n = num;
	while(n != 0){
		len++;
		n /= 10;
	}
	if(num == 0) len = 1;
	for(i = 0; i < len; i++){
		rem = num % 10;
		num = num / 10;
		str[len -(i + 1)] = rem + '0';
	}
	str[len] = '\0';
	uart_puts(str);
}
