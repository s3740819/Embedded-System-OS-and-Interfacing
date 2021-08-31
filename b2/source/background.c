#include "../header/wallStreet.h"


void display_background(const unsigned int picture[]){
	for (unsigned int i = 0, x = 0, y =0 ; i < 786432; i++){
		drawPixelARGB32(x, y, picture[i]);
		x++;
		if (x == 1024){
			y++;
			x = 0;
		}
	}
}

void displayPicture(int width, int height, const unsigned int picture[]) {
	for (int y = 0, x = 0, i = 0; y < height; i++) {
		drawPixelARGB32(x, y, picture[i]);
		x++;
		if (x == width){
			y++;
			x = 0;
		}
	}
}

void displayPictureScrollUp(int width, int height, int current_y_pos){
	//Moving 32px each time the user press "w"
	int start_y = current_y_pos;
	for (int y = start_y , x = 0, i = 0; y < height; i++) {
		drawPixelARGB32(x, y, wallStreet[i]);
		x++;
		if (x == width){
			y++;
			x = 0;
		}
	}
}


void displayPictureScrollDown(int width, int height, int current_y_pos){
	//Moving 32px each time the user press "s"
	int start_y = current_y_pos;
	for (int y = start_y , x = 0, i = 0; y < height; i++) {
		drawPixelARGB32(x, y, wallStreet[i]);
		x++;
		if (x == width){
			y++;
			x = 0;
		}
	}
}

