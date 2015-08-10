#ifndef WORLD_HPP
#define WORLD_HPP

#include <ostream>
#include <iostream>
#include <stdint.h>
#include <vector>

using namespace std;

typedef std::pair<int32_t, int32_t> pii;

void jsonHeader(ostream& out);
void jsonOutput(bool first, int id, int seed, string commands, ostream& out);
void jsonFooter(ostream& out);

enum act_cmd
{
  idle,
  east,
  west,
  s_east,
  s_west,
  rotate_cw,
  count_rotate
};

act_cmd charToCmd(char inch);

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

class CC {
 public:
  int x;
  int y;
  int z;

  CC();
  CC(const CC &obj);
  CC(pii input);

  void fromOddR(pii input);
  pii  toOddR();

  CC operator+(const CC& rhs);
  CC operator-(const CC& rhs);
};

pii rotateAround(pii pivot, pii point, bool cw);


class Unit {
  public:
    vector<pii> subunits;
    vector<CC> ccunits;
    CC ccpivot;
    pii pivot;
    int dir;

    int left_x;
    int right_x;
    int top_y;
    int bot_y;

    Unit();
    // Need a copy constructor
    Unit(const Unit &obj);

    void ccConvert();
    void setBorders();
    // return false if this would push me over a border.
    // Also need world to check right/bot 
    bool rotate(bool cw);
    bool e();
    bool w();
    bool se();
    bool sw();

    void print() const;
    // Is there a subunit at this coord?
    bool suAt(pii pt) const;

    bool operator==(const Unit& rhs);
};

class Board {
  // AKA the Honeycomb
  public:
    // Note that it is indexed as c[y-val][x-val]
    vector< vector < bool > > c;
    size_t w;
    size_t h;
    Unit* au;

    Board(size_t width, size_t height);
    // Need a copy constructor
    Board(const Board &obj);

    // Freeze the au in place
    void lock();

    void doAct(act_cmd cmd);
    void clearrows();
    void resize(size_t width, size_t height);
    bool val(const Unit &ut) const;

    void print(ostream& os = std::cout) const;
};

class World {
  public:
    Board initialBoard;
    Board board;
    vector< Unit > units;
    Unit activeUnit;
    vector< uint32_t > seeds;
    int id;
    int sourcelength;
    int currentSource;
    int stepDelay;

    int initialSeed;
    int seed;

    World();

    int import(const char *filename);
    size_t nextUnit();
    // Returns false if over the source limit size.
    bool actNextUnit();

    void runAll();
    string mark1();
    string mark2();
};

#endif

