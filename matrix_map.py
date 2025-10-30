#!/usr/bin/env python3
"""
Haas Panel Matrix Mapper - Portable Persistent Version
------------------------------------------------------
- Detects key presses from serial: "Key: Row X  Col Y"
- Saves & reloads mappings in CSV located in the current folder
- Detects and toggles shift automatically
- Prompts before overwriting existing mappings
"""

import serial
import csv
import os

PORT = "/dev/ttyACM0"
BAUD = 115200
OUTFILE = os.path.join(os.getcwd(), "haas_keymap.csv")

print("Listening for key presses... (Ctrl+C to quit)")
print(f"Mapping file: {OUTFILE}")

# --- Load existing map ---
keymap = {}
shift_row, shift_col = None, None

if os.path.exists(OUTFILE):
    with open(OUTFILE, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            r, c = row["Row"], row["Col"]
            keymap[(r, c)] = {"Normal": row["Normal"], "Shifted": row["Shifted"]}
            if row["Normal"].lower() == "shift":
                shift_row, shift_col = r, c
    print(f"Loaded existing map with {len(keymap)} keys.")
else:
    print("No existing keymap found. A new one will be created.")

# --- Open serial ---
ser = serial.Serial(PORT, BAUD, timeout=1)
shift_held = False

def save_map():
    with open(OUTFILE, "w", newline='') as f:
        writer = csv.writer(f)
        writer.writerow(["Row", "Col", "Normal", "Shifted"])
        for (r, c), vals in keymap.items():
            writer.writerow([r, c, vals.get("Normal", ""), vals.get("Shifted", "")])
    print(f"✅ Map saved to: {OUTFILE}\n")

try:
    while True:
        line = ser.readline().decode(errors="ignore").strip()
        if not line.startswith("Key: Row"):
            continue

        parts = line.split()
        r, c = parts[2], parts[4]

        # Handle shift key toggle
        if shift_row and shift_col and (r == shift_row and c == shift_col):
            shift_held = not shift_held
            print(f"[Shift {'ON' if shift_held else 'OFF'}]")
            continue

        existing = keymap.get((r, c), {"Normal": "", "Shifted": ""})

        # Show existing value if any
        prev = existing["Shifted"] if shift_held else existing["Normal"]
        if prev:
            choice = input(f"Row {r}, Col {c} already mapped to '{prev}'. Overwrite? (y/n): ").lower()
            if choice != "y":
                continue

        # Ask for name
        if shift_held:
            name = input(f"Shift active → Row {r}, Col {c} = ")
            existing["Shifted"] = name
        else:
            name = input(f"Row {r}, Col {c} = ")
            if name.lower() == "shift":
                shift_row, shift_col = r, c
                print(f"Shift key mapped at Row {r}, Col {c}")
            existing["Normal"] = name

        keymap[(r, c)] = existing
        save_map()

except KeyboardInterrupt:
    print("\nExiting cleanly.")
    save_map()
