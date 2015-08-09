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

// Could be a class...
uint32_t numberFromSeed(uint32_t seed);
uint32_t iterateRNG(uint32_t seed);
void testRNG(uint32_t seed, size_t count);


class Unit {
  public:
    vector<pii> subunits;
    pii pivot;
    int dir;

    int left_x;
    int right_x;
    int top_y;
    int bot_y;

    Unit();
    // Need a copy constructor
    Unit(const Unit &obj);

    void setBorders();
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

    int seed;

    World();

    int import(const char *filename);
    size_t nextUnit();
};

#endif

