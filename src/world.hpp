#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>

using namespace std;

typedef std::pair<int32_t, int32_t> pii;

enum Dir {
  Normal = 0,
  Right,
  Right2,
  Reversed,
  Left2,
  Left,
  DirMAX
};


class Unit {
  public:
    vector<pii> subunits;
    pii pivot;
    int dir;

    Unit();

    void print();
};

class Board {
  // AKA the Honeycomb
  public:
    Board();
    // Need a copy constructor
    Board(const Board &obj);

    void print();
};

#endif

