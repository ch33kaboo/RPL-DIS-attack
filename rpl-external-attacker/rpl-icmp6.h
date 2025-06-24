#ifndef RPL_ICMP6_H
#define RPL_ICMP6_H

#include "contiki.h"

void send_dis_flood(void);
void rpl_icmp6_init(void);
void rpl_icmp6_dio_output(void);
void rpl_icmp6_dis_output(void);
void rpl_icmp6_dao_output(void);

#endif /* RPL_ICMP6_H */

