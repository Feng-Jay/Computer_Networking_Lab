#include <stdio.h>

extern struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent 
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };

extern int TRACE;
extern int YES;
extern int NO;

struct distance_table 
{
  int costs[4][4];
} dt2;


int neighbors[4]={3,1,0,2};
/* students to write the following two routines, and maybe some others */

void rtinit2() 
{
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++)
    dt2.costs[i][j]=999;
  }
  for(int i=0;i<4;i++){
    dt2.costs[2][i]=neighbors[i];
  }

  for(int i=0;i<4;i++){
    if(neighbors[i]!=999&&i!=2){
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


void rtupdate2(struct rtpkt *rcvdpkt)
{
  int sourceid=rcvdpkt->sourceid;
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      dt2.costs[i][j]=min(dt2.costs[i][j],dt2.costs[i][sourceid]+rcvdpkt->mincost[j]);
    }
  }
  for(int i=0;i<4;i++){
    if(neighbors[i]!=999&&i!=2){
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
  printf("                via     \n");
  printf("   D2 |    0     1    3 \n");
  printf("  ----|-----------------\n");
  printf("     0|  %3d   %3d   %3d\n",dtptr->costs[0][0],
	 dtptr->costs[0][1],dtptr->costs[0][3]);
  printf("dest 1|  %3d   %3d   %3d\n",dtptr->costs[1][0],
	 dtptr->costs[1][1],dtptr->costs[1][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][0],
	 dtptr->costs[3][1],dtptr->costs[3][3]);
}







