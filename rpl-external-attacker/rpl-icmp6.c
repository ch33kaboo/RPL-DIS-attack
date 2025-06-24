/*
 * Copyright (c) 2010, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \addtogroup rpl-lite
 * @{
 *
 * \file
 *         ICMP6 I/O for RPL control messages.
 *
 * \author Joakim Eriksson <joakime@sics.se>, Nicolas Tsiftes <nvt@sics.se>,
 * Simon Duquennoy <simon.duquennoy@inria.fr>
 * Contributors: Niclas Finne <nfi@sics.se>, Joel Hoglund <joel@sics.se>,
 *               Mathieu Pouillot <m.pouillot@watteco.com>,
 *               George Oikonomou <oikonomou@users.sourceforge.net> (multicast)
 */

#include "contiki.h"
#include "net/ipv6/uip-icmp6.h"
#include "sys/energest.h"
#include <stdio.h>

void send_dis_flood(void)
{
  /* Send DIS message to all nodes (multicast) */
  uip_ipaddr_t addr;
  /* ff02::1a is the all-RPL-nodes multicast address */
  uip_ip6addr(&addr, 0xff02,0,0,0,0,0,0,0x1a);
  uip_icmp6_send(&addr, 155, 0, 2); // 155 = ICMP6_RPL, 0 = RPL_CODE_DIS
}

PROCESS(energy_log_process, "Energy Log Process");
PROCESS_THREAD(energy_log_process, ev, data)
{
  static struct etimer timer;
  PROCESS_BEGIN();

  energest_flush();
  printf("[ENERGY] CPU: %lu LPM: %lu TX: %lu RX: %lu\n",
    (unsigned long)energest_type_time(ENERGEST_TYPE_CPU),
    (unsigned long)energest_type_time(ENERGEST_TYPE_LPM),
    (unsigned long)energest_type_time(ENERGEST_TYPE_TRANSMIT),
    (unsigned long)energest_type_time(ENERGEST_TYPE_LISTEN)
  );

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 10);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    energest_flush();
    printf("[ENERGY] CPU: %lu LPM: %lu TX: %lu RX: %lu\n",
      (unsigned long)energest_type_time(ENERGEST_TYPE_CPU),
      (unsigned long)energest_type_time(ENERGEST_TYPE_LPM),
      (unsigned long)energest_type_time(ENERGEST_TYPE_TRANSMIT),
      (unsigned long)energest_type_time(ENERGEST_TYPE_LISTEN)
    );
  }

  PROCESS_END();
}

void rpl_icmp6_init(void) {}
void rpl_icmp6_dio_output(void) {}
void rpl_icmp6_dis_output(void) {}
void rpl_icmp6_dao_output(void) {}

/** @}*/
