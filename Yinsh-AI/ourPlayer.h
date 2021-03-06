#ifndef OURPLAYER_H
#define	OURPLAYER_H

#include"ourGame.h"
#include <bits/stdc++.h>
#include <unordered_map>

#define lli long long int

using namespace std;

struct transpositionNode{
  lli score;
  lli max_minus_depth;
};

struct childSortNode{
  ourGame gameNode;
  lli value;
};

struct transitionMove{
  string move;
  long long int value;
};

class ourPlayer{
  int playerNumber;//Gives the playerNumber to our BOT
  int totalRings;//Number of rings that needs to be placed on the board
  int timeLeft;//timeLeft for playing (in secs)
  int myRingsRemoved;
  int markersNeededToRemove;
  //vector < pair<int,int> > rings; // Length 5 vector which contains the position of the rings in the format (x,y)  (ie. axial Coordinates)
  //vector < pair<int,int> > markers; //Contains the position of the markers in (x,y) format (ie. axial coordinates)


public:
  ourGame* game;
  ourPlayer(int playerNumber, int timeLeft,int numberOfRings,int markersNeededToRemove);
  void addMarker(int playerNo, int x, int y, ourGame* game);
  void placeRing(int num, int a, int b, ourGame* game); //Returns the move which needs to be printed
  void moveRing(int num, int xs, int ys, int b, int c, ourGame* game); //Returns the move which should be printed
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
  vector<string> allDeletions(int playerNo, ourGame* game);
  void inverseMarker(int playerNo, int xStart, int yStart, int endX, int endY, ourGame* game);
  unordered_map <string,long long int> htMap;
  void play();
  string minimax_decision(ourGame game);
  struct transitionMove minimax(int depth,bool isMax,long long int alpha,long long int beta,int max_depth);
  struct transitionMove idMinimax(int max_depth,double maxTime);
  vector<string> sortChildren(vector<string> moves,bool forMax);
  void initialPlacing();

};

#endif
