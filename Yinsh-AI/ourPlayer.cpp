#include<iostream>
#include"ourPlayer.h"
#include"ourGame.h"
#include <bits/stdc++.h> //Can we use this??

#define pb push_back
#define mp make_pair

using namespace std;

/*General Comments*/
// I think we should use the approach of double rows naming and use the approach of hexagon naming just for input and output, as that is only stored in the game which is used by Yinsh.js
/*GeneralComments over*/

//Assuming PLayer 0 moves first and PLayer 1 follows
class ourPLayer{
  int playerNumber;//Gives the playerNumber to our BOT
  int totalRings;//Number of rings that needs to be placed on the board
  int TimeLeft;//timeLeft for playing (in secs)
  vector<pair<int,int>> rings; // Length 5 vector which contains the position of the rings in the format (hexagon number, point number int hat hgexagon)
  vector <pair<int,int>> markers; //Contains the position of the markers

public:
  ourPlayer(int a);
  void placeRing();
  
}
//Constructor
ourPlayer::ourPlayer(int playerNumber){
  //cin>>player_no>>total_rings>>time_left;
  this.playerNumber =  playerNumber - 1;
  this.totalRings =  5; //This version only has to deal with 5 rings
  this.timeLeft =  120; //will be initialised with full time
  rings = new vector

}

//Using main function for testing our players, @Aditya see how to combine this with other clientX
int main(){
  int myPLayerNumber = 1;
  ourPlayer playah = new ourPlayer(myPlayerNumber);
}
