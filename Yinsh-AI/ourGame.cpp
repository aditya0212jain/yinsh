#include"ourGame.h"
#include "ourPlayer.h"

#define pb push_back
#define mp make_pair
#define PI 3.14159

using namespace std;

ourGame::ourGame(int boardSize){
  currPlayer = 1; //Player 1 will start
  playerOneRingsOnBoard = 0; // Initially there are 0 player 1 rings on the board
  playerTwoRingsOnBoard = 0; // Initially there are 0 player 2 rings on the board
  playerOneMarkersOnBoard = 0; // Initially there are 0 player 1 rings on the board
  playerTwoMarkersOnBoard = 0; // Initially there are 0 player 2 rings on the board
  //boardSize = 85; //Atleast this is true for this part, we'll initialise it with a variable for the next part
  rows = 2*boardSize+1;

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

double ourGame::computeHeuristicValue(int player){
  //This function computes the heuristic for the player according to the current configuration of the boardSize
  lli score=0;
  int count=0,count2=0;
  vector<int> valuesForConsecutiveMarkers,valuesForConsecutiveMarkers2;
  int myRingsInitial=5,opponentRingsInitial=5;
  int otherPlayer = (player%2)+1;
  //
  for(int i=0;i<rows+1;i++){
    valuesForConsecutiveMarkers.push_back(0);
    valuesForConsecutiveMarkers2.push_back(0);
  }

  for(int i=0;i<rows;i++){
    for(int j=0;j<rows;j++){
      if(!board[i][j].canBeUsed){
        if(count!=0){
          valuesForConsecutiveMarkers[count]++;
          count=0;
        }
        if(count2!=0){
          valuesForConsecutiveMarkers2[count2]++;
          count2=0;
        }
      }else{
        if(board[i][j].player!=player){
          if(count!=0){
            valuesForConsecutiveMarkers[count]++;
            count=0;
          }
        }else{
          pair<int,int> h = cartesianToHex(i,j,rows);

          if(board[i][j].containsMarker){
            count++;
          }else if(board[i][j].containsRings){
            count+=0.5;
          }
        }

        if(board[i][j].player!=otherPlayer){
          if(count2!=0){
            valuesForConsecutiveMarkers2[count2]++;
            count2=0;
          }
        }else{
          pair<int,int> h = cartesianToHex(i,j,rows);

          if(board[i][j].containsMarker){
            count2++;
          }else if(board[i][j].containsRings){
            count2+=0.5;
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
        if(count2!=0){
          valuesForConsecutiveMarkers2[count2]++;
          count2=0;
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

        if(board[i][j].player!=otherPlayer){
          if(count2!=0){
            valuesForConsecutiveMarkers2[count2]++;
            count2=0;
          }
        }else{
          pair<int,int> h = cartesianToHex(i,j,rows);

          if(board[i][j].containsMarker){
            count2++;
          }else if(board[i][j].containsRings){
            count2+=0.5;
          }
        }
      }
    }
  }


  int weight[] = {0,1,3,9,27,81,100,120,140,160,180,81,27,9};
  int opponentWeight[] = {0,0,-2,-5,-50,-1000,-1000,-1000,-1000,-1000,-1000,-1000, -1000,-1000};
  for(int i=0;i<rows+1;i++){
    score+=weight[i]*valuesForConsecutiveMarkers[i];
    // score+=opponentWeight[i]*valuesForConsecutiveMarkers2[i];
  }


  if(player==1){
    score+=100000*(myRingsInitial-this->playerOneRingsOnBoard);
    score-=100000*(opponentRingsInitial-this->playerTwoRingsOnBoard);
    // score+=this->playerOneMarkersOnBoard;
  }else{
    score+=100000*(myRingsInitial-this->playerTwoRingsOnBoard);
    score-=100000*(opponentRingsInitial-this->playerOneRingsOnBoard);////teen zeros aur the
    // score+=this->playerTwoMarkersOnBoard;
  }


  return score;

}

void ourGame::copyTheBoard(ourGame* game){
  currPlayer = game->currPlayer;
  playerOneRingsOnBoard = game->playerOneRingsOnBoard;
  playerTwoRingsOnBoard = game->playerTwoRingsOnBoard;
  playerOneMarkersOnBoard = game->playerOneMarkersOnBoard;
  playerTwoMarkersOnBoard = game->playerTwoMarkersOnBoard;
  // boardSize = game->boardSize; // For this part, it is 85
  board = game->board;// index = (0,0)->0, others -> ((h)*(h+1))/2 + pos
}


bool ourGame::ended(){
  return false;//change this after
}

void ourGame::moveUndo(int playerNo, string s){
  ourPlayer temp(1,150,rows/2,5);//Change 5 with a configuration
  int length = s.length();
  //cout << "Length of input: " << length << endl;

  ourGame* game = this;
  int rows = (game->rows);
  //cout << rows << " Yeh toh number of rows hai!!" << endl;
  //Here the coordinates are in hex and pos form
  vector<string> p = split(s,' ');
  int pointer = p.size()-1;

  while(pointer>=0){
    if(p[pointer].compare("RS")==0){
      int startX = stoi(p[pointer+1]);
      int startY = stoi(p[pointer+2]);
      int endX = stoi(p[pointer+4]);
      int endY = stoi(p[pointer+5]);
      pair<int,int> convertStart= hexToCartesian(startX, startY, rows);
      pair<int,int> convertEnd = hexToCartesian(endX,endY,rows);
      addRow(playerNo, convertStart.first, convertStart.second, convertEnd.first, convertEnd.second);
      // pointer += 6;
    }
    else if(p[pointer].compare("X")==0){
      // cout << "Did I come here?" << endl;
      int x = stoi(p[pointer+1]);
      int y = stoi(p[pointer+2]);
      pair<int,int> coor = hexToCartesian(x,y,rows);
      //cout << coor.first << " " << coor.second << endl;
      temp.placeRing(playerNo,coor.first,coor.second, game);
      // pointer += 3;
    }
    else if(p[pointer].compare("S")==0){
      int xStart = stoi(p[pointer+1]);
      int yStart = stoi(p[pointer+2]);
      int x = stoi(p[pointer+4]);
      int y = stoi(p[pointer+5]);
      pair<int,int> convertStart= hexToCartesian(xStart, yStart, rows);
      pair<int,int> convertEnd = hexToCartesian(x,y,rows);
      temp.moveRing(playerNo,convertEnd.first, convertEnd.second, convertStart.first, convertStart.second, game);
      removeMarker(playerNo,convertEnd.first,convertEnd.second);
      // pointer += 6;
    }
    else if(p[pointer].compare("P")==0){
      int x = stoi(p[pointer+1]);
      int y = stoi(p[pointer+2]);
      pair<int,int> coor = hexToCartesian(x,y,rows);
      temp.removeRing(playerNo,coor.first,coor.second, game);
      // pointer += 3;
    }
    pointer--;
    // printBoard();cout << endl;
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

bool ourGame::equalsTo(ourGame *game){
  for(int i=0;i<board.size();i++){
    for(int j=0;j<board[i].size();j++){
      boardCell t1 = board[i][j];
      boardCell t2 = game->board[i][j];
      if(t1.player!=t2.player){
        cout<<"Found mismatch "<<i<<" ,"<<j<<endl;
        return false;
      }
      // if(t1.containsMarker)
    }
  }
return true;

}
