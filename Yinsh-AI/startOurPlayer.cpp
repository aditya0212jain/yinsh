#include<iostream>
#include"ourPlayer.h"
#include"ourGame.h"

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
  int p_id,bs,ti;//bs->boardsize ,ti - time left, p_id -> id
  cin>>p_id>>bs>>ti;
  
  // int myPlayerNumber = 1;
  ourPlayer playah(p_id+1);
  return 0;
}
