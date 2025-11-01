HAAS CONTROL PANEL → LINUXCNC / PROBE BASIC INTEGRATION
Version: v10 - Raspberry Pi Pico RP2040 Keyboard Interface
====================================================================

Overview
--------------------------------------------------------------------
This project connects a real Haas CNC control panel to LinuxCNC
running the Probe Basic (QtPyVCP) GUI.

A Raspberry Pi Pico scans the Haas keyboard matrix and emulates
a USB keyboard. The Haas keys send regular keyboard inputs that
Probe Basic responds to through custom shortcut bindings.

These shortcuts are defined inside Probe Basic so the panel can
control functions like coolant, overrides, jog increments,
and spindle direction directly from the Haas keypad.

--------------------------------------------------------------------
System Layout
--------------------------------------------------------------------
Haas Panel  →  Pico RP2040  →  USB HID Keyboard  →  LinuxCNC / Probe Basic

Hardware:
- Haas panel connected to Pico GPIOs
- Pico running custom firmware for matrix scanning
- Outputs plain keycodes over USB

Software:
- LinuxCNC with Probe Basic GUI
- Shortcuts added to template_user_buttons.py

--------------------------------------------------------------------
Shortcut Binding
--------------------------------------------------------------------
Probe Basic does not have a built-in keybinding system.
Shortcuts are created in Python using QShortcut inside:

  ~/linuxcnc/configs/probe_basic/user_buttons/template_user_buttons.py

Example:
  sc = QShortcut(QKeySequence("Alt+F24"), win)
  sc.setContext(Qt.ApplicationShortcut)
  sc.activated.connect(lambda: actions.machine.rapid_override.set(5))



--------------------------------------------------------------------
Files
--------------------------------------------------------------------
haas_keymap_v10.txt                Full Haas matrix and key outputs
template_user_buttons.py           Shortcut definitions
qtpyvcp_action_strings.txt         List of valid Probe Basic actions
qtpyvcp_probe_basic_reference.md   QtPyVCP action reference
README.txt                         This project overview

--------------------------------------------------------------------
Testing
--------------------------------------------------------------------
1. Start LinuxCNC with the Probe Basic configuration.
2. Open probe_basic.log to watch for shortcut messages.
3. Press a Haas key and verify the corresponding GUI function reacts.
4. Restart Probe Basic if shortcuts are changed.

--------------------------------------------------------------------
Author:  Ben
Project: haasPanel
License: MIT
====================================================================
