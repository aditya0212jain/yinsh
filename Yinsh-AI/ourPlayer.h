#ifndef OURPLAYER_H
#define	OURPLAYER_H

#include"ourGame.h"
#include <bits/stdc++.h>
#include <unordered_map>

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
};

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
  void placeRing(int num, int a, int b, ourGame* game); //Returns the move which needs to be printed
  void moveRing(int num, int xs, int ys, int b, int c, ourGame* game); //Returns the move which should be printed
  int selectRing(); //gives the index of the ring which is selected for moving
  void removeRing(int num, int x, int y, ourGame* game);
  void removeRow(int num, int a, int b, int c, int d, ourGame* game);
  void moveDecider(int playerNo, string s, ourGame* game);
  vector<pair<pair<int,int>,pair<int,int> > > selectAndMoveHelper(int playerNo, int x, int y, int dirX, int yDir, ourGame* game);
  vector<pair<pair<int,int>,pair<int,int> > > selectAndMove(int playerNo, ourGame* game);
  string markerDeletionHelper(int playerNo, int x, int y, int dirX, int dirY, ourGame* game);
  vector<string> markerDeletion(int playerNo, ourGame* game);
  vector<string> selectAndMoveFinal(int playerNo, ourGame* game);
  vector<string> moveList(int playerNo, ourGame* game);
  vector<string> removeRingFinal(int playerNo, ourGame* game);
  vector<string> removeMarkerAndRing(int playerNo, ourGame* game);

  //void play();
  unordered_map <string,transpositionNode> htMap;
  //void play();

  string minimax_decision(ourGame game);
  long long int minimax(ourGame gameNode,int depth,bool isMax,long long int alpha,long long int beta);
  long long int idMinimax(ourGame gameNode,int max_depth,double maxTime);
  vector<ourGame> sortChildren(vector<ourGame> childNodes,bool forMax);

};

//What's This??-> used as the ending syntax for the if condition applied above for not including the header file more than once
#endif
