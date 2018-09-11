#include"ourGame.h"

#define
ourGame::ourGame(){
  currPlayer = 1; //Player 1 will start
  playerOneRingsOnBoard = 0; // Initially there are 0 player 1 rings on the board
  playerTwoRingsOnBoard = 0; // Initially there are 0 player 1 rings on the board
  playerOneMarkersOnBoard = 0; // Initially there are 0 player 1 rings on the board
  playerTwoMarkerssOnBoard = 0; // Initially there are 0 player 1 rings on the board
  boardSize = 85; //Atleast this is true for this part, we'll initialise it with a variable for the next part
  struct boardCell tempBoardCell = {0,false,false};
  int rows = 11;

  for(int i=0;i<rows;i++){
    vector<struct boardCell> boardRow;
    for(int j=0; j<rows; j++){
      boardRow.pb(tempBoardCell);
    }
    board[i].pb(boardRow);
  }
  //First index corresponds to vertical and second to y=x waali type ka diagnol
}

/*Big Question, how are we storing the board configuration*/
//Let's suppose for now, we have the function which can transform(hex,pos)->(x,y) and inverse of it

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

pair<int,int> axialToRadialDatabase(int x, int y){
  int hex=0,pos=0;
  //Do the transformation
  return mp(hex,pos);
}

pair<int,int> radialToAxialDatabase(int hex, int pos){
  int x=0,y=0;
  //Do the transformation
  return mp(x,y);
}
