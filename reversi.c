#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//
// Author: Ava Jakob


void printBoard(char board[][26], int n) {
  char alphabet[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
  printf("  ");

  //Print the letters as a header
  for(int i = 0; i<n ; i++){
    printf("%c", alphabet[i]);
  }

  printf("\n");

  //Print current board configuration
  for(int row = 0; row<n ; row++){
    printf("%c", alphabet[row]);
    printf(" ");
    for(int col = 0; col<n; col++){
      printf("%c", board[row][col]);
    }
   printf("\n");
  }

}

char oppositecolour(char colour){
  if(colour=='W'){
    return 'B';
  }else if(colour=='B'){
    return 'W';
  }
}

bool positionInBounds(int n, int row, int col) {
  //Check if row and column are valid
  if(row >= 0 && row < n && col >= 0 && col < n){
    return true;
  } 

  return false;
}

bool checkLegalInDirection(char board[][26], int n, int row, 
        int col, char colour, int deltaRow, int deltaCol)
{
  //Check if row and column are valid
  if (!positionInBounds(n, row + deltaRow, col + deltaCol)){
    return false;
  }

  //Check if colour is valid
  if (board[row + deltaRow][col + deltaCol] != oppositecolour(colour)){
    return false; 
  }
    
  bool done = false;
  bool returnValue = false;
  
  row += deltaRow * 2;
  col += deltaCol * 2;
  
  //Check if move is legal in the given direction
  while (!done) {
    if (!positionInBounds(n, row, col)){
      done = true; 
    } else if (board[row][col] == 'U'){
      done = true;
    } else if (board[row][col] == colour) {
      done = true;
      returnValue = true;
    } else {
      row += deltaRow;
      col += deltaCol;
    }
  }
  return returnValue;
}



int flipInDirection(char board[][26], int row, int col, char colour, int deltaRow, int deltaCol, bool flip)
{
  int count = 0;
  row += deltaRow;
  col += deltaCol;

  //Flip tiles until a piece of the same initial colour is reached
  while (board[row][col] == oppositecolour(colour)) {
    if (flip){
      board[row][col] = colour;
    }
    count++;
    row += deltaRow;
    col += deltaCol;
  }

  //Return number of pieces flipped
  return count;
}

int checkLegalAndFlip(char board[][26], int row, int col, char colour, int size, bool flip){  
  int score = 0;

  //Check if board space is empty
  if (board[row][col] != 'U'){
    return score;
  } else if (!positionInBounds(size, row, col)){
    return score;
  }

  //Only flip associated pieces if requested
  if (flip){
    board[row][col] = colour;
  }

  //Flip all pieces that switch colours as a result of this move
  if (checkLegalInDirection(board, size, row, col, colour, -1, 0)) //North
    score += flipInDirection(board, row, col, colour, -1, 0, flip);
  if (checkLegalInDirection(board, size, row, col, colour, -1, 1)) //NorthEast
    score += flipInDirection(board, row, col, colour, -1, 1, flip);
  if (checkLegalInDirection(board, size, row, col, colour, 0, 1)) //East
    score += flipInDirection(board, row, col, colour, 0, 1, flip);
  if (checkLegalInDirection(board, size, row, col, colour, 1, 1)) //SouthEast
    score += flipInDirection(board, row, col, colour, 1, 1, flip);
  if (checkLegalInDirection(board, size, row, col, colour, 1, 0)) //South
    score += flipInDirection(board, row, col, colour, 1, 0, flip);
  if (checkLegalInDirection(board, size, row, col, colour, 1, -1)) //SouthWest
    score += flipInDirection(board, row, col, colour, 1, -1, flip);
  if (checkLegalInDirection(board, size, row, col, colour, 0, -1)) //West
    score += flipInDirection(board, row, col, colour, 0, -1, flip);  
  if (checkLegalInDirection(board, size, row, col, colour, -1, -1)) //NorthWest
    score += flipInDirection(board, row, col, colour, -1, -1, flip);
  
  //Return number of pieces flipped
  return score;
}

bool computerMakesMove(char board[26][26], int size, char colour ){
  //Check every possible move
  int score = 0;
  int highScore = 0;
  int position[3]={0,0,0};

  //Evaluate all possible moves and associate a score to then choose best move
  for(int i = 0 ; i < size; i++){
    for(int j = 0; j < size; j++){
      score = checkLegalAndFlip(board, i, j, colour, size, false);
      if(score > highScore){
        highScore = score;
        position[0] = i;
        position[1] = j;
      } else if (score == highScore) {
        if(position[0]>i){
          highScore = score;
          position[0] = i;
          position[1] = j;
        }
      }
      score = 0;
    }
  }

  //Ensure move is legal, then announce move and flip associated tiles
  if(checkLegalAndFlip(board, position[0], position[1], colour, size, false )){
    printf("Computer places %c at %c%c.\n", colour, position[0]+'a', position[1]+'a');
    checkLegalAndFlip(board, position[0], position[1], colour, size, true );
    printBoard(board, size);
    return true;
  }

  return false;
}

bool playerMakesMove(char board[26][26], int size, char colour){
  char config[3];
  int row, col;

  //Collect move data from the player
  printf("Enter move for colour %c (RowCol): ", colour);
  scanf("%s", config);
  row = config[0];
  col = config[1];

  //Check if move is legal, then flip pieces accordingly
  if(checkLegalAndFlip(board, row-'a', col-'a', colour, size, false)){
    checkLegalAndFlip(board, row-'a', col-'a', colour, size, true);
    printBoard(board, size);
    return true;
  }else{
    printf("Invalid move.");
    return false;
  }
  
}

int countPieces(char board[26][26], int size, char colour){
  int count;
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      if(board[i][j]==colour){
        count++;
      }
    }
  }
  return count;
}

int main(void) {
  int size;
  char comp;

  //Collect board dimensions
  printf("Enter the board dimension: ");
  scanf("%d", &size);

  //Board Setup
  char board[size+1][26];
  for(int row = 0; row < size; row++){
    for(int col = 0; col < size; col++){
      board[row][col] = 'U';
    }
  }

  //Change centre pieces for game setup
  board[size/2-1][size/2-1]='W';
  board[size/2-1][size/2]='B';
  board[size/2][size/2-1]='B';
  board[size/2][size/2]='W';

  //Check which colour Computer is
  printf("Computer plays (B/W): ");
  scanf(" %c", &comp);
  printBoard(board, size);


  bool otherMove = false;
  int white, black;
  char winner;
  bool done = false;

  //Make sure W player goes first, alternate until board is full
  if(comp=='W'){
    while(playerMakesMove(board, size, 'B') && !done)
      if(computerMakesMove(board, size, 'W')){
        continue;
      }else if(!otherMove){
        printf("%c player has no valid move.\n", comp);
        otherMove = true;
        computerMakesMove(board, size, 'W');
        done = true;
      }
  }else if(comp=='B'){
        while(!done && computerMakesMove(board, size, 'B')){
            if(playerMakesMove(board, size, 'W')){
              continue;
            }else if (!done){
                done = true;
                printf("\n");
            }
        }
  }

  //Evaluate winning conditions
  white = countPieces(board, size, 'W');
  black = countPieces(board, size, 'B');
  if(white>black){
    winner = 'W';
  }else{
    winner = 'B';
  }

  //Announce Winner
  printf("%c player wins.", winner);
  
  return 0;
}