#include<iostream>
#include"ourPlayer.h"
#include"ourGame.h"
#include <bits/stdc++.h>
using namespace std;



//Using main function for testing our players, @Aditya see how to combine this with other clientX
int main(){
  srand ( time(NULL) );
  //int myPlayerNumber = 1;
  int p_id,bs,tl;
  string inp;
  getline(cin,inp);
  vector<string> argus= split(inp,' ');
  p_id = stoi(argus[0]);
  bs = stoi(argus[1]);
  tl = stoi(argus[2]);
  ourPlayer playah(p_id,tl);
  playah.play();
  string input;

  playah.addMarker(2,2,6,playah.game);
  playah.addMarker(1,1,3,playah.game);
  playah.addMarker(1,0,0,playah.game);
  playah.addMarker(1,1,0,playah.game);
  playah.addMarker(1,2,0,playah.game);
  playah.addMarker(1,3,0,playah.game);
  playah.addMarker(1,4,0,playah.game);
  playah.placeRing(1,9,4,playah.game);
  playah.game->printBoard();

  vector<string> ans = playah.moveList(1,playah.game);

  cout << ans.size() << endl;
  for(int i=0; i<ans.size(); i++){
    cout << ans[i] << endl;
  }

}

// Moves to start
// S 0 0 M 1 3 RS 4 0 RE 0 0 X 1 3
//// exit
