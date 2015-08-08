#include "parser.hpp"

#include <bits/stdc++.h>
#include <stdio.h>
#include <iostream>
using namespace std;


#define  MAX_ROW_NUM   100
#define  MAX_COLUM_NUM 100
#define  MAX_UNIT_NUM  100
#define  MAX_SEED_NUM  100
#define  MAX_FILLED_NUM 10000
#define  MAX_INPUT_SIZE 100000
/* filled board data */

int board[MAX_COLUM_NUM][MAX_ROW_NUM];
int width;
int height;
int sourcelength;
unsigned int sourceSeeds[MAX_SEED_NUM];
int seedNum;
int id;
int sub_cell_num[MAX_UNIT_NUM];
int unit_idx;

char move_w[6] = {'p','\'','!','.','0','3'};
char move_e[6] = {'b','c','e','f','y','2'};
char move_sw[6] = {'a','g','h','i','j','4'};
char move_se[6] = {'l','m','n','o',' ','5'};
char rot_clock[6] = {'d','q','r','v','z','1'};
char rot_count[6] = {'k','s','t','u','w','x'};
char ignore[3] = {'\t','\n','\r'};

int unit_x[MAX_UNIT_NUM][MAX_ROW_NUM];
int unit_y[MAX_UNIT_NUM][MAX_ROW_NUM];
int pivot_x[MAX_UNIT_NUM];
int pivot_y[MAX_UNIT_NUM];
 
void draw_board(void)
{
    fstream outFile("board.txt", fstream::out);
    int r,c;
    for(r=0;r<height;r++)
    {
        if(r%2==1)
           outFile << " ";
        for(c=0;c<width;c++)
        {
            if(board[c][r]==1)
                outFile << "# ";
            else
                outFile << ". ";          
        }
        outFile << endl;
    }
    outFile.close();
}

void draw_unit(void)
{
    int x,y, i, j;
    int is_pivot, is_cell=0;
    fstream outFile("unit.txt", fstream::out);
    outFile << "width: " << width << " height: "<< height << " sourcelength: "<<sourcelength << " id: "<<id<<" unit number: "<<unit_idx<<endl;
    for(j=0;j<unit_idx;j++)
    {
        outFile << "unit " << j << ":==========="<<endl;
        for(y=0;y<height;y++)
        {
            if(y%2==1)
               outFile << " ";
            for(x=0;x<width;x++)
            {
                is_pivot=0;
                is_cell=0;
                if((pivot_x[j]==x)&&(pivot_y[j]==y)) /* pivot point */
                    is_pivot=1;
                for(i=0;i<sub_cell_num[j];i++)
                {
                    if((unit_y[j][i]==y)&&(unit_x[j][i]==x)) /* unit cell point */
                        is_cell=1;
                }
                if((is_pivot==1) && (is_cell==1))
                    outFile << "@ ";
                else if(is_pivot==1)
                    outFile << "o ";
                else if(is_cell==1)
                    outFile<<"+ ";
                else
                    outFile<<"  ";
            }
            outFile << endl;
        }
    }
    outFile.close();

    
}

char masterStr[MAX_INPUT_SIZE];

char *readline(FILE *fp)
{
   char str[MAX_INPUT_SIZE];
   char *line = masterStr;
   
   if(fp==NULL)
       return 0;
   if(fgets(str,sizeof(str),fp) != NULL)
   {
      // strip trailing '\n' if it exists
      int len = strlen(str)-1;
      if(str[len] == '\n') 
      {
         str[len] = 0;
      }
      strncpy(line, str, MAX_INPUT_SIZE-1);
      return line;
   }   
   return 0;
}

int readFile(const char *filename)
{
  int i,j, x, y, sub_cell;
  char *line;
  char *token, *cp;
  char check[80];
  char pivot_check[80];
  char fileName[40];
  FILE *fp;
  int go;
  const char delimiters[]=" :=,\"{}[]";

  fstream inFile(filename, fstream::in);
  memset(&board, 0, sizeof(board));
  memset(&sourceSeeds, 0, sizeof(sourceSeeds));
  seedNum=0;
  for(i=0;i<MAX_UNIT_NUM;i++)
  {
      for(j=0;j<MAX_ROW_NUM;j++)
      {
         unit_x[i][j]=-1;
         unit_y[i][j]=-1;
         pivot_x[i]=-1;
         pivot_y[i]=-1;
      }
  }
  sprintf(fileName, "%s", filename);
  fp=fopen( fileName, "r");
  unit_idx=0;          
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
       strtok(NULL, delimiters);/* sourceSeeds */
       strcpy(check,"units");
       for(i=0;i<MAX_SEED_NUM;i++)
       {
         token = strtok(NULL, delimiters);
         printf("++++ %s\n",token);
         if(strncmp(token,check,5)==0)
              break;
         if(i==MAX_SEED_NUM-1)
         {
              printf("WARNING!!!!!!!  SEED number exceed to MAX SEED NUM. ABORT!!!!!!!!!!\n");
              return 0;
         }
         sourceSeeds[i] =  atoi(token);
       }
       seedNum = i;
       printf("++++++++++seedNum=%d\n", seedNum);
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
       i=0;
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
          x=atoi( strtok(NULL, delimiters));
          token = strtok(NULL, delimiters);
          y=atoi( strtok(NULL, delimiters));
          if(x>MAX_COLUM_NUM-2)
          {
              printf("WARNING!!!!!!!! x = %d exceed the MAX COLUM NUM abort!!!!!\n", x);
              return 0;
          }
          if(y>MAX_ROW_NUM-2)
          {
              printf("WARNING!!!!!!! y = %d exceed to MAX ROW NUM. ABORT!!!!!!!!!!\n", y);
              return 0;
          }
          board[x][y]=1;
          //printf("filled %d: x: %d, y %d\n", i,  filled_x[i],filled_y[i]);
          i++;
       }
       sourcelength=atoi( strtok(NULL, delimiters));
       draw_board();


       /* get unit */
       cp=strdup(line);
       /* first skip the first 6 token */
       strtok(cp, delimiters);
       for(i=0;i<5+seedNum;i++)
           token = strtok(NULL, delimiters);
       strcpy(check,"members");
       strcpy(pivot_check,"pivot");
       while(1)
       {
           sub_cell=0;          
           token = strtok(NULL, delimiters);
           if(strncmp(token,check,7)==0) /* get a unit members */
           {
               if(unit_idx>=MAX_UNIT_NUM)
               {
                           printf("WARNING!!!!!!! Unit number exceed MAX. ABORT!!!!!!!!!!\n");
                           return 0;
               }
               printf("=========== new test cases %d\n", unit_idx);
               go = 1;
               while(go)
               {
                   token = strtok(NULL, delimiters);
                   if(strncmp(token,pivot_check,5)==0)
                   {
                      strtok(NULL, delimiters);
                      pivot_x[unit_idx] = atoi( strtok(NULL, delimiters));
                      if(pivot_x[unit_idx]>=width)
                      {
                           printf("WARNING!!!!!!! %d: pivot_x %d exceed to width %d. ABORT!!!!!!!!!!\n", unit_idx, pivot_x[unit_idx], width);
                           return 0;

                      }
                      strtok(NULL, delimiters);
                      pivot_y[unit_idx] = atoi( strtok(NULL, delimiters));
                      if(pivot_y[unit_idx]>=height)
                      {
                           printf("WARNING!!!!!!! %d: pivot_y %d exceed to height %d. ABORT!!!!!!!!!!\n", unit_idx, pivot_y[unit_idx], height);
                           return 0;
                      }
                       printf("test case %d===== pivot, x %d, y %d\n", unit_idx, pivot_x[unit_idx], pivot_y[unit_idx]);

                      go = 0;
                   }
                   else
                   {
                       unit_x[unit_idx][sub_cell] = atoi( strtok(NULL, delimiters));
                       if(unit_x[unit_idx][sub_cell]>=width)
                       {
                           printf("WARNING!!!!!!! %d:  unit[%d].x %d exceed to width %d. ABORT!!!!!!!!!!\n", unit_idx,sub_cell, unit_x[unit_idx][sub_cell], width);
                           return 0;

                       }
                       strtok(NULL, delimiters);
                       unit_y[unit_idx][sub_cell] = atoi( strtok(NULL, delimiters));
                       if(unit_y[unit_idx][sub_cell]>=height)
                       {
                           printf("WARNING!!!!!!! %d: unit[%d].y %d exceed to height %d. ABORT!!!!!!!!!!\n", unit_idx,sub_cell, unit_y[unit_idx][sub_cell], height);
                           return 0;

                       }
                       printf("test case %d===== sub_cell %d, x %d, y %d\n", unit_idx, sub_cell,  unit_x[unit_idx][sub_cell], unit_y[unit_idx][sub_cell]);
                       sub_cell++;
                       sub_cell_num[unit_idx]=sub_cell;
                       if(sub_cell==MAX_UNIT_NUM)
                       {
                              printf("WARNING!!!!!!! sub_cell exceed to MAX NUM 100. ABORT!!!!!!!!!!\n");
                              return 0;
                       }
                   }
               }
               unit_idx++;
           }
           else
           {
               break;
           }
       }
       draw_unit();
       fclose(fp);
  }

   return 0;

}
