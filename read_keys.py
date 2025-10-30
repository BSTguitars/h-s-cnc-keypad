#!/usr/bin/env python3
"""
Haas Panel Reader - Portable Version
------------------------------------
Reads serial data from microcontroller ("Key: Row X  Col Y")
Looks up key names from haas_keymap.csv in the current folder
Handles shift toggle automatically
"""

import serial
import csv
import os
import sys

PORT = "/dev/ttyACM0"   # adjust if needed
BAUD = 115200
CSV_FILE = os.path.join(os.getcwd(), "haas_keymap.csv")

# --- Load keymap ---
keymap = {}
shift_row, shift_col = None, None

if not os.path.exists(CSV_FILE):
    print(f"❌ Keymap file not found: {CSV_FILE}")
    sys.exit(1)

with open(CSV_FILE, "r") as f:
    reader = csv.DictReader(f)
    for row in reader:
        r, c = row["Row"], row["Col"]
        normal = row.get("Normal", "").strip()
        shifted = row.get("Shifted", "").strip()
        keymap[(r, c)] = {"normal": normal, "shifted": shifted}
        if normal.lower() == "shift":
            shift_row, shift_col = r, c

print(f"✅ Loaded {len(keymap)} keys from {CSV_FILE}")
if shift_row is not None:
    print(f"Shift key detected at Row {shift_row}, Col {shift_col}")
else:
    print("⚠️ No shift key defined in keymap.")

# --- Open serial port ---
try:
    ser = serial.Serial(PORT, BAUD, timeout=1)
except Exception as e:
    print(f"❌ Failed to open serial port {PORT}: {e}")
    sys.exit(1)

shift_active = False
print("\nListening for key presses... (Ctrl+C to quit)\n")

# --- Main loop ---
try:
    while True:
        line = ser.readline().decode(errors="ignore").strip()
        if not line.startswith("Key: Row"):
            continue

        parts = line.split()
        row, col = parts[2], parts[4]

        # Handle shift toggle
        if shift_row == row and shift_col == col:
            shift_active = not shift_active
            print(f"[Shift {'ON' if shift_active else 'OFF'}]")
            continue

        key = keymap.get((row, col))
        if not key:
            print(f"Unknown key at Row {row}, Col {col}")
            continue

        output = key["shifted"] if (shift_active and key["shifted"]) else key["normal"]
        if output:
            print(f">> {output}")
        else:
            print(f"(Row {row}, Col {col}) has no name assigned yet.")

except KeyboardInterrupt:
    print("\nExiting cleanly.")
    ser.close()
