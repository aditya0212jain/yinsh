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
  int rows = 11;

  for(int i=0;i<rows;i++){
    boardCell tempBoardCell;// {0,false,false};
    vector<boardCell> boardRow;
    for(int j=0; j<rows; j++){
      boardRow.pb(tempBoardCell);
    }
    board.pb(boardRow);
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

void ourGame::copyTheBoard(ourGame* game){
  currPlayer = game->currPlayer;
  playerOneRingsOnBoard = game->playerOneRingsOnBoard;
  playerTwoRingsOnBoard = game->playerTwoRingsOnBoard;
  playerOneMarkersOnBoard = game->playerOneMarkersOnBoard;
  playerTwoMarkersOnBoard = game->playerTwoMarkersOnBoard;

  boardSize = game->boardSize; // For this part, it is 85
  board = game->board;// index = (0,0)->0, others -> ((h)*(h+1))/2 + pos
}





