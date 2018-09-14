#ifndef OURPLAYER_H
#define	OURPLAYER_H

#include"ourGame.h"
#include <bits/stdc++.h>
/*
class ourPlayer{
private:
  int total_rings;
  int player_no;
  double time_left;

public:
  ourPlayer();
  void play();
  move placeRing();
  move selectRing();
  move moveRing();
  move removeRowStart();
  move removeRowEnd();
  move removeRing();

};
*/

using namespace std;

class ourPlayer{
  int playerNumber;//Gives the playerNumber to our BOT
  int totalRings;//Number of rings that needs to be placed on the board
  int timeLeft;//timeLeft for playing (in secs)
  int myRingsRemoved;
  vector < pair<int,int> > rings; // Length 5 vector which contains the position of the rings in the format (x,y)  (ie. axial Coordinates)
  vector < pair<int,int> > markers; //Contains the position of the markers in (x,y) format (ie. axial coordinates)
  ourGame* game;

public:
  ourPlayer(int a);
  struct move placeRing(int a, int b); //Returns the move which needs to be printed
  struct move moveRing(int a, int b, int c); //Returns the move which should be printed
  int selectRing(); //gives the index of the ring which is selected for moving
  move removeRing(int index);

};

//What's This??-> used as the ending syntax for the if condition applied above for not including the header file more than once
#endif
