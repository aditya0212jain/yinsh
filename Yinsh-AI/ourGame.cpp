#include"ourGame.h"

#define pb push_back
#define mp make_pair
#define PI 3.14159

ourGame::ourGame(){
  currPlayer = 1; //Player 1 will start
  playerOneRingsOnBoard = 0; // Initially there are 0 player 1 rings on the board
  playerTwoRingsOnBoard = 0; // Initially there are 0 player 1 rings on the board
  playerOneMarkersOnBoard = 0; // Initially there are 0 player 1 rings on the board
  playerTwoMarkerssOnBoard = 0; // Initially there are 0 player 1 rings on the board
  boardSize = 85; //Atleast this is true for this part, we'll initialise it with a variable for the next part
  struct boardCell tempBoardCell = {0,false,false};
  int rows = 11;

  for(int i=0;i<rows;i++){
    vector<struct boardCell> boardRow;
    for(int j=0; j<rows; j++){
      boardRow.pb(tempBoardCell);
    }
    board[i].pb(boardRow);
  }
  //First index corresponds to vertical and second to y=x waali type ka diagnol
}

/*Big Question, how are we storing the board configuration*/
//Let's suppose for now, we have the function which can transform(hex,pos)->(x,y) and inverse of it

double ourGame::computeHeuristicValue(){
  //This function computes the heuristic for the player according to the current configuration of the boardSize

  double valPlayer1;
  //Compute for Player 1, ignore player 2 in this

  double valPlayer2;
  //Compute for player 2, ignore player 1

  if(currPlayer==1)
    return valPlayer1 - valPlayer2;
  else
    return valPlayer2 - valPlayer1;
}

int getNearestInteger(double a){
    double eps = 0.001;
    int p = (int)a;
    int q = (int)a +1;
    // cout << "p=" << p << " " << "q=" << q << endl;
    // cout << (abs((double)p-a)<eps) << endl;
    // cout << (abs((double)q-a)<eps) << endl;
    if(abs((double)p-a)<eps)
        return p;
    else return q;

}

pair<int,int> getRadialFromAxial(int x, int y, int mid){
    int mid = (rows-1)/2;
    int poss = -1;
    int hexx = -1;
    int xNew = x - mid;
    int yNew = y - mid;
    double xOld = sqrt(3)/2 * xNew;
    double yOld = (double)yNew - (double)xNew/2;
    int hex,posInt;//,pos;
    double pos;
    double dist = sqrt(xOld*xOld+yOld*yOld);
    if(dist-(int)dist==0){
        hex = (int)dist;
    }
    else{
        hex = int(dist) + 1;
    }
    if(hex==0){
        posInt = 0;
    }
    else{
        double theta = 60/hex;
        if(xOld==0){
            if(yOld>0){
                posInt = 0;
            }
            else{
                posInt = 3*hex;
            }
        }
        else{
            //-90 to 90
            double angle = 0;
             if(xOld>0 && yOld>=0){
                angle = 90 - (atan(yOld/xOld) * 180/PI);
                pos = angle/theta;
            }
            else if(xOld>0 && yOld<0){
                angle = 90 - (atan(yOld/xOld) * 180/PI);
                pos = angle/theta;
            }
            else if(xOld<0 && yOld<=0){
                angle = 270 - (atan(yOld/xOld) * 180/PI);
                pos = angle/theta;
            }
            else{
                angle = 270 - (atan(yOld/xOld) * 180/PI);
                pos = angle/theta;
            }
            posInt = getNearestInteger(pos);
            //cout << "Theta="<<theta << " " << "Angle=" << angle << endl;
            //cout << pos << " " << endl;
            //cout << ((int)pos+1) << endl;
            //cout << "Theta="<<(int)theta << " " << "Angle=" << (int)angle << endl;
        }
    }
    if(hex<=(rows-1)/2){
        poss = posInt;
        hexx = hex;
    }
    if(!(poss==-1 && hexx==-1)){
        //cout << "Cartesian (" <<x<<","<<y<<") is mapped to Radial ("<<hexx<<","<<poss<<")."<<endl;
        pair<int,int> ans = mp(hexx,poss);
        return ans;
    }
    else{
      pair<int,int> ans = mp(-1,-1);
      cout << "Invalid conversion";
      return ans;
    }

}


  int x=0,y=0;
  pair<int,int> radialToAxialDatabase(int hex, int pos){
  //Do the transformation
  return mp(x,y);
}
