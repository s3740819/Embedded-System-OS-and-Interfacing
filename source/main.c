#include "../header/main.h"

void task_handler(char input[]){
	if (str_cmp(input, "game")){
		uart_puts(BMAG "Executed: \"Road Crossing\" Game!\n\n" END);
		StateMachine();
		display_hello_bg();
	}
	else if (str_cmp(input, "video")){
		uart_puts(BMAG "Executed: Display a video!\n\n" END);
		execute_video();
		display_hello_bg();
	}
	else if (str_cmp(input, "picture")){
		uart_puts(BMAG "Executed: Display a picture!\n\n" END);
		display_pic();
		display_hello_bg();
	}
	else if (str_cmp(input, "s-picture")){
		uart_puts(BMAG "Executed: Display a scrollable picture!\n\n" END);
		execute_scrollable_pic();
		display_hello_bg();
	}
	else if(str_cmp(input, "font")) {
		uart_puts(BMAG "Executed: Display font!\n\n" END);
		execute_drawing_fonts();
		display_hello_bg();
	}
	else if(str_cmp(input, "help")){
		uart_puts(BMAG "Executed: Display help!\n\n" END);
		display_help();
	}
	else uart_puts("Invalid Command! Enter 'Help' for more information!\n\n");
}

// Compare 2 strings
int str_cmp(char* c1, char* c2){
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

void display_help(){
	uart_puts("Command				Description\n");
	uart_puts("----------------------------------------------------\n");
	uart_puts("game				Execute the game\n");
	uart_puts("picture				Display a picture\n");
	uart_puts("s-picture			Display a scrollable picture\n");
	uart_puts("font				Display team members' names with the OS font\n");
	uart_puts("video				Display a video\n");
	uart_puts("help				Display commands and descriptions\n\n");
	uart_puts("***NOTE: "
			"\n  Press ESC to exit the execution and return to the main menu!\n\n");

}

void display_hello_bg(){
	for (int i = 0, x = 0, y = 0; y < 768; x++, i++){
		drawPixelARGB32(x, y, hello_background[i], 0);
		if (x == 1024){
			x= 0;
			y++;
		}
	}
}

void initialize(){
	uart_init();
	framebf_init();
	uart_puts("\n");
	display_help();
	uart_puts(BLK GRNB "BARE_OS>" END);
	display_hello_bg();
}

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
		else if (index < 10 || c == '\n') {
			// Send what the user type to the screen and store the input
			uart_sendc(c);
			if (c!= '\n' && c != 8){
				input[index] = c;
				index +=1;
			}
		}
		
		// If there is an enter, it will execute the input command
		if(c == '\n'){
			// Handle input command
			task_handler(input);

			// Reset the input field
			uart_puts(BLK GRNB "BARE_OS>" END);
			index = 0;
			for(int i = 0; i < 10; i++) input[i] = 0;
		}
	}
}
