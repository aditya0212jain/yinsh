
#include"ourPlayer.h"
#include"ourGame.h"
#include"utils.h"
#include <bits/stdc++.h> //Can we use this??
#include<math.h>
#include <random>

#define pb push_back
#define mp make_pair
#define lli long long int
int totalNodes=0;

using namespace std;

/*General Comments*/
// I think we should use the approach of double rows naming and use the approach of hexagon naming just for input and output, as that is only stored in the game which is used by Yinsh.js
/*GeneralComments over*/


bool compareForMax(const transitionMove &a, const transitionMove &b){
    return a.value>b.value;
}

bool compareForMin(const transitionMove &a,const transitionMove &b){
  return a.value<b.value;
}

vector<string> ourPlayer::sortChildren(vector<string> moves,bool forMax){
  vector<transitionMove> v;
  int playerNumber;
  if(forMax){
    playerNumber = this->playerNumber;
  }else{
    playerNumber = (this->playerNumber==1) ? 2:1;
  }
  for(int i=0;i<moves.size();i++){
    if(htMap.find(moves[i])==htMap.end()){

    moveDecider(playerNumber,moves[i],this->game);
    lli valueTemp = this->game->computeHeuristicValue(this->playerNumber);
    transitionMove temp;
    temp.move = moves[i];
    temp.value = valueTemp;
    v.push_back(temp);
    htMap[moves[i]]=valueTemp;
    this->game->moveUndo(playerNumber,moves[i]);
    }else{
      transitionMove temp;
      temp.value = htMap[moves[i]];
      temp.move = moves[i];
      v.push_back(temp);
    }
  }
  // for(int i=0;i<moves.size();i++){
  // moveDecider(playerNumber,moves[i],this->game);
  //   lli valueTemp = this->game->computeHeuristicValue(this->playerNumber);
  //   transitionMove temp;
  //   temp.move = moves[i];
  //   temp.value = valueTemp;
  //   v.push_back(temp);
  //   // htMap[moves[i]]=valueTemp;
  //   this->game->moveUndo(playerNumber,moves[i]);
  // }
  if(forMax){
    sort(v.begin(),v.end(),compareForMax);
  }else{
    sort(v.begin(),v.end(),compareForMin);
  }
  vector<string> ans;
  for(int i=0;i<v.size();i++){
    ans.push_back(v[i].move);
  }
  return ans;
}

//Assuming PLayer 0 moves first and PLayer 1 follows

ourPlayer::ourPlayer(int playerNumber,int timeLeft,int numberOfRings,int markersNeededToRemove){
  int boardSize = numberOfRings;
  this->playerNumber =  playerNumber; //1-> Player 1, 2-> Player 2
  this->totalRings =  numberOfRings; //This version only has to deal with 5 rings
  this->timeLeft =  timeLeft; //will be initialised with full time
  this->myRingsRemoved = 0;//starting with 0 rings
  this->game = new ourGame(boardSize);
  this->markersNeededToRemove = markersNeededToRemove;
}

//These hexagon and position will be decided by Alpha-Beta pruning
void ourPlayer::placeRing(int playerNo, int x, int y, ourGame* game){

  struct boardCell tempboardCell; // Ring is placed, and not marker
  tempboardCell.player = playerNo;//this->playerNumber;
  tempboardCell.containsMarker = false;
  tempboardCell.containsRings = true;
  tempboardCell.canBeUsed = true;
  game->board[x][y]  = tempboardCell;
  if(playerNo==1){
    game->playerOneRingsOnBoard++;
    game->playerOneRingPositions.push_back(make_pair(x,y));
  }
  else{
    game->playerTwoRingsOnBoard++;
    game->playerTwoRingPositions.push_back(make_pair(x,y));
  }
}

void ourPlayer::addMarker(int playerNo, int hex, int pos, ourGame* game){
  pair<int,int> cartesian = hexToCartesian(hex,pos,2*totalRings + 1);//11 or 13 as per board size @3Nov
  int x = cartesian.first;
  int y = cartesian.second;
  struct boardCell tempboardCell; // Ring is placed, and not marker
  tempboardCell.player = playerNo;//this->playerNumber;
  tempboardCell.containsMarker = true;
  tempboardCell.containsRings = false;
  tempboardCell.canBeUsed = true;
  game->board[x][y]  = tempboardCell;
  // game->allMarkerPosition.push_back(mp(x,y));
  if(playerNo==1){
    game->playerOneMarkersOnBoard++;
    // game->playerOneMarkerPosition.push_back(mp(x,y));
  }
  else{
    game->playerTwoMarkersOnBoard++;
    // game->playerTwoMarkerPosition.push_back(mp(x,y));
  }
}

void ourPlayer::moveRing(int playerNo,int xStart, int yStart, int x, int y, ourGame* game){
  //index is the index in rings of the ring which we need to move_end, hexagon and position same  as above
  //We assume that index is never out of bounds and move to be moved is a valid move
  struct move temp;// = {"",0,0};

  //There will be a marker at original position

  pair<int,int> newMarkerPosition = mp(xStart,yStart);
  if(!(game->board[xStart][yStart]).containsRings){
    return ;
  }
  struct boardCell tempBoardCellRing;// {this->playerNumber, true, false}; //Adding ring to the new positions
  tempBoardCellRing.player = playerNo;//this->playerNumber;
  tempBoardCellRing.containsRings = true;
  tempBoardCellRing.containsMarker = false;
  tempBoardCellRing.canBeUsed = true;
  game->board[x][y] = tempBoardCellRing;
  //We can say that this position isn't occupied by any other marker, hence pushBack it
  boardCell tempBoardCellMarker;// = {this->playerNumber,false,true}; // Marker is placed and not ring
  tempBoardCellMarker.player = playerNo;//this->playerNumber;
  tempBoardCellMarker.containsMarker = true;
  tempBoardCellMarker.containsRings = false;
  tempBoardCellMarker.canBeUsed = true;



  game->board[newMarkerPosition.first][newMarkerPosition.second] = tempBoardCellMarker;
  inverseMarker(playerNo,xStart,yStart,x,y,game);
  // game->allMarkerPosition.push_back(mp(xStart,yStart));
  if(playerNo==1){
    game->playerOneMarkersOnBoard++;
    auto it = find(game->playerOneRingPositions.begin(),game->playerOneRingPositions.end(),mp(xStart,yStart));
    if(it!=game->playerOneRingPositions.end())
      *it = mp(x,y);
    // game->playerOneMarkerPosition.push_back(mp(xStart,yStart));
  }
  else{
    game->playerTwoMarkersOnBoard++;
    auto it = find(game->playerTwoRingPositions.begin(),game->playerTwoRingPositions.end(),mp(xStart,yStart));
    if(it!=game->playerTwoRingPositions.end())
      *it = mp(x,y);
    // game->playerTwoMarkerPosition.push_back(mp(xStart,yStart));
  }
}

void ourPlayer::removeRing(int playerNo, int x, int y, ourGame* game){
  //Remove the ring, removal of markers will be done separately
  boardCell tempBoard = game->board[x][y];
  // pair<int,int> rad = cartesianToHex(x,y,11);//rows = 11
  // struct move temp;// = {"",0,0};
  // temp.type = "X";
  // temp.hex = rad.first;
  // temp.pos = rad.second;
  boardCell tempBoardCell;// = {this->playerNumber,false,true}; // Marker is placed and not ring
  tempBoardCell.player = 0;
  tempBoardCell.containsMarker = false;
  tempBoardCell.containsRings = false;
  tempBoardCell.canBeUsed = true;
  game->board[x][y] = tempBoardCell;
  if(playerNo==1){
    game->playerOneRingsOnBoard--;
    auto it = find(game->playerOneRingPositions.begin(),game->playerOneRingPositions.end(),make_pair(x,y));
    if(it!=game->playerOneRingPositions.end())
      game->playerOneRingPositions.erase(it);
  }
  else{
    game->playerTwoRingsOnBoard--;
    auto it = find(game->playerTwoRingPositions.begin(),game->playerTwoRingPositions.end(),make_pair(x,y));
    if(it!=game->playerTwoRingPositions.end())
      game->playerTwoRingPositions.erase(it);
  }
}

void ourPlayer::removeRow(int playerNo, int startX, int startY, int endX, int endY, ourGame* game){
  int movementX = endX - startX;
  int movementY = endY - startY;
  if(movementX==0){
    //Vertical move
    int mov = movementY/abs(movementY);
    for(int i = min(startY,endY); i<=max(endY,startY); i++){
      // auto it = find(game->allMarkerPosition.begin(),game->allMarkerPosition.end(),mp(startX,i));
      // if(it!=game->allMarkerPosition.end())
      //   game->allMarkerPosition.erase(it);
      //Removing marker position from vector
      // if(game->board[startX][i].player==1){
      //   auto it = find(game->playerOneMarkerPosition.begin(),game->playerOneMarkerPosition.end(),make_pair(startX,i));
      //   if(it!=game->playerOneMarkerPosition.end())
      //     game->playerOneMarkerPosition.erase(it);
      // }else{
      //   auto it = find(game->playerTwoMarkerPosition.begin(),game->playerTwoMarkerPosition.end(),make_pair(startX,i));
      //   if(it!=game->playerTwoMarkerPosition.end())
      //     game->playerTwoMarkerPosition.erase(it);
      // }
      //done removing
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
      // auto it = find(game->allMarkerPosition.begin(),game->allMarkerPosition.end(),mp(i,startY));
      // if(it!=game->allMarkerPosition.end())
      //   game->allMarkerPosition.erase(it);
      //Removing marker position from vector
      // if(game->board[i][startY].player==1){
      //   auto it = find(game->playerOneMarkerPosition.begin(),game->playerOneMarkerPosition.end(),make_pair(i,startY));
      //   if(it!=game->playerOneMarkerPosition.end())
      //     game->playerOneMarkerPosition.erase(it);
      // }else{
      //   auto it = find(game->playerTwoMarkerPosition.begin(),game->playerTwoMarkerPosition.end(),make_pair(i,startY));
      //   if(it!=game->playerTwoMarkerPosition.end())
      //     game->playerTwoMarkerPosition.erase(it);
      // }
      //done removing
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
      // auto it = find(game->allMarkerPosition.begin(),game->allMarkerPosition.end(),mp(i,j));
      // if(it!=game->allMarkerPosition.end())
      //   game->allMarkerPosition.erase(it);
      //Removing marker position from vector
      // if(game->board[i][j].player==1){
      //   auto it = find(game->playerOneMarkerPosition.begin(),game->playerOneMarkerPosition.end(),make_pair(i,j));
      //   if(it!=game->playerOneMarkerPosition.end())
      //     game->playerOneMarkerPosition.erase(it);
      // }else{
      //   auto it = find(game->playerTwoMarkerPosition.begin(),game->playerTwoMarkerPosition.end(),make_pair(i,j));
      //   if(it!=game->playerTwoMarkerPosition.end())
      //     game->playerTwoMarkerPosition.erase(it);
      // }
      //done removing
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
  if(playerNo==1){
    game->playerOneMarkersOnBoard = game->playerOneMarkersOnBoard-markersNeededToRemove;
  }
  else{
    game->playerTwoMarkersOnBoard = game->playerTwoMarkersOnBoard-markersNeededToRemove;
  }
}

void ourPlayer::inverseMarker(int playerNo, int startX, int startY, int endX, int endY, ourGame* game){
  int movementX = endX - startX;
  int movementY = endY - startY;
  if(movementX==0){
    //Vertical move
    int mov = movementY/abs(movementY);
    for(int i = min(startY,endY)+1; i<=max(endY,startY)-1; i++){
      boardCell tempBoardCell = game->board[startX][i];
      if(tempBoardCell.player!=0){
        tempBoardCell.player = 3 - tempBoardCell.player;
      }
      if(tempBoardCell.player==1){
        game->playerOneMarkersOnBoard++;
        game->playerTwoMarkersOnBoard--;
        // game->playerOneMarkerPosition.push_back(mp(startX,i));
        // auto it = find(game->playerTwoMarkerPosition.begin(),game->playerTwoMarkerPosition.end(),make_pair(startX,i));
        // if(it!=game->playerTwoMarkerPosition.end())
        //   game->playerTwoMarkerPosition.erase(it);
      }
      else{
        game->playerTwoMarkersOnBoard++;
        game->playerOneMarkersOnBoard--;
        // game->playerTwoMarkerPosition.push_back(mp(startX,i));
        // auto it = find(game->playerOneMarkerPosition.begin(),game->playerOneMarkerPosition.end(),make_pair(startX,i));
        // if(it!=game->playerOneMarkerPosition.end())
        //   game->playerOneMarkerPosition.erase(it);
      }
      game->board[startX][i] = tempBoardCell;
    }
  }
  else if(movementY==0){
    //Vertical move
    int mov = movementX/abs(movementX);
    for(int i = min(startX,endX)+1; i<=max(endX,startX)-1; i++){
      boardCell tempBoardCell = game->board[i][startY];
      if(tempBoardCell.player!=0){
        tempBoardCell.player = 3 - tempBoardCell.player;
      }
      if(tempBoardCell.player==1){
        game->playerOneMarkersOnBoard++;
        game->playerTwoMarkersOnBoard--;
        // game->playerOneMarkerPosition.push_back(mp(i,startY));
        // auto it = find(game->playerTwoMarkerPosition.begin(),game->playerTwoMarkerPosition.end(),make_pair(i,startY));
        // if(it!=game->playerTwoMarkerPosition.end())
        //   game->playerTwoMarkerPosition.erase(it);
      }
      else{
        game->playerTwoMarkersOnBoard++;
        game->playerOneMarkersOnBoard--;
        // game->playerTwoMarkerPosition.push_back(mp(i,startY));
        // auto it = find(game->playerOneMarkerPosition.begin(),game->playerOneMarkerPosition.end(),make_pair(i,startY));
        // if(it!=game->playerOneMarkerPosition.end())
        //   game->playerOneMarkerPosition.erase(it);
      }
      game->board[i][startY] = tempBoardCell;
    }
  }
  else if(movementX==movementY){
    int mov = movementX/abs(movementX);
    int i,j;
    for(i = min(startX,endX)+1,j=min(endY,startY)+1; i<=max(startX,endX)-1,j<=max(endY,startY)-1; i++,j++){
      boardCell tempBoardCell = game->board[i][j];
      if(tempBoardCell.player!=0){
        tempBoardCell.player = 3 - tempBoardCell.player;
      }
      if(tempBoardCell.player==1){
        game->playerOneMarkersOnBoard++;
        game->playerTwoMarkersOnBoard--;
        // game->playerOneMarkerPosition.push_back(mp(i,j));
        // auto it = find(game->playerTwoMarkerPosition.begin(),game->playerTwoMarkerPosition.end(),make_pair(i,j));
        // if(it!=game->playerTwoMarkerPosition.end())
        //   game->playerTwoMarkerPosition.erase(it);
      }
      else{
        game->playerTwoMarkersOnBoard++;
        game->playerOneMarkersOnBoard--;
        // game->playerTwoMarkerPosition.push_back(mp(i,j));
        // auto it = find(game->playerOneMarkerPosition.begin(),game->playerOneMarkerPosition.end(),make_pair(i,j));
        // if(it!=game->playerOneMarkerPosition.end())
        //   game->playerOneMarkerPosition.erase(it);
      }
      game->board[i][j] = tempBoardCell;
    }
  }
  else{
    cout << "Galat chal rahe ho, sahi Karo" << endl;
  }
}

void ourPlayer::moveDecider(int playerNo, string s, ourGame* game){
  int length = s.length();
  int pointer = 0;
  int rows = (game->rows);
  //Here the coordinates are in hex and pos form
  vector<string> p = split(s,' ');

  while(pointer<p.size()){
    if(p[pointer].compare("RS")==0){
      int startX = stoi(p[pointer+1]);
      int startY = stoi(p[pointer+2]);
      int endX = stoi(p[pointer+4]);
      int endY = stoi(p[pointer+5]);
      pair<int,int> convertStart= hexToCartesian(startX, startY, rows);
      pair<int,int> convertEnd = hexToCartesian(endX,endY,rows);
      removeRow(playerNo, convertStart.first, convertStart.second, convertEnd.first, convertEnd.second, game);
      pointer += 6;
    }
    else if(p[pointer].compare("P")==0){
      int x = stoi(p[pointer+1]);
      int y = stoi(p[pointer+2]);
      pair<int,int> coor = hexToCartesian(x,y,rows);
      //cout << coor.first << " " << coor.second << endl;
      placeRing(playerNo,coor.first,coor.second, game);
      pointer += 3;
    }
    else if(p[pointer].compare("S")==0){
      int xStart = stoi(p[pointer+1]);
      int yStart = stoi(p[pointer+2]);
      int x = stoi(p[pointer+4]);
      int y = stoi(p[pointer+5]);
      pair<int,int> convertStart= hexToCartesian(xStart, yStart, rows);
      pair<int,int> convertEnd = hexToCartesian(x,y,rows);
      //cout << convertStart.first << " " << convertStart.second << endl;
      //cout << convertEnd.first << " " << convertEnd.second << endl;
      moveRing(playerNo,convertStart.first, convertStart.second, convertEnd.first, convertEnd.second, game);
      pointer += 6;
    }
    else if(p[pointer].compare("X")==0){
      int x = stoi(p[pointer+1]);
      int y = stoi(p[pointer+2]);
      pair<int,int> coor = hexToCartesian(x,y,rows);
      removeRing(playerNo,coor.first,coor.second, game);
      pointer += 3;
    }
    else{
      cout << "Move is incorrect!!" << endl;
      return;
    }
  }
}

vector<pair<pair<int,int>,pair<int,int> > > ourPlayer::selectAndMoveHelper(int playerNo, int x, int y, int dirX, int dirY, ourGame* game){
  //x,y -> current position of the ring
  //dirX,dirY -> each can be 1,0,-1 denotes kaha check karna hai
  vector<pair<pair<int,int>,pair<int,int> > > ans; //first is the current position of the ring, second is the position in which it can travel
  int i,j;
  pair<int,int> f = mp(x,y);
  int oneMarker=0;
  int rows = game->rows;
  for(i=x+dirX,j=y+dirY;;i=i+dirX,j=j+dirY){
    if(i<0 || j<0 || i>=rows || j>=rows){
      return ans;
    }
    else if(!(game->board[i][j].canBeUsed)){
      //cout << ans.size() << endl;
      //cout << "At this value I came here: " << i << " " << j << endl;
      return ans;
    }
    else if(game->board[i][j].containsRings){
      return ans;
    }
    else if(game->board[i][j].containsMarker && oneMarker==0){
        oneMarker=1;
        //ans.pb(mp(f,mp(i,j)));
    }
    else if(game->board[i][j].containsMarker && oneMarker==1){
      //Nothing
    }
    else if(game->board[i][j].player==0){
      if(oneMarker==1){
        ans.pb(mp(f,mp(i,j)));
        return ans;
      }
      else{
        //cout << i << " " << j << endl;
        ans.pb(mp(f,mp(i,j)));
      }
    }
  }
}

vector<pair<pair<int,int>,pair<int,int> > > ourPlayer::selectAndMove(int playerNo,ourGame* game){
  vector<pair<pair<int,int>,pair<int,int> > > ans;
  vector<pair<pair<int,int>,pair<int,int> > > temp;

  if(playerNo==1){
    for(int p=0;p<game->playerOneRingPositions.size();p++){
      int i = game->playerOneRingPositions[p].first;
      int j = game->playerOneRingPositions[p].second;

      if(game->board[i][j].containsRings){
        if(game->board[i][j].player==playerNo){
           temp = selectAndMoveHelper(playerNo,i,j,0,1,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());
           temp = selectAndMoveHelper(playerNo,i,j,0,-1,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());
           temp = selectAndMoveHelper(playerNo,i,j,1,1,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());
           temp = selectAndMoveHelper(playerNo,i,j,-1,-1,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());
           temp = selectAndMoveHelper(playerNo,i,j,1,0,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());

           temp = selectAndMoveHelper(playerNo,i,j,-1,0,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());

        }
      }
    }
  }
  else{
    for(int p=0;p<game->playerTwoRingPositions.size();p++){
      int i = game->playerTwoRingPositions[p].first;
      int j = game->playerTwoRingPositions[p].second;

      if(game->board[i][j].containsRings){
        if(game->board[i][j].player==playerNo){
           temp = selectAndMoveHelper(playerNo,i,j,0,1,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());
           temp = selectAndMoveHelper(playerNo,i,j,0,-1,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());
           temp = selectAndMoveHelper(playerNo,i,j,1,1,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());
           temp = selectAndMoveHelper(playerNo,i,j,-1,-1,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());
           temp = selectAndMoveHelper(playerNo,i,j,1,0,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());

           temp = selectAndMoveHelper(playerNo,i,j,-1,0,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());

        }
      }
    }

  }
  return ans;
}

string ourPlayer::markerDeletionHelper(int playerNo, int x, int y, int dirX, int dirY,ourGame* game){
  //vetor<string> ans;
  string temp="";
  // cout << "Board[10][10]:" << game->board[10][10].canBeUsed << endl;
  int markerCount=0;
  int i,j;
  int rows = game->rows;
  for(i=x,j=y;;i=i+dirX,j=j+dirY){
    if(i<0 || j<0 || i>=rows || j>=rows){
      break;
    }
    // cout << "i=" << i << " j=" << j << endl;
    if(game->board[i][j].containsMarker && game->board[i][j].canBeUsed){
      if(game->board[i][j].player==playerNo){
        markerCount++;
      }
      else{
        break;
      }
      if(markerCount == markersNeededToRemove){//To edit here for removal of more markers @3Nov
        pair<int,int> initial = cartesianToHex(x,y,rows);
        pair<int,int> finall = cartesianToHex(i,j,rows);
        string result;
        ostringstream convert;
        convert << initial.first;
        result = convert.str();

        string result1;
        ostringstream convert1;
        convert1 << initial.second;
        result1 = convert1.str();

        string result2;
        ostringstream convert2;
        convert2 << finall.first;
        result2 = convert2.str();

        string result3;
        ostringstream convert3;
        convert3 << finall.second;
        result3 = convert3.str();

        temp = "RS " + result + " " + result1 + " RE " + result2 + " " + result3;
        break;
      }
    }
    else{
      break;
    }
  }
  // cout << "MarkerCount=" << markerCount << endl;
  return temp;
}

vector<string> ourPlayer::markerDeletion(int playerNo, ourGame* game){
  int rows = game->rows;
  vector<string> ans;
  string temp = "";

  /*
  Below stored positions were used
  */

  // if(playerNo==1){
  //   for(int k=0;k<game->playerOneMarkerPosition.size();k++){
  //     pair<int,int> tp = game->playerOneMarkerPosition[k];
  //     int i = tp.first;
  //     int j = tp.second;
  //     if(game->board[i][j].containsMarker && game->board[i][j].canBeUsed && game->board[i][j].player ==playerNo){
  //       // cout << "i=" << i << " j=" << j << endl;
  //       temp = markerDeletionHelper(playerNo,i,j,0,1,game);//Total 6 directions
  //       if(temp.length()!=0){
  //         ans.pb(temp);
  //       }
  //       // cout << "a" << endl;
  //       // cout << ans.size() << " ";
  //       temp = markerDeletionHelper(playerNo,i,j,0,-1,game);//Total 6 directions
  //       if(temp.length()!=0){
  //         ans.pb(temp);
  //       }
  //       // cout << "b" << endl;
  //       // cout << ans.size() << " ";

  //       temp = markerDeletionHelper(playerNo,i,j,1,1,game);//Total 6 directions
  //       if(temp.length()!=0){
  //         ans.pb(temp);
  //       }
  //       // cout << ans.size() << " ";
  //       // cout << "c" << endl;
  //       temp = markerDeletionHelper(playerNo,i,j,-1,-1,game);//Total 6 directions
  //       if(temp.length()!=0){
  //         ans.pb(temp);
  //       }
  //       // cout << ans.size() << " ";

  //       temp = markerDeletionHelper(playerNo,i,j,1,0,game);//Total 6 directions
  //       if(temp.length()!=0){
  //         ans.pb(temp);
  //       }
  //       // cout << ans.size() << " ";

  //       temp = markerDeletionHelper(playerNo,i,j,-1,0,game);//Total 6 directions
  //       if(temp.length()!=0){
  //         ans.pb(temp);
  //       }
  //       // cout << ans.size();
  //     }
  //   }
  //   return ans;
  // }else{
  //   for(int k=0;k<game->playerTwoMarkerPosition.size();k++){
  //     pair<int,int> tp = game->playerTwoMarkerPosition[k];
  //     int i = tp.first;
  //     int j = tp.second;
  //     if(game->board[i][j].containsMarker && game->board[i][j].canBeUsed && game->board[i][j].player ==playerNo){
  //       temp = markerDeletionHelper(playerNo,i,j,0,1,game);//Total 6 directions
  //       if(temp.length()!=0)
  //         ans.pb(temp);
  //       temp = markerDeletionHelper(playerNo,i,j,0,-1,game);//Total 6 directions
  //       if(temp.length()!=0)
  //         ans.pb(temp);
  //       temp = markerDeletionHelper(playerNo,i,j,1,1,game);//Total 6 directions
  //       if(temp.length()!=0)
  //         ans.pb(temp);
  //       temp = markerDeletionHelper(playerNo,i,j,-1,-1,game);//Total 6 directions
  //       if(temp.length()!=0)
  //         ans.pb(temp);
  //       temp = markerDeletionHelper(playerNo,i,j,1,0,game);//Total 6 directions
  //       if(temp.length()!=0)
  //         ans.pb(temp);
  //       temp = markerDeletionHelper(playerNo,i,j,-1,0,game);//Total 6 directions
  //       if(temp.length()!=0)
  //         ans.pb(temp);
  //     }
  //   }
  //   return ans;
  // }

  // for(int k=0;k<game->allMarkerPosition.size();k++){
  //   pair<int,int> tp = game->allMarkerPosition[k];
  //   int i = tp.first;
  //   int j = tp.second;
  //   if(game->board[i][j].containsMarker && game->board[i][j].canBeUsed && game->board[i][j].player ==playerNo){
  //       // cout << "i=" << i << " j=" << j << endl;
  //       temp = markerDeletionHelper(playerNo,i,j,0,1,game);//Total 6 directions
  //       if(temp.length()!=0){
  //         ans.pb(temp);
  //       }
  //       // cout << "a" << endl;
  //       // cout << ans.size() << " ";
  //       temp = markerDeletionHelper(playerNo,i,j,0,-1,game);//Total 6 directions
  //       if(temp.length()!=0){
  //         ans.pb(temp);
  //       }
  //       // cout << "b" << endl;
  //       // cout << ans.size() << " ";

  //       temp = markerDeletionHelper(playerNo,i,j,1,1,game);//Total 6 directions
  //       if(temp.length()!=0){
  //         ans.pb(temp);
  //       }
  //       // cout << ans.size() << " ";
  //       // cout << "c" << endl;
  //       temp = markerDeletionHelper(playerNo,i,j,-1,-1,game);//Total 6 directions
  //       if(temp.length()!=0){
  //         ans.pb(temp);
  //       }
  //       // cout << ans.size() << " ";

  //       temp = markerDeletionHelper(playerNo,i,j,1,0,game);//Total 6 directions
  //       if(temp.length()!=0){
  //         ans.pb(temp);
  //       }
  //       // cout << ans.size() << " ";

  //       temp = markerDeletionHelper(playerNo,i,j,-1,0,game);//Total 6 directions
  //       if(temp.length()!=0){
  //         ans.pb(temp);
  //       }
  //       // cout << ans.size();
  //     }
  // }

  for(int i=0; i<rows; i++){
    for(int j=0; j<rows; j++){
      // cout << i << " " << j << " " ;
      if(game->board[i][j].containsMarker && game->board[i][j].canBeUsed && game->board[i][j].player ==playerNo){
        // cout << "i=" << i << " j=" << j << endl;
        temp = markerDeletionHelper(playerNo,i,j,0,1,game);//Total 6 directions
        if(temp.length()!=0){
          ans.pb(temp);
        }
        // cout << "a" << endl;
        // cout << ans.size() << " ";
        // temp = markerDeletionHelper(playerNo,i,j,0,-1,game);//Total 6 directions
        // if(temp.length()!=0){
        //   ans.pb(temp);
        // }
        // cout << "b" << endl;
        // cout << ans.size() << " ";

        temp = markerDeletionHelper(playerNo,i,j,1,1,game);//Total 6 directions
        if(temp.length()!=0){
          ans.pb(temp);
        }
        // cout << ans.size() << " ";
        // cout << "c" << endl;
        // temp = markerDeletionHelper(playerNo,i,j,-1,-1,game);//Total 6 directions
        // if(temp.length()!=0){
        //   ans.pb(temp);
        // }
        // cout << ans.size() << " ";

        temp = markerDeletionHelper(playerNo,i,j,1,0,game);//Total 6 directions
        if(temp.length()!=0){
          ans.pb(temp);
        }
        // cout << ans.size() << " ";

        // temp = markerDeletionHelper(playerNo,i,j,-1,0,game);//Total 6 directions
        // if(temp.length()!=0){
        //   ans.pb(temp);
        // }
        // cout << ans.size();
      }
      // cout << endl;
    }
  }
  // cout <<"-----------------------------" << endl;
  // for(int i=0; i<ans.size(); i++)
    // cout << ans[i] << endl;
  // cout << "++++++++++++++++++++++++++" << endl;
  return ans;

}

vector<string> ourPlayer::selectAndMoveFinal(int playerNo, ourGame* game){
  vector<string> ans;
  vector<pair<pair<int,int>, pair<int,int> > > list = selectAndMove(playerNo, game);
  string temp="";
  int rows = game->rows;

  for(int i=0; i<list.size(); i++){
    int s1 = list[i].first.first;
    int s2 = list[i].first.second;
    int m1 = list[i].second.first;
    int m2 = list[i].second.second;
    pair<int,int> initial = cartesianToHex(s1,s2,rows);
    pair<int,int> finall = cartesianToHex(m1,m2,rows);

    string result;
    ostringstream convert;
    convert << initial.first;
    result = convert.str();

    string result1;
    ostringstream convert1;
    convert1 << initial.second;
    result1 = convert1.str();

    string result2;
    ostringstream convert2;
    convert2 << finall.first;
    result2 = convert2.str();

    string result3;
    ostringstream convert3;
    convert3 << finall.second;
    result3 = convert3.str();

    temp = "S " + result + " " + result1 + " M " + result2 + " " + result3;
    ans.pb(temp);
  }
  return ans;
}

vector<string> ourPlayer::removeRingFinal(int playerNo, ourGame* game){
  int rows = game->rows;
  vector<string> ans;
  string temp;
  pair<int,int> initial;
  if(playerNo==1){
    for(int i=0;i<game->playerOneRingPositions.size();i++){
      initial = cartesianToHex(game->playerOneRingPositions[i].first,game->playerOneRingPositions[i].second,rows);
        string result;
        ostringstream convert;
        convert << initial.first;
        result = convert.str();

        string result1;
        ostringstream convert1;
        convert1 << initial.second;
        result1 = convert1.str();

        temp = "X " + result + " " + result1;
        ans.pb(temp);
    }
  }else{
    for(int i=0;i<game->playerTwoRingPositions.size();i++){
      initial = cartesianToHex(game->playerTwoRingPositions[i].first,game->playerTwoRingPositions[i].second,rows);
        string result;
        ostringstream convert;
        convert << initial.first;
        result = convert.str();

        string result1;
        ostringstream convert1;
        convert1 << initial.second;
        result1 = convert1.str();

        temp = "X " + result + " " + result1;
        ans.pb(temp);
    }
  }
  return ans;
}

vector<string> ourPlayer::removeMarkerAndRing(int playerNo, ourGame* game){
  //This function checks if we can remove the rings or not and returns a vector of strings
  vector<string> ans;
  vector<string> first = markerDeletion(playerNo, game);

  // cout << "And Here?" << endl;
  string temp;
  // cout << "removeMarkerAndRing STARTS!!" << endl;
  for(int i=0; i<first.size(); i++){
    string removeMarkerOne = first[i];
    vector<string> firstRing = removeRingFinal(playerNo, game);
    for(int j=0; j<firstRing.size(); j++){
      string t = removeMarkerOne + " " + firstRing[j];
      ans.pb(t);
    }
  }
  // cout << "removeMarkerAndRing ENDS!" << endl;
  return ans;
}

vector<string> ourPlayer::allDeletions(int playerNo, ourGame* game){
    vector<string> ans;
    vector<string> firstDeletion = removeMarkerAndRing(playerNo,game);
    bool flag1, flag2;
    if(firstDeletion.size()==0)
      return ans;
    else{
      for(int i=0; i<firstDeletion.size(); i++){
        flag1=false;
        string firstMove = firstDeletion[i];
        moveDecider(playerNo,firstMove,game);
        // cout << "Marker 2" << endl;
        if((playerNo==1 && game->playerOneRingsOnBoard==totalRings-3)||(playerNo==2 && game->playerTwoRingsOnBoard==totalRings-3)){
          ans.pb(firstMove);
          game->moveUndo(playerNo,firstMove);
          flag1 = true;
        }
        else{
          vector<string> secondDeletion = removeMarkerAndRing(playerNo,game);
          if(secondDeletion.size()==0){
            ans.pb(firstMove);
            game->moveUndo(playerNo,firstMove);
            flag1=true;
          }
          else{
            for(int j=0; j<secondDeletion.size(); j++){
              flag2=false;
              string secondMove = secondDeletion[j];
              // ourGame* secondGame = new ourGame();
              // secondGame->copyTheBoard(firstGame);
              moveDecider(playerNo,secondMove,game);
              if((playerNo==1 && game->playerOneRingsOnBoard==totalRings-3)||(playerNo==2 && game->playerTwoRingsOnBoard==totalRings-3)){
                string temp = firstMove + " " + secondMove;
                ans.pb(temp);
                game->moveUndo(playerNo,secondMove);
                flag2=true;
              }
              else{
                // vector<string> thirdDeletion = removeMarkerAndRing(playerNo, secondGame);///<- is this correct @sarthakVishnoi
                vector<string> thirdDeletion = removeMarkerAndRing(playerNo,game);
                if(thirdDeletion.size()==0){
                  string temp = firstMove + " " + secondMove;
                  ans.pb(temp);
                  game->moveUndo(playerNo,secondMove);
                  flag2=true;
                }
                else{
                  for(int k=0; k<thirdDeletion.size(); k++){
                    string thirdMove = thirdDeletion[k];
                    // ourGame* thirdGame = new ourGame();
                    // thirdGame-> copyTheBoard(secondGame);
                    moveDecider(playerNo,thirdMove,game);
                    string tempp = firstMove+ " " + secondMove + " " + thirdMove;
                    ans.pb(tempp);
                    game->moveUndo(playerNo,thirdMove);
                  }
                }
              }
              if(!flag2){
                game->moveUndo(playerNo,secondMove);
              }
            }
          }
        }
        if(!flag1){
          game->moveUndo(playerNo,firstMove);
        }
      }
    }
    return ans;
}

vector<string> ourPlayer::moveList(int playerNo, ourGame* game){
  //Returns a list of moves
  vector<string> ans;
  vector<string> firstRound = allDeletions(playerNo, game);
  // cout << "Do I Reach here?" << endl;
  // cout << "First Move, first Round: " << firstRound.size() << endl;
  if(firstRound.size()==0){
    //Nothing to delete
    vector<string> fr = selectAndMoveFinal(playerNo, game);
    int frSize = fr.size();
    if(fr.size()==0){
      // cout << "NO MOVE LEFT" << endl;
    }
    else{
      // no deletions but move
      for(int i=0; i<frSize; i++){
        string firstMove = fr[i];
        moveDecider(playerNo, firstMove, game);

        vector<string> dr = allDeletions(playerNo, game);
        game->moveUndo(playerNo,firstMove);
        if(dr.size()==0){
          //move and then no deletion
          ans.pb(firstMove);
        }
        else{
          //move and then delete
          for(int j=0; j<dr.size(); j++){
            string temp = firstMove + " " + dr[j];
            ans.pb(temp);
          }
        }
      }
    }
  }
  else{
    //There are deletions possible
    for(int i=0; i<firstRound.size(); i++){
      string fm = firstRound[i];
      moveDecider(playerNo, fm, game);
      vector<string> sr = selectAndMoveFinal(playerNo,game);
      if(sr.size()==0){
        //No move to play ring
        ans.pb(fm);
      }
      else{
        //delete then move
        for(int j=0; j<sr.size(); j++){
          string sm = sr[j];
          moveDecider(playerNo, sm, game);
          vector<string> tr = allDeletions(playerNo, game);
          game->moveUndo(playerNo,sm);
          if(tr.size()==0){
            //Nothing to delete
            string temp = fm + " " + sm;
            ans.pb(temp);
          }
          else{
            //delete move and then delete
            for(int k=0; k<tr.size(); k++){
              string temp = fm + " " + sm + " " + tr[k];
              ans.pb(temp);
            }
          }
        }
      }
      game->moveUndo(playerNo,fm);
    }
  }
  return ans;
}

/*
*The iterative deepening version of minimax with
*some max time and depth alloted
*/
struct transitionMove ourPlayer::idMinimax(int max_depth,double maxTime){
  int depth = 0;
  double tempTime=0;
  htMap.clear();
  totalNodes=0;
  //start noting time
  struct transitionMove bestMove;
  struct transitionMove tempMove;
  bestMove.value=-INFINITY;
  double timeRemaining= this->timeLeft-maxTime;
  if(timeRemaining<=5){
    max_depth = 2;
  }
  for(depth=1;depth<=max_depth;depth++){
    tempMove = minimax(0,true,-INFINITY,INFINITY,depth);
    if(bestMove.value<=tempMove.value){
      bestMove = tempMove;
    }
    cerr<<"totalNodes: "<<totalNodes<<endl;
    if(totalNodes>5000){
      return bestMove;
      break;
    }
    // bestScore = max(bestScore,tempMove.value);
    // if(totalNodes<2000&&depth==3){
    //   if(timeRemaining>20){
    //     cerr<<"Doing 4 depth"<<endl;
    //     max_depth=4;
    //   }
    // }
  }
  htMap.clear();
  return tempMove;//initially it was tempMove
}

/*
*Implementing this assuming undo function is already made
*undoMove(ourGame game)
*/
//initialize with alpha = -INFINITY & beta = INFINITY
struct transitionMove ourPlayer::minimax(int depth,bool isMax,long long int alpha,long long int beta,int max_depth){
  totalNodes++;
  //depth will never be zero
  if(depth==max_depth){
    struct transitionMove ans;
    ans.move="Reached";
    int myRings,opponentRings;
    ans.value=this->game->computeHeuristicValue(this->playerNumber);
    return ans;
  }

  transitionMove bestMove;

  //if our player's turn
  if(isMax){
    // cout<<"max"<<endl;
    bestMove.value=-INFINITY;
    vector<string> possible_moves;
    possible_moves = moveList(this->playerNumber,this->game);

    //Commenting Sorting for a while
    possible_moves = sortChildren(possible_moves,true);

    for(int i=0;i<possible_moves.size();i++){

      moveDecider(this->playerNumber,possible_moves[i],this->game);
      transitionMove tempMove = minimax(depth+1,false,alpha,beta,max_depth);
      htMap[possible_moves[i]]=tempMove.value;
      if(alpha<=tempMove.value){
        alpha = tempMove.value;
      }
      if(bestMove.value<=tempMove.value){
        bestMove = tempMove;
        bestMove.move = possible_moves[i];
      }
      this->game->moveUndo(this->playerNumber,possible_moves[i]);
      if(alpha>=beta){
        tempMove.move = possible_moves[i];
        return tempMove;
      }
    }
    return bestMove;
  }

  //if opponent's turn
  if(!isMax){
    // cout<<"min1"<<endl;
    int opponent_player_number;
    opponent_player_number = (this->playerNumber==1) ? 2 : 1;
    bestMove.value = INFINITY;
    vector<string> possible_moves;
    possible_moves = moveList(opponent_player_number,this->game);
    possible_moves = sortChildren(possible_moves,false);
    // cout<<possible_moves[2]<<endl;
    for(int i=0;i<possible_moves.size();i++){
      moveDecider(opponent_player_number,possible_moves[i],this->game);
      transitionMove tempMove= minimax(depth+1,true,alpha,beta,max_depth);
      htMap[possible_moves[i]]=tempMove.value;
      // beta = min(beta,value);
      if(beta>=tempMove.value){
        beta = tempMove.value;
      }
      if(bestMove.value>=tempMove.value){
        bestMove = tempMove;
        bestMove.move = possible_moves[i];
      }
      // cout<<"undo move opponent"<<endl;
      this->game->moveUndo(opponent_player_number,possible_moves[i]);
      // cout<<"undo done"<<endl;
      if(alpha>=beta){
        tempMove.move = possible_moves[i];
        return tempMove;
      }
    }
    return bestMove;
  }

}

void ourPlayer::initialPlacing(){
  int width = 2 * this->totalRings + 1;
  int count1=0,count2=0;
  string oppMove;
  if(this->playerNumber==2){
   getline(cin,oppMove);
   this->moveDecider(1,oppMove,this->game);
   count2++;
  }
  cerr << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
  cerr << "NUmber of Rings= " << this->totalRings << endl;
  while(count1!=this->totalRings||count2!=this->totalRings){
    while(true){
      if(this->game->board[this->totalRings][this->totalRings].player==0){
        string beta = "P 0 0";
        cout<<beta<<endl;
        this->moveDecider(this->playerNumber,beta,this->game);
        count1++;
        break;
      }

      // //placing 4th ring
      if(count1==3){
        int a = 5;
        bool assigned=false;
        //checking if already opponent ring is there
        for(int b=1;b<(6*a);b++){
          pair<int,int> h = hexToCartesian(a,b,width);
          if(this->game->board[h.first][h.second].canBeUsed&&this->game->board[h.first][h.second].player==((this->playerNumber%2)+1)){
            pair<int,int> h1 = hexToCartesian(a,b+1,width);
            pair<int,int> h2 = hexToCartesian(a,b-1,width);
            if(this->game->board[h1.first][h1.second].canBeUsed&&this->game->board[h1.first][h1.second].player==0){
              string beta = "P ";
              beta = beta+to_string(a)+" "+to_string(b+1);
              cout<<beta<<endl;
              this->moveDecider(this->playerNumber,beta,this->game);
              count1++;
              assigned = true;
              break;
            }else if(this->game->board[h2.first][h2.second].canBeUsed&&this->game->board[h2.first][h2.second].player==0){
              string beta = "P ";
              beta = beta+to_string(a)+" "+to_string(b-1);
              cout<<beta<<endl;
              this->moveDecider(this->playerNumber,beta,this->game);
              count1++;
              assigned = true;
              break;
            }

          }
        }
        if(assigned){
          break;
        }else{
          //assigning random from the edges
          bool smalloop=true;
          while(smalloop){
            int b = rand()%(6*a);
            pair<int,int> h = hexToCartesian(a,b,width);
            if(this->game->board[h.first][h.second].canBeUsed&&this->game->board[h.first][h.second].player==0){
              string beta = "P ";
              beta = beta+to_string(a)+" "+to_string(b);
              cout<<beta<<endl;
              this->moveDecider(this->playerNumber,beta,this->game);
              count1++;
              assigned = true;
              break;
            }
          }
          break;
        }

      }
      if(count1==4){
        int a =5;
        bool assigned=false;
        for(int b=0;b<(6*a);b++){
          pair<int,int> h = hexToCartesian(a,b,width);
          if(this->game->board[h.first][h.second].canBeUsed&&this->game->board[h.first][h.second].player==this->playerNumber){
            int c = b/a;
            int d = (rand()%3)+1 + (4*c);
            pair<int,int > h1 = hexToCartesian(4,d,width);
            if(this->game->board[h1.first][h1.second].player==0){
              string beta = "P ";
              beta = beta+to_string(4)+" "+to_string(d);
              cout<<beta<<endl;
              this->moveDecider(this->playerNumber,beta,this->game);
              count1++;
              assigned = true;
              break;
            }else{
              break;
            }
          }
        }
        if(assigned){
          break;
        }
      }
      int a = rand()%3;
      int b;
      if(a==0){
        b=0;
      }else{
        b = rand()%(6*a);
      }
      // cout<<"a,b: "<<a<<" "<<b<<endl;
      pair<int,int> h = hexToCartesian(a,b,width);
      // cout<<"h "<<h.first<<" "<<h.second<<endl;
      if(this->game->board[h.first][h.second].player==0){
        string beta = "P ";
        beta = beta+to_string(a)+" "+to_string(b);
        cout<<beta<<endl;
        this->moveDecider(this->playerNumber,beta,this->game);
        count1++;
        break;
      }
    }
    if(count1==this->totalRings&&count2==this->totalRings){
      break;
    }
    getline(cin,oppMove);
    this->moveDecider((this->playerNumber%2)+1,oppMove,this->game);
    // cout<<"haggu"<<endl;
    count2++;
  }
}


void ourPlayer::play(){
  string opponentMove;
  int opponent_player_number;
  int count=0;

  this->initialPlacing();
  double seconds;
  clock_t t1,t2;
  t1 = clock();
  if(this->playerNumber==1){
    // if(this->game->pla)
    t2 = clock();
    seconds = t2-t1;
    seconds = seconds/CLOCKS_PER_SEC;
    transitionMove m;
    if(this->timeLeft > 20 && this->timeLeft < 147){
      m = idMinimax(3,seconds);
    }
    else{
      m = idMinimax(3,seconds);
    }
    // cout<<"o1"<<endl;
    cout<<m.move<<endl;
    // this->game->printBoard();
    moveDecider(this->playerNumber,m.move,this->game);
    // this->game->printBoard();
    //getline(cin,opponentMove);
  }
  getline(cin,opponentMove);
  opponent_player_number = (this->playerNumber==1) ? 2:1;
  while(!this->game->ended()){
    moveDecider(opponent_player_number,opponentMove,this->game);
    // this->game->printBoard();
    t2 = clock();
    seconds = t2-t1;
    seconds = seconds/CLOCKS_PER_SEC;
    cerr<<"time elapsed: "<<seconds<<endl;
    transitionMove m;
    if(this->timeLeft > 20 && this->timeLeft < 147){
      cerr << "I commmmmm here !!!!" << endl;
      m = idMinimax(3,seconds);
    }
    else{
      cerr << "I don't want to be hereeeeeeeeeeeeeeeee" << endl ;
      m = idMinimax(3,seconds);
    }

    cout<<m.move<<endl;
    moveDecider(this->playerNumber,m.move,this->game);
    // this->game->printBoard();

    getline(cin,opponentMove);
  }
}
