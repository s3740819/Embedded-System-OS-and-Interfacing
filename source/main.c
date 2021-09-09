#include "../header/main.h"

/**
 * Initialize program
 */
void initialize(){
	// Initialize pi's stuffs
	uart_init();
	framebf_init();	
	
	// Print Welcome message
	uart_puts("\n\n"  YEL);
	uart_puts( "8888888888 8888888888 8888888888 88888888888 .d8888b.      d8888   .d8888b.   .d8888b.  \n");
	uart_puts( "888        888        888            888    d88P  Y88b    d8P888  d88P  Y88b d88P  Y88b \n");
	uart_puts( "888        888        888            888           888   d8P 888  888    888 888    888 \n");
	uart_puts( "8888888    8888888    8888888        888         .d88P  d8P  888  Y88b. d888 888    888 \n");
	uart_puts( "888        888        888            888     .od888P   d88   888    Y888P888 888    888 \n");
	uart_puts( "888        888        888            888    d88P      8888888888        '888 888    888 \n");
	uart_puts( "888        888        888            888    888              888  Y88b  d88P Y88b  d88P \n");
	uart_puts( "8888888888 8888888888 8888888888     888    888888888        888   'Y8888P'   'Y8888P'  \n");
	uart_puts( "                                                                                        \n");
	uart_puts( "                                                                                        \n");
	uart_puts( "888888b.         d8888 8888888b.  8888888888     .d88888b.   .d8888b.  \n");
	uart_puts( "888  '88b       d88888 888   Y88b 888           d88P' 'Y88b d88P  Y88b \n");
	uart_puts( "888  .88P      d88P888 888    888 888           888     888 Y88b.      \n");
	uart_puts( "8888888K.     d88P 888 888   d88P 8888888       888     888  'Y888b.   \n");
	uart_puts( "888  'Y88b   d88P  888 8888888P'  888           888     888     'Y88b. \n");
	uart_puts( "888    888  d88P   888 888 T88b   888           888     888       '888 \n");
	uart_puts( "888   d88P d8888888888 888  T88b  888           Y88b. .d88P Y88b  d88P \n");
	uart_puts( "8888888P' d88P     888 888   T88b 8888888888     'Y88888P'   'Y8888P'  \n");
	uart_puts( "                                                                        \n");
	uart_puts( "                                                                        \n" END);
	uart_puts(CYN  "           Developed By: Bao Tran - S3740819                            \n");
	uart_puts(	   "                         Phuc To - s3758272                            \n");
	uart_puts(     "                         Nguyen Le - s3777242                            \n" END);
	uart_puts("\n \n");
	
	// Display help to introduce our commands
	displayHelp();
	
	// Command header
	uart_puts(BLK GRNB "BARE_OS>" END);
	
	// Draw hello background to qemu
	displayHelloBg();
}

/**
 * Main function
 */
void main(){
	initialize();
	// ---------------- Input Variables ------------//
	char input[] = {0,0,0,0,0,0,0,0,0,0};
	int index = 0;
	
	while(1) {
		char c =  uart_get_CLI_input();
		// If get backspace
		if (c == 8){
			// To avoid delete the header "BARE_OS>"
			if (index > 0){
				uart_sendc(c);
				uart_sendc(' ');
				uart_sendc(c);
				index -= 1;
				input[index] = 0;
			}
		}
		// If there is a character and no more 10 words (avoid jumping to new line automatically)
		else if ((c >= 32 && c < 127) || c == '\n') {
			// Send what the user type to the screen and store the input
			uart_sendc(c);
			if (c!= '\n' && index < 10){
				input[index] = c;
			}
			index +=1;

		}
		
		// If there is an enter, it will execute the input command
		if(c == '\n'){
			// Handle input command
			taskHandler(input);

			// Reset the input field
			uart_puts(BLK GRNB "BARE_OS>" END);
			index = 0;
			for(int i = 0; i < 10; i++) input[i] = 0;
		}
	}
}


/**
 *  Task handler after getting the input command line 
 */
void taskHandler(char input[]){
	if (strCmp(input, "game")){
		uart_puts(BMAG "Executed: \"Road Crossing\" Game!\n\n" END);
		uart_puts(YEL "** Press Esc to terminate the task!" END);
		
		// Task event handler
		executeGame();
		
		// Reset
		displayHelloBg();
		uart_puts("\033[2K \r");

	}
	else if (strCmp(input, "video")){
		uart_puts(BMAG "Executed: Display a video!\n\n" END);
		uart_puts(YEL "** Press Esc to terminate the task!" END);
		
		// Task event handler
		executeVideoTask();
		
		// Reset
		displayHelloBg();
		uart_puts("\033[2K \r");
	}
	else if (strCmp(input, "picture")){
		uart_puts(BMAG "Executed: Display a picture!\n\n" END);
		uart_puts(YEL "** Press Esc to terminate the task!" END);
		
		// Task event handler
		displayPic();
		
		// Reset
		displayHelloBg();
		uart_puts("\033[2K \r");
	}
	else if (strCmp(input, "s-picture")){
		uart_puts(BMAG "Executed: Display a scrollable picture!\n\n" END);
		uart_puts(YEL "** Press Esc to terminate the task!" END);
		
		// Task event handler
		executeScrollablePic();
		
		// Reset
		displayHelloBg();
		uart_puts("\033[2K \r");
	}
	else if(strCmp(input, "font")) {
		uart_puts(BMAG "Executed: Display font!\n\n" END);
		uart_puts(YEL "** Press Esc to terminate the task!" END);
		
		// Task event handler
		executeFontTask();
		
		// Reset
		displayHelloBg();
		uart_puts("\033[2K \r");
	}
	else if(strCmp(input, "help")){
		uart_puts(BMAG "Executed: Display help!\n\n" END);
		displayHelp();
	}
	else if(strCmp(input, "cls")){
		uart_puts("\033c");
	}
	else uart_puts("Invalid Command! Enter 'Help' for more information!\n\n");
}

/**
 * Compare 2 strings 
 */
int strCmp(char* c1, char* c2){
	if (sizeof(c1) == sizeof(c2)){
		for (int i = 0; i < sizeof(c1); i++){
			if (c1[i] != c2[i]){
				return 0;
			}
		}
		return 1;
	}
	return 0;
}

/**
 *  Display help : all available commands
 */
void displayHelp(){
	uart_puts("Command				Description\n");
	uart_puts("----------------------------------------------------\n");
	uart_puts("game				Execute the game\n");
	uart_puts("picture				Display a picture\n");
	uart_puts("s-picture			Display a scrollable picture\n");
	uart_puts("font				Display team members' names with the OS font\n");
	uart_puts("video				Display a video\n");
	uart_puts("cls				Clear the CLI\n");
	uart_puts("help				Display commands and descriptions\n\n");
	uart_puts("***NOTE: "
			"\n  Press ESC to exit the current task and return to the main menu!\n\n");

}

/**
 *  Print hello back ground in qemu
 */
void displayHelloBg(){
	for (int i = 0, x = 0, y = 0; y < 768; i++){
		drawPixelARGB32(x, y, helloBackground[i], 0);
		x++;
		
		// If it reaches the width of the pic -> jump to next row of pixel
		if (x == 1024){
			x= 0;
			y++;
		}
	}
}
