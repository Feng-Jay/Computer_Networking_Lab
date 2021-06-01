#include <router.h>

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
} dt3;


int neighbors[4]={7,999,2,0};
/* students to write the following two routines, and maybe some others */

void rtinit3() 
{
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++)
    dt3.costs[i][j]=999;
  }
  for(int i=0;i<4;i++){
    dt3.costs[3][i]=neighbors[i];
  }

  for(int i=0;i<4;i++){
    if(neighbors[i]!=999&&i!=3){
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


void rtupdate3(struct rtpkt *rcvdpkt) 
{
  int sourceid=rcvdpkt->sourceid;
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      dt3.costs[i][j]=min(dt3.costs[i][j],dt3.costs[i][sourceid]+rcvdpkt->mincost[j]);
    }
  }
  for(int i=0;i<4;i++){
    if(neighbors[i]!=999&&i!=3){
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
  printf("             via     \n");
  printf("   D3 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 1|  %3d   %3d\n",dtptr->costs[1][0], dtptr->costs[1][2]);
  printf("     2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);

}







