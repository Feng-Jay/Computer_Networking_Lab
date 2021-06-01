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

int connectcosts1[4] = { 1,  0,  1, 999 };

struct distance_table 
{
  int costs[4][4];
} dt1;



int neighbors[4]={1,0,1,999};
/* students to write the following two routines, and maybe some others */
void rtinit1() 
{
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++)
      dt1.costs[i][j]=999;
  }
  for(int i=0;i<4;i++){
    dt1.costs[1][i]=neighbors[i];
  }

  for(int i=0;i<4;i++){
    if(neighbors[i]!=999&&i!=1){
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


void rtupdate1(struct rtpkt *rcvdpkt)
{
  int sourceid=rcvdpkt->sourceid;
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      dt1.costs[i][j]=min(dt1.costs[i][j],dt1.costs[i][sourceid]+rcvdpkt->mincost[j]);
    }
  }
  for(int i=0;i<4;i++){
    if(neighbors[i]!=999&&i!=1){
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


void printdt1(dtptr)
  struct distance_table *dtptr;
  
{
  printf("             via   \n");
  printf("   D1 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);
  printf("     3|  %3d   %3d\n",dtptr->costs[3][0], dtptr->costs[3][2]);

}


/* called when cost from 1 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
void linkhandler1(int linkid,int newcost)     
{
}


