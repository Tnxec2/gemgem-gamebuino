
void newSave() {
  strcpy_P(gameState.id, GAME_ID);

  for (int row = 0; row < BOARDHEIGHT; row++) {
    for (int col = 0; col < BOARDWIDTH; col++) {
      gameState.board[row][col] = 0;
    }
  }
  gameState.score = 0;
  gameState.highscore = 0;
  writeEeprom();
}


void saveGame() {
  for (int row = 0; row < BOARDHEIGHT; row++) {
    for (int col = 0; col < BOARDWIDTH; col++) {
      gameState.board[row][col] = gameOver ? 0 : board[row][col];
    }
  }
  writeEeprom();
  gb.sound.playOK();
  gb.popup(F("Game saved."), 40);
}


void saveNewHighScore() {
  for (int row = 0; row < BOARDHEIGHT; row++) {
    for (int col = 0; col < BOARDWIDTH; col++) {
      gameState.board[row][col] =  0;
    }
  }
  writeEeprom();
}


void writeEeprom() {
  for (int x = 0; x < sizeof(gameState); x++) {
    EEPROM.write(x, ((uint8_t*)&gameState)[x]);
  }
}


void loadGame() {
  for (int x = 0; x < sizeof(gameState); x++) {
    ((uint8_t*)&gameState)[x] = EEPROM.read(x);
  }
}


void restoreGame() {
  initVars();
  if (gameState.board[0][0] == 0) {
    fillGems();
  } else {
    restoreGems();
  }
  startProcessing();
  gb.sound.playOK();
}


boolean isValidGame() {
  for (int x = 0; x < sizeof(GAME_ID); x++) {
    ((uint8_t*)&gameState)[x] = EEPROM.read(x);
  }
  return strcmp_P(gameState.id, GAME_ID) == 0;
}


void initGame() {
  initVars();
  gameState.score = 0;
  fillGems();
  startProcessing();
  gb.sound.playOK();
}


void initVars() {
  prevTime = 0;
  paused = false;
  gameOver = false; 
  showHint = false;
  gameStarted = false;

  cursor.x = BOARDWIDTH / 2;
  cursor.y = BOARDHEIGHT / 2;
  marked.x = -1;
  marked.y = -1;
  switched.x = -1;
  switched.y = -1;
}

