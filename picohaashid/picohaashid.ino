/* ────────────────────────────────────────────────────────────────
   Haas Panel → USB Keyboard (Full 11x13 Matrix)
   Raspberry Pi Pico (RP2040, TinyUSB HID)

   Version: v10.3 – HID-SAFE Ctrl+Alt + Full Map
   Features:
     • Latched Shift (A–Z, 0–9, (), -, ., +A/-A→+B/-B)
     • Correct Haas-style shifted symbols
     • Axis jog keys (← ↑ → ↓ PgUp PgDn)
     • F1–F4 = Haas soft keys
     • CANCEL / SPACE / ENTER / EOB intact
     • All Haas control keys mapped to safe Ctrl+Alt combos
     • Clean comments for readability
──────────────────────────────────────────────────────────────── */

#include <Keyboard.h>

/* ────────────────────────────────────────────────
   HID keycodes for RP2040 Keyboard library
──────────────────────────────────────────────── */
#define KEY_A 0x04
#define KEY_1 0x1E
#define KEY_2 0x1F
#define KEY_3 0x20
#define KEY_4 0x21
#define KEY_5 0x22
#define KEY_6 0x23
#define KEY_7 0x24
#define KEY_8 0x25
#define KEY_9 0x26
#define KEY_0 0x27

const uint8_t ROWS = 11, COLS = 13;
const uint8_t rowPins[ROWS] = {0,1,2,3,4,5,6,7,8,9,10};
const uint8_t colPins[COLS] = {11,12,13,14,15,16,17,18,19,20,21,22,26};

const uint8_t SHIFT_ROW = 6;
const uint8_t SHIFT_COL = 2;
bool shiftActive = false;

/* ────────────────────────────────────────────────
   KEY MATRIX MAP
──────────────────────────────────────────────── */
struct KeyMapEntry { uint8_t row; uint8_t col; const char* key; };
KeyMapEntry MAP[] = {
  // Letters
  {6,1,"a"}, {6,0,"b"}, {6,10,"c"}, {6,9,"d"}, {6,8,"e"},
  {7,2,"f"}, {7,1,"g"}, {7,0,"h"}, {7,10,"i"}, {7,9,"j"},
  {7,8,"k"}, {8,2,"l"}, {8,1,"m"}, {8,0,"n"}, {8,10,"o"},
  {8,9,"p"}, {8,8,"q"}, {9,2,"r"}, {9,1,"s"}, {9,0,"t"},
  {9,10,"u"}, {9,9,"v"}, {9,8,"w"}, {10,2,"x"}, {10,1,"y"}, {10,0,"z"},

  // Numbers & punctuation
  {8,7,"1"}, {8,6,"2"}, {8,5,"3"}, {7,7,"4"}, {7,6,"5"},
  {7,5,"6"}, {6,7,"7"}, {6,6,"8"}, {6,5,"9"}, {9,6,"0"},
  {9,5,"."}, {9,7,"-"}, {10,9,"("}, {10,8,")"},

  // Control keys
  {10,7,"CANCEL"}, {10,6,"SPACE"}, {10,5,"ENTER"}, {10,10,"EOB"},

  // Axis jogs
  {4,3,"LEFT"}, {4,12,"RIGHT"}, {3,3,"DOWN"}, {5,12,"UP"},
  {5,4,"PGDN"}, {3,4,"PGUP"}, {4,4,"Ctrl+Alt+J"},

  // Rotary / A-axis
  {3,12,"Ctrl+Alt+A"}, {5,3,"Ctrl+Alt+B"},

  // Softkeys
  {1,11,"F1"}, {1,12,"F2"}, {1,4,"F3"}, {1,3,"F4"},

  // ────────────────────────────────────────────────
  // Machine / Tool functions
  // ────────────────────────────────────────────────
  {0,12,"Ctrl+Alt+R"},  // RESET
  {0,4,"Ctrl+Alt+P"},   // POWER UP / RESTART
  {0,3,"Ctrl+Alt+V"},   // RECOVER
  {2,11,"Ctrl+Alt+T"},  // TOOL OFFSET MEASURE
  {2,12,"Ctrl+Alt+N"},  // NEXT TOOL
  {2,4,"Ctrl+Alt+U"},   // TOOL RELEASE
  {2,3,"Ctrl+Alt+Z"},   // PART ZERO SET
  {2,5,"Ctrl+Alt+Y"},   // ATC REV
  {2,6,"Ctrl+Alt+O"},   // ATC FWD

  // ────────────────────────────────────────────────
  // Coolant / Chip
  // ────────────────────────────────────────────────
  {3,11,"Ctrl+Alt+C"},  // CHIP FWD
  {4,11,"Ctrl+Alt+H"},  // CHIP STOP
  {5,11,"Ctrl+Alt+X"},  // CHIP REV
  {3,2,"Ctrl+Alt+L"},   // CLNT UP
  {4,2,"Ctrl+Alt+D"},   // CLNT DOWN
  {5,2,"Ctrl+Alt+E"},   // AUX CLNT (MIST)

  // ────────────────────────────────────────────────
  // Feed / Spindle Overrides and Controls
  // ────────────────────────────────────────────────
  {7,11,"Ctrl+Alt+F"},  // -10% FEED
  {7,12,"Ctrl+Alt+G"},  // 100% FEED
  {7,4,"Ctrl+Alt+W"},   // +10% FEED
  {7,3,"Ctrl+Alt+Q"},   // HAND FEED

  {8,11,"Ctrl+Alt+S"},  // -10% SPINDLE
  {8,4,"Ctrl+Alt+K"},   // +10% SPINDLE
  {8,3,"Ctrl+Alt+M"},   // HAND SPINDLE
  {8,12,"Ctrl+Alt+1"},  // 100% SPINDLE

  {9,11,"Ctrl+Alt+2"},  // SPINDLE CW (FORWARD)
  {9,12,"Ctrl+Alt+3"},  // SPINDLE STOP
  {9,4,"Ctrl+Alt+4"},   // SPINDLE CCW (REVERSE)

  // ────────────────────────────────────────────────
  // Rapid Overrides
  // ────────────────────────────────────────────────
  {10,11,"Ctrl+Alt+5"}, // 5% RAPID
  {10,12,"Ctrl+Alt+6"}, // 25% RAPID
  {10,4,"Ctrl+Alt+7"},  // 50% RAPID
  {10,3,"Ctrl+Alt+8"},  // 100% RAPID

  // ────────────────────────────────────────────────
  // Display / Mode Buttons
  // ────────────────────────────────────────────────
  {0,2,"Ctrl+Alt+9"},   // PROGRAM / CONVERSATIONAL
  {0,1,"Ctrl+Alt+0"},   // POSITION
  {0,0,"Ctrl+Alt+I"},   // OFFSET
  {0,10,"Ctrl+Alt+O"},  // CURRENT COMMANDS

  {1,2,"Ctrl+Alt+A"},   // ALARM MESSAGES
  {1,1,"Ctrl+Alt+B"},   // PARAMETERS / DIAGNOSTICS
  {1,0,"Ctrl+Alt+C"},   // SETTINGS / GRAPH
  {1,10,"Ctrl+Alt+D"},  // HELP / CALCULATOR
  {3,1,"Ctrl+Alt+E"},   // HOME

  // ────────────────────────────────────────────────
  // Mode Cluster
  // ────────────────────────────────────────────────
  {0,9,"Ctrl+Alt+F"},   // EDIT
  {1,9,"Ctrl+Alt+G"},   // MEM
  {2,9,"Ctrl+Alt+H"},   // MDI / DNC
  {3,9,"Ctrl+Alt+J"},   // HAND JOG
  {4,9,"Ctrl+Alt+K"},   // ZERO RETURN
  {5,9,"Ctrl+Alt+L"},   // LIST PROGRAM

  // ────────────────────────────────────────────────
  // Edit / Option Strip
  // ────────────────────────────────────────────────
  {0,8,"Ctrl+Alt+M"},   // INSERT
  {1,8,"Ctrl+Alt+N"},   // SINGLE BLOCK
  {2,8,"Ctrl+Alt+P"},   // COOLANT
  {3,8,"Ctrl+Alt+R"},   // .0001 (JOG INC)
  {4,8,"Ctrl+Alt+S"},   // ALL (ZERO ALL)
  {5,8,"Ctrl+Alt+T"},   // SELECT PROGRAM

  {0,7,"Ctrl+Alt+U"},   // ALTER
  {1,7,"Ctrl+Alt+V"},   // DRY RUN
  {2,7,"Ctrl+Alt+W"},   // ORIENT SPINDLE
  {3,7,"Ctrl+Alt+X"},   // .001 (JOG INC)
  {4,7,"Ctrl+Alt+Y"},   // ORIGIN
  {5,7,"Ctrl+Alt+Z"},   // SEND

  {0,6,"Ctrl+Alt+1"},   // DELETE
  {1,6,"Ctrl+Alt+2"},   // OPTION STOP
  {3,6,"Ctrl+Alt+3"},   // .01 (JOG INC)
  {4,6,"Ctrl+Alt+4"},   // SINGLE (SINGL)
  {5,6,"Ctrl+Alt+5"},   // RECV

  {0,5,"Ctrl+Alt+6"},   // UNDO
  {1,5,"Ctrl+Alt+7"},   // BLOCK DELETE
  {3,5,"Ctrl+Alt+8"},   // .1 (JOG INC)
  {4,5,"Ctrl+Alt+9"},   // HOME G28
  {5,5,"Ctrl+Alt+0"}    // ERASE PROGRAM

};
const size_t MAP_LEN = sizeof(MAP)/sizeof(MAP[0]);

/* ────────────────────────────────────────────────
   Shift symbol conversion (Haas-style)
──────────────────────────────────────────────── */
char getHaasShiftedChar(char c) {
  switch(c) {
    case '1': return '*'; case '2': return '\''; case '3': return '?';
    case '4': return '%'; case '5': return '$'; case '6': return '!';
    case '7': return '&'; case '8': return '@'; case '9': return ':';
    case '0': return '='; case '-': return '+'; case '.': return '#';
    default: return c;
  }
}

/* ────────────────────────────────────────────────
   Lookup helper
──────────────────────────────────────────────── */
const char* findKey(uint8_t r, uint8_t c) {
  for(size_t i=0;i<MAP_LEN;i++)
    if(MAP[i].row==r && MAP[i].col==c)
      return MAP[i].key;
  return nullptr;
}

/* ────────────────────────────────────────────────
   Latched Shift
──────────────────────────────────────────────── */
void toggleShift() {
  shiftActive=!shiftActive;
  delay(200);
}

/* ────────────────────────────────────────────────
   Send key logic
──────────────────────────────────────────────── */
void sendKey(const char* key) {
  if(!key) return;

  // Basic control keys
  if(strcmp(key,"CANCEL")==0){ Keyboard.press(KEY_BACKSPACE); delay(5); Keyboard.releaseAll(); return; }
  if(strcmp(key,"SPACE")==0){ Keyboard.press(' '); delay(5); Keyboard.releaseAll(); return; }
  if(strcmp(key,"ENTER")==0){ Keyboard.press(KEY_RETURN); delay(5); Keyboard.releaseAll(); return; }
  if(strcmp(key,"EOB")==0){ Keyboard.press(shiftActive ? '/' : ';'); delay(5); Keyboard.releaseAll(); return; }

  // Axis jog / arrows
  if(strcmp(key,"LEFT")==0){ Keyboard.press(KEY_LEFT_ARROW); delay(5); Keyboard.releaseAll(); return; }
  if(strcmp(key,"RIGHT")==0){ Keyboard.press(KEY_RIGHT_ARROW); delay(5); Keyboard.releaseAll(); return; }
  if(strcmp(key,"UP")==0){ Keyboard.press(KEY_UP_ARROW); delay(5); Keyboard.releaseAll(); return; }
  if(strcmp(key,"DOWN")==0){ Keyboard.press(KEY_DOWN_ARROW); delay(5); Keyboard.releaseAll(); return; }
  if(strcmp(key,"PGUP")==0){ Keyboard.press(KEY_PAGE_UP); delay(5); Keyboard.releaseAll(); return; }
  if(strcmp(key,"PGDN")==0){ Keyboard.press(KEY_PAGE_DOWN); delay(5); Keyboard.releaseAll(); return; }

  // Softkeys F1–F4
  if(strcmp(key,"F1")==0){ Keyboard.press(KEY_F1); delay(5); Keyboard.releaseAll(); return; }
  if(strcmp(key,"F2")==0){ Keyboard.press(KEY_F2); delay(5); Keyboard.releaseAll(); return; }
  if(strcmp(key,"F3")==0){ Keyboard.press(KEY_F3); delay(5); Keyboard.releaseAll(); return; }
  if(strcmp(key,"F4")==0){ Keyboard.press(KEY_F4); delay(5); Keyboard.releaseAll(); return; }

  // ───────── Ctrl+Alt+ combos (safe printable method) ─────────
  if(strncmp(key,"Ctrl+Alt+",9)==0) {
    char combo = key[9];
    // Hold Ctrl + Alt modifiers
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    // Send the printable character (lowercase)
    Keyboard.press(tolower(combo));
    delay(10);
    Keyboard.release(tolower(combo));
    Keyboard.release(KEY_LEFT_ALT);
    Keyboard.release(KEY_LEFT_CTRL);
    return;
  }

  // ───────── Normal printable keys (letters, numbers, etc.) ─────────
  if(strlen(key)==1) {
    char c = key[0];
    if(shiftActive) {
      if(isdigit(c) || c=='-' || c=='.') c = getHaasShiftedChar(c);
      else if(c=='(') c='['; else if(c==')') c=']';
      else if(isalpha(c)) c = toupper(c);
    }
    Keyboard.press(c);
    delay(5);
    Keyboard.release(c);
  }
}


/* ────────────────────────────────────────────────
   Setup / Loop
──────────────────────────────────────────────── */
void setup() {
  delay(800);
  Keyboard.begin();
  for(uint8_t r=0;r<ROWS;r++){ pinMode(rowPins[r],OUTPUT); digitalWrite(rowPins[r],HIGH); }
  for(uint8_t c=0;c<COLS;c++){ pinMode(colPins[c],INPUT_PULLUP); }
}

void loop() {
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
