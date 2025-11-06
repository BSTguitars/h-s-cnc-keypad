this is a simple project to reuse as much as possible of a hass cnc panle, 
to control linuxcnc and probe basic gui. 
the decoding software could be used to decode any keyboard matrix.

 project ues a pi pico, sends usb keyboard shortcuts to control linuxcnc/probe basic. 
 
 matrixtest folder is arduino mega code i wrote before getting the pico. 
 its to send serial info to help map the keyboard matrix.
 
 pico_matrix_decoder has the pico code for decoding. 
 
 
 picohaashid folder has the code that pushing my keyboard shortcuts.
 
 haas_keymap.csv list the mapping of the keys i genrated based on the way i wried the pic0.
 
 
 haas_keymap_ref is just a list of matrix numbers, lable on haas key, and shortcut it transmits.
 
 matrix_map.py will read you keyboard inputs and ask you to name them.
 if you inout the same key it will ask if you want to rename them. it will genratea a csv file listing
 rw/col and what you named it. 
 
 picosetup.txt trys to explain the wiring. 
 
 read_key.py when you press a key , it will print the key name from the csv file
 
 
