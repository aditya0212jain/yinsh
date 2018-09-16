#include<iostream>
#include"ourPlayer.h"
#include"ourGame.h"
#include <bits/stdc++.h>
using namespace std;



// int main(){
//   string str;
//   ourPlayer player_object();
//   // cout<<"this is our player"<<endl;
//   // std::cerr << "/* error message First error */" << '\n';
//   cout<<"p 5 4"<<endl;
//   return 0;
// }


//Using main function for testing our players, @Aditya see how to combine this with other clientX
int main(){
  //int myPlayerNumber = 1;
  ourPlayer playah(1,150);
  //ourPlayer nonPlayah = playah;
  //nonPlayah.game = new ourGame();
  string input;

  input = "P 1 3";

  playah.moveDecider(1,input,playah.game);
  playah.game -> printBoard();
  input = "S 1 3 P 0 0";
  // playah.moveDecider(1, input, (playah.game));
  // input = "S 0 0 P 1 0";
  // playah.moveDecider(1, input, (playah.game));
  // input = "S 1 0 P 2 0";
  // playah.moveDecider(1, input, (playah.game));
  // input = "S 2 0 P 3 0";
  // playah.moveDecider(1, input, (playah.game));
  // input = "S 3 0 P 4 0";
  // playah.moveDecider(1, input, (playah.game));
  //
  //
  // input = "P 1 4";
  // playah.moveDecider(1, input, (playah.game));
  // playah.game -> printBoard();
  //
  // input = "RS 1 3 RE 3 0 X 1 4 S 4 0 M 5 6";
  playah.moveDecider(1, input, playah.game);

  cout << endl << endl ;
  playah.game -> printBoard();
  cout << endl << endl;
  playah.game -> moveUndo(1,input);
  playah.game -> printBoard();

  // vector<string> ans = playah.moveList(1,playah.game);
  // cout << ans.size() << endl;
  pair<int,int>p = hexToCartesian(2,5,11);
  cout << p.first << " " << p.second << endl;
  pair<int,int> q = cartesianToHex(p.first,p.second,11);
  cout << "YOLO ";
  cout << q.first << " " << q.second << endl;
  // for(int i=0; i<ans.size(); i++)
  //   cout << ans[i] << endl;


  //ourGame Gamer;
  // cout << "HUh" << endl;
  // playah.placeRing(1,2,1);
  //playah.game->printBoard();
  // move tempMove;
  // playah.moveRing(1,2,1,3,2);
  // playah.moveRing(1,3,2,4,3);
  // playah.moveRing(1,4,3,5,4);
  // playah.moveRing(1,5,4,6,5);
  // playah.moveRing(1,6,5,7,6);
  // playah.game->printBoard();
  // //cout << (playah1.rings[0].first) << " " << (playah1.rings[0].second) << endl;
  // //playah1.removeRing(0);
  // playah.removeRow(1,2,1,7,6);

  /*Shayad in this way we can solve board ki BT*/
  // playah.game -> printBoard();
  // int count=0;
  // int player = 1;
  // string input;
  // input = "P 4 0";
  // playah.moveDecider(player,input);
  // playah.game -> printBoard();
  // *(nonPlayah.game) = *(playah.game);
  //
  // input = "S 4 0 M 3 0";
  // playah.moveDecider(player,input);
  // cout << "Playah Ka Board" << endl;
  // playah.game->printBoard();
  //
  // cout << "nonPlayah ka Board" << endl;
  // nonPlayah.game -> printBoard();
  /*Board ki BT shayad ends here!!*/


  //pair<int,int> p = hexToCartesian(0,0,11);
  //cout << p.first << " " << p.second << endl;
  //
  // while(true){
  //   count++;
  //   cout << "Player " << player << ", your move:" << endl;
  //   string input;
  //
  //   if(count==1){
  //     input = "P 4 0";
  //     playah.moveDecider(player,input);
  //     //cout << input << " " << input.length() << endl;
  //     playah.game -> printBoard();
  //     input = "S 4 0 M 3 0";
  //     playah.moveDecider(player,input);
  //     playah.game->printBoard();
  //
  //     input = "S 3 0 M 2 0";
  //     playah.moveDecider(player,input);
  //     playah.game->printBoard();
  //
  //     input = "S 2 0 M 1 0";
  //     playah.moveDecider(player,input);
  //     playah.game->printBoard();
  //
  //     input = "S 1 0 M 0 0";
  //     playah.moveDecider(player,input);
  //     playah.game->printBoard();
  //   }
  //   else{
  //     getline(cin,input);
  //     if(input.compare("exit")==0||input.compare("Exit")==0){
  //       break;
  //     }
  //     playah.moveDecider(player,input);
  //     playah.game -> printBoard();
  //   }
  // }
}

// Moves to start
// S 0 0 M 1 3 RS 4 0 RE 0 0 X 1 3
//// exit
