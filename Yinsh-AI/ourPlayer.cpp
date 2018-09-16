
#include"ourPlayer.h"
#include"ourGame.h"
#include"utils.h"
#include <bits/stdc++.h> //Can we use this??

#define pb push_back
#define mp make_pair
#define lli long long int

using namespace std;

struct childSortNode{
  ourGame gameNode;
  lli value;
}

/*General Comments*/
// I think we should use the approach of double rows naming and use the approach of hexagon naming just for input and output, as that is only stored in the game which is used by Yinsh.js
/*GeneralComments over*/


bool compareForMax(const childSortNode &a, const childSortNode &b)
{
    return a.value>b.value;
}

bool compareForMin(const childSortNode &a,const childSortNode &b){
  return a.value<b.value;
}


vector<ourGame> ourPlayer::sortChildren(vector<ourGame> childNodes,bool forMax){
  vector<childSortNode> v;
  for(int i=0;i<childNodes.size();i++){
    lli valueTemp = childNodes[i].computeHeuristicValue();
    childSortNode temp;
    temp.gameNode = childNodes[i];
    temp.value = valueTemp;
    v.push_back(temp);
  }
  if(forMax){
    sort(v.begin(),v.end(),compareForMax);
  }else{
    sort(v.begin(),v.end(),compareForMin);
  }
  return v;
}



//Assuming PLayer 0 moves first and PLayer 1 follows

//Constructor
ourPlayer::ourPlayer(int playerNumber,int timeLeft){
  //cin>>player_no>>total_rings>>time_left;
  this->playerNumber =  playerNumber; //1-> Player 1, 2-> Player 2
  this->totalRings =  5; //This version only has to deal with 5 rings
  this->timeLeft =  timeLeft; //will be initialised with full time
  this->myRingsRemoved = 0;//starting with 0 rings
  this->game = new ourGame();
}

//These hexagon and position will be decided by Alpha-Beta pruning
void ourPlayer::placeRing(int playerNo, int x, int y){
  //Hexagon defines the hexagon in which we will place the new rings
  //position defines the position of that new ring in the hexagon
  // move temp;//it will be automatically initialized by {"",0,0}
  // pair<int,int> p = getRadialFromAxial(x,y,11);//rows = 11
  // //cout << p.first << " " << p.second << endl;
  // temp.type = "P";
  // temp.hex = p.first; // hexagon
  // temp.pos = p.second; //position
  //cout << "DO I";
  //rings.pb(mp(x,y));
  //cout << " Reach here?" << endl;
  struct boardCell tempboardCell; // Ring is placed, and not marker
  tempboardCell.player = playerNo;//this->playerNumber;
  tempboardCell.containsMarker = false;
  tempboardCell.containsRings = true;
  tempboardCell.canBeUsed = true;
  //cout << "And here?" << endl;
  // cout << (game->board.size()) << endl;
  // cout << game->board[x][y].player << " ";
  // cout << "Definitely I don't reach here, Do I?" << endl;
  game->board[x][y]  = tempboardCell;
  //cout << game->board[x][y].player << endl;
  if(/*this->playerNumber*/playerNo==1){
    game->playerOneRingsOnBoard++;
  }
  else{
    game->playerTwoRingsOnBoard++;
  }
  //return temp;
}

void ourPlayer::moveRing(int playerNo,int xStart, int yStart, int x, int y){
  //index is the index in rings of the ring which we need to move_end, hexagon and position same  as above
  //We assume that index is never out of bounds and move to be moved is a valid move
  struct move temp;// = {"",0,0};

  // pair<int,int> p = getRadialFromAxial(x,y,11);//rows = 11
  // temp.type = "M";
  // temp.hex = p.first; // hexagon
  // temp.pos = p.second; //position
  //There will be a marker at original position

  pair<int,int> newMarkerPosition = mp(xStart,yStart);
  if(!(game->board[xStart][yStart]).containsRings){
    cout << "Galat Chaal chali hai Aapne!" << endl;
    return ;
  }
  //rings[index] = mp(x,y);
  struct boardCell tempBoardCellRing;// {this->playerNumber, true, false}; //Adding ring to the new positions
  tempBoardCellRing.player = playerNo;//this->playerNumber;
  tempBoardCellRing.containsRings = true;
  tempBoardCellRing.containsMarker = false;
  tempBoardCellRing.canBeUsed = true;
  game->board[x][y] = tempBoardCellRing;
  //We can say that this position isn't occupied by any other marker, hence pushBack it
  //markers.pb(newMarkerPosition);
  boardCell tempBoardCellMarker;// = {this->playerNumber,false,true}; // Marker is placed and not ring
  tempBoardCellMarker.player = playerNo;//this->playerNumber;
  tempBoardCellMarker.containsMarker = true;
  tempBoardCellMarker.containsRings = false;
  tempBoardCellMarker.canBeUsed = true;

  game->board[newMarkerPosition.first][newMarkerPosition.second] = tempBoardCellMarker;
  if(/*this->playerNumber*/playerNo==1)
    game->playerOneMarkersOnBoard++;
  else
    game->playerTwoMarkersOnBoard++;
  //return temp;
}

void ourPlayer::removeRing(int playerNo, int x, int y){
  //Remove the ring, removal of markers will be done separately
  boardCell tempBoard = game->board[x][y];
  //cout << p.first << " " << p.second << " Yolo" <<  endl;
  pair<int,int> rad = getRadialFromAxial(x,y,11);//rows = 11
  struct move temp;// = {"",0,0};
  temp.type = "X";
  temp.hex = rad.first;
  temp.pos = rad.second;
  //rings.erase(rings.begin()+index);
  boardCell tempBoardCell;// = {this->playerNumber,false,true}; // Marker is placed and not ring
  tempBoardCell.player = 0;
  tempBoardCell.containsMarker = false;
  tempBoardCell.containsRings = false;
  tempBoardCell.canBeUsed = true;
  game->board[x][y] = tempBoardCell;
  if(/*this->playerNumber*/playerNo==1){
    game->playerOneRingsOnBoard--;
  }
  else{
    game->playerTwoRingsOnBoard--;
  }
  //return temp;
}

void ourPlayer::removeRow(int playerNo, int startX, int startY, int endX, int endY){
  int movementX = endX - startX;
  int movementY = endY - startY;
  cout << movementX << " " << movementY << endl;
  if(movementX==0){
    //Vertical move
    int mov = movementY/abs(movementY);
    for(int i = min(startY,endY); i<=max(endY,startY); i++){
      boardCell tempBoardCell;
      tempBoardCell.player = 0;
      tempBoardCell.containsMarker = false;
      tempBoardCell.containsRings = false;
      tempBoardCell.canBeUsed = true;
      game->board[startX][i] = tempBoardCell;
    }
  }
  else if(movementY==0){
    //Vertical move
    int mov = movementX/abs(movementX);
    for(int i = min(startX,endX); i<=max(endX,startX); i++){
      boardCell tempBoardCell;
      tempBoardCell.player = 0;
      tempBoardCell.containsMarker = false;
      tempBoardCell.containsRings = false;
      tempBoardCell.canBeUsed = true;
      game->board[i][startY] = tempBoardCell;
    }
  }
  else if(movementX==movementY){
    int mov = movementX/abs(movementX);
    int i,j;
    for(i = min(startX,endX),j=min(endY,startY); i<=max(startX,endX),j<=max(endY,startY); i++,j++){
      boardCell tempBoardCell;
      tempBoardCell.player = 0;
      tempBoardCell.containsMarker = false;
      tempBoardCell.containsRings = false;
      tempBoardCell.canBeUsed = true;
      game->board[i][j] = tempBoardCell;
    }
  }
  else{
    cout << "Galat chal rahe ho, sahi Karo!!!" << endl;
  }
  if(/*this->playerNumber*/playerNo==1){
    game->playerOneMarkersOnBoard = game->playerOneMarkersOnBoard-5;
  }
  else{
    game->playerTwoMarkersOnBoard = game->playerTwoMarkersOnBoard-5;
  }
}

void ourPlayer::moveDecider(int playerNo, string s){
  int length = s.length();
  //cout << "Length of input: " << length << endl;
  int pointer = 0;
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
      removeRow(playerNo, convertStart.first, convertStart.second, convertEnd.first, convertEnd.second);
      pointer += 14;
    }
    else if(s[pointer]=='P'){
      int x = s[pointer+2]-'0';
      int y = s[pointer+4]-'0';
      pair<int,int> coor = hexToCartesian(x,y,rows);
      cout << coor.first << " " << coor.second << endl;
      placeRing(playerNo,coor.first,coor.second);
      pointer += 6;
    }
    else if(s[pointer]=='S'){
      int xStart = s[pointer+2]-'0';
      int yStart = s[pointer+4]-'0';
      int x = s[pointer+8]-'0';
      int y = s[pointer+10]-'0';
      pair<int,int> convertStart= hexToCartesian(xStart, yStart, rows);
      pair<int,int> convertEnd = hexToCartesian(x,y,rows);
      cout << convertStart.first << " " << convertStart.second << endl;
      cout << convertEnd.first << " " << convertEnd.second << endl;
      moveRing(playerNo,convertStart.first, convertStart.second, convertEnd.first, convertEnd.second);
      pointer += 12;
    }
    else if(s[pointer]=='X'){
      int x = s[pointer+2]-'0';
      int y = s[pointer+4]-'0';
      pair<int,int> coor = hexToCartesian(x,y,rows);
      removeRing(playerNo,coor.first,coor.second);
      pointer += 6;
    }
    else{
      cout << "Move is incorrect!!" << endl;
      return;
    }
  }
}

/*
*The iterative deepening version of minimax with 
*some max time and depth alloted
*/
lli ourPlayer::idMinimax(ourGame gameNode,int max_depth,double maxTime){
  int depth = 0;
  double tempTime=0;
  struct timespec start_time,move_time;
  htMap.clear();
  
  //start noting time
  clock_gettime(CLOCK_REALTIME, &start_time);
  lli bestScore=-INIFINITY;
  for(depth=0;depth<=max_depth;depth++){
    lli value = minimax(gameNode,depth,true,-INFINITY,INFINITY);
    //compute time to solve for depth 
    clock_gettime(CLOCK_REALTIME, &move_time);
    double seconds = (double)((move_time.tv_sec+move_time.tv_nsec*1e-9) - (double)(start_time.tv_sec+start_time.tv_nsec*1e-9));
    //return value if time exceeded
    if(seconds>=maxTime){
      htMap.clear();
      return value;
    }
  }
  htMap.clear();
  return value;
}


//initialize with alpha = -INFINITY & beta = INFINITY
long long int ourPlayer::minimax(ourGame gameNode,int depth,bool isMax,lli alpha,lli beta){


  if(depth==0){
    return gameNode.computeHeuristicValue();
  }

  lli bestScore;

  //if our player's turn
  if(isMax){
    bestScore=-INFINITY;
    vector<ourGame> childs;
    childVector = gameNode.children();//assuming children function returns an vector of possible gameNodes
    childVector = sortChildren(childVector,true);
    for(int i=0;i<childVector.size();i++){
      lli value = minimax(childVector[i],depth+1,false,alpha,beta);
      alpha = max(alpha,value);
      bestScore = max(value,bestScore);
      if(alpha>=beta){
        return value;
      }
    }
    return bestScore;
  }

  //if opponent's turn
  if(!isMax){
    bestScore=INFINITY;
    vector<ourGame> childVector;
    childVector = gameNode.children();//assuming children function returns an vector of possible gameNodes
    childVector = sortChildren(childVector,false);
    for(int i=0;i<childVector.size();i++){
      lli value = minimax(childVector[i],depth+1,true,alpha,beta);
      beta = min(beta,value);
      bestScore = min(value,bestScore);
      if(alpha>=beta){
        return value;
      }
    }
    return bestScore;
  }

}

/*
void ourPlayer::play(){
  string opponentMove;
  if(this->playerNumber==1){
    move m = this->game->getMove(this->playerNumber);
    this->game->execute_move(m);
    cout<<toString(m);
    //cin>>opponentMove;
  }
  cin>>opponentMove;
  while(!this->game->ended()){
    this->game->execute_move(toMove(opponentMove));
    move m = this->game->getMove(this->playerNumber);
    this->game->execute_move(m);
    cout<<toString(m);
    cin>>opponentMove;
  }
}
*/
