#include "contiki.h"
#include "rpl-icmp6.h"

PROCESS(rpl_icmp6_client_process, "RPL ICMP6 Client Process");
AUTOSTART_PROCESSES(&rpl_icmp6_client_process);

PROCESS_THREAD(rpl_icmp6_client_process, ev, data)
{
  PROCESS_BEGIN();

  rpl_icmp6_init();  // Start ICMP6 client process

  PROCESS_END();
}

