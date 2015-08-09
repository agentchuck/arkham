#include "world.hpp"

#include <iostream>
#include <bits/stdc++.h>
#include <stdio.h>
#include <iostream>

#include "parser.hpp"

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

void
Unit::print()
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
        cout << "  " ;
      }
    }
    cout << endl;
  }

}


//
//
//
//
//

Board::Board(size_t width, size_t height)
  : w(width),
    h(height)
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


void
Board::print()
{
    for(size_t i = 0; i < h; i++)
    {
      if (i & 1) {
          cout << " ";
      }
      for(size_t j = 0; j < w; j++)
      {
          cout << (c[i][j] ? "* " : ". ") ;
      }
      cout << endl;
    }
    cout << endl;
}


//
//
//
//
//


World::World()
  : board(0,0),
    id(0),
    sourcelength(0),
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
       printf("first token = %s\n", token);
       height = atoi( strtok(NULL, delimiters));
       printf("height=%d\n",height);
       strtok(NULL, delimiters);  /*width */
       width =  atoi( strtok(NULL, delimiters));

       board.resize(width, height);

       

       strtok(NULL, delimiters);/* sourceSeeds */
       strcpy(check,"units");
       while (true)
       {
         token = strtok(NULL, delimiters);
         printf("++++ %s\n",token);
         if(strncmp(token,check,5)==0)
              break;
         seeds.push_back(atoi(token));
       }
       printf("++++++++++seedNum=%lu\n", seeds.size());
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
               printf("=========== new test cases %d\n", unit_idx);
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
                      // printf("test case %d===== pivot, x %d, y %d\n",
                          // unit_idx, newUnit.pivot.first, newUnit.pivot.second);

                      go = 0;
                   }
                   else
                   {
                       pii subunit;
                       subunit.first = atoi( strtok(NULL, delimiters));
                       strtok(NULL, delimiters);
                       subunit.second = atoi( strtok(NULL, delimiters));
                       // printf("test case %d===== sub_cell %d, x %d, y %d\n",
                           // unit_idx, sub_cell,
                           // subunit.first, subunit.second);
                       newUnit.subunits.push_back(subunit);
                   }
               }
               newUnit.setBorders();
               newUnit.print();
               cout << endl;
               units.push_back(newUnit);
               unit_idx++;
           }
           else
           {
               break;
           }
       }
       board.print();
       fclose(fp);
  }

   return 0;

}

size_t
World::nextUnit() {
  return (numberFromSeed(seed) % units.size());
}
