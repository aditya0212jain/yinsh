#ifndef OURGAME_H
#define	OURGAME_H
#include <bits/stdc++.h>
struct move{
  string type="";
  int hex=0;
  int pos=0;
};

struct boardCell{
  int player=0; //0 -> Empty, 1->Player 1, 2-> Player 2
  //bool empty = true; //No marker or ring
  bool containsRings = false; // True if this position has a ring
  bool containsMarker = false; //True if this position contains a marker
}

class ourGame{

  int currPlayer; //If our player is 1 then 1 else 2
  int playerOneRingsOnBoard; // Number of rings belonging to player 1
  int playerTwoRingsOnBoard; // Number of rings belonging to player 2
  int playerOneMarkersOnBoard; //Number of markers of player 1 on boardCell
  int playerTwoMarkersOnBoard; //Number of markers of player 2 on board

  int boardSize; // For this part, it is 85
  vector<vector<struct boardCell>> board;// index = (0,0)->0, others -> ((h)*(h+1))/2 + pos
  //vector<pair<int,int>> axialToRadialDatabase; //  index = (0,0)->0, others -> ((h)*(h+1))/2 + pos, dataBaseIndex contains a pair which gives

public:

  ourGame(int n);
  void execute_move(move m);
  bool check_validity(move m);

};

#endif
