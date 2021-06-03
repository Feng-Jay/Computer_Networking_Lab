#ifndef _ROUTER_H_
#define _ROUTER_H_

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

struct event {
   float evtime;           /* event time */
   int evtype;             /* event type code */
   int eventity;           /* entity where event occurs */
   struct rtpkt *rtpktptr; /* ptr to packet (if any) assoc w/ this event */
   struct event *prev;
   struct event *next;
};

extern int TRACE;             /* for my debugging */
extern int YES;
extern int NO;




/*functions*/
void init();
float jimsrand();
void insertevent(struct event *p);
void printevlist();
void tolayer2(struct rtpkt packet);

/*nodes functions*/
void rtinit0();
void rtupdate0(struct rtpkt *rcvdpkt);
void linkhandler0(int linkid, int newcost);

void rtinit1();
void rtupdate1(struct rtpkt *rcvdpkt);
void linkhandler1(int linkid,int newcost);

void rtinit2();
void rtupdate2(struct rtpkt *rcvdpkt);

void rtinit3();
void rtupdate3(struct rtpkt *rcvdpkt);

extern int min(int a, int b);
#endif