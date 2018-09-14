
#include"ourPlayer.h"
#include"ourGame.h"
#include"utils.h"
#include <bits/stdc++.h> //Can we use this??

#define pb push_back
#define mp make_pair

using namespace std;

/*General Comments*/
// I think we should use the approach of double rows naming and use the approach of hexagon naming just for input and output, as that is only stored in the game which is used by Yinsh.js
/*GeneralComments over*/

//Assuming PLayer 0 moves first and PLayer 1 follows

//Constructor
ourPlayer::ourPlayer(int playerNumber){
  //cin>>player_no>>total_rings>>time_left;
  this->playerNumber =  playerNumber; //1-> Player 1, 2-> Player 2
  this->totalRings =  5; //This version only has to deal with 5 rings
  this->timeLeft =  120; //will be initialised with full time
}

//These hexagon and position will be decided by Alpha-Beta pruning
struct move ourPlayer::placeRing(int x, int y){
  //Hexagon defines the hexagon in which we will place the new rings
  //position defines the position of that new ring in the hexagon
  move temp;//it will be automatically initialized by {"",0,0}
  pair<int,int> p = getRadialFromAxial(x,y,11);//rows = 11 
  temp.type = "P";
  temp.hex = p.first; // hexagon
  temp.pos = p.second; //position
  rings.pb(mp(x,y));
  struct boardCell tempboardCell; // Ring is placed, and not marker
  tempboardCell.player = this->playerNumber;
  tempboardCell.containsMarker = false;
  tempboardCell.containsRings = true;
  game->board[x][y]  = tempboardCell;
  if(this->playerNumber==1){
    game->playerOneRingsOnBoard++;
  }
  else
    game->playerTwoRingsOnBoard++;

  return temp;
}

struct move ourPlayer::moveRing(int index, int x, int y){
  //index is the index in rings of the ring which we need to move_end, hexagon and position same  as above
  //We assume that index is never out of bounds and move to be moved is a valid move
  struct move temp;// = {"",0,0};

  pair<int,int> p = getRadialFromAxial(x,y,11);//rows = 11
  temp.type = "M";
  temp.hex = p.first; // hexagon
  temp.pos = p.second; //position
  //There will be a marker at original position
  pair<int,int> newMarkerPosition = rings[index];
  rings[index] = mp(x,y);
  struct boardCell tempBoardCellRing;// {this->playerNumber, true, false}; //Adding ring to the new positions
  tempBoardCellRing.player = this->playerNumber;
  tempBoardCellRing.containsRings = true;
  tempBoardCellRing.containsMarker = false;
  game->board[x][y] = tempBoardCellRing;
  //We can say that this position isn't occupied by any other marker, hence pushBack it
  markers.pb(newMarkerPosition);
  boardCell tempBoardCellMarker;// = {this->playerNumber,false,true}; // Marker is placed and not ring
  tempBoardCellMarker.player = this->playerNumber;
  tempBoardCellMarker.containsMarker = true;
  tempBoardCellMarker.containsRings = false;

  game->board[newMarkerPosition.first][newMarkerPosition.second] = tempBoardCellMarker;
  if(this->playerNumber==1)
    game->playerOneMarkersOnBoard++;
  else
    game->playerTwoMarkersOnBoard++;
  return temp;
}

struct move ourPlayer::removeRing(int index){
  //Remove the ring, removal of markers will be done separately
  pair<int,int> p= rings[index];
  pair<int,int> rad = getRadialFromAxial(p.first,p.second,11);//rows = 11
  struct move temp;// = {"",0,0};
  temp.type = "X";
  temp.hex = rad.first;
  temp.pos = rad.second;
  rings.erase(rings.begin()+index);
  if(this->playerNumber==1){
    game->playerOneRingsOnBoard--;
  }
  else
    game->playerTwoRingsOnBoard--;

  return temp;
}

