#!/usr/bin/env python3
import sys

if len(sys.argv) != 2:
    print(f"Usage: {sys.argv[0]} <log_file>")
    sys.exit(1)

log_file = sys.argv[1]

count_dis = 0
count_dio = 0
count_dao = 0
count_dao_ack = 0

with open(log_file, 'r', encoding='utf-8', errors='ignore') as f:
    for line in f:
        if "received a DIS from" in line:
            count_dis += 1
        if ("sending a unicast-DIO" in line) or ("sending a multicast-DIO" in line):
            count_dio += 1
        if "sending a DAO " in line:
            count_dao += 1
        if "received a DAO-ACK" in line:
            count_dao_ack += 1

total_rpl = count_dis + count_dio + count_dao + count_dao_ack

print(f"DIS messages: {count_dis}")
print(f"DIO messages: {count_dio}")
print(f"DAO messages: {count_dao}")
print(f"DAO-ACK messages: {count_dao_ack}")
print(f"Total RPL messages: {total_rpl}") 