#include "world.hpp"

#include <iostream>
#include <bits/stdc++.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

#include "parser.hpp"

string pp[2] = { "ei!a", "ia! ia!a" };
const int numpp = 2;

void jsonHeader(ostream& out)
{
  out << "[" << endl;
}

void jsonOutput(bool first, int id, int seed, string commands, ostream& out)
{
  out << (first ? " " : ",") << " { \"problemId\": " << id << endl;
  out << "  , \"seed\": " << seed << endl;
  out << "  , \"tag\": \"" << id << ":" << seed << "-mark4\"" << endl;
  out << "  , \"solution\": \"" << commands << "\"" << endl;
  out << "  }" << endl;
}

void jsonFooter(ostream& out)
{
  out << "]" << endl;
}


act_cmd charToCmd(char inch) {
  act_cmd cmd=idle;
  switch (inch)
  {
    case 'b':
    case 'c':
    case 'e':
    case 'f':
    case 'y':
    case '2':
      cmd=east;
      break;
    case 'p':
    case '\'':
    case '!':
    case '.':
    case '0':
    case '3':
      cmd=west;
      break;
    case 'a':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case '4':
      cmd=s_west;
      break;
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case ' ':
    case '5':
      cmd=s_east;
      break;
    case 'd':
    case 'q':
    case 'r':
    case 'v':
    case 'z':
    case '1':
      cmd=rotate_cw;
      break;
    case 'k':
    case 's':
    case 't':
    case 'u':
    case 'w':
    case 'x':
      cmd=count_rotate;
      break; 
    default: 
      break;
  }  

  return cmd;
}



uint32_t numberFromSeed(uint32_t seed) {
  // The random number associated with a seed consists of bits 30..16 of that seed
  return (seed >> 16) & 0x7fff;
}

uint32_t iterateRNG(uint32_t seed) {
  uint64_t temp = uint64_t(seed) * uint64_t(1103515245);
  temp += 12345;
  return temp & 0xffffffff;
}

void testRNG(uint32_t seed, size_t count) {
  cerr << "First " << count << " elements of seed: " << seed << endl;
  for(size_t i = 0; i < count; i++) {
    cout << numberFromSeed(seed) << endl;
    seed = iterateRNG(seed);
  }
}

CC::CC() :
  x(0),
  y(0),
  z(0)
{
}

CC::CC(const CC & obj) :
  x(obj.x),
  y(obj.y),
  z(obj.z)
{
}

CC::CC(pii input)
{
  fromOddR(input);
}

void
CC::fromOddR(pii input)
{
  // convert odd-r offset to cube
  x = input.first - (input.second - (input.second & 1)) / 2;
  z = input.second;
  y = (-x) - z;
}

pii
CC::toOddR()
{
  // convert cube to odd-r offset
  pii output;
  output.first = x + (z - (z&1)) / 2;
  output.second = z;
  return output;
}

CC
CC::operator+(const CC& rhs)
{
  CC retval(*this);
  retval.x += rhs.x;
  retval.y += rhs.y;
  retval.z += rhs.z;

  return retval;
}

CC
CC::operator-(const CC& rhs)
{
  CC retval(*this);
  retval.x -= rhs.x;
  retval.y -= rhs.y;
  retval.z -= rhs.z;

  return retval;
}


pii rotateAround(pii pivot, pii point, bool cw)
{
  CC ccpiv(pivot);
  CC ccpt(point);

  CC ccdelta = ccpt - ccpiv;
  CC ccrot;
  
  if (cw) {
    ccrot.x = -ccdelta.z;
    ccrot.y = -ccdelta.x;
    ccrot.z = -ccdelta.y;
  } else {
    ccrot.x = -ccdelta.y;
    ccrot.y = -ccdelta.z;
    ccrot.z = -ccdelta.x;
  }

  return (ccrot + ccpiv).toOddR();
}



Unit::Unit()
  : dir(0),
    left_x(-1),
    right_x(-1),
    top_y(-1),
    bot_y(-1)
{
}

Unit::Unit(const Unit &obj)
  : dir (obj.dir),
    left_x(obj.left_x),
    right_x(obj.right_x),
    top_y(obj.top_y),
    bot_y(obj.bot_y)
{
  pivot = obj.pivot;
  subunits = obj.subunits;
}

void
Unit::ccConvert()
{

}

void
Unit::setBorders()
{
  left_x = subunits[0].first;
  right_x = subunits[0].first;
  top_y = subunits[0].second;
  bot_y = subunits[0].second;

  for(size_t i = 1; i < subunits.size(); i++)
  {
     if(subunits[i].first < left_x) {
       left_x = subunits[i].first;
     }
     if(subunits[i].first > right_x) {
       right_x = subunits[i].first;
     }
     if(subunits[i].second < top_y) {
       top_y = subunits[i].second;
     }
     if(subunits[i].second > bot_y) {
       bot_y = subunits[i].second;
     }
  }
}

bool
Unit::rotate(bool cw)
{
  for(size_t i = 0; i < subunits.size(); i++)
  {
    pii newPoint = rotateAround(pivot, subunits[i], cw);
    subunits[i] = newPoint;
  }
  setBorders();
  bool outside = false;
  if ((left_x < 0) || (top_y < 0)) {
    outside = true;
  }

  return !outside;
}

bool
Unit::e()
{
  for(size_t i = 0; i < subunits.size(); i++)
  {
    subunits[i].first++;
  }
  pivot.first++;
  setBorders();
  bool outside = false;
  if ((left_x < 0) || (top_y < 0)) {
    outside = true;
  }

  return !outside;
}

bool
Unit::w()
{
  for(size_t i = 0; i < subunits.size(); i++)
  {
    subunits[i].first--;
  }
  pivot.first--;
  setBorders();
  bool outside = false;
  if ((left_x < 0) || (top_y < 0)) {
    outside = true;
  }

  return !outside;
}

bool
Unit::se()
{
  for(size_t i = 0; i < subunits.size(); i++)
  {
    if (subunits[i].second & 1) {
      subunits[i].first++;
    }
    subunits[i].second++;
  }
  if (pivot.second & 1) {
    pivot.first++;
  }
  pivot.second++;
  setBorders();
  bool outside = false;
  if ((left_x < 0) || (top_y < 0)) {
    outside = true;
  }

  return !outside;
}

bool
Unit::sw()
{
  for(size_t i = 0; i < subunits.size(); i++)
  {
    if (!(subunits[i].second & 1)) {
      subunits[i].first--;
    }
    subunits[i].second++;
  }
  if (!(pivot.second & 1)) {
    pivot.first--;
  }
  pivot.second++;

  setBorders();
  bool outside = false;
  if ((left_x < 0) || (top_y < 0)) {
    outside = true;
  }

  return !outside;
}

void
Unit::print() const
{
  // cout << "Unit::print" << endl;
  cout << left_x << ":" << right_x << " " << top_y << ":" << bot_y << endl;
  // cout << subunits.size() << " units\n";
  for(size_t y = top_y; y <= bot_y; y++)
  {
    if (y & 1) {
      cout << " " ;
    }

    for(size_t x = left_x; x <= right_x; x++)
    {
      bool pm = false;
      if ((pivot.first == x) && (pivot.second == y)) {
        pm = true; 
      }
      bool match = false;
      for(size_t cnt = 0; cnt < subunits.size(); cnt++) {
        if ((subunits[cnt].first == x) && (subunits[cnt].second == y)) {
          match = true; 
        }
      }
      if (match && pm) {
        cout << "& " ;
      } else if (match) {
        cout << "+ " ;
      } else if (pm) {
        cout << "o " ;
      } else {
        cout << ". " ;
      }
    }
    cout << endl;
  }
}

bool
Unit::suAt(pii pt) const
{
  for(size_t cnt = 0; cnt < subunits.size(); cnt++) {
    if (subunits[cnt] == pt) {
      return true;
    }
  }

  return false;
}

bool
Unit::operator==(const Unit& rhs)
{
  //cout << "Compare: \n";
  //print();
  //cout << "vs: \n";
  //rhs.print();
  // This will likely be slow.
  // Check if the pivot and all points match.
  if (pivot != rhs.pivot) {
    return false;
  }
  // Check for an equivalent # of subunits.
  if (subunits.size() != rhs.subunits.size()) {
    return false;
  }

  // For each subunit, check if there is a corresponding unit in rhs.
  for (size_t cnt = 0; cnt < subunits.size(); cnt++) {
    bool fnd = false;
    for (size_t cnt2 = 0; cnt2 < rhs.subunits.size(); cnt2++) {
      if (subunits[cnt] == rhs.subunits[cnt2]) {
        fnd = true;
        break;
      }
    }
    if (!fnd) {
      return false;
    }
  }

  return true;
}


//
//
//
//
//

Board::Board(size_t width, size_t height)
  : w(width),
    h(height),
    au(nullptr)
{
    vector<bool> rowvector(w);
    for(size_t i = 0; i < h; i++)
    {
        c.push_back(rowvector);
    }
}

Board::Board(const Board &obj)
  : w(obj.w),
    h(obj.h)

{
    c.clear();
    for(size_t i = 0; i < h; i++)
    {
        c.push_back(obj.c[i]);
    }

}

void
Board::lock()
{
  if (au == nullptr) {
    // nothing to do with no active unit
    return;
  }

  for (size_t cnt = 0; cnt < au->subunits.size(); cnt++) {
    const pii &sub = au->subunits[cnt];
    c[sub.second][sub.first] = true;
  }

  // Now there is no active unit.
  au = nullptr;
}

void
Board::doAct(act_cmd cmd) {
  if (au == nullptr) {
    // nothing to do with no active unit
    return;
  }

  // Copy the active unit and do the action with it.
  Unit testUnit(*au);
  switch(cmd) {
    case east:
      testUnit.e();
      break;
    case west:
      testUnit.w();
      break;
    case s_east:
      testUnit.se();
      break;
    case s_west:
      testUnit.sw();
      break;
    case rotate_cw:
      testUnit.rotate(true);
      break;
    case count_rotate:
      testUnit.rotate(false);
      break;
    default:
      return;
  }
  // If the new position is valid then execute it. Otherwise lock the au.
  if (val(testUnit)) {
    *au = testUnit;
  } else {
    lock();
    clearrows();
  }

}


void
Board::clearrows()
{
  for(size_t y = 0; y < h; y++)
  {
    bool rowFull = true;
    for(size_t x = 0; x < w; x++)
    {
      if (!c[y][x]) {
        rowFull = false;
        break;
      }
    }
    if (rowFull) {
      for(int back = y - 1; back >= 0; back--) {
        c[back+1] = c[back];
      }
      vector<bool> rowvector(w);
      c[0].assign(w, false);
    }
  }
}

void
Board::resize(size_t width, size_t height)
{
    c.clear();
    w = width;
    h = height;
    vector<bool> rowvector(w);
    for(size_t i = 0; i < h; i++)
    {
        c.push_back(rowvector);
    }
}

bool
Board::val(const Unit &ut) const
{
  // See if anything's outside the edges.
  if ((ut.left_x < 0) ||
      (ut.right_x >= w) ||
      (ut.top_y < 0) ||
      (ut.bot_y >= h)) {
    return false;
  }

  // See if any units overlap with a filled hex.
  for (size_t cnt = 0; cnt < ut.subunits.size(); cnt++) {
    const pii &sub = ut.subunits[cnt];
    if (c[sub.second][sub.first]) {
      return false;
    }
  }

  return true;
}


void
Board::print(ostream& os) const
{
    for(size_t i = 0; i < h; i++)
    {
      if (i & 1) {
          os << " ";
      }
      for(size_t j = 0; j < w; j++)
      {
          bool ispvt(false);
          bool isut(false);
          if (au != nullptr) {
            ispvt = ((au->pivot.first == j) && (au->pivot.second == i));
            isut = au->suAt(pii(j,i));
          }

          if (c[i][j]) {
            if (ispvt) {
              os << "@ " ;
            } else {
              os << "# " ;
            }
          } else if ((ispvt) && (isut)) {
            os << "& " ;
          } else if (ispvt) {
            os << "o " ;
          } else if (isut) {
            os << "+ " ;
          } else {
            os << ". " ;
          }
      }
      os << endl;
    }
    os << endl;
}


//
//
//
//
//


World::World()
  : initialBoard(0,0),
    board(0,0),
    id(0),
    sourcelength(0),
    currentSource(0),
    stepDelay(0),
    initialSeed(0),
    seed(0)
{
}

int
World::import(const char *filename)
{
  int i;
  char *line;
  char *token, *cp;
  char check[80];
  char pivot_check[80];
  char fileName[40];
  FILE *fp;
  int go;
  const char delimiters[]=" :=,\"{}[]";
  int unit_idx = 0;
  int width;
  int height;

  fstream inFile(filename, fstream::in);

  sprintf(fileName, "%s", filename);
  fp=fopen( fileName, "r");
  if(fp!=NULL)
  {
       line = readline(fp); 
       cp=strdup(line);
       token = strtok(cp, delimiters); /* height */
       //printf("first token = %s\n", token);
       height = atoi( strtok(NULL, delimiters));
       //printf("height=%d\n",height);
       strtok(NULL, delimiters);  /*width */
       width =  atoi( strtok(NULL, delimiters));

       board.resize(width, height);

       

       strtok(NULL, delimiters);/* sourceSeeds */
       strcpy(check,"units");
       while (true)
       {
         token = strtok(NULL, delimiters);
         //printf("++++ %s\n",token);
         if(strncmp(token,check,5)==0)
              break;
         seeds.push_back(atoi(token));
       }
       //printf("++++++++++seedNum=%lu\n", seeds.size());
       strtok(NULL, delimiters);/* units */
       /* skip units*/
       strcpy(check,"id");
       while(1)
       {
          token = strtok(NULL, delimiters); /* looking for id */
          if(token==NULL)
              break;
          if(strncmp(token,check,2)==0)
          {
              break;
          }
       }
       id = atoi( strtok(NULL, delimiters));

       /* filled */
       strtok(NULL, delimiters); /*filled*/
       strcpy(check,"sourceLength");
       while(1)
       {
          token = strtok(NULL, delimiters); /* looking for sourcelength */
          if(token==NULL)
          {
             break;
          }

          if(strncmp(token,check,10)==0)
          {
              break;
          }
          int x=atoi( strtok(NULL, delimiters));
          token = strtok(NULL, delimiters);
          int y=atoi( strtok(NULL, delimiters));

          board.c[y][x]=true;
       }
       sourcelength=atoi( strtok(NULL, delimiters));

       /* get unit */
       cp=strdup(line);
       /* first skip the first 6 token */
       strtok(cp, delimiters);
       for(i=0; i < 5+seeds.size(); i++)
           token = strtok(NULL, delimiters);

       strcpy(check,"members");
       strcpy(pivot_check,"pivot");
       while(1)
       {
           token = strtok(NULL, delimiters);
           if(strncmp(token,check,7)==0) /* get a unit members */
           {
               //printf("=========== new test cases %d\n", unit_idx);
               Unit newUnit;
               go = 1;
               while(go)
               {
                   token = strtok(NULL, delimiters);
                   if(strncmp(token,pivot_check,5)==0)
                   {
                      strtok(NULL, delimiters);
                      newUnit.pivot.first = atoi( strtok(NULL, delimiters));

                      strtok(NULL, delimiters);
                      newUnit.pivot.second = atoi( strtok(NULL, delimiters));
                      //printf("test case %d===== pivot, x %d, y %d\n",
                          //unit_idx, newUnit.pivot.first, newUnit.pivot.second);

                      go = 0;
                   }
                   else
                   {
                       pii subunit;
                       subunit.first = atoi( strtok(NULL, delimiters));
                       strtok(NULL, delimiters);
                       subunit.second = atoi( strtok(NULL, delimiters));
                       //printf("test case %d===== x %d, y %d\n",
                           //unit_idx,
                           //subunit.first, subunit.second);
                       newUnit.subunits.push_back(subunit);
                   }
               }
               newUnit.setBorders();
               //newUnit.print();
               //cout << endl;
               units.push_back(newUnit);
               unit_idx++;
           }
           else
           {
               break;
           }
       }
       //board.print();
       fclose(fp);
  }

  initialBoard = board;
  return 0;
}

size_t
World::nextUnit() {
  return (numberFromSeed(seed) % units.size());
}

bool
World::actNextUnit() {
  if (currentSource >= sourcelength) {
    return false;
  }
  // Get a copy from the source
  activeUnit = ( units[ nextUnit() ] );
  board.au = &activeUnit;
  size_t move_x = (board.w + activeUnit.left_x - activeUnit.right_x - 1) / 2;
  while (move_x > 0) {
    activeUnit.e();
    // board.print();
    move_x--;
  }

  currentSource++;
  seed = iterateRNG(seed);

  return true;
}

void
World::runAll()
{
  jsonHeader(std::cout);
  bool first = true;
  for (int rnum = 0; rnum < seeds.size(); rnum++) {
    // Reset the state
    initialSeed = seeds[rnum];
    board = initialBoard;
    currentSource = 0;

    string commands;
    commands += mark4();

    // Dump the output
    jsonOutput(first, id, seeds[rnum], commands, std::cout);
    first = false;
  }
  jsonFooter(std::cout);
}

// Dumb! Got us to about 140th place
string
World::mark1()
{
  string commands;

  // Run as many units as possible
  bool done(false);
  do {
    if (!actNextUnit()) {
      // Source is empty.
      break;
    }
    if (!board.val(activeUnit)) {
      // Board is full.
      break;
    }

    bool innerdone = false;
    do {
      fstream outputfile("board.txt", ios_base::out);
      board.print(outputfile);
      outputfile.close();

      Unit test(activeUnit);
      test.se();
      if (board.val(test)) {
        board.doAct(s_east);
        commands += 'm';
      } else {
        board.doAct(west);
        commands += '0';
      }

      if (board.au == nullptr) {
        break;
      } else {
        usleep(stepDelay);
      }
    } while (!innerdone);
  } while (!done);

  return commands;
}

string
World::mark2()
{
  string commands;

  // Run as many units as possible
  bool done(false);
  do {
    if (!actNextUnit()) {
      // Source is empty.
      break;
    }
    if (!board.val(activeUnit)) {
      // Board is full.
      break;
    }

    act_cmd lastCmd = idle;
    bool unitdone = false;
    do {
      if (stepDelay > 0) {
        fstream outputfile("board.txt", ios_base::out);
        board.print(outputfile);
        outputfile.close();
        usleep(stepDelay);
      }

      Unit test(activeUnit);
      test.se();
      if (board.val(test)) {
        board.doAct(s_east);
        lastCmd = s_east;
        commands += 'm';
      } else {
        test = activeUnit;
        test.sw();
        if (board.val(test)) {
          board.doAct(s_west);
          lastCmd = s_west;
          commands += 'a';
        } else {
          test = activeUnit;
          test.e();
          if ((lastCmd != west) && (board.val(test))) {
            board.doAct(east);
            lastCmd = east;
            commands += 'e';
          } else {
            test = activeUnit;
            test.w();
            if ((lastCmd != east) && (board.val(test))) {
              board.doAct(west);
              lastCmd = west;
              commands += '0';
            } else {
              // Go SW to lock
              board.doAct(s_west);
              lastCmd = s_west;
              commands += 'a';
            }
          }
        }
      }

      if (board.au == nullptr) {
        break;
      }
    } while (!unitdone);
  } while (!done);

  return commands;
}

bool
World::runString(string cmds, bool testOnly) {
  Unit testUnit(activeUnit);

  for (int cnt = 0; cnt < cmds.size(); cnt++) {
    char cmdch = cmds[cnt];
    act_cmd cmd = charToCmd(cmdch);
    
    switch(cmd) {
      case east:
        testUnit.e();
        break;
      case west:
        testUnit.w();
        break;
      case s_east:
        testUnit.se();
        break;
      case s_west:
        testUnit.sw();
        break;
      case rotate_cw:
        testUnit.rotate(true);
        break;
      case count_rotate:
        testUnit.rotate(false);
        break;
      default:
        return false;
    }
    if (!board.val(testUnit)) {
      return false;
    }
  }

  if (testOnly) {
    return true;
  }

  // Everything checked out, so run the commands.
  for (int cnt = 0; cnt < cmds.size(); cnt++) {
    char cmdch = cmds[cnt];
    act_cmd cmd = charToCmd(cmdch);
    board.doAct(cmd);
  }

  return true;
}

string
World::mark3()
{
  string commands;

  // Run as many units as possible
  bool done(false);
  do {
    if (!actNextUnit()) {
      // Source is empty.
      break;
    }
    if (!board.val(activeUnit)) {
      // Board is full.
      break;
    }

    act_cmd lastCmd = idle;
    bool unitdone = false;
    do {
      if (stepDelay > 0) {
        fstream outputfile("board.txt", ios_base::out);
        board.print(outputfile);
        outputfile.close();
        usleep(stepDelay);
      }

      // First try to run 'ei!a' when possible.
      // The extra a is to avoid loops.
      if(runString("ei!a", false)) {
        commands += "ei!a";
        lastCmd = s_west;
      } else {
        Unit test(activeUnit);
        test.se();
        if (board.val(test)) {
          board.doAct(s_east);
          lastCmd = s_east;
          commands += 'm';
        } else {
          test = activeUnit;
          test.sw();
          if (board.val(test)) {
            board.doAct(s_west);
            lastCmd = s_west;
            commands += 'a';
          } else {
            test = activeUnit;
            test.e();
            if ((lastCmd != west) && (board.val(test))) {
              board.doAct(east);
              lastCmd = east;
              commands += 'e';
            } else {
              test = activeUnit;
              test.w();
              if ((lastCmd != east) && (board.val(test))) {
                board.doAct(west);
                lastCmd = west;
                commands += '0';
              } else {
                // Go SW to lock
                board.doAct(s_west);
                lastCmd = s_west;
                commands += 'a';
              }
            }
          }
        }
      }

      if (board.au == nullptr) {
        break;
      }
    } while (!unitdone);
  } while (!done);

  return commands;
}

int
World::tryAllPP(int start)
{
  for (int cnt = 0; cnt < numpp; cnt++) {
    int idx = (cnt + start) % numpp;
    if (runString(pp[idx], false)) {
      return idx;
    }
  }
  return -1;
}

string
World::mark4()
{
  string commands;
  int nextpp = 0;

  // Run as many units as possible
  bool done(false);
  do {
    if (!actNextUnit()) {
      // Source is empty.
      break;
    }
    if (!board.val(activeUnit)) {
      // Board is full.
      break;
    }

    act_cmd lastCmd = idle;
    bool unitdone = false;
    do {
      if (stepDelay > 0) {
        fstream outputfile("board.txt", ios_base::out);
        board.print(outputfile);
        outputfile.close();
        usleep(stepDelay);
      }

      // First try to run a phrase when possible.

      nextpp++;
      nextpp %= numpp;
      int idx = tryAllPP(nextpp);
      if (idx != -1) {
        commands += pp[idx];
        lastCmd = s_west;
      } else {
        Unit test(activeUnit);
        test.se();
        if (board.val(test)) {
          board.doAct(s_east);
          lastCmd = s_east;
          commands += 'm';
        } else {
          test = activeUnit;
          test.sw();
          if (board.val(test)) {
            board.doAct(s_west);
            lastCmd = s_west;
            commands += 'a';
          } else {
            test = activeUnit;
            test.e();
            if ((lastCmd != west) && (board.val(test))) {
              board.doAct(east);
              lastCmd = east;
              commands += 'e';
            } else {
              test = activeUnit;
              test.w();
              if ((lastCmd != east) && (board.val(test))) {
                board.doAct(west);
                lastCmd = west;
                commands += '0';
              } else {
                // Go SW to lock
                board.doAct(s_west);
                lastCmd = s_west;
                commands += 'a';
              }
            }
          }
        }
      }

      if (board.au == nullptr) {
        break;
      }
    } while (!unitdone);
  } while (!done);

  return commands;
}
