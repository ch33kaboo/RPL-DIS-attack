#!/usr/bin/env python3
import sys
import re
from collections import defaultdict

RTIMER_SECOND = 32768
VOLTAGE = 3.0

# mA values for each state
CPU_mA = 10.0
LPM_mA = 0.023
TX_mA = 17.4
RX_mA = 18.8

# Time points in minutes
TIME_POINTS_MIN = [1, 30, 60, 90, 120, 150, 180, 210, 240, 270, 299]

if len(sys.argv) != 2:
    print(f"Usage: {sys.argv[0]} <log_file>")
    sys.exit(1)

log_file = sys.argv[1]

# Updated regex to match both mm:ss.xxx and h:mm:ss.xxx
energy_re = re.compile(r"^(?P<time>(\d+:)?\d+:\d+\.\d+)\s+ID:(?P<id>\d+)\s+\[ENERGY\] CPU: (?P<cpu>\d+) LPM: (?P<lpm>\d+) TX: (?P<tx>\d+) RX: (?P<rx>\d+)")

def time_to_minutes(timestr):
    parts = timestr.split(":")
    if len(parts) == 2:
        # mm:ss.xxx
        m, s = parts
        return int(m) + float(s) / 60
    elif len(parts) == 3:
        # h:mm:ss.xxx
        h, m, s = parts
        return int(h) * 60 + int(m) + float(s) / 60
    else:
        raise ValueError(f"Unrecognized time format: {timestr}")

def ticks_to_mJ(cpu, lpm, tx, rx):
    return ((cpu * CPU_mA) + (lpm * LPM_mA) + (tx * TX_mA) + (rx * RX_mA)) * VOLTAGE / RTIMER_SECOND

# Collect all [ENERGY] entries for each node, sorted by time
node_entries = defaultdict(list)  # node_id -> list of (time_min, timestr, cpu, lpm, tx, rx)

with open(log_file, 'r', encoding='utf-8', errors='ignore') as f:
    for line in f:
        m = energy_re.match(line)
        if m:
            timestr = m.group('time')
            node_id = int(m.group('id'))
            cpu = int(m.group('cpu'))
            lpm = int(m.group('lpm'))
            tx = int(m.group('tx'))
            rx = int(m.group('rx'))
            time_min = time_to_minutes(timestr)
            node_entries[node_id].append((time_min, timestr, cpu, lpm, tx, rx))

for nid in sorted(node_entries.keys()):
    print(f"node {nid}:")
    entries = node_entries[nid]
    idx = 0
    for tp in TIME_POINTS_MIN:
        # Advance idx to the first entry at or after tp
        while idx < len(entries) and entries[idx][0] < tp:
            idx += 1
        if idx < len(entries):
            time_min, timestr, cpu, lpm, tx, rx = entries[idx]
            energy = ticks_to_mJ(cpu, lpm, tx, rx)
            print(f"{tp} minute : {energy:.2f} (log time: {timestr})")
        else:
            print(f"{tp} minute : -")
    print()

print("Total:")
node_indices = {nid: 0 for nid in node_entries.keys()}
for tp in TIME_POINTS_MIN:
    total = 0.0
    valid = False
    for nid in sorted(node_entries.keys()):
        entries = node_entries[nid]
        idx = node_indices[nid]
        while idx < len(entries) and entries[idx][0] < tp:
            idx += 1
        node_indices[nid] = idx
        if idx < len(entries):
            time_min, timestr, cpu, lpm, tx, rx = entries[idx]
            energy = ticks_to_mJ(cpu, lpm, tx, rx)
            total += energy
            valid = True
    if valid:
        print(f"{tp} minute : {total:.2f}")
    else:
        print(f"{tp} minute : -") 