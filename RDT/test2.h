/* ******************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for PA2, unidirectional or bidirectional
   data transfer protocols (from A to B. Bidirectional transfer of data
   is for extra credit and is not required).  Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets will be delivered in the order in which they were sent
     (although some can be lost).
**********************************************************************/

struct event {
   float evtime;           /* event time */
   int evtype;             /* event type code */
   int eventity;           /* entity where event occurs */
   struct pkt *pktptr;     /* ptr to packet (if any) assoc w/ this event */
   struct event *prev;
   struct event *next;
 };

/* possible events: */
#define  TIMER_INTERRUPT 0  
#define  FROM_LAYER5     1
#define  FROM_LAYER3     2

#define  OFF             0
#define  ON              1
#define   A    0
#define   B    1



int TRACE = 0;             /* for my debugging */
int nsim = 0;              /* number of messages from 5 to 4 so far */ 
int nsimmax = 10;           /* number of msgs to generate, then stop */
float time = 10.000;
float lossprob = 0.000;            /* probability that a packet is dropped  */
float corruptprob = 0.000;         /* probability that one bit is packet is flipped */
float lambda = 20.0;              /* arrival rate of messages from layer 5 */   
int   ntolayer3;           /* number sent into layer 3 */
int   nlost = 0.000;               /* number lost in media */
int ncorrupt = 0.500;              /* number corrupted by media*/

#define BIDIRECTIONAL 0    /* change to 1 if you're doing extra credit */
                           /* and write a routine called B_output */

/* a "msg" is the data unit passed from layer 5 (teachers code) to layer  */
/* 4 (students' code).  It contains the data (characters) to be delivered */
/* to layer 5 via the students transport level protocol entities.         */
struct msg {
  char data[20];
};

/* a packet is the data unit passed from layer 4 (students code) to layer */
/* 3 (teachers code).  Note the pre-defined packet structure, which all   */
/* students must follow. */
struct pkt {
  //  // judge pkt is valid, only used in initialize.
  //  int valid;
   int seqnum;
   int acknum;
   int checksum;
   char payload[20];
};

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

#define ACK ((int)1)
#define NAK ((int)0)


// the size of windows
// const int window_size = 5; 
#define WINDOW_SIZE ((int)5)

// status of a
int base;

int nextseqnum;

// status of b, which means expected seqnum
int status_b;

// functions define
void A_output(struct msg message);
void B_output(struct msg message);
void A_input(struct pkt packet);
void A_timerinterrupt();
void A_init();
void B_input(struct pkt packet);
void B_timerinterrupt();
void B_init();
void init();
float jimsrand();
void generate_next_arrival();
void insertevent(struct event *p);
void printevlist();
void stoptimer(int AorB);
void starttimer(int AorB, float increment);
void tolayer3(int AorB, struct pkt packet);
void tolayer5(int AorB, char datasent[20]);

void init_packet(struct pkt* packet) {
    for(int i=0; i<20; i++) packet->payload[i] = 0;
    packet->acknum = 0;
    packet->checksum = 0;
    packet->seqnum = 0;
}