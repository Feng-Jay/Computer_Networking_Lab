#include "router.h"
#include <stdio.h>
#include <stdlib.h>

struct distance_table 
{
  int costs[4][4];
} dt2;


int neighbors2[4]={3,1,0,2};
/* students to write the following two routines, and maybe some others */

void rtinit2() 
{
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++)
    dt2.costs[i][j]=999;
  }
  for(int i=0;i<4;i++){
    dt2.costs[2][i]=neighbors2[i];
  }

  for(int i=0;i<4;i++){
    if(neighbors2[i]!=999&&i!=2){
      struct rtpkt pkt;
      pkt.sourceid=2;
      pkt.destid=i;
      for(int j=0;j<4;j++){
        pkt.mincost[j]=dt2.costs[2][j];
      }
      tolayer2(pkt);
    }
  }
  return ;
}

void printdt2(struct distance_table *dtptr)
{
  printf("This is %d to other nodes' distances.\n", 2);
  printf("---------------------------------------\n");
  printf("Node|- 0 ----- 1 ------ 2 ----- 3 ----------\n");
  printf("D%d  |  %d       %d        %d       %d         \n", 2,
  dtptr->costs[2][0], dtptr->costs[2][1], 
  dtptr->costs[2][2], dtptr->costs[2][3]);
}

void rtupdate2(struct rtpkt *rcvdpkt)
{
  int sourceid=rcvdpkt->sourceid;
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      dt2.costs[i][j]=min(dt2.costs[i][j],dt2.costs[i][sourceid]+rcvdpkt->mincost[j]);
    }
  }
  for(int i=0;i<4;i++){
    if(neighbors2[i]!=999&&i!=2){
      struct rtpkt pkt;
      pkt.sourceid=2;
      pkt.destid=i;
      for(int j=0;j<4;j++){
        pkt.mincost[j]=dt2.costs[2][j];
      }
      tolayer2(pkt);
    }
  }
  printdt2(&dt2);
  return ;
}










