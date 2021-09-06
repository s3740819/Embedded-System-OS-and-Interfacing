// Draw functions
void displayBackground(int width, int height, const unsigned int picture[]);
void drawCharacterAva(int x, int y, int character);
void select(int character);
void drawCharacter(int x, int y, int character, int isMoveUp);
void eraseCharacter(int x, int y, int round);
void drawGate(int round);
void drawMap(int round, int isLose);
void drawLog(int x, int y, int isLose);
void drawCar(int x, int y, int isLeft, int isLose);
void displaySkull();
void displayWelcomeScreen(int character);
int displayInstruction(int round);
void drawTrain(int x, int isLose);

// Erase functions
void eraseCar(int x, int y, int round, int isLose);
void eraseLog(int x, int y, int isLose);

// Check function
int isTree(int x, int y, int round);
