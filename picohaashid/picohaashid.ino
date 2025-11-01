/* ────────────────────────────────────────────────────────────────
   Haas Panel → USB Keyboard (Full 11x13 Matrix)
   Raspberry Pi Pico (RP2040, TinyUSB HID)

   Version: v9 – Full matrix mapping
   Features:
     • All letter, number, and punctuation keys
     • True toggle-based Shift behavior
     • Correct Haas-style shifted symbols
     • Dedicated axis jog keys (← ↑ → ↓ PgUp PgDn)
     • F1–F4 mapped to Haas panel softkeys
     • Extended F13–F61 mapping for all special Haas buttons
     • +A / -A with Shift for +B / -B support
     • Compatible with LinuxCNC, Probe Basic, and standard HID input

   Hardware:
     Rows → GP0–GP10
     Cols → GP11–GP22, GP26

   Author: Ben (BST Designs)
   Date: 2025-10-31
──────────────────────────────────────────────────────────────── */


#include <Keyboard.h>

const uint8_t ROWS = 11, COLS = 13;
const uint8_t rowPins[ROWS] = {0,1,2,3,4,5,6,7,8,9,10};
const uint8_t colPins[COLS] = {11,12,13,14,15,16,17,18,19,20,21,22,26};

const uint8_t SHIFT_ROW = 6;
const uint8_t SHIFT_COL = 2;

bool shiftActive = false;

struct KeyMapEntry { uint8_t row; uint8_t col; const char* key; };
KeyMapEntry MAP[] = {
  // Letters
  {6,1,"a"},{6,0,"b"},{6,10,"c"},{6,9,"d"},{6,8,"e"},
  {7,2,"f"},{7,1,"g"},{7,0,"h"},{7,10,"i"},{7,9,"j"},
  {7,8,"k"},{8,2,"l"},{8,1,"m"},{8,0,"n"},{8,10,"o"},
  {8,9,"p"},{8,8,"q"},{9,2,"r"},{9,1,"s"},{9,0,"t"},
  {9,10,"u"},{9,9,"v"},{9,8,"w"},{10,2,"x"},{10,1,"y"},{10,0,"z"},

  // Numbers & punctuation
  {8,7,"1"},{8,6,"2"},{8,5,"3"},{7,7,"4"},{7,6,"5"},
  {7,5,"6"},{6,7,"7"},{6,6,"8"},{6,5,"9"},{9,6,"0"},
  {9,5,"."},{9,7,"-"},
  {10,9,"("},{10,8,")"},

  // Special control keys
  {10,7,"CANCEL"}, {10,6,"SPACE"}, {10,5,"ENTER"}, {10,10,"EOB"},

  // Axis jogs (← ↑ → ↓ PgUp PgDn)
  {4,3,"LEFT"},    // –X
  {4,12,"RIGHT"},  // +X
  {3,3,"DOWN"},    // –Y
  {5,12,"UP"},     // +Y
  {5,4,"PGDN"},    // –Z
  {3,4,"PGUP"},    // +Z

  // Haas F1–F4 keys
  {1,11,"F1"},{1,12,"F2"},{1,4,"F3"},{1,3,"F4"},

  // Arrow-labeled keys (general-purpose F-keys)
  {3,0,"F13"},      // UP arrow cluster key
  {3,10,"F14"},     // PAGE UP cluster key
  {4,1,"F15"},      // LEFT arrow cluster key
  {4,10,"F16"},     // RIGHT arrow cluster key
  {5,1,"F17"},      // END / DOWN cluster key
  {5,10,"F18"},     // PAGE DOWN cluster key

  // Rotary +A/-A and shifted +B/-B
  {3,12,"F19"},        // +A
  {5,3,"F20"},         // -A
  {3,12,"Shift+F25"},  // +B (Shifted)
  {5,3,"Shift+F26"},   // -B (Shifted)


  // Machine / Tool functions
  {0,12,"F21"},        // Reset
  {0,4,"F22"},         // Power up / restart
  {0,3,"F23"},         // Recover
  {2,11,"F24"},        // Tool offset measure
  {2,12,"Shift+F13"},  // Next tool
  {2,4,"Shift+F14"},   // Tool release
  {2,3,"Shift+F15"},   // Part zero set
  {2,5,"Shift+F16"},   // ATC rev
  {2,6,"Shift+F17"},   // ATC fwd

  // Coolant / Chip control
  {3,11,"Ctrl+F13"},   // Chip fwd
  {4,11,"Ctrl+F14"},   // Chip stop
  {5,11,"Ctrl+F15"},   // Chip rev
  {3,2,"Ctrl+F16"},    // Clnt up
  {4,2,"Ctrl+F17"},    // Clnt down
  {5,2,"Ctrl+F18"},    // Aux clnt

  // Feed / Spindle overrides
  {7,11,"Alt+F13"},    // -10 feed
  {7,12,"Alt+F14"},    // 100% feed
  {7,4,"Alt+F15"},     // +10 feed
  {7,3,"Alt+F16"},     // Hand ctrl feed
  {8,11,"Alt+F17"},    // -10 spindle
  {8,4,"Alt+F18"},     // +10 spindle
  {8,3,"Alt+F19"},     // Hand ctrl spin
  {8,12,"Alt+F20"},    // 100% spindle
  {9,11,"Alt+F21"},    // CW spindle
  {9,12,"Alt+F22"},    // Stop spindle
  {9,4,"Alt+F23"},     // CCW spindle

  // Rapid overrides
  {10,11,"Alt+F24"},   // 5% rapid
  {10,12,"Ctrl+F19"},  // 25% rapid
  {10,4,"Ctrl+F20"},   // 50% rapid
  {10,3,"Ctrl+F21"},   // 100% rapid

  // Display / Mode buttons (Shift+F13 → Shift+F21)
  {0,2,"Shift+F13"},   // Program / Conversational
  {0,1,"Shift+F14"},   // Position
  {0,0,"Shift+F15"},   // Offset
  {0,10,"Shift+F16"},  // Current Commands
  {1,2,"Shift+F17"},   // Alarm Messages
  {1,1,"Shift+F18"},   // Parameters / Diagnostics
  {1,0,"Shift+F19"},   // Settings / Graph
  {1,10,"Shift+F20"},  // Help / Calculator
  {3,1,"Shift+F21"},   // Home

  // Mode cluster (Ctrl+F22 → Ctrl+F24 and continue Alt modifiers)
  {0,9,"Ctrl+F22"},    // Edit
  {1,9,"Ctrl+F23"},    // Mem
  {2,9,"Ctrl+F24"},    // MDI / DNC
  {3,9,"Alt+F13"},     // Hand Jog
  {4,9,"Alt+F14"},     // Zero Return
  {5,9,"Alt+F15"},     // List Program

  // Edit / Option strip (Alt+F16 → Alt+F24 then Shift+F13+...)
  {0,8,"Alt+F16"},     // Insert
  {1,8,"Alt+F17"},     // Single Block
  {2,8,"Alt+F18"},     // Coolant
  {3,8,"Alt+F19"},     // .0001
  {4,8,"Alt+F20"},     // All
  {5,8,"Alt+F21"},     // Select Program
  {0,7,"Alt+F22"},     // Alter
  {1,7,"Alt+F23"},     // Dry Run
  {2,7,"Alt+F24"},     // Orient Spindle
  {3,7,"Shift+F13"},   // .001
  {4,7,"Shift+F14"},   // Origin
  {5,7,"Shift+F15"},   // Send
  {0,6,"Shift+F16"},   // Delete
  {1,6,"Shift+F17"},   // Option Stop
  {3,6,"Shift+F18"},   // .01
  {4,6,"Shift+F19"},   // Singl
  {5,6,"Shift+F20"},   // Recv
  {0,5,"Shift+F21"},   // Undo
  {1,5,"Shift+F22"},   // Block Delete
  {3,5,"Shift+F23"},   // .1
  {4,5,"Shift+F24"},   // Home G28
  {5,5,"Ctrl+F13"}     // Erase Program

};
const size_t MAP_LEN = sizeof(MAP)/sizeof(MAP[0]);


// Haas-shift map
char getHaasShiftedChar(char c){
  switch(c){
    case '1': return '*'; case '2': return '\'';
    case '3': return '?'; case '4': return '%';
    case '5': return '$'; case '6': return '!';
    case '7': return '&'; case '8': return '@';
    case '9': return ':'; case '0': return '=';
    case '-': return '+'; case '.': return '#';
    default: return c;
  }
}

const char* findKey(uint8_t r, uint8_t c) {
  const char* found = nullptr;
  for (size_t i = 0; i < MAP_LEN; i++) {
    if (MAP[i].row == r && MAP[i].col == c) {
      // Prefer "Shift+" entries when shiftActive is true
      if (shiftActive && strncmp(MAP[i].key, "Shift+", 6) == 0)
        return MAP[i].key;
      // Otherwise prefer normal (non-Shift) keys
      if (!shiftActive && strncmp(MAP[i].key, "Shift+", 6) != 0)
        found = MAP[i].key;
    }
  }
  return found;
}


void toggleShift(){
  shiftActive = !shiftActive;
  Serial.println(shiftActive ? "[SHIFT ON]" : "[SHIFT OFF]");
  delay(200);
}

void setup(){
  delay(800);
  Keyboard.begin();
  Serial.begin(115200);
  Serial.println("Haas keyboard v9 – Full matrix + Shift aware");

  for(uint8_t r=0;r<ROWS;r++){ pinMode(rowPins[r],OUTPUT); digitalWrite(rowPins[r],HIGH); }
  for(uint8_t c=0;c<COLS;c++){ pinMode(colPins[c],INPUT_PULLUP); }
}

void sendKey(const char* key){
  if(!key) return;

  // control keys
  if(strcmp(key,"CANCEL")==0){ Keyboard.press(KEY_BACKSPACE); delay(5); Keyboard.release(KEY_BACKSPACE); return; }
  if(strcmp(key,"SPACE")==0){ Keyboard.press(' '); delay(5); Keyboard.release(' '); return; }
  if(strcmp(key,"ENTER")==0){ Keyboard.press(KEY_RETURN); delay(5); Keyboard.release(KEY_RETURN); return; }
  if(strcmp(key,"EOB")==0){ if(shiftActive){ Keyboard.press('/'); delay(5); Keyboard.release('/'); Serial.println("EOB (shift) -> /"); } else { Keyboard.press(';'); delay(5); Keyboard.release(';'); Serial.println("EOB -> ;"); } return; }

  // arrows / pages
  if(strcmp(key,"LEFT")==0){ Keyboard.press(KEY_LEFT_ARROW); delay(5); Keyboard.release(KEY_LEFT_ARROW); return; }
  if(strcmp(key,"RIGHT")==0){ Keyboard.press(KEY_RIGHT_ARROW); delay(5); Keyboard.release(KEY_RIGHT_ARROW); return; }
  if(strcmp(key,"UP")==0){ Keyboard.press(KEY_UP_ARROW); delay(5); Keyboard.release(KEY_UP_ARROW); return; }
  if(strcmp(key,"DOWN")==0){ Keyboard.press(KEY_DOWN_ARROW); delay(5); Keyboard.release(KEY_DOWN_ARROW); return; }
  if(strcmp(key,"PGUP")==0){ Keyboard.press(KEY_PAGE_UP); delay(5); Keyboard.release(KEY_PAGE_UP); return; }
  if(strcmp(key,"PGDN")==0){ Keyboard.press(KEY_PAGE_DOWN); delay(5); Keyboard.release(KEY_PAGE_DOWN); return; }

  // function keys
  if(strcmp(key,"F1")==0){ Keyboard.press(KEY_F1); delay(5); Keyboard.release(KEY_F1); return; }
  if(strcmp(key,"F2")==0){ Keyboard.press(KEY_F2); delay(5); Keyboard.release(KEY_F2); return; }
  if(strcmp(key,"F3")==0){ Keyboard.press(KEY_F3); delay(5); Keyboard.release(KEY_F3); return; }
  if(strcmp(key,"F4")==0){ Keyboard.press(KEY_F4); delay(5); Keyboard.release(KEY_F4); return; }

  // normal printables
  if(strlen(key)==1){
    char c = key[0];
    if(shiftActive){
      if(isdigit(c) || c=='-' || c=='.') c = getHaasShiftedChar(c);
      else if(c=='(') c='['; else if(c==')') c=']';
      else if(isalpha(c)) c = toupper(c);
    }
    Keyboard.press(c); delay(5); Keyboard.release(c);
  }
}

void loop(){
  for(uint8_t r=0;r<ROWS;r++){
    digitalWrite(rowPins[r],LOW);
    for(uint8_t c=0;c<COLS;c++){
      if(digitalRead(colPins[c])==LOW){
        if(r==SHIFT_ROW && c==SHIFT_COL){
          toggleShift();
          while(digitalRead(colPins[c])==LOW) delay(5);
        } else {
          const char* key=findKey(r,c);
          sendKey(key);
          delay(10);
          while(digitalRead(colPins[c])==LOW) delay(5);
        }
      }
    }
    digitalWrite(rowPins[r],HIGH);
  }
}
