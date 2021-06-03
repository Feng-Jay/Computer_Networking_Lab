#include "router.h"
#include <stdio.h>
#include <stdlib.h>

// void printdt3(struct distance_table *dtptr);

struct distance_table 
{
  int costs[4][4];
} dt3;


int neighbors3[4]={7,999,2,0};
/* students to write the following two routines, and maybe some others */

void rtinit3() 
{
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++)
    dt3.costs[i][j]=999;
  }
  for(int i=0;i<4;i++){
    dt3.costs[3][i]=neighbors3[i];
  }

  for(int i=0;i<4;i++){
    if(neighbors3[i]!=999&&i!=3){
      struct rtpkt pkt;
      pkt.sourceid=3;
      pkt.destid=i;
      for(int j=0;j<4;j++){
        pkt.mincost[j]=dt3.costs[3][j];
      }
      tolayer2(pkt);
    }
  }
  return ;
}

void printdt3(struct distance_table *dtptr)
{
  printf("This is %d to other nodes' distances.\n", 3);
  printf("---------------------------------------\n");
  printf("Node|- 0 ----- 1 ------ 2 ----- 3 ----------\n");
  printf("D%d  |  %d       %d        %d       %d         \n", 3,
  dtptr->costs[3][0], dtptr->costs[3][1], 
  dtptr->costs[3][2], dtptr->costs[3][3]);
}
void rtupdate3(struct rtpkt *rcvdpkt) 
{
  int sourceid=rcvdpkt->sourceid;
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      dt3.costs[i][j]=min(dt3.costs[i][j],dt3.costs[i][sourceid]+rcvdpkt->mincost[j]);
    }
  }
  for(int i=0;i<4;i++){
    if(neighbors3[i]!=999&&i!=3){
      struct rtpkt pkt;
      pkt.sourceid=3;
      pkt.destid=i;
      for(int j=0;j<4;j++){
        pkt.mincost[j]=dt3.costs[3][j];
      }
      tolayer2(pkt);
    }
  }
   printdt3(&dt3);
  return ;
}










