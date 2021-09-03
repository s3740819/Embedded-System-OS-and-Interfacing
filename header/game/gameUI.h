// Draw functions
void displayBackground(int width, int height, const unsigned int picture[]);
void drawCharacterAva(int x, int y, int animal);
void select(int animal);
void drawCharacter(int x, int y, int animal, int is_goup);
void eraseCharacter(int x, int y, int round);
void drawGate(int round);
void drawMap(int round, int is_lose);
void drawLog(int x, int y, int is_lose);
void drawCar(int x, int y, int is_left, int is_lose);
void displaySkull();
void displayWelcomeScreen(int character);
int displayInstruction(int round);
void drawTrain(int x, int is_lose);

// Erase functions
void eraseCar(int x, int y, int round, int is_lose);
void eraseLog(int x, int y, int is_lose);

// Check function
int isTree(int x, int y, int round);
