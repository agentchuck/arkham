#ifndef WORLD_HPP
#define WORLD_HPP

#include <stdint.h>
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
    // Need a copy constructor
    Unit(const Unit &obj);

    void print();
};

class Board {
  // AKA the Honeycomb
  public:
    vector< vector < bool > > c;
    size_t w;
    size_t h;

    Board(size_t width, size_t height);
    // Need a copy constructor
    Board(const Board &obj);

    void resize(size_t width, size_t height);

    void print();
};

class World {
  public:
    Board board;
    vector< Unit > units;
    vector< uint32_t > seeds;
    int id;
    int sourcelength;

    World();

    int import(const char *filename);
};

#endif

