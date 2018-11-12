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

  for(int i=0;i<rows+1;i++){
    valuesForConsecutiveMarkers.push_back(0);
    valuesForConsecutiveMarkers2.push_back(0);
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

  // lli score=0;
  // int count=0,count2=0;
  // vector<int> valuesForConsecutiveMarkers,valuesForConsecutiveMarkers2;
  // int myRingsInitial=rows/2,opponentRingsInitial=rows/2;
  // int otherPlayer = (player%2)+1;
  // if(player==1){
  //   score+=100000*(myRingsInitial-this->playerOneRingsOnBoard);
  //   score-=110000*(opponentRingsInitial-this->playerTwoRingsOnBoard);
  //   score+=this->playerOneMarkersOnBoard;
  // }else{
  //   score+=100000*(myRingsInitial-this->playerTwoRingsOnBoard);
  //   score-=110000*(opponentRingsInitial-this->playerOneRingsOnBoard);////teen zeros aur the
  //   score+=this->playerTwoMarkersOnBoard;
  // }
  // return score;



  //This function computes the heuristic for the player according to the current configuration of the boardSize
  lli score=0;
  int count=0,count2=0;
  vector<int> valuesForConsecutiveMarkers,valuesForConsecutiveMarkers2;
  int myRingsInitial=5,opponentRingsInitial=5;
  int otherPlayer = (player%2)+1;
  bool gotOneRingAlready = false;
  bool gotOneRingAlready2 = false;
  //
  for(int i=0;i<rows+1;i++){
    valuesForConsecutiveMarkers.push_back(0);
    valuesForConsecutiveMarkers2.push_back(0);
  }
  bool doesMyLineContainRing = false;
  bool doesMyLineContainRing2 = false;
  for(int i=0;i<rows;i++){
    for(int j=0;j<rows;j++){
      if(!board[i][j].canBeUsed){
        //if not on board
        if(count!=0){
          valuesForConsecutiveMarkers[count]++;
          if(doesMyLineContainRing){
            valuesForConsecutiveMarkers[count]++;
            doesMyLineContainRing = false;
          }
          count=0;
          gotOneRingAlready = false;
        }
        if(count2!=0){
          valuesForConsecutiveMarkers2[count2]++;
          if(doesMyLineContainRing2){
            valuesForConsecutiveMarkers2[count2]++;
            doesMyLineContainRing2 = false;
          }
          count2=0;
          gotOneRingAlready2 = false;
        }
      }else{
        //if usable

        if(board[i][j].player!=player){
          if(count!=0){
            valuesForConsecutiveMarkers[count]++;
            if(doesMyLineContainRing){
              valuesForConsecutiveMarkers[count]++;
              doesMyLineContainRing = false;
            }
            gotOneRingAlready = false;
            count=0;
          }
        }else{
          // pair<int,int> h = cartesianToHex(i,j,rows);

          if(board[i][j].containsMarker){
            count++;
          }else if(board[i][j].containsRings){
            if(gotOneRingAlready){

            // doesMyLineContainRing = true;
            // count++;//=0.5;
            }else{
              gotOneRingAlready = true;
              doesMyLineContainRing = true;
              count++;
            }
          }
        }

        if(board[i][j].player!=otherPlayer){
          if(count2!=0){
            valuesForConsecutiveMarkers2[count2]++;
            if(doesMyLineContainRing2){
              valuesForConsecutiveMarkers2[count2]++;
              doesMyLineContainRing2 = false;
            }
            count2=0;
            gotOneRingAlready2 = false;
          }
        }else{
          // pair<int,int> h = cartesianToHex(i,j,rows);

          if(board[i][j].containsMarker){
            count2++;
          }else if(board[i][j].containsRings&&!gotOneRingAlready2){
            doesMyLineContainRing2 = true;
            count2++;//=0.5;
            gotOneRingAlready2 = true;
          }
        }
      }
    }
  }

  doesMyLineContainRing = false;
  doesMyLineContainRing2 = false;
  count = 0;
  count2 = 0;
  gotOneRingAlready = false;
  gotOneRingAlready2 = false;
  for(int j=0;j<rows;j++){
    for(int i=0;i<rows;i++){
      if(!board[i][j].canBeUsed){
        if(count!=0){
          valuesForConsecutiveMarkers[count]++;
          if(doesMyLineContainRing){
            valuesForConsecutiveMarkers[count]++;
            doesMyLineContainRing = false;
          }
          count=0;
          gotOneRingAlready = false;
        }
        if(count2!=0){
          valuesForConsecutiveMarkers2[count2]++;
          if(doesMyLineContainRing2){
            valuesForConsecutiveMarkers2[count2]++;
            doesMyLineContainRing2 = false;
          }
          count2=0;
          gotOneRingAlready2 = false;
        }
      }else{
        //usable index
        if(board[i][j].player!=player){
          if(count!=0){
            valuesForConsecutiveMarkers[count]++;
            if(doesMyLineContainRing){
              valuesForConsecutiveMarkers[count]++;
              doesMyLineContainRing = false;
            }
            count=0;
            gotOneRingAlready = false;
          }
        }else{
          if(board[i][j].containsMarker){
            count++;
          }else if(board[i][j].containsRings&&!gotOneRingAlready){
            doesMyLineContainRing = true;
            count++;//=0.5;
            gotOneRingAlready = true;
          }
        }
        if(board[i][j].player!=otherPlayer){
          if(count2!=0){
            valuesForConsecutiveMarkers2[count2]++;
            if(doesMyLineContainRing2){
              valuesForConsecutiveMarkers2[count2]++;
              doesMyLineContainRing2 = false;
            }
            count2=0;
            gotOneRingAlready2 = false;
          }
        }else{
          pair<int,int> h = cartesianToHex(i,j,rows);
          if(board[i][j].containsMarker){
            count2++;
          }else if(board[i][j].containsRings&&gotOneRingAlready2==false){
            doesMyLineContainRing2 = true;
            count2++;//=0.5;
            gotOneRingAlready2 = true;
          }
        }
      }
    }
  }

  doesMyLineContainRing = false;
  doesMyLineContainRing2 = false;
  count = 0;
  count2 = 0;
  gotOneRingAlready = false;
  gotOneRingAlready2 = false;
  for(int k=0;k<rows;k++){
    for(int kk=0;kk<rows;kk++){
      int i = k + kk;
      int j = kk;
      if(i<rows && j <rows){
        if(!board[i][j].canBeUsed){
          if(count!=0){
            valuesForConsecutiveMarkers[count]++;
            if(doesMyLineContainRing){
              valuesForConsecutiveMarkers[count]++;
              doesMyLineContainRing = false;
            }
            count=0;
            gotOneRingAlready = false;
          }
          if(count2!=0){
            valuesForConsecutiveMarkers2[count2]++;
            if(doesMyLineContainRing2){
              valuesForConsecutiveMarkers2[count2]++;
              doesMyLineContainRing2 = false;
            }
            count2=0;
            gotOneRingAlready2 = false;
          }
        }else{
          if(board[i][j].player!=player){
            if(count!=0){
              valuesForConsecutiveMarkers[count]++;
              if(doesMyLineContainRing){
                valuesForConsecutiveMarkers[count]++;
                doesMyLineContainRing = false;
              }
              count=0;
              gotOneRingAlready = false;
            }
          }else{
            if(board[i][j].containsMarker){
              count++;
            }else if(board[i][j].containsRings&&!gotOneRingAlready){
              doesMyLineContainRing = true;
              count++;//=0.5;
              gotOneRingAlready = true;
            }
          }
          if(board[i][j].player!=otherPlayer){
            if(count2!=0){
              valuesForConsecutiveMarkers2[count2]++;
              if(doesMyLineContainRing2){
                valuesForConsecutiveMarkers2[count2]++;
                doesMyLineContainRing2 = false;
              }
              count2=0;
              gotOneRingAlready2 = false;
            }
          }else{
            pair<int,int> h = cartesianToHex(i,j,rows);
            if(board[i][j].containsMarker){
              count2++;
            }else if(board[i][j].containsRings&&!gotOneRingAlready2){
              doesMyLineContainRing2 = true ;
              count2++;//=0.5;
              gotOneRingAlready2 = true;
            }
          }
        }
      }  
    }
  }

  // int weight[] = {0,0,0,50,100,500,1000,500,500,500,500,500,500,500};
  int opponentWeight[] = {0,0,0,-5,-50,-1000,-1000,-1000,-1000,-1000,-1000,-1000, -1000,-1000};
  int weight[]={0,1,3,9,27,100,100,100,100,100,100,100,1876,1876,1876};
  // int opponentWeight[] = {0,0,-1,-2,-10,-393,-393,-400,-400,-400,-2000,-1123,-1123,-1123};
  for(int i=0;i<rows+1;i++){
    score+=weight[i]*valuesForConsecutiveMarkers[i];
    score+=opponentWeight[i]*valuesForConsecutiveMarkers2[i];
  }


  if(player==1){
    score+=1000*(myRingsInitial-this->playerOneRingsOnBoard);//100000
    score-=1000*(opponentRingsInitial-this->playerTwoRingsOnBoard);//200000
    // score+=2*this->playerOneMarkersOnBoard;
  }else{
    score+=1000*(myRingsInitial-this->playerTwoRingsOnBoard);
    score-=1000*(opponentRingsInitial-this->playerOneRingsOnBoard);////teen zeros aur the
    // score+=2*this->playerTwoMarkersOnBoard;
  }

    // ourPlayer tempPlayer(player-1,time_limit,rows/2,markersNeededToRemove);
    // auto tempV = tempPlayer.selectAndMove(player,this);
    // score+=tempV.size();
    // auto tempV = tempPlayer.selectAndMove(3-player,this);
    // score-=tempV.size();

  return score;
}

double ourGame::heuristicForSort(int player){
  lli score=0;
  int count=0,count2=0;
  vector<int> valuesForConsecutiveMarkers,valuesForConsecutiveMarkers2;
  int myRingsInitial=rows/2,opponentRingsInitial=rows/2;
  int otherPlayer = (player%2)+1;
  if(player==1){
    score+=100000*(myRingsInitial-this->playerOneRingsOnBoard);
    score-=140000*(opponentRingsInitial-this->playerTwoRingsOnBoard);
    score+=2*this->playerOneMarkersOnBoard;
  }else{
    score+=100000*(myRingsInitial-this->playerTwoRingsOnBoard);
    score-=140000*(opponentRingsInitial-this->playerOneRingsOnBoard);////teen zeros aur the
    score+=2*this->playerTwoMarkersOnBoard;
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
      // this->allMarkerPosition.push_back(make_pair(startX,i));
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
      // this->allMarkerPosition.push_back(make_pair(i,startY));
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
      // this->allMarkerPosition.push_back(make_pair(i,j));
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
  // auto it = find(this->allMarkerPosition.begin(),this->allMarkerPosition.end(),make_pair(x,y));
  // if(it!=this->allMarkerPosition.end())
  //   this->allMarkerPosition.erase(it);
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
