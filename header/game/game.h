// Main functions
void gameInit();
void executeGame();

// User input handler functions
void controlCharacter(char c, int round);
void buttonHandler(char c, int round);

// Other functions
void logRun();
void carRun(int round);
void trainRun();
int isHit(int round);
void reset(int round);
void gameOverHandler(char c, int round);