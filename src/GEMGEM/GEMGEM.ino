//imports the SPI library (needed to communicate with Gamebuino's screen)
#include <SPI.h>
#include <EEPROM.h>
#include <Gamebuino.h>

//creates a Gamebuino object named gb
Gamebuino gb;

// Game id for save state
const char GAME_ID[] PROGMEM = "GEMGEM GAMEBUINO V1";


const byte logo[] PROGMEM = {
  64,
  30,
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

const byte gem1[] PROGMEM = {
  8,
  7,
  B10000001,
  B00111100,
  B00000000,
  B00111100,
  B00000000,
  B00111100,
  B10000001,
};

const byte gem2[] PROGMEM = {
  8,
  7,
  B10000001,
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B10000001,
};

const byte gem3[] PROGMEM = {
  8,
  7,
  B10000001,
  B00011000,
  B00111100,
  B01111110,
  B00111100,
  B00011000,
  B10000001,
};

const byte gem4[] PROGMEM = {
  8,
  7,
  B10000001,
  B01101110,
  B01111010,
  B01110110,
  B00111100,
  B00011000,
  B10000001,
};

const byte gem5[] PROGMEM = {
  8,
  7,
  B10000001,
  B00100100,
  B00011000,
  B00011000,
  B00011000,
  B00100100,
  B10000001,
};

const byte gem6[] PROGMEM = {
  8,
  7,
  B10000001,
  B00111100,
  B01111110,
  B01111010,
  B01100110,
  B00111100,
  B10000001,
};

const byte gem7[] PROGMEM = {
  8,
  7,
  B10000001,
  B00000000,
  B01111110,
  B01111010,
  B01111110,
  B00000000,
  B10000001,
};


#define BOARDWIDTH 9      // how many columns in the board
#define BOARDHEIGHT 5     // how many rows in the board
#define GEMWIDTH 8        // width of Gem-Bitmap in px
#define GEMHEIGHT 7       // height of Gem-Bitmap in px
#define CURSORWIDTH 9     // width of cursor in px
#define CURSORHEIGHT 8    // height of cursor in px
#define NUMGEMIMAGES 7    // number of gem types

typedef struct
{
  int x;
  int y;
} coords;

bool gameOver = false;
bool showGameOverDialog = false;
bool paused = false;

// Processing Speed
unsigned long prevTime = 0;   // for processing delay (ms)
const int delayTime = 200;    // Processing step speed in ms

#define STEP_ERASE 0
#define STEP_ERASED 1
#define STEP_MOVE 2
#define STEP_DONE 3

coords hint;
bool showHint = false;

char board[BOARDHEIGHT][BOARDWIDTH];
coords cursor;    
coords marked;    // coords of marked gem
coords switched;  // coords of marked gem for undo
bool processing;  // if true - this is running processing: remove and move gems
bool gameStarted;
char step;
int erasedCount;

struct Gamestate {
  char id[sizeof(GAME_ID)];
  int highscore;
  int score;
  char board[BOARDHEIGHT][BOARDWIDTH];
} gameState;

// #########################################

void setup() {
  gb.begin();  // initialize the Gamebuino object
  gb.battery.show = false;
  titleMenu(true);
}


void loop() {
  if (gb.update()) {
    if (!processing && gb.buttons.pressed(BTN_C)) {
      saveGame();
      titleMenu(false);
    }

    if (! gameOver ) {
      if (gb.buttons.pressed(BTN_B)) {
        paused = !paused;
        showHint = paused;
      }
      if (!paused) {
        if (processing) {
          process();
        } else {
          checkInput();
        }
      }
    } else {
      if (gb.buttons.pressed(BTN_A) ) {
        showGameOverDialog = !showGameOverDialog;
      } else if (gb.buttons.pressed(BTN_B) ) {
        initGame();
      }
    }

    drawField();
    drawScore();
    if (!processing && !gameOver) drawCursor();
    updatePopup();
    if (paused) {
      drawPauseDialog();
    }
    if (gameOver && showGameOverDialog) {
      drawGameOverDialog();
    }
  }
}


void checkInput() {
  if (gb.buttons.pressed(BTN_A)) {
    switchGems();
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


void titleMenu(bool firstRun) {
  gb.titleScreen(F("Gem-Gem by TnxEc2"), logo);
  if (firstRun) gameMenu();
  else initGame();
}


void gameMenu() {
  bool newGame = true;
  if (isValidGame()) {
    loadGame();
  } else {
    newSave();
  }
  while (1) {
    if (gb.update()) {
      gb.display.cursorY = 0;
      printCentered(F("GemGem"));

      gb.display.cursorX = 10;
      gb.display.cursorY = 10;
      gb.display.print(F("High Score: "));
      gb.display.print(gameState.highscore);

      gb.display.cursorX = 20;
      gb.display.cursorY = 20;
      gb.display.print(F("New Game"));
      gb.display.cursorX = 20;
      gb.display.cursorY = 30;
      gb.display.print(F("Load saved"));

      if (newGame) {
        gb.display.cursorY = 20;
      } else {
        gb.display.cursorY = 30;
      }
      gb.display.cursorX = 10;
      gb.display.print(F("\20"));

      gb.display.cursorY = 40;
      printCentered(F("[c,b] exit, [a] start"));

      if (gb.buttons.pressed(BTN_A)) {
        if (newGame) {
            initGame();
            popup(F("New game"), 40);
        } else {
            restoreGame();
            popup(F("Game restored"), 40);
        }
        return;
      }

      if (gb.buttons.pressed(BTN_UP) || gb.buttons.pressed(BTN_DOWN)) {
        newGame = !newGame;
      }

      if (gb.buttons.pressed(BTN_C) || gb.buttons.pressed(BTN_B)) {
        titleMenu(false);
      }
    }
  }
}


void drawField() {

  gb.display.fillRect(1, 0, LCDWIDTH - 2, 7);

  gb.display.drawFastHLine(1, 7, LCDWIDTH - 2);
  gb.display.drawFastHLine(1, 15, LCDWIDTH - 2);
  gb.display.drawFastHLine(1, 23, LCDWIDTH - 2);
  gb.display.drawFastHLine(1, 31, LCDWIDTH - 2);
  gb.display.drawFastHLine(1, 39, LCDWIDTH - 2);
  gb.display.drawFastHLine(1, 47, LCDWIDTH - 2);

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

  for (int row = 0; row < BOARDHEIGHT; row++) {
    int y = 8 + row * (GEMHEIGHT + 1);
    for (int col = 0; col < BOARDWIDTH; col++) {
      int x = 2 + col * (GEMWIDTH + 1);
      if (marked.x == col && marked.y == row) {
        gb.display.setColor(BLACK);
        gb.display.fillRect(x, y, GEMWIDTH, GEMHEIGHT);
        gb.display.setColor(WHITE);
      } else {
        gb.display.setColor(BLACK);
      }
      switch (board[row][col]) {
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

  gb.display.cursorY = 1;
  gb.display.cursorX = 2;

  gb.display.print(gameState.score);

  if (showHint) {
    gb.display.cursorX = LCDWIDTH - 3 * gb.display.fontSize * gb.display.fontWidth - 2;
    gb.display.print(hint.x);
    gb.display.print(":");
    gb.display.print(hint.y);
  } else {
    char text[16];
    itoa(gameState.highscore, text, 10);
    gb.display.cursorX = LCDWIDTH - strlen(text) * gb.display.fontSize * gb.display.fontWidth - 2;
    gb.display.print(text);
  }

  gb.display.setColor(BLACK);
}


void drawCursor() {
  int x = 1 + cursor.x * (CURSORWIDTH);
  int y = 7 + cursor.y * (CURSORHEIGHT);
  gb.display.setColor(WHITE);
  gb.display.drawFastHLine(x + 1, y, CURSORWIDTH - 1);                 // top
  gb.display.drawFastHLine(x + 1, y + CURSORHEIGHT, CURSORWIDTH - 1);  // bottom
  gb.display.drawFastVLine(x, y + 1, CURSORHEIGHT - 1);                // left
  gb.display.drawFastVLine(x + CURSORWIDTH, y + 1, CURSORHEIGHT - 1);  // right

  gb.display.setColor(BLACK);
  // on top
  gb.display.drawPixel(x + 3, y);
  gb.display.drawPixel(x + 5, y);
  // on bottom
  gb.display.drawPixel(x + 3, y + CURSORHEIGHT);
  gb.display.drawPixel(x + 5, y + CURSORHEIGHT);
  // on left
  gb.display.drawPixel(x, y + 3);
  gb.display.drawPixel(x, y + 5);
  // on right
  gb.display.drawPixel(x + CURSORWIDTH, y + 3);
  gb.display.drawPixel(x + CURSORWIDTH, y + 5);
}


void drawGameOverDialog() {
  int top = LCDHEIGHT / 2 - (( gb.display.fontHeight + 1 ) * 4 / 2) - 1 ;
  gb.display.setColor(WHITE);
  gb.display.fillRect(0, top, LCDWIDTH, (gb.display.fontHeight + 1) * 4 + 2);
  gb.display.setColor(BLACK);
  gb.display.cursorY = top + 1;
  printCentered(F(" GAME OVER "));
  gb.display.cursorX = 10;
  gb.display.cursorY = gb.display.cursorY + gb.display.fontHeight + 1;
  gb.display.print("Score: ");
  gb.display.print(gameState.score);
  gb.display.cursorX = 10;
  gb.display.cursorY = gb.display.cursorY + gb.display.fontHeight + 1;
  gb.display.print("Highscore: ");
  gb.display.print(gameState.highscore);
  gb.display.cursorY = gb.display.cursorY + gb.display.fontHeight + 1;
  printCentered("a: board, b: new");
}

void drawPauseDialog() {
  gb.display.cursorY = LCDHEIGHT / 2 - gb.display.fontHeight / 2;
  gb.display.setColor(WHITE);
  gb.display.fillRect(0, LCDHEIGHT / 2 - gb.display.fontHeight / 2 - 1, LCDWIDTH, gb.display.fontHeight + 2);
  gb.display.setColor(BLACK);
  printCentered(F(" PAUSE "));
}
