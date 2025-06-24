#ifndef RPL_ICMP6_H
#define RPL_ICMP6_H

#include "contiki.h"

// Declare a function to start the ICMP6 client process
void rpl_icmp6_init(void);
void send_dis_flood(void);

#endif /* RPL_ICMP6_H */

