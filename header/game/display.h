void displayPicture(int width, int height, const unsigned int picture[]);
void display_avatars(int x, int y, int animal);
void select(int animal);
void drawAvatar(int x, int y, int animal, int is_goup);
void avatarMove(int x, int y, int round);
void draw_gate(int round);
void display_map(int round, int is_lose);
void drawLog(int x, int y, int is_lose);
void drawCar(int x, int y, int is_left, int is_lose);
void carMove(int x, int y, int round, int is_lose);
void logMove(int x, int y, int is_lose);
void display_skull();
void drawChar(unsigned char c, int x1, int y1, unsigned int attr);
void drawString(char *s, int x1, int y1, unsigned int attr);
int isTree(int x, int y, int round);
int display_instruction(int round);
void run_train(int x, int is_lose);