#include"ourGame.h"
#include "ourPlayer.h"

#define pb push_back
#define mp make_pair
#define PI 3.14159

using namespace std;

ourGame::ourGame(){
  currPlayer = 1; //Player 1 will start
  playerOneRingsOnBoard = 0; // Initially there are 0 player 1 rings on the board
  playerTwoRingsOnBoard = 0; // Initially there are 0 player 2 rings on the board
  playerOneMarkersOnBoard = 0; // Initially there are 0 player 1 rings on the board
  playerTwoMarkersOnBoard = 0; // Initially there are 0 player 2 rings on the board
  boardSize = 85; //Atleast this is true for this part, we'll initialise it with a variable for the next part
  rows = 11;

  for(int i=0;i<rows;i++){
    boardCell tempBoardCell;// {0,false,false};
    vector<boardCell> boardRow;
    for(int j=0; j<rows; j++){
      boardRow.pb(tempBoardCell);
    }
    if(i==0){
      for(int j=1; j<5; j++){
        boardRow[j].canBeUsed = true;
      }
    }
    else if(i==rows-1){
      for(int j=rows-5; j<rows-1; j++){
        boardRow[j].canBeUsed = true;
      }
    }
    else if(i<(rows-1)/2){
      for(int j=0; j<=(5+i); j++){
        boardRow[j].canBeUsed = true;
      }
    }
    else if(i>(rows)/2){
      for(int j=(i-rows/2); j<rows; j++){
        boardRow[j].canBeUsed = true;
      }
    }
    else{
      for(int j=1; j<rows-1; j++){
        boardRow[j].canBeUsed = true;
      }
    }
    board.pb(boardRow);

  }
  //cout << "Board[0][5]: " << board[0][5].canBeUsed << endl;

  //First index corresponds to vertical and second to y=x waali type ka diagnol

  //Checking the board configuration
  /*
  for(int i=0; i<rows; i++){
    for(int j=0; j<rows; j++){
      if(board[i][j].canBeUsed)
        cout << j << " ";
    }
    cout << endl;
  }
  */
  //cout << (board.size()) << " " << (board[0].size()) << endl;
}

/*Big Question, how are we storing the board configuration*/
//Let's suppose for now, we have the function which can transform(hex,pos)->(x,y) and inverse of it
void ourGame::printBoard(){
  for(int i=0; i<rows; i++){
    for(int j=0; j<rows;j++){
      if(board[i][j].canBeUsed){
        if(board[i][j].containsRings){
          cout << board[i][j].player << "R" << " ";
        }
        else if(board[i][j].containsMarker){
          cout << board[i][j].player << "M" << " ";
        }
        else
          cout << "_" << " ";
      }
      else{
        cout << "* " ;
      }
    }
    cout << endl;
  }
}
double ourGame::computeHeuristicValue(){
  //This function computes the heuristic for the player according to the current configuration of the boardSize

  int player;//take it as input
  int count=0;
  vector<int> valuesForConsecutiveMarkers;
  int myRingsInitial,opponentRingsInitial;

// void ourGame::miniMax(){
//   int depth = 4;//Determines the depth of the trees till which we will see
//   for(int i=0; i<depth; i++){
//
//   }
// }

  for(int i=0;i<12;i++){
    valuesForConsecutiveMarkers.push_back(0);
  }

  for(int i=0;i<rows;i++){
    for(int j=0;j<rows;j++){
      if(!board[i][j].canBeUsed){
        if(count!=0){
          valuesForConsecutiveMarkers[count]++;
          count=0;
        }
      }else{
        if(board[i][j].player!=player){
          if(count!=0){
            valuesForConsecutiveMarkers[count]++;
            count=0;
          }
        }else{
          if(board[i][j].containsMarker){
            count++;
          }else if(board[i][j].containsRings){
            count+=0.5;
          }
        }
      }
    }
  }

  for(int j=0;j<rows;j++){
    for(int i=0;i<rows;i++){
      if(!board[i][j].canBeUsed){
        if(count!=0){
          valuesForConsecutiveMarkers[count]++;
          count=0;
        }
      }else{
        if(board[i][j].player!=player){
          if(count!=0){
            valuesForConsecutiveMarkers[count]++;
            count=0;
          }
        }else{
          if(board[i][j].containsMarker){
            count++;
          }else if(board[i][j].containsRings){
            count+=0.5;
          }
        }
      }
    }
  }

  lli score=0;
  int weight[] = {0,1,3,9,27,81,81,81,81,81,81,81};
  for(int i=0;i<12;i++){
    score+=weight[i]*valuesForConsecutiveMarkers[i];
  }
  if(player==1){
    score+=10000*(myRingsInitial-this->playerOneRingsOnBoard);
    score-=10000*(opponentRingsInitial-this->playerTwoRingsOnBoard);
  }else{
    score+=10000*(myRingsInitial-this->playerTwoRingsOnBoard);
    score-=10000*(opponentRingsInitial-this->playerOneRingsOnBoard);
  }

  return score;

  // if(player==1){
  //   return this->playerOneMarkersOnBoard - this->playerTwoMarkersOnBoard;
  // }else{
  //   return this->playerTwoMarkersOnBoard - this->playerOneMarkersOnBoard;
  // }

}

void ourGame::copyTheBoard(ourGame* game){
  currPlayer = game->currPlayer;
  playerOneRingsOnBoard = game->playerOneRingsOnBoard;
  playerTwoRingsOnBoard = game->playerTwoRingsOnBoard;
  playerOneMarkersOnBoard = game->playerOneMarkersOnBoard;
  playerTwoMarkersOnBoard = game->playerTwoMarkersOnBoard;
  boardSize = game->boardSize; // For this part, it is 85
  board = game->board;// index = (0,0)->0, others -> ((h)*(h+1))/2 + pos
}

void ourGame::moveUndo(int playerNo, string s){
  ourPlayer temp(1,150);
  int length = s.length();
  //cout << "Length of input: " << length << endl;
  int pointer = 0;
  ourGame* game = this;
  int rows = (game->rows);
  //cout << rows << " Yeh toh number of rows hai!!" << endl;
  //Here the coordinates are in hex and pos form
  while(pointer<length){
    if(s[pointer]=='R'){
      int startX = s[pointer+3]-'0';
      int startY = s[pointer+5]-'0';
      int endX = s[pointer+10]-'0';
      int endY = s[pointer+12]-'0';
      pair<int,int> convertStart= hexToCartesian(startX, startY, rows);
      pair<int,int> convertEnd = hexToCartesian(endX,endY,rows);
      addRow(playerNo, convertStart.first, convertStart.second, convertEnd.first, convertEnd.second);
      pointer += 14;
    }
    else if(s[pointer]=='X'){
      cout << "Did I come here?" << endl;
      int x = s[pointer+2]-'0';
      int y = s[pointer+4]-'0';
      pair<int,int> coor = hexToCartesian(x,y,rows);
      //cout << coor.first << " " << coor.second << endl;
      temp.placeRing(playerNo,coor.first,coor.second, game);
      pointer += 6;
    }
    else if(s[pointer]=='S'){
      int xStart = s[pointer+2]-'0';
      int yStart = s[pointer+4]-'0';
      int x = s[pointer+8]-'0';
      int y = s[pointer+10]-'0';
      pair<int,int> convertStart= hexToCartesian(xStart, yStart, rows);
      pair<int,int> convertEnd = hexToCartesian(x,y,rows);
      //cout << convertStart.first << " " << convertStart.second << endl;
      //cout << convertEnd.first << " " << convertEnd.second << endl;
      temp.moveRing(playerNo,convertEnd.first, convertEnd.second, convertStart.first, convertStart.second, game);
      removeMarker(playerNo,convertEnd.first,convertEnd.second);
      pointer += 12;
    }
    else if(s[pointer]=='P'){
      int x = s[pointer+2]-'0';
      int y = s[pointer+4]-'0';
      pair<int,int> coor = hexToCartesian(x,y,rows);
      temp.removeRing(playerNo,coor.first,coor.second, game);
      pointer += 6;
    }
    else{
      cout << "Move is incorrect!!" << endl;
      return;
    }
  }
}

void ourGame::addRow(int playerNo, int startX, int startY, int endX, int endY){
  int movementX = endX - startX;
  int movementY = endY - startY;
  // cout << movementX << " " << movementY << endl;
  if(movementX==0){
    //Vertical move
    int mov = movementY/abs(movementY);
    for(int i = min(startY,endY); i<=max(endY,startY); i++){
      boardCell tempBoardCell;
      tempBoardCell.player = playerNo;
      tempBoardCell.containsMarker = true;
      tempBoardCell.containsRings = false;
      tempBoardCell.canBeUsed = true;
      board[startX][i] = tempBoardCell;
    }
  }
  else if(movementY==0){
    //Vertical move
    int mov = movementX/abs(movementX);
    for(int i = min(startX,endX); i<=max(endX,startX); i++){
      boardCell tempBoardCell;
      tempBoardCell.player = playerNo;
      tempBoardCell.containsMarker = true;
      tempBoardCell.containsRings = false;
      tempBoardCell.canBeUsed = true;
      board[i][startY] = tempBoardCell;
    }
  }
  else if(movementX==movementY){
    int mov = movementX/abs(movementX);
    int i,j;
    for(i = min(startX,endX),j=min(endY,startY); i<=max(startX,endX),j<=max(endY,startY); i++,j++){
      boardCell tempBoardCell;
      tempBoardCell.player = playerNo;
      tempBoardCell.containsMarker = true;
      tempBoardCell.containsRings = false;
      tempBoardCell.canBeUsed = true;
      board[i][j] = tempBoardCell;
    }
  }
  else{
    cout << "Galat chal rahe ho, sahi Karo!!!" << endl;
  }
  if(/*this->playerNumber*/playerNo==1){
    playerOneMarkersOnBoard = playerOneMarkersOnBoard+5;
  }
  else{
    playerTwoMarkersOnBoard = playerTwoMarkersOnBoard+5;
  }
}

void ourGame::removeMarker(int playerNo, int x, int y){
  boardCell tempBoardCell;
  tempBoardCell.player = 0;
  tempBoardCell.containsMarker = false;
  tempBoardCell.containsRings = false;
  tempBoardCell.canBeUsed = true;
  board[x][y] = tempBoardCell;
}
