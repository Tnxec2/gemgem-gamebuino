

/*
    Patterns for canMakeMove function

    The patterns in oneOffPatterns represent gems that are configured
    in a way where it only takes one move to make a triplet.
*/
const char oneOffPatterns[8][3][2] = {
  { { 0, 1 }, { 1, 0 }, { 2, 0 } },
  { { 0, 1 }, { 1, 1 }, { 2, 0 } },
  { { 0, 0 }, { 1, 1 }, { 2, 0 } },
  { { 0, 1 }, { 1, 0 }, { 2, 1 } },
  { { 0, 0 }, { 1, 0 }, { 2, 1 } },
  { { 0, 0 }, { 1, 1 }, { 2, 1 } },
  { { 0, 0 }, { 0, 2 }, { 0, 3 } },
  { { 0, 0 }, { 0, 1 }, { 0, 3 } }
};

bool canErased[BOARDHEIGHT][BOARDWIDTH];  // temp array for mark gems to remove


// ###########################################


void process() {
  unsigned long currentTime = millis();
  if (currentTime - prevTime >= delayTime) {
    nextstep();
    prevTime = currentTime;
  }
}


void nextstep() {
  switch (step) {
    case STEP_ERASE:
      eraseGems();
      step = STEP_ERASED;
      break;
    case STEP_ERASED:
      if (erasedCount > 0) {
        gb.sound.playOK();
        step = STEP_MOVE;
        if (gameStarted) gameState.score += scoreAdd;
      } else {
        gb.sound.playCancel();
        undoSwitch();
        step = STEP_DONE;
      }
      switched.x = -1;
      switched.y = -1;
      break;
    case STEP_MOVE:
      if (!moveDown()) step = STEP_ERASE;
      // step = STEP_DONE; // test
      break;
    default:
      gameOver = !canMakeMove();
      showGameOverDialog = gameOver;
      if ( gameOver && gameState.score > gameState.highscore ) {
        gameState.highscore = gameState.score;
        saveNewHighScore();
      }
      processing = false;  // end of gems move
      if (!gameStarted) gameStarted = true;
      break;
  }
}


bool canMakeMove() {
  /*
     source: https://inventwithpython.com/pygame/chapter10.html

     Return True if the board is in a state where a matching
     move can be made on it. Otherwise return False.
    */

  /*
    
     The x and y variables iterate over each space on the board.
     If we use + to represent the currently iterated space on the
     board, then this pattern: ((0,1), (1,0), (2,0))refers to identical
     gems being set up like this:
         +A
         B
         C
     That is, gem A is offset from the + by (0,1), gem B is offset
     by (1,0), and gem C is offset by (2,0). In this case, gem A can
     be swapped to the left to form a vertical three-in-a-row triplet.
     
     There are eight possible ways for the gems to be one move
     away from forming a triple, hence oneOffPattern has 8 patterns.

    */
  hint.x = -1;
  hint.y = -1;
  for (int x = 0; x < BOARDWIDTH; x++) {
    for (int y = 0; y < BOARDHEIGHT; y++) {
      for (auto& pat : oneOffPatterns) {
        // check each possible pattern of "match in next move" to
        // see if a possible move can be made.
        if (
            (    y + pat[0][1] < BOARDHEIGHT && x + pat[0][0] < BOARDWIDTH 
              && y + pat[1][1] < BOARDHEIGHT && x + pat[1][0] < BOARDWIDTH
              && y + pat[2][1] < BOARDHEIGHT && x + pat[2][0] < BOARDWIDTH
              && board[y + pat[0][1]][x + pat[0][0]] == board[y + pat[1][1]][x + pat[1][0]]
              && board[y + pat[0][1]][x + pat[0][0]] == board[y + pat[2][1]][x + pat[2][0]]
            )
          || 
            (
                 y + pat[0][0] < BOARDHEIGHT && x + pat[0][1] < BOARDWIDTH 
              && y + pat[1][0] < BOARDHEIGHT && x + pat[1][1] < BOARDWIDTH
              && y + pat[2][0] < BOARDHEIGHT && x + pat[2][1] < BOARDWIDTH
              && board[y + pat[0][0]][x + pat[0][1]] == board[y + pat[1][0]][x + pat[1][1]] 
              && board[y + pat[0][0]][x + pat[0][1]] == board[y + pat[2][0]][x + pat[2][1]]
            )
          )
        {
          hint.x = x;
          hint.y = y;
          showHint = false; // TODO - set to false for production
          return true;  // return True the first time you find a pattern
        }
      }
    }
  }

  return false;
}


void undoSwitch() {
  if (switched.x >= 0) {
    char temp = board[switched.y][switched.x];
    board[switched.y][switched.x] = board[cursor.y][cursor.x];
    board[cursor.y][cursor.x] = temp;
  }
}


bool moveDown() {
  bool result = false;
  // fill empty cells in first row
  for (int col = 0; col < BOARDWIDTH; col++) {
    if (board[0][col] == 0) board[0][col] = random(1, NUMGEMIMAGES + 1);
  }
  // move gems down
  for (int row = BOARDHEIGHT - 1; row > 0; row--) {
    for (int col = 0; col < BOARDWIDTH; col++) {
      if (board[row][col] == 0 && board[row - 1][col] > 0) {
        board[row][col] = board[row - 1][col];
        board[row - 1][col] = 0;
        result = true;
      }
    }
  }

  return result;
}


void eraseGems() {
  erasedCount = 0;
  scoreAdd = 0;
  showHint = false;

  // mark gems to remove
  for (int row = 0; row < BOARDHEIGHT; row++) {
    for (int col = 0; col < BOARDWIDTH; col++) {
      if (
        // horizontal from left to right
          (col + 2 < BOARDWIDTH && board[row][col] == board[row][col + 1] && board[row][col] == board[row][col + 2])
        ||  
        // vertical from top to bottom
          (row + 2 < BOARDHEIGHT && board[row][col] == board[row + 1][col] && board[row][col] == board[row + 2][col])
       ) {
        canErased[row][col] = true;
        int count = 1;
        int offset = col + 1;
        // horizontal from left to right
        if ( col + 2 < BOARDWIDTH && board[row][col] == board[row][col + 1] && board[row][col] == board[row][col + 2]){
          while (offset < BOARDWIDTH && board[row][offset] == board[row][col]) {
            if ( ! canErased[row][offset] ) {
              canErased[row][offset] = true;
              count++;
            }
            offset++;
          }
        }
        // vertical from top to bottom
        if ( (row + 2 < BOARDHEIGHT && board[row][col] == board[row + 1][col] && board[row][col] == board[row + 2][col]) ) {
          offset = row + 1;
          while (offset < BOARDHEIGHT && board[offset][col] == board[row][col]) {
            if ( ! canErased[offset][col] ) {
              canErased[offset][col] = true;
              count++;
            }
            offset++;
          }
        }
        scoreAdd += (10 + (count - 3) * 10); // every 3rd match get 10 points + 10 points for every extra gem
      }
    }
  }

  // erase gems
  for (int row = 0; row < BOARDHEIGHT; row++) {
    for (int col = 0; col < BOARDWIDTH; col++) {
      if (canErased[row][col]) {
        board[row][col] = 0;
        erasedCount++;
        canErased[row][col] = false;
      }
    }
  }
}

void fillGems() {
  gb.pickRandomSeed();
  for (int row = 0; row < BOARDHEIGHT; row++) {
    for (int col = 0; col < BOARDWIDTH; col++) {
      board[row][col] = random(1, NUMGEMIMAGES + 1);
      canErased[row][col] = false;
    }
  }
}

void restoreGems() {
  for (int row = 0; row < BOARDHEIGHT; row++) {
    for (int col = 0; col < BOARDWIDTH; col++) {
      board[row][col] =  gameState.board[row][col];
      canErased[row][col] = false;
    }
  }
}

void switchGems() {
  if (isNeighbor()) {
    char temp = board[cursor.y][cursor.x];
    board[cursor.y][cursor.x] = board[marked.y][marked.x];
    board[marked.y][marked.x] = temp;

    switched.x = marked.x;
    switched.y = marked.y;
    startProcessing();

    marked.x = -1;
    marked.y = -1;
    return;
  }

  marked.x = cursor.x;
  marked.y = cursor.y;
}


void startProcessing() {
  processing = true;
  step = STEP_ERASE;
  prevTime = 0;
}


bool isNeighbor() {
  if (marked.x < 0) return false;
  if (cursor.x == marked.x) {
    if (cursor.y - 1 == marked.y) return true;
    if (cursor.y + 1 == marked.y) return true;
  }
  if (cursor.y == marked.y) {
    if (cursor.x - 1 == marked.x) return true;
    if (cursor.x + 1 == marked.x) return true;
  }
  return false;
}