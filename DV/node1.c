#include "router.h"
#include <stdio.h>
#include <stdlib.h>

int connectcosts1[4] = { 1,  0,  1, 999 };

struct distance_table 
{
  int costs[4][4];
} dt1;



int neighbors1[4]={1,0,1,999};
/* students to write the following two routines, and maybe some others */
void rtinit1() 
{
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++)
      dt1.costs[i][j]=999;
  }
  for(int i=0;i<4;i++){
    dt1.costs[1][i]=neighbors1[i];
  }

  for(int i=0;i<4;i++){
    if(neighbors1[i]!=999&&i!=1){
      struct rtpkt pkt;
      pkt.sourceid=1;
      pkt.destid=i;
      for(int j=0;j<4;j++){
        pkt.mincost[j]=dt1.costs[1][j];
      }
      tolayer2(pkt);
    }
  }
  return ;
}

void printdt1(struct distance_table *dtptr)
{
  printf("This is %d to other nodes' distances.\n", 1);
  printf("---------------------------------------\n");
  printf("Node|- 0 ----- 1 ------ 2 ----- 3 ----------\n");
  printf("D%d  |  %d       %d        %d       %d         \n", 1,
  dtptr->costs[1][0], dtptr->costs[1][1], 
  dtptr->costs[1][2], dtptr->costs[1][3]);
}
void rtupdate1(struct rtpkt *rcvdpkt)
{

  int sourceid=rcvdpkt->sourceid;
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      dt1.costs[i][j]=min(dt1.costs[i][j],dt1.costs[i][sourceid]+rcvdpkt->mincost[j]);
    }
  }
  for(int i=0;i<4;i++){
    if(neighbors1[i]!=999&&i!=1){
      struct rtpkt pkt;
      pkt.sourceid=1;
      pkt.destid=i;
      for(int j=0;j<4;j++){
        pkt.mincost[j]=dt1.costs[1][j];
      }
      tolayer2(pkt);
    }
  }
   printdt1(&dt1);
  return ;
}





/* called when cost from 1 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
void linkhandler1(int linkid,int newcost)     
{
  neighbors1[linkid]=newcost;
  rtinit0();
  rtinit1();
  rtinit2();
  rtinit3();
}


