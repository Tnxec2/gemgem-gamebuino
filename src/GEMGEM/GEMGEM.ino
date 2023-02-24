//imports the SPI library (needed to communicate with Gamebuino's screen)
#include <SPI.h>
//importe the Gamebuino library
#include <Gamebuino.h>
//creates a Gamebuino object named gb
Gamebuino gb;

const byte logo[] PROGMEM = {64,30,
B00000000,B00001000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B00011100,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B00101010,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B01001001,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B10001000,B10000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000001,B00001000,B01000000,B00000001,B00000000,B00000000,B11000000,B00000000,
B00000010,B00001000,B00100000,B00111110,B00000000,B11101100,B01100000,B00000000,
B00000100,B00010100,B00010000,B01100010,B00011100,B01110111,B01100000,B00000000,
B00001100,B00100010,B00110000,B11000010,B00110110,B01100110,B01101111,B00000000,
B00001010,B01001001,B01010001,B10000000,B01100110,B01100110,B01100000,B00000000,
B00001001,B10000000,B10010001,B10000001,B01111100,B01100110,B01100000,B00000000,
B00001001,B00000000,B10010001,B10011111,B00110000,B01100110,B01110000,B00000000,
B00001001,B00000000,B10010000,B11000010,B00110001,B01100110,B00000000,B00000000,
B00001001,B00000000,B10010000,B01100110,B00011110,B00000000,B00000000,B00000000,
B00001001,B00000000,B10010000,B00111110,B00000000,B00000000,B00000000,B00000000,
B00001001,B00000010,B10010000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00001001,B00000010,B10010000,B00000000,B00001000,B00000000,B00000110,B00000000,
B00001001,B00000110,B10010000,B00000001,B11110000,B00000111,B01100011,B00000000,
B00001001,B00000110,B10010000,B00000011,B00010000,B11100011,B10111011,B00000000,
B00001010,B10001100,B10010000,B00000110,B00010001,B10110011,B00110011,B00000000,
B00001100,B01001001,B01010000,B00001100,B00000011,B00110011,B00110011,B00000000,
B00000100,B00100010,B00110000,B00001100,B00001011,B11100011,B00110011,B00000000,
B00000010,B00010100,B00100000,B00001100,B11111001,B10000011,B00110011,B10000000,
B00000001,B00001000,B01000000,B00000110,B00010001,B10001011,B00110000,B00000000,
B00000000,B10001000,B10000000,B00000011,B00110000,B11110000,B00000000,B00000000,
B00000000,B01001001,B00000000,B00000001,B11110000,B00000000,B00000000,B00000000,
B00000000,B00101010,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B00011100,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B00001000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
};

const byte gem1[] PROGMEM = {8,7,
B10000001,
B00111100,
B00000000,
B00111100,
B00000000,
B00111100,
B10000001,
};

const byte gem2[] PROGMEM = {8,7,
B10000001,
B00011000,
B00011000,
B00011000,
B00011000,
B00011000,
B10000001,
};

const byte gem3[] PROGMEM = {8,7,
B10000001,
B00011000,
B00111100,
B01111110,
B00111100,
B00011000,
B10000001,
};

const byte gem4[] PROGMEM = {8,7,
B10000001,
B01101110,
B01111010,
B01110110,
B00111100,
B00011000,
B10000001,
};

const byte gem5[] PROGMEM = {8,7,
B10000001,
B00100100,
B00011000,
B00011000,
B00011000,
B00100100,
B10000001,
};

const byte gem6[] PROGMEM = {8,7,
B10000001,
B00111100,
B01111110,
B01111010,
B01100110,
B00111100,
B10000001,
};

const byte gem7[] PROGMEM = {8,7,
B10000001,
B00000000,
B01111110,
B01111010,
B01111110,
B00000000,
B10000001,
};


#define BOARDWIDTH 9 // how many columns in the board
#define BOARDHEIGHT 5 // how many rows in the board
#define GEMWIDTH 8
#define GEMHEIGHT 7
#define CURSORWIDTH 9
#define CURSORHEIGHT 8


// NUMGEMIMAGES is the number of gem types. You will need .png image
#define NUMGEMIMAGES 7

char fillLine[BOARDWIDTH];
char board[BOARDHEIGHT][BOARDWIDTH];

int score = 0;

typedef struct
{
  char x;
  char y;
}  coords;

coords cursor;

bool gameOver = false;
bool paused = false;

byte popupTimeLeft;
const __FlashStringHelper* popupText;

void setup() {
    // initialize the Gamebuino object
    gb.begin();
    gb.battery.show = false;
    mainMenu();
}

void loop() {
  if (gb.update()) {
    if (gb.buttons.pressed(BTN_C)) {
      mainMenu();
    }

    if (gameOver) {
      popup(F("\21 Game over! \20"));
    } else {
      if (gb.buttons.pressed(BTN_B)) {
        paused = !paused;
      }
      if (gb.buttons.pressed(BTN_RIGHT)) {
        cursor.x++;
        if (cursor.x >= BOARDWIDTH) cursor.x = BOARDWIDTH - 1;
      } else if (gb.buttons.pressed(BTN_LEFT)) {
        cursor.x--;
        if (cursor.x < 0) cursor.x = 0;
      } else if (gb.buttons.pressed(BTN_DOWN)) {
        cursor.y++;
        if (cursor.y >= BOARDHEIGHT) cursor.y = BOARDHEIGHT - 1;
      } else if (gb.buttons.pressed(BTN_UP)) {
        cursor.y--;
        if (cursor.y < 0) cursor.y = 0;
      }
    }
    
    drawField();
    drawScore();
    drawCursor();
    updatePopup();
  }
}

void mainMenu() {
    gb.titleScreen(F("Gem-Gem by TnxEc2"), logo);
    initGame();
}

void initGame() {
    gb.pickRandomSeed();
    paused = false;
    gameOver = false;
    cursor.x = BOARDWIDTH / 2;
    cursor.y = BOARDHEIGHT / 2;
    fillGems();
}

void fillGems() {
    for (int i = 0; i < BOARDHEIGHT; i++)
    {
        for (int j = 0; j < BOARDWIDTH; j++)
        {
            board[i][j] = random(1, NUMGEMIMAGES + 1);
        } 
    }
}

void drawField() {

    gb.display.fillRect(1, 0, LCDWIDTH-2, 7);

    gb.display.drawFastHLine(1, 7, LCDWIDTH-2);
    gb.display.drawFastHLine(1, 15, LCDWIDTH-2);
    gb.display.drawFastHLine(1, 23, LCDWIDTH-2);
    gb.display.drawFastHLine(1, 31, LCDWIDTH-2);
    gb.display.drawFastHLine(1, 39, LCDWIDTH-2);
    gb.display.drawFastHLine(1, 47, LCDWIDTH-2);

    gb.display.drawFastVLine(1, 7, 40);
    gb.display.drawFastVLine(10, 7, 40);
    gb.display.drawFastVLine(19, 7, 40);
    gb.display.drawFastVLine(28, 7, 40);
    gb.display.drawFastVLine(37, 7, 40);
    gb.display.drawFastVLine(46, 7, 40);
    gb.display.drawFastVLine(55, 7, 40);
    gb.display.drawFastVLine(64, 7, 40);
    gb.display.drawFastVLine(73, 7, 40);
    gb.display.drawFastVLine(82, 7, 40);
    
    for (int row = 0; row < BOARDHEIGHT; row++)
    {
        int y = 8 + row * (GEMHEIGHT + 1);
        for (int col = 0; col < BOARDWIDTH; col++)
        {
            int x = 2 + col * (GEMWIDTH + 1);
            
            switch (board[row][col])
            {
            case 1:
                gb.display.drawBitmap(x, y, gem1);
                break;
            case 2:
                gb.display.drawBitmap(x, y, gem2);
                break;
            case 3:
                gb.display.drawBitmap(x, y, gem3);
                break;
            case 4:
                gb.display.drawBitmap(x, y, gem4);
                break;
            case 5:
                gb.display.drawBitmap(x, y, gem5);
                break;
            case 6:
                gb.display.drawBitmap(x, y, gem6);
                break;
            case 7:
                gb.display.drawBitmap(x, y, gem7);
                break;
            default:
                break;
            }
        }
    }
}

void drawScore() {
  gb.display.setColor(WHITE);
  gb.display.fontSize = 1;
  gb.display.cursorX = 2;
  gb.display.cursorY = 1;
  gb.display.print(score);
}

void drawCursor() {
    int x = 1 + cursor.x * (CURSORWIDTH);
    int y = 7 + cursor.y * (CURSORHEIGHT);
    gb.display.setColor(WHITE);
    gb.display.drawFastHLine(x+1, y, CURSORWIDTH-1); // top
    gb.display.drawFastHLine(x+1, y+CURSORHEIGHT, CURSORWIDTH-1); // bottom
    gb.display.drawFastVLine(x, y+1, CURSORHEIGHT-1); // left
    gb.display.drawFastVLine(x+CURSORWIDTH, y+1, CURSORHEIGHT-1); // right

    gb.display.setColor(BLACK);
    // on top
    gb.display.drawPixel(x+3, y);
    gb.display.drawPixel(x+5, y);
    // on bottom
    gb.display.drawPixel(x+3, y+CURSORHEIGHT);
    gb.display.drawPixel(x+5, y+CURSORHEIGHT);
    // on left
    gb.display.drawPixel(x, y+3);
    gb.display.drawPixel(x, y+5);
    // on right
    gb.display.drawPixel(x+CURSORWIDTH, y+3);
    gb.display.drawPixel(x+CURSORWIDTH, y+5);
}

void printCentered(const __FlashStringHelper* text) {
  gb.display.cursorX = (LCDWIDTH / 2) - (strlen_PF((unsigned long)text) * gb.display.fontSize * gb.display.fontWidth / 2);
  gb.display.print(text);
}

void printCentered(char* text) {
  gb.display.cursorX = (LCDWIDTH / 2) - (strlen(text) * gb.display.fontSize * gb.display.fontWidth / 2);
  gb.display.print(text);
}

void popup(const __FlashStringHelper* text) {
  popup(text, 20);
}
void popup(const __FlashStringHelper* text, uint8_t duration) {
  popupText = text;
  popupTimeLeft = duration + 12;
}

void updatePopup() {
  if (popupTimeLeft) {
    uint8_t yOffset = 0;
    if (popupTimeLeft < 12) {
      yOffset = popupTimeLeft - 12;
    }
    byte width = strlen_PF((unsigned long)popupText) * gb.display.fontSize * gb.display.fontWidth;
    gb.display.fontSize = 1;
    gb.display.setColor(BLACK);
    gb.display.drawRect(LCDWIDTH / 2 - width / 2 - 2, yOffset - 1, width + 2, gb.display.fontHeight + 2);
    gb.display.setColor(WHITE);
    gb.display.fillRect(LCDWIDTH / 2 - width / 2 - 1, yOffset - 1, width + 1, gb.display.fontHeight + 1);
    gb.display.setColor(BLACK);
    gb.display.cursorY = yOffset;
    printCentered(popupText);
    popupTimeLeft--;
  }
}