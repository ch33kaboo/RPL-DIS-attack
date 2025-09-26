#!/usr/bin/env python3
import sys
import re
from collections import defaultdict
import argparse

# --- Command-line arguments ---
parser = argparse.ArgumentParser(description="Calculate average energy for affected and non-affected nodes.")
parser.add_argument("--affected", required=True, help="Comma-separated list of affected node numbers, e.g., 4,6,11")
parser.add_argument("--ignored", default="", help="Comma-separated list of ignored node numbers, e.g., 1,2,3")
args = parser.parse_args()

affected_nodes = set(int(n) for n in args.affected.split(","))
ignored_nodes = set(int(n) for n in args.ignored.split(","))

# --- Read data from stdin ---
data = sys.stdin.read()

# --- Parse the data ---
node_data = defaultdict(dict)
current_node = None

for line in data.splitlines():
    line = line.strip()
    if line.startswith("node"):
        current_node = int(re.findall(r'\d+', line)[0])
    elif current_node and ":" in line and "Total" not in line:
        minute = int(re.findall(r'\d+', line.split(":")[0])[0])
        energy = float(re.findall(r'[\d.]+', line.split(":")[1])[0])
        node_data[current_node][minute] = energy

# --- Separate nodes ---
all_nodes = set(node_data.keys())
not_affected_nodes = all_nodes - affected_nodes - ignored_nodes

# --- Calculate averages ---
minutes = sorted(next(iter(node_data.values())).keys())

avg_affected = {}
avg_not_affected = {}

for m in minutes:
    avg_affected[m] = sum(node_data[n][m] for n in affected_nodes) / len(affected_nodes) if affected_nodes else 0
    avg_not_affected[m] = sum(node_data[n][m] for n in not_affected_nodes) / len(not_affected_nodes) if not_affected_nodes else 0

# --- Print results ---
print("Minute | Avg Affected | Avg Not Affected")
for m in minutes:
    print(f"{m:>6} | {avg_affected[m]:>12.2f} | {avg_not_affected[m]:>16.2f}")
