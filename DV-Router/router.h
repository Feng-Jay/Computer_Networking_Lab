#include <stdio.h>
#define LINKCHANGES 1 
/* ******************************************************************
Programming assignment 3: implementing distributed, asynchronous,
                          distance vector routing.

THIS IS THE MAIN ROUTINE.  IT SHOULD NOT BE TOUCHED AT ALL BY STUDENTS!

**********************************************************************/


/* a rtpkt is the packet sent from one routing update process to
   another via the call tolayer3() */
struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent 
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };

int TRACE = 1;             /* for my debugging */
int YES = 1;
int NO = 0;

/*functions*/
void init();
float jimsrand();
void insertevent(struct event *p);
void printevlist();
void tolayer2(struct rtpkt packet);