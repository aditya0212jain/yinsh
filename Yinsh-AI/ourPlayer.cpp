
#include"ourPlayer.h"
#include"ourGame.h"
#include"utils.h"
#include <bits/stdc++.h> //Can we use this??
#include<math.h>
#include <random>

#define pb push_back
#define mp make_pair
#define lli long long int
int totalNodes=0;

using namespace std;

struct childSortNode{
  ourGame gameNode;
  lli value;
};

/*General Comments*/
// I think we should use the approach of double rows naming and use the approach of hexagon naming just for input and output, as that is only stored in the game which is used by Yinsh.js
/*GeneralComments over*/


bool compareForMax(const transitionMove &a, const transitionMove &b){
    return a.value>b.value;
}

bool compareForMin(const transitionMove &a,const transitionMove &b){
  return a.value<b.value;
}


vector<string> ourPlayer::sortChildren(vector<string> moves,bool forMax){
  vector<transitionMove> v;
  int playerNumber;
  if(forMax){
    playerNumber = this->playerNumber;
  }else{
    playerNumber = (this->playerNumber==1) ? 2:1;
  }
  // cout<<"in sorting , playerNumber "<<playerNumber<<" moves size:"<<moves.size()<<endl;
  // ourGame gameTemp;
  // gameTemp.copyTheBoard(this->game);
  for(int i=0;i<moves.size();i++){
    ourGame xgame;
    xgame.copyTheBoard(this->game);
    moveDecider(playerNumber,moves[i],this->game);
    // if(i==0){
    //   cout << "Printing Board:::" << endl;
    //   this->game->printBoard();
    // }
    lli valueTemp = this->game->computeHeuristicValue(this->playerNumber);
    transitionMove temp;
    temp.move = moves[i];
    temp.value = valueTemp;
    v.push_back(temp);
    this->game->moveUndo(playerNumber,moves[i]);
    if(!xgame.equalsTo(this->game)){
      cout<<"Not equal"<<endl;
      cout<<moves[i]<<" i:"<<i<<endl;
    }
  }
  // cout<<"--------------------------- In sorting CHECK BOARDS BELOW"<<endl;
  //   this->game->printBoard();
  //   cout<<"below is gameTemp"<<endl;;
  //   gameTemp.printBoard();
  //   cout<<" Equal or not: "<<gameTemp.equalsTo(this->game)<<endl;
  if(forMax){
    sort(v.begin(),v.end(),compareForMax);
  }else{
    sort(v.begin(),v.end(),compareForMin);
  }
  vector<string> ans;
  for(int i=0;i<v.size();i++){
    ans.push_back(v[i].move);
  }
  return ans;
}

//Assuming PLayer 0 moves first and PLayer 1 follows

//Constructor
ourPlayer::ourPlayer(int playerNumber,int timeLeft){
  //cin>>player_no>>total_rings>>time_left;
  this->playerNumber =  playerNumber; //1-> Player 1, 2-> Player 2
  this->totalRings =  5; //This version only has to deal with 5 rings
  this->timeLeft =  timeLeft; //will be initialised with full time
  this->myRingsRemoved = 0;//starting with 0 rings
  this->game = new ourGame();
}
//These hexagon and position will be decided by Alpha-Beta pruning
void ourPlayer::placeRing(int playerNo, int x, int y, ourGame* game){
  //Hexagon defines the hexagon in which we will place the new rings
  //position defines the position of that new ring in the hexagon
  // move temp;//it will be automatically initialized by {"",0,0}
  // pair<int,int> p = cartesianToHex(x,y,11);//rows = 11
  // //cout << p.first << " " << p.second << endl;
  // temp.type = "P";
  // temp.hex = p.first; // hexagon
  // temp.pos = p.second; //position
  //cout << "DO I";
  //rings.pb(mp(x,y));
  //cout << " Reach here?" << endl;
  struct boardCell tempboardCell; // Ring is placed, and not marker
  tempboardCell.player = playerNo;//this->playerNumber;
  tempboardCell.containsMarker = false;
  tempboardCell.containsRings = true;
  tempboardCell.canBeUsed = true;
  //cout << "And here?" << endl;
  // cout << (game.board.size()) << endl;
  // cout << game.board[x][y].player << " ";
  // cout << "Definitely I don't reach here, Do I?" << endl;
  game->board[x][y]  = tempboardCell;
  //cout << game.board[x][y].player << endl;
  if(/*this->playerNumber*/playerNo==1){
    game->playerOneRingsOnBoard++;
  }
  else{
    game->playerTwoRingsOnBoard++;
  }
  //return temp;
}

void ourPlayer::moveRing(int playerNo,int xStart, int yStart, int x, int y, ourGame* game){
  //index is the index in rings of the ring which we need to move_end, hexagon and position same  as above
  //We assume that index is never out of bounds and move to be moved is a valid move
  struct move temp;// = {"",0,0};

  // pair<int,int> p = cartesianToHex(x,y,11);//rows = 11
  // temp.type = "M";
  // temp.hex = p.first; // hexagon
  // temp.pos = p.second; //position
  //There will be a marker at original position

  pair<int,int> newMarkerPosition = mp(xStart,yStart);
  if(!(game->board[xStart][yStart]).containsRings){
    //cout << "Kaise kar lete hai aap!" << endl;
    return ;
  }
  //rings[index] = mp(x,y);
  struct boardCell tempBoardCellRing;// {this->playerNumber, true, false}; //Adding ring to the new positions
  tempBoardCellRing.player = playerNo;//this->playerNumber;
  tempBoardCellRing.containsRings = true;
  tempBoardCellRing.containsMarker = false;
  tempBoardCellRing.canBeUsed = true;
  game->board[x][y] = tempBoardCellRing;
  //We can say that this position isn't occupied by any other marker, hence pushBack it
  //markers.pb(newMarkerPosition);
  boardCell tempBoardCellMarker;// = {this->playerNumber,false,true}; // Marker is placed and not ring
  tempBoardCellMarker.player = playerNo;//this->playerNumber;
  tempBoardCellMarker.containsMarker = true;
  tempBoardCellMarker.containsRings = false;
  tempBoardCellMarker.canBeUsed = true;

  game->board[newMarkerPosition.first][newMarkerPosition.second] = tempBoardCellMarker;
  inverseMarker(playerNo,xStart,yStart,x,y,game);
  if(/*this->playerNumber*/playerNo==1)
    game->playerOneMarkersOnBoard++;
  else
    game->playerTwoMarkersOnBoard++;
  //return temp;
}

void ourPlayer::removeRing(int playerNo, int x, int y, ourGame* game){
  //Remove the ring, removal of markers will be done separately
  boardCell tempBoard = game->board[x][y];
  //cout << p.first << " " << p.second << " Yolo" <<  endl;
  pair<int,int> rad = cartesianToHex(x,y,11);//rows = 11
  struct move temp;// = {"",0,0};
  temp.type = "X";
  temp.hex = rad.first;
  temp.pos = rad.second;
  //rings.erase(rings.begin()+index);
  boardCell tempBoardCell;// = {this->playerNumber,false,true}; // Marker is placed and not ring
  tempBoardCell.player = 0;
  tempBoardCell.containsMarker = false;
  tempBoardCell.containsRings = false;
  tempBoardCell.canBeUsed = true;
  game->board[x][y] = tempBoardCell;
  if(/*this->playerNumber*/playerNo==1){
    game->playerOneRingsOnBoard--;
  }
  else{
    game->playerTwoRingsOnBoard--;
  }
  //return temp;
}

void ourPlayer::removeRow(int playerNo, int startX, int startY, int endX, int endY, ourGame* game){
  int movementX = endX - startX;
  int movementY = endY - startY;
  // cout << movementX << " " << movementY << endl;
  if(movementX==0){
    //Vertical move
    int mov = movementY/abs(movementY);
    for(int i = min(startY,endY); i<=max(endY,startY); i++){
      boardCell tempBoardCell;
      tempBoardCell.player = 0;
      tempBoardCell.containsMarker = false;
      tempBoardCell.containsRings = false;
      tempBoardCell.canBeUsed = true;
      game->board[startX][i] = tempBoardCell;
    }
  }
  else if(movementY==0){
    //Vertical move
    int mov = movementX/abs(movementX);
    for(int i = min(startX,endX); i<=max(endX,startX); i++){
      boardCell tempBoardCell;
      tempBoardCell.player = 0;
      tempBoardCell.containsMarker = false;
      tempBoardCell.containsRings = false;
      tempBoardCell.canBeUsed = true;
      game->board[i][startY] = tempBoardCell;
    }
  }
  else if(movementX==movementY){
    int mov = movementX/abs(movementX);
    int i,j;
    for(i = min(startX,endX),j=min(endY,startY); i<=max(startX,endX),j<=max(endY,startY); i++,j++){
      boardCell tempBoardCell;
      tempBoardCell.player = 0;
      tempBoardCell.containsMarker = false;
      tempBoardCell.containsRings = false;
      tempBoardCell.canBeUsed = true;
      game->board[i][j] = tempBoardCell;
    }
  }
  else{
    cout << "Galat chal rahe ho, sahi Karo!!!" << endl;
  }
  if(/*this->playerNumber*/playerNo==1){
    game->playerOneMarkersOnBoard = game->playerOneMarkersOnBoard-5;
  }
  else{
    game->playerTwoMarkersOnBoard = game->playerTwoMarkersOnBoard-5;
  }
}

void ourPlayer::inverseMarker(int playerNo, int startX, int startY, int endX, int endY, ourGame* game){
  int movementX = endX - startX;
  int movementY = endY - startY;
  // cout << movementX << " " << movementY << endl;
  if(movementX==0){
    //Vertical move
    int mov = movementY/abs(movementY);
    for(int i = min(startY,endY)+1; i<=max(endY,startY)-1; i++){
      boardCell tempBoardCell = game->board[startX][i];
      // tempBoardCell.player = 0;
      if(tempBoardCell.player!=0){
        tempBoardCell.player = 3 - tempBoardCell.player;
      }
      if(tempBoardCell.player==1){
        game->playerOneMarkersOnBoard++;
        game->playerTwoMarkersOnBoard--;
      }
      else{
        game->playerTwoMarkersOnBoard++;
        game->playerOneMarkersOnBoard--;
      }
      game->board[startX][i] = tempBoardCell;
    }
  }
  else if(movementY==0){
    //Vertical move
    int mov = movementX/abs(movementX);
    for(int i = min(startX,endX)+1; i<=max(endX,startX)-1; i++){
      boardCell tempBoardCell = game->board[i][startY];
      // tempBoardCell.player = 0;
      if(tempBoardCell.player!=0){
        tempBoardCell.player = 3 - tempBoardCell.player;
      }
      if(tempBoardCell.player==1){
        game->playerOneMarkersOnBoard++;
        game->playerTwoMarkersOnBoard--;
      }
      else{
        game->playerTwoMarkersOnBoard++;
        game->playerOneMarkersOnBoard--;
      }
      game->board[i][startY] = tempBoardCell;
    }
  }
  else if(movementX==movementY){
    int mov = movementX/abs(movementX);
    int i,j;
    for(i = min(startX,endX)+1,j=min(endY,startY)+1; i<=max(startX,endX)-1,j<=max(endY,startY)-1; i++,j++){
      boardCell tempBoardCell = game->board[i][j];
      // tempBoardCell.player = 0;
      if(tempBoardCell.player!=0){
        tempBoardCell.player = 3 - tempBoardCell.player;
      }
      if(tempBoardCell.player==1){
        game->playerOneMarkersOnBoard++;
        game->playerTwoMarkersOnBoard--;
      }
      else{
        game->playerTwoMarkersOnBoard++;
        game->playerOneMarkersOnBoard--;
      }
      game->board[i][j] = tempBoardCell;
    }
  }
  else{
    cout << "Galat chal rahe ho, sahi Karo" << endl;
  }
}

void ourPlayer::moveDecider(int playerNo, string s, ourGame* game){
  int length = s.length();
  //cout << "Length of input: " << length << endl;
  int pointer = 0;
  int rows = (game->rows);
  //cout << rows << " Yeh toh number of rows hai!!" << endl;
  //Here the coordinates are in hex and pos form
  vector<string> p = split(s,' ');

  while(pointer<p.size()){
    if(p[pointer].compare("RS")==0){
      int startX = stoi(p[pointer+1]);
      int startY = stoi(p[pointer+2]);
      int endX = stoi(p[pointer+4]);
      int endY = stoi(p[pointer+5]);
      pair<int,int> convertStart= hexToCartesian(startX, startY, rows);
      pair<int,int> convertEnd = hexToCartesian(endX,endY,rows);
      removeRow(playerNo, convertStart.first, convertStart.second, convertEnd.first, convertEnd.second, game);
      pointer += 6;
    }
    else if(p[pointer].compare("P")==0){
      // cout << "Did I come here?" << endl;
      int x = stoi(p[pointer+1]);
      int y = stoi(p[pointer+2]);
      pair<int,int> coor = hexToCartesian(x,y,rows);
      //cout << coor.first << " " << coor.second << endl;
      placeRing(playerNo,coor.first,coor.second, game);
      pointer += 3;
    }
    else if(p[pointer].compare("S")==0){
      int xStart = stoi(p[pointer+1]);
      int yStart = stoi(p[pointer+2]);
      int x = stoi(p[pointer+4]);
      int y = stoi(p[pointer+5]);
      pair<int,int> convertStart= hexToCartesian(xStart, yStart, rows);
      pair<int,int> convertEnd = hexToCartesian(x,y,rows);
      //cout << convertStart.first << " " << convertStart.second << endl;
      //cout << convertEnd.first << " " << convertEnd.second << endl;
      moveRing(playerNo,convertStart.first, convertStart.second, convertEnd.first, convertEnd.second, game);
      pointer += 6;
    }
    else if(p[pointer].compare("X")==0){
      int x = stoi(p[pointer+1]);
      int y = stoi(p[pointer+2]);
      pair<int,int> coor = hexToCartesian(x,y,rows);
      removeRing(playerNo,coor.first,coor.second, game);
      pointer += 3;
    }
    else{
      cout << "Move is incorrect!!" << endl;
      return;
    }
  }
}

vector<pair<pair<int,int>,pair<int,int> > > ourPlayer::selectAndMoveHelper(int playerNo, int x, int y, int dirX, int dirY, ourGame* game){
  //x,y -> current position of the ring
  //dirX,dirY -> each can be 1,0,-1 denotes kaha check karna hai
  vector<pair<pair<int,int>,pair<int,int> > > ans; //first is the current position of the ring, second is the position in which it can travel
  int i,j;
  pair<int,int> f = mp(x,y);
  int oneMarker=0;
  int rows = game->rows;
  for(i=x+dirX,j=y+dirY;;i=i+dirX,j=j+dirY){
    if(i<0 || j<0 || i>=rows || j>=rows){
      return ans;
    }
    else if(!(game->board[i][j].canBeUsed)){
      //cout << ans.size() << endl;
      // cout << "At this value I came here: " << i << " " << j << endl;
      return ans;
    }
    else if(game->board[i][j].containsRings){
      return ans;
    }
    else if(game->board[i][j].containsMarker && oneMarker==0){
        oneMarker=1;
        //ans.pb(mp(f,mp(i,j)));
    }
    else if(game->board[i][j].containsMarker && oneMarker==1){
      //Nothing
    }
    else if(game->board[i][j].player==0){
      if(oneMarker==1){
        ans.pb(mp(f,mp(i,j)));
        return ans;
      }
      else{
        //cout << i << " " << j << endl;
        ans.pb(mp(f,mp(i,j)));
      }
    }
    // if(i==0 && j==5){
    //   cout << game->board[i][j].canBeUsed << endl;
    // }
  }
}

vector<pair<pair<int,int>,pair<int,int> > > ourPlayer::selectAndMove(int playerNo,ourGame* game){
  vector<pair<pair<int,int>,pair<int,int> > > ans;
  vector<pair<pair<int,int>,pair<int,int> > > temp;

  // v.insert( v.end(), w.begin(), w.end());
  for(int i=0; i<game->rows; i++){
    for (int j=0; j<game->rows; j++){
      if(game->board[i][j].containsRings){
        // cout<<"containsRings"<<endl;
        // cout<<game->board[i][j].player<<" :player"<<endl;
        // cout<<playerNo<<endl;
        if(game->board[i][j].player==playerNo){
          //cout << i << " " << j << endl;
           temp = selectAndMoveHelper(playerNo,i,j,0,1,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());
          //  cout << ans.size() << endl;
           temp = selectAndMoveHelper(playerNo,i,j,0,-1,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());
           // cout << ans.size() << endl;
           temp = selectAndMoveHelper(playerNo,i,j,1,1,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());
           // cout << ans.size() << endl;
           temp = selectAndMoveHelper(playerNo,i,j,-1,-1,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());
           // cout << ans.size() << endl;
           // cout << "KYU " << temp[4].second.first << " " << temp[4].second.second << endl;
           temp = selectAndMoveHelper(playerNo,i,j,1,0,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());
           // cout << ans.size() << endl;
           temp = selectAndMoveHelper(playerNo,i,j,-1,0,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());
          //  cout <<"ans size"<< ans.size() << endl;

        }
      }
    }
  }
  return ans;
}

string ourPlayer::markerDeletionHelper(int playerNo, int x, int y, int dirX, int dirY,ourGame* game){
  //vetor<string> ans;
  string temp="";
  // cout << "Board[10][10]:" << game->board[10][10].canBeUsed << endl;
  int markerCount=0;
  int i,j;
  int rows = game->rows;
  for(i=x,j=y;;i=i+dirX,j=j+dirY){
    if(i<0 || j<0 || i>=rows || j>=rows){
      break;
    }
    // cout << "i=" << i << " j=" << j << endl;
    if(game->board[i][j].containsMarker && game->board[i][j].canBeUsed){
      if(game->board[i][j].player==playerNo){
        markerCount++;
      }
      else{
        break;
      }
      if(markerCount==5){
        pair<int,int> initial = cartesianToHex(x,y,rows);
        pair<int,int> finall = cartesianToHex(i,j,rows);
        string result;
        ostringstream convert;
        convert << initial.first;
        result = convert.str();

        string result1;
        ostringstream convert1;
        convert1 << initial.second;
        result1 = convert1.str();

        string result2;
        ostringstream convert2;
        convert2 << finall.first;
        result2 = convert2.str();

        string result3;
        ostringstream convert3;
        convert3 << finall.second;
        result3 = convert3.str();

        temp = "RS " + result + " " + result1 + " RE " + result2 + " " + result3;
        //ans.pb(temp);
        break;
      }
    }
    else{ 
      break;
    }
  }
  // cout << "MarkerCount=" << markerCount << endl;
  return temp;
}

vector<string> ourPlayer::markerDeletion(int playerNo, ourGame* game){
  int rows = game->rows;
  vector<string> ans;
  string temp = "";
  // cout << "playerNo=" << playerNo << endl;
  for(int i=0; i<rows; i++){
    for(int j=0; j<rows; j++){
      // cout << i << " " << j << " " ;
      if(game->board[i][j].containsMarker && game->board[i][j].canBeUsed && game->board[i][j].player ==playerNo){
        // cout << "i=" << i << " j=" << j << endl;
        temp = markerDeletionHelper(playerNo,i,j,0,1,game);//Total 6 directions
        if(temp.length()!=0){
          ans.pb(temp);
        }
        // cout << "a" << endl;
        // cout << ans.size() << " ";
        temp = markerDeletionHelper(playerNo,i,j,0,-1,game);//Total 6 directions
        if(temp.length()!=0){
          ans.pb(temp);
        }
        // cout << "b" << endl;
        // cout << ans.size() << " ";

        temp = markerDeletionHelper(playerNo,i,j,1,1,game);//Total 6 directions
        if(temp.length()!=0){
          ans.pb(temp);
        }
        // cout << ans.size() << " ";
        // cout << "c" << endl;
        temp = markerDeletionHelper(playerNo,i,j,-1,-1,game);//Total 6 directions
        if(temp.length()!=0){
          ans.pb(temp);
        }
        // cout << ans.size() << " ";

        temp = markerDeletionHelper(playerNo,i,j,1,0,game);//Total 6 directions
        if(temp.length()!=0){
          ans.pb(temp);
        }
        // cout << ans.size() << " ";

        temp = markerDeletionHelper(playerNo,i,j,-1,0,game);//Total 6 directions
        if(temp.length()!=0){
          ans.pb(temp);
        }
        // cout << ans.size();
      }
      // cout << endl;
    }
  }
  // cout <<"-----------------------------" << endl;
  // for(int i=0; i<ans.size(); i++)
    // cout << ans[i] << endl;
  // cout << "++++++++++++++++++++++++++" << endl;
  return ans;
}

vector<string> ourPlayer::selectAndMoveFinal(int playerNo, ourGame* game){
  vector<string> ans;
  vector<pair<pair<int,int>, pair<int,int> > > list = selectAndMove(playerNo, game);
  string temp="";
  int rows = game->rows;
  // cout << list.size() << endl;

  for(int i=0; i<list.size(); i++){
    int s1 = list[i].first.first;
    int s2 = list[i].first.second;
    int m1 = list[i].second.first;
    int m2 = list[i].second.second;
    pair<int,int> initial = cartesianToHex(s1,s2,rows);
    pair<int,int> finall = cartesianToHex(m1,m2,rows);

    string result;
    ostringstream convert;
    convert << initial.first;
    result = convert.str();

    string result1;
    ostringstream convert1;
    convert1 << initial.second;
    result1 = convert1.str();

    string result2;
    ostringstream convert2;
    convert2 << finall.first;
    result2 = convert2.str();

    string result3;
    ostringstream convert3;
    convert3 << finall.second;
    result3 = convert3.str();

    temp = "S " + result + " " + result1 + " M " + result2 + " " + result3;
    ans.pb(temp);
  }
  return ans;
}

vector<string> ourPlayer::removeRingFinal(int playerNo, ourGame* game){
  int rows = game->rows;
  vector<string> ans;
  string temp;
  for(int i=0; i<rows; i++){
    for(int j=0; j<rows; j++){
      if(game->board[i][j].player==playerNo && game->board[i][j].containsRings){
        pair<int,int> initial = cartesianToHex(i,j,rows);
        string result;
        ostringstream convert;
        convert << initial.first;
        result = convert.str();

        string result1;
        ostringstream convert1;
        convert1 << initial.second;
        result1 = convert1.str();

        temp = "X " + result + " " + result1;
        ans.pb(temp);
      }
    }
  }
  return ans;
}

vector<string> ourPlayer::removeMarkerAndRing(int playerNo, ourGame* game){
  //This function checks if we can remove the rings or not and returns a vector of strings
  vector<string> ans;
  // cout << "Do I come Here?" << endl;
  vector<string> first = markerDeletion(playerNo, game);

  // cout << "And Here?" << endl;
  string temp;
  // cout << "removeMarkerAndRing STARTS!!" << endl;
  for(int i=0; i<first.size(); i++){
    string removeMarkerOne = first[i];
    // cout << first[i] << endl;
    ourGame* firstGame = new ourGame();
    firstGame->copyTheBoard(game);
    moveDecider(playerNo,removeMarkerOne,firstGame);
    vector<string> firstRing = removeRingFinal(playerNo, game);
    for(int j=0; j<firstRing.size(); j++){
      string t = removeMarkerOne + " " + firstRing[j];
      ans.pb(t);
    }
  }
  // cout << "removeMarkerAndRing ENDS!" << endl;
  return ans;
}

vector<string> ourPlayer::allDeletions(int playerNo, ourGame* game){
    vector<string> ans;
    // cout << "Fuckkk!" << endl;
    vector<string> firstDeletion = removeMarkerAndRing(playerNo,game);
    // cout << "Marker 1" << endl;
    // string move="";
    if(firstDeletion.size()==0)
      return ans;
    else{
      for(int i=0; i<firstDeletion.size(); i++){
        string firstMove = firstDeletion[i];
        // move = firstMove;
        // cout << firstMove << endl;
        ourGame* firstGame = new ourGame();
        firstGame->copyTheBoard(game);
        moveDecider(playerNo,firstMove,firstGame);
        // cout << "Marker 2" << endl;
        if((playerNo==1 && firstGame->playerOneRingsOnBoard==2)||(playerNo==2 && firstGame->playerTwoRingsOnBoard==2)){
          ans.pb(firstMove);
        }
        else{
          vector<string> secondDeletion = removeMarkerAndRing(playerNo,firstGame);
          if(secondDeletion.size()==0){
            ans.pb(firstMove);
          }
          else{
            for(int j=0; j<secondDeletion.size(); j++){
              string secondMove = secondDeletion[j];
              ourGame* secondGame = new ourGame();
              secondGame->copyTheBoard(firstGame);
              moveDecider(playerNo,secondMove,secondGame);
              if((playerNo==1 && secondGame->playerOneRingsOnBoard==2)||(playerNo==2 && secondGame->playerTwoRingsOnBoard==2)){
                string temp = firstMove + " " + secondMove;
                ans.pb(temp);
              }
              else{
                vector<string> thirdDeletion = removeMarkerAndRing(playerNo, firstGame);
                if(thirdDeletion.size()==0){
                  string temp = firstMove + " " + secondMove;
                  ans.pb(temp);
                }
                else{
                  for(int k=0; k<thirdDeletion.size(); k++){
                    string thirdMove = thirdDeletion[k];
                    ourGame* thirdGame = new ourGame();
                    thirdGame-> copyTheBoard(secondGame);
                    moveDecider(playerNo,thirdMove,thirdGame);
                    string tempp = firstMove+ " " + secondMove + " " + thirdMove;
                    ans.pb(tempp);
                  }
                }
              }
            }
          }
        }
      }
    }
    return ans;
}

vector<string> ourPlayer::moveList(int playerNo, ourGame* game){
  //Returns a list of moves
  vector<string> ans;
  vector<string> firstRound = allDeletions(playerNo, game);
  // cout << "Do I Reach here?" << endl;
  if(firstRound.size()==0){
    //Nothing to delete
    vector<string> fr = selectAndMoveFinal(playerNo, game);
    // cout<<"And here?"<<endl;
    // cout<<fr.size()<<endl;
    int frSize = fr.size();
    if(fr.size()==0){
      cout << "NO MOVE LEFT" << endl;
    }
    else{
      // cout << "IDHAR" << endl;

      // cout << "frSize: " << frSize << endl;

      for(int i=0; i<frSize; i++){
        // cout << "Yahoo" << endl;
        // cout<<"i: "<<i;
        // ourGame* afterFirstMove = new ourGame();
        // afterFirstMove->copyTheBoard(game);
        string firstMove = fr[i];
        // cout<<" "<<firstMove<<endl;
        moveDecider(playerNo, firstMove, game);
        // cout<<" 02";
        // cout<<"i: "<<i<<" "<<firstMove<<endl;
        // cout<<afterFirstMove->board[8][7].player<<endl;
        // cout<<afterFirstMove->board[8][8].player<<endl;
        
        vector<string> dr = allDeletions(playerNo, game);
        // cout << "MoveUndo" << endl;
        game->moveUndo(playerNo,firstMove);
        // cout << "Done and Dusted!" << endl;
        // cout << "drSize:" << dr.size() << endl;
        // cout << i << endl;
        // cout<<" "<<dr.size()<<endl;
        if(dr.size()==0){
          //Nothing to delete
          // game->moveUndo(playerNo,firstMove);
          ans.pb(firstMove);
        }
        else{
          // cout << "drSize: " << dr.size() << endl;
          for(int j=0; j<dr.size(); j++){
            string temp = firstMove + " " + dr[j];
            ans.pb(temp);
          }
          // cout << "Do I!!!!!!!!!!!!" << endl;
        }
      }
    }
  }
  else{
    //There are deletions possible
    // cout << "First Round Size: " << firstRound.size() << endl;
    for(int i=0; i<firstRound.size(); i++){
      // cout << "Count" << endl;
      ourGame* afterFirstMove = new ourGame();
      afterFirstMove->copyTheBoard(game);
      string fm = firstRound[i];
      moveDecider(playerNo, fm, afterFirstMove);
      vector<string> sr = selectAndMoveFinal(playerNo,afterFirstMove);
      if(sr.size()==0){
        //No move to play ring
        ans.pb(fm);
      }
      else{
        // cout << sr.size() << endl;
        // cout << sr[18] << endl;
        // cout << fm << endl;
        for(int j=0; j<sr.size(); j++){
          ourGame*  afterSecondMove = new ourGame();
          afterSecondMove->copyTheBoard(afterFirstMove);
          string sm = sr[j];
          moveDecider(playerNo, sm, afterSecondMove);
          vector<string> tr = allDeletions(playerNo, afterSecondMove);
          //cout << tr.size() << endl;
          // cout << sm << " " << j << "\n";
          if(tr.size()==0){
            //Nothing to delete
            string temp = fm + " " + sm;
            //cout << temp << endl;
            ans.pb(temp);
            // if(j==18)
            //   cout << "What happens?" << endl;
          }
          else{
            //Something to delete
            for(int k=0; k<tr.size(); k++){
              string temp = fm + " " + sm + " " + tr[k];
              ans.pb(temp);
            }
          }
        }
      }
    }
  }
  return ans;
}

/*
*The iterative deepening version of minimax with
*some max time and depth alloted
*/
struct transitionMove ourPlayer::idMinimax(int max_depth,double maxTime){
  int depth = 0;
  double tempTime=0;
  struct timespec start_time,move_time;
  htMap.clear();

  //start noting time
  clock_gettime(CLOCK_REALTIME, &start_time);
  struct transitionMove bestMove;
  struct transitionMove tempMove;
  bestMove.value=-INFINITY;
  for(depth=1;depth<=max_depth;depth++){
    // cout<<"depth: "<<depth<<endl;
    // ourGame gameTemp;
    // gameTemp.copyTheBoard(this->game);
    tempMove = minimax(0,true,-INFINITY,INFINITY,depth);
    // this->game->printBoard();
    // cout<<"tempMove: "<<tempMove.move<<" "<<tempMove.value<<endl;
    // cout<<"yes"<<endl;
    if(bestMove.value<=tempMove.value){
      bestMove = tempMove;
    }
    // cout<<"totalNodes: "<<totalNodes<<endl;
    // bestScore = max(bestScore,tempMove.value);
    //compute time to solve for depth
    clock_gettime(CLOCK_REALTIME, &move_time);
    double seconds = (double)((move_time.tv_sec+move_time.tv_nsec*1e-9) - (double)(start_time.tv_sec+start_time.tv_nsec*1e-9));
    //return value if time exceeded
    if(seconds>=maxTime){
      htMap.clear();
      return bestMove;
    }
  }
  htMap.clear();
  return tempMove;
}

/*
*Implementing this assuming undo function is already made
*undoMove(ourGame game)
*/
//initialize with alpha = -INFINITY & beta = INFINITY
struct transitionMove ourPlayer::minimax(int depth,bool isMax,long long int alpha,long long int beta,int max_depth){
  totalNodes++;
  //depth will never be zero
  if(depth==max_depth){
    struct transitionMove ans;
    ans.move="Reached";

    ans.value=this->game->computeHeuristicValue(this->playerNumber);


    return ans;
  }

  transitionMove bestMove;

  //if our player's turn
  if(isMax){
    // cout<<"max"<<endl;
    bestMove.value=-INFINITY;
    vector<string> possible_moves;
    // this->game->printBoard();
    // cout<<"fuck it"<<endl;
    possible_moves = moveList(this->playerNumber,this->game);
    // childVector = gameNode.children();//assuming children function returns an vector of possible gameNodes
    // if(possible_moves.size()==0){
    //       struct transitionMove ans;
    //       ans.move="Reached";
    //       ans.value=INFINITY;
    //       // cout<<"returning value:"<<ans.value<<endl;
    //       return ans;
    // }
    // cout<<"possible moves count in max:"<<possible_moves.size()<<endl;
    // cout<<"sorting"<<endl;
    // ourGame gameTemp;
    //   gameTemp.copyTheBoard(this->game);
    possible_moves = sortChildren(possible_moves,true);
    // cout<<"sorted"<<endl;
    // cout<<"CHECK BOARDS BELOW"<<endl;
    // this->game->printBoard();
    // cout<<"below is gameTemp"<<endl;;
    // gameTemp.printBoard();
    // cout<<"Sorting done"<<endl;
    for(int i=0;i<possible_moves.size();i++){

      // cout<<possible_moves[i]<<" <- move"<<endl;

      moveDecider(this->playerNumber,possible_moves[i],this->game);
      transitionMove tempMove = minimax(depth+1,false,alpha,beta,max_depth);
      if(alpha<=tempMove.value){
        alpha = tempMove.value;
      }
      // alpha = max(alpha,value);
      if(bestMove.value<=tempMove.value){
        bestMove = tempMove;
        bestMove.move = possible_moves[i];
      }
      // bestScore = max(value,bestScore);
      // cout<<"undo starting"<<endl;
      this->game->moveUndo(this->playerNumber,possible_moves[i]);

      // cout<<"undo done"<<endl;
      if(alpha>=beta){
        tempMove.move = possible_moves[i];
        return tempMove;
      }
    }
    return bestMove;
  }

  //if opponent's turn
  if(!isMax){
    // cout<<"min1"<<endl;
    int opponent_player_number;
    opponent_player_number = (this->playerNumber==1) ? 2 : 1;
    bestMove.value = INFINITY;
    vector<string> possible_moves;
    // cout<<"opponent_player "<<opponent_player_number<<endl;
    // this->game->printBoard();
    possible_moves = moveList(opponent_player_number,this->game);
    // cout<<"possible moves: "<<possible_moves.size()<<endl;
    // cout<<possible_moves[2]<<endl;
    // if(possible_moves.size()==0){
    //       struct transitionMove ans;
    //       ans.move="Reached";
    //       ans.value=-INFINITY;
    //       // cout<<"returning value:"<<ans.value<<endl;
    //       return ans;
    // }
    // cout<<"possible moves count in min:"<<possible_moves.size()<<endl;
    possible_moves = sortChildren(possible_moves,false);
    // cout<<possible_moves[2]<<endl;
    for(int i=0;i<possible_moves.size();i++){
      moveDecider(opponent_player_number,possible_moves[i],this->game);
      transitionMove tempMove= minimax(depth+1,true,alpha,beta,max_depth);
      // beta = min(beta,value);
      if(beta>=tempMove.value){
        beta = tempMove.value;
      }
      if(bestMove.value>=tempMove.value){
        bestMove = tempMove;
        bestMove.move = possible_moves[i];
      }
      // cout<<"undo move opponent"<<endl;
      this->game->moveUndo(opponent_player_number,possible_moves[i]);
      // cout<<"undo done"<<endl;
      if(alpha>=beta){
        tempMove.move = possible_moves[i];
        return tempMove;
      }
    }
    return bestMove;
  }

}

void ourPlayer::initialPlacing(){
  int count1=0,count2=0;
  string oppMove;
  if(this->playerNumber==2){
   getline(cin,oppMove);
   this->moveDecider(1,oppMove,this->game);
   count2++; 
  }
  while(count1!=5||count2!=5){
    while(true){

      int a = rand()%3;
      int b;
      if(a==0){
        b=0;
      }else{
        b = rand()%(6*a);
      }
      // cout<<"a,b: "<<a<<" "<<b<<endl;
      pair<int,int> h = hexToCartesian(a,b,11);
      // cout<<"h "<<h.first<<" "<<h.second<<endl;
      if(this->game->board[h.first][h.second].player==0){
        string beta = "P ";
        beta = beta+to_string(a)+" "+to_string(b);
        cout<<beta<<endl;
        this->moveDecider(this->playerNumber,beta,this->game);
        count1++;
        break;
      }
    }
    if(count1==5&&count2==5){
      break;
    }
    getline(cin,oppMove);
    this->moveDecider((this->playerNumber%2)+1,oppMove,this->game);
    // cout<<"haggu"<<endl; 
    count2++;
  }
}


void ourPlayer::play(){
  string opponentMove;
  int opponent_player_number;
  int count=0;

  this->initialPlacing();

  // // cout<<"fuck this"<<endl;
  // moveDecider(1,"P 0 0",this->game);
  // cout<<"P 0 0"<<endl;
  // getline(cin,opponentMove);
  // moveDecider(2,opponentMove,this->game);
  // // moveDecider(2,"P 0 0",this->game);
  // moveDecider(1,"P 2 9",this->game);
  // cout<<"P 2 9"<<endl;
  // getline(cin,opponentMove);
  // moveDecider(2,opponentMove,this->game);
  // // moveDecider(2,"P 2 2",this->game);
  // moveDecider(1,"P 2 0",this->game);
  // cout<<"P 2 0"<<endl;
  // getline(cin,opponentMove);
  // moveDecider(2,opponentMove,this->game);
  // // moveDecider(2,"P 2 4",this->game);
  // moveDecider(1,"P 3 1",this->game);
  // cout<<"P 3 1"<<endl;
  // getline(cin,opponentMove);
  // moveDecider(2,opponentMove,this->game);
  // // moveDecider(2,"P 4 12",this->game);
  // moveDecider(1,"P 4 4",this->game);
  // cout<<"P 4 4"<<endl;
  // getline(cin,opponentMove);
  // moveDecider(2,opponentMove,this->game);
  // // moveDecider(2,"P 3 0",this->game);
  // // cout<<"this is it"<<endl;

  // this->game->printBoard();
  if(this->playerNumber==1){
    transitionMove m = idMinimax(3,40);//max_depth,time
    // cout<<"o1"<<endl;
    cout<<m.move<<endl;
    // this->game->printBoard();
    moveDecider(this->playerNumber,m.move,this->game);
    // this->game->printBoard();
    //getline(cin,opponentMove);
  }
  getline(cin,opponentMove);
  opponent_player_number = (this->playerNumber==1) ? 2:1;
  while(!this->game->ended()){
    moveDecider(opponent_player_number,opponentMove,this->game);
    // this->game->printBoard();
    transitionMove m = idMinimax(3,40);
    cout<<m.move<<endl;
    moveDecider(this->playerNumber,m.move,this->game);
    // this->game->printBoard();

    getline(cin,opponentMove);
  }
}

/*
vector<string> ans;
vector<string> beforeDeletion = markerDeletion(playerNo, game);
//string deletionString="";
for(int i=0; i<beforeDeletion.size(); i++){
  string temp = beforeDeletion[i];
  ourGame* tempGame = new ourGame();
  tempGame->copyTheBoard(game);
  moveDecider(playerNo,temp,tempGame);
  //deletionString = deletionString + temp + " ";
  //vector<pair<pair<int,int>,pair<int,int> > > tt = selectAndMove(playerNo, game);
  vector<string> selectAndMoveM = selectAndMoveFinal(playerNo,tempGame);
  for(int j=0; j<selectAndMoveM.size(); j++){
    string tempTemp = selectAndMoveM[j];
    ourGame* tempTempGame = new ourGame();
    tempTempGame->copyTheBoard(tempGame);
    moveDecider(playerNo,tempTemp,tempTempGame);

    vector<string> markerDelete = markerDeletion(playerNo,tempTempGame);
    if(markerDelete.size()==0){
      string ansMember = temp + " " + tempTemp;
      ans.pb(ansMember);
    }
    else{
      for(int k=0; k<markerDelete.size(); k++){
        string tempTemp_Temp = markerDelete[k];
        // ourGame tempTemp_TempGame = new ourGame();
        // *(tempTemp_TempGame) = *(tempTempGame);
        // moveDecider(playerNo,tempTemp_Temp, tempTemp_TempGame);
        string ansMember = temp + " " + tempTemp + " " + tempTemp_Temp;
        ans.pb(ansMember);
      }
    }
  }
}
//vector<pair<pair<int,int>,pair<int,int> > > tt = selectAndMove(playerNo,game);
vector<string> snm = selectAndMoveFinal(playerNo, game);
for(int i=0; i<snm.size(); i++){
  string s = snm[i];
  ourGame* tempGame = new ourGame();
  tempGame->copyTheBoard(game);
  moveDecider(playerNo,s,tempGame);
  vector<string> snms = markerDeletion(playerNo, tempGame);
  if(snms.size()==0){
    ans.pb(s);
  }
  else{
    for(int j=0; j<snms.size(); j++){
      string ss = snms[j];
      string ansMember = s + " " + ss;
      ans.pb(ansMember);
    }
  }
}
return ans;
//All the markers deleted
*/
