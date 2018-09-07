#ifndef OURPLAYER_H
#define	OURPLAYER_H

#include"ourGame.h"

class ourPlayer{
private:
  int total_rings;
  int player_no;
  double time_left;

public:
  ourPlayer();
  void play();
  move placeRing();
  move selectRing();
  move moveRing();
  move removeRowStart();
  move removeRowEnd();
  move removeRing();

};

#endif
