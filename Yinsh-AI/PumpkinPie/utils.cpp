#include "utils.h"

#define pb push_back
#define mp make_pair
#define PI 3.14159

using namespace std;

vector<string> split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;
 
  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }
 
  return internal;
}

int getNearestInteger(double a){
    double eps = 0.3;
    int p = (int)a;
    int q = (int)a +1;
    if(abs((double)p-a)<eps)
        return p;
    else return q;
}


pair<int,int> cartesianToHex(int x, int y, int rows){
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
        }
    }
    if(hex<=(rows-1)/2){
        poss = posInt;
        hexx = hex;
    }
    if(!(poss==-1 && hexx==-1)){
        pair<int,int> ans = mp(hexx,poss);
        return ans;
    }
    else{
      pair<int,int> ans = mp(-1,-1);
      cout << "Invalid conversion";
      return ans;
    }

}


pair<int,int> hexToCartesian(int a,int b,int rows){
  int x0 = (rows-1)/2;
  int y0 = (rows-1)/2;
  int x,y;
  if(a==0){
    x = x0;
    y = y0;
  }
  else if(b<=a){
    x = x0+b;
    y=y0+a;
  }else if(b<=(2*a)&&b>a){
    x=x0+a;
    y=y0+(2*a)-b;
  }else if(b<=(3*a)&&b>(2*a)){
    x=x0+(3*a)-b;
    y = y0-(b-(2*a));
  }else if(b<=(4*a)&&b>(3*a)){
    x = x0-(b-(3*a));
    y = y0-a;
  }else if(b<=(5*a)&&b>(4*a)){
    x = x0-a;
    y = y0-((5*a)-b);
  }else if(b<(6*a)&&b>(5*a)){
    x =x0 -((6*a)-b);
    y=y0+(b-(5*a));
  }

  return mp(x,y);
}
