#include"ourGame.h"

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
      for(int j=rows-4; j<rows; j++){
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
  cout << (board.size()) << " " << (board[0].size()) << endl;
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

  double valPlayer1;
  //Compute for Player 1, ignore player 2 in this


  double valPlayer2;
  //Compute for player 2, ignore player 1

  if(currPlayer==1)
    return valPlayer1 - valPlayer2;
  else
    return valPlayer2 - valPlayer1;
}
