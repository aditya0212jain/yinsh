#include "utils.h"

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

void hexToCartesian(int a,int b,int x0,int y0){
  int x,y;
  if((b/a<=1){
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
}