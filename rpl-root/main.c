/*
 * This file is a wrapper to initialize and run the RPL root node.
 * It was added because Contiki Cooja cannot run 'rpl-dag-root.c' directly.
 */

#include "contiki.h"
#include "net/netstack.h"
#include "rpl-dag-root.h"

PROCESS(wrapper_rpl_dag_root_process, "Wrapper for RPL DAG Root");
AUTOSTART_PROCESSES(&wrapper_rpl_dag_root_process);

PROCESS_THREAD(wrapper_rpl_dag_root_process, ev, data)
{
  PROCESS_BEGIN();

  // Call the root start function from rpl-dag-root.c
  rpl_dag_root_start();

  PROCESS_END();
}

