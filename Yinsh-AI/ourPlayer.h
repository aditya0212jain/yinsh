#ifndef OURPLAYER_H
#define	OURPLAYER_H

#include"ourGame.h"
#include <bits/stdc++.h>

#define lli long long int
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

struct transpositionNode{
  lli score;
  lli max_minus_depth;
}

class ourPlayer{
  int playerNumber;//Gives the playerNumber to our BOT
  int totalRings;//Number of rings that needs to be placed on the board
  int timeLeft;//timeLeft for playing (in secs)
  int myRingsRemoved;
  //vector < pair<int,int> > rings; // Length 5 vector which contains the position of the rings in the format (x,y)  (ie. axial Coordinates)
  //vector < pair<int,int> > markers; //Contains the position of the markers in (x,y) format (ie. axial coordinates)


public:
  ourGame* game;
  ourPlayer(int a, int b);
  unordered_map <string,transpositionNode> htMap;
  void placeRing(int num, int a, int b); //Returns the move which needs to be printed
  void moveRing(int num, int xs, int ys, int b, int c); //Returns the move which should be printed
  int selectRing(); //gives the index of the ring which is selected for moving
  void removeRing(int num, int x, int y);
  void removeRow(int num, int a, int b, int c, int d);
  void moveDecider(int playerNo, string s);
  //void play();

  string minimax_decision(ourGame game);
  long long int minimax(ourGame gameNode,int depth,bool isMax,long long int alpha,long long int beta);
  long long int idMinimax(ourGame gameNode,int max_depth,double maxTime);
  vector<ourGame> sortChildren(vector<ourGame> childNodes,bool forMax);

};

//What's This??-> used as the ending syntax for the if condition applied above for not including the header file more than once
#endif
