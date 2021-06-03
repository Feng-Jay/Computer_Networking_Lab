#include "router.h"
#include <stdio.h>
#include <stdlib.h>

struct distance_table 
{
  int costs[4][4];
} dt0;

int neighbors0[4]={0,1,3,7};
/* students to write the following two routines, and maybe some others */

void rtinit0() 
{
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            dt0.costs[i][j]=999;
        }
    }
    for(int i=0;i<4;i++)
    dt0.costs[0][i]=neighbors0[i];//本地初始化，将0结点直达结点距离设置好，其他先设置为999
    //开始向直接相连的结点:node1,2,3发送本地初始化内容.
    for(int i=1;i<4;i++){//向node0的各个邻居发送初始化内容
      struct rtpkt pkt;
      pkt.sourceid=0;
      pkt.destid=i;//发送给1,2,3结点
      for(int j=0;j<4;j++){
        pkt.mincost[j]=dt0.costs[0][j];
      }
      tolayer2(pkt);
    }
}

void printdt0(struct distance_table *dtptr)
{
  printf("This is %d to other nodes' distances.\n", 0);
  printf("---------------------------------------\n");
  printf("Node|- 0 ----- 1 ------ 2 ----- 3 ----------\n");
  printf("D%d  |  %d       %d        %d       %d         \n", 0,
  dtptr->costs[0][0], dtptr->costs[0][1], 
  dtptr->costs[0][2], dtptr->costs[0][3]);
}


void rtupdate0(struct rtpkt *rcvdpkt)
{
  int sourceid=rcvdpkt->sourceid;
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      dt0.costs[i][j]=min(dt0.costs[i][j],dt0.costs[i][sourceid]+rcvdpkt->mincost[j]);
    }
  }
  for(int i=1;i<4;i++){//向node0的各个邻居发送内容
      struct rtpkt pkt;
      pkt.sourceid=0;
      pkt.destid=i;//将更新完的信息发送给1,2,3结点
      for(int j=0;j<4;j++){
        pkt.mincost[j]=dt0.costs[0][j];
      }
      tolayer2(pkt);
    }
    printdt0(&dt0);
}



/* called when cost from 0 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
void linkhandler0(int linkid, int newcost)   
{
  neighbors0[linkid]=newcost;
  rtinit0();
  rtinit1();
  rtinit2();
  rtinit3();
}

