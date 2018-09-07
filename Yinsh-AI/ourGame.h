#ifndef OURGAME_H
#define	OURGAME_H

struct move{
  string type;
  int hex;
  int pos;
};

class ourGame{
private:
  int total_rings;
  int board_size;
  vector<int> board[];

public:

  ourGame(int n);
  void execute_move(move m);
  bool check_validity(move m);

};

#endif
