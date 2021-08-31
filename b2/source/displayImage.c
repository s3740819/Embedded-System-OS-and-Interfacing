void displayImage(int width, int height, const unsigned int picture[]) {
	for (int y = 0, x = 0, i = 0; y < height; i++) {
		drawPixelARGB32(x, y, picture[i]);
		x++;
		if (x == width){
			y++;
			x = 0;
		}
	}
}
