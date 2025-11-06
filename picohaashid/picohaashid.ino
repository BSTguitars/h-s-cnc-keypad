/* ────────────────────────────────────────────────────────────────
   Haas Panel → USB Keyboard (Full 11x13 Matrix)
   Raspberry Pi Pico (RP2040 – TinyUSB HID)
   Version: v1.0 – Alphabetized Combos
──────────────────────────────────────────────────────────────── */

#include <Keyboard.h>



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

struct KeyMapEntry { uint8_t row; uint8_t col; const char* key; };
KeyMapEntry MAP[] = {

  // Letters / numbers (typing)
  {6,1,"a"},{6,0,"b"},{6,10,"c"},{6,9,"d"},{6,8,"e"},
  {7,2,"f"},{7,1,"g"},{7,0,"h"},{7,10,"i"},{7,9,"j"},
  {7,8,"k"},{8,2,"l"},{8,1,"m"},{8,0,"n"},{8,10,"o"},
  {8,9,"p"},{8,8,"q"},{9,2,"r"},{9,1,"s"},{9,0,"t"},
  {9,10,"u"},{9,9,"v"},{9,8,"w"},{10,2,"x"},{10,1,"y"},{10,0,"z"},
  {8,7,"1"},{8,6,"2"},{8,5,"3"},{7,7,"4"},{7,6,"5"},
  {7,5,"6"},{6,7,"7"},{6,6,"8"},{6,5,"9"},{9,6,"0"},
  {9,5,"."},{9,7,"-"},{10,9,"("},{10,8,")"},

  // Control keys
  {10,7,"CANCEL"},{10,6,"SPACE"},{10,5,"ENTER"},{10,10,"EOB"},


 
   // Axis jogs switched to nav keys to match keyboard shorcuts 
 {4,1,"LEFT"},{4,10,"RIGHT"},{5,0,"DOWN"},{3,0,"UP"},
 {5,10,"PGDN"},{3,8,"PGUP"},
 

  // Softkeys
  {1,11,"F1"},{1,12,"F2"},{1,4,"F3"},{1,3,"F4"},

// ────────────────────────────────────────────────
// ALL SPECIAL COMBOS — GROUPED BY TYPE (ALPHABETICAL)
// Clean v13.6 Debian-XFCE-Safe Unified Edition (A-Z Ordered)
// ────────────────────────────────────────────────

// ─────────────── Ctrl+Alt+ LETTERS ───────────────
{0,2,"Ctrl+Alt+A"}, // PROGRAM / CONVERSATIONAL
{0,1,"Ctrl+Alt+C"}, // POSITION
{0,0,"Ctrl+Alt+E"}, // OFFSET
{0,10,"Ctrl+Alt+G"}, // CURRENT COMMANDS
{1,2,"Ctrl+Alt+H"}, // ALARM MESSAGES
{1,1,"Ctrl+Alt+I"}, // PARAMETERS / DIAGNOSTICS
{1,0,"Ctrl+Alt+J"}, // SETTINGS / GRAPH
{3,1,"Ctrl+Alt+K"}, // HOME
{5,1,"Ctrl+Alt+M"}, // END (nav)
{4,8,"Ctrl+Alt+N"}, // ZERO ALL
{4,4,"Ctrl+Alt+O"}, // JOG LOCK
{1,9,"Ctrl+Alt+R"}, // MEM
{0,3,"Ctrl+Alt+U"}, // RECOVER
{2,9,"Ctrl+Alt+V"}, // MDI / DNC
{2,11,"Ctrl+Alt+X"}, // TOOL OFFSET MEASURE
{5,9,"Ctrl+Alt+Y"}, // LIST PROGRAM


// ─────────────── Ctrl-only LETTERS ───────────────
{4,7,"Ctrl+A"}, // ORIGIN
{5,7,"Ctrl+B"}, // SEND
{0,6,"Ctrl+D"}, // DELETE
{1,6,"Ctrl+E"}, // OPTION STOP
{3,6,"Ctrl+H"}, // .01
{4,6,"Ctrl+I"}, // SINGLE
{5,6,"Ctrl+J"}, // RECV
{1,5,"Ctrl+M"}, // BLOCK DELETE
{0,8,"Ctrl+P"}, // INSERT (common paste shortcut)
{3,5,"Ctrl+R"}, // .1
{4,5,"Ctrl+U"}, // HOME G28
{5,5,"Ctrl+Y"}, // ERASE PROGRAM
{0,5,"Ctrl+Z"}, // UNDO (common undo shortcut


// ─────────────── Ctrl+Shift+ LETTERS ───────────────

{0,12,"Ctrl+Shift+C"}, // RESET
{7,11,"Ctrl+Shift+D"}, // -10% FEED
{7,12,"Ctrl+Shift+E"}, // 100% FEED
{7,4,"Ctrl+Shift+F"}, // +10% FEED
{7,3,"Ctrl+Shift+H"}, // HAND FEED
{8,11,"Ctrl+Shift+I"}, // -10% SPINDLE
{8,4,"Ctrl+Shift+J"}, // +10% SPINDLE
{0,9,"Ctrl+Shift+L"}, // EDIT
{3,9,"Ctrl+Shift+M"}, // HAND JOG
{4,9,"Ctrl+Shift+O"}, // ZERO RETURN
{2,12,"Ctrl+Shift+P"}, // NEXT TOOL
{5,9,"Ctrl+Shift+R"}, // LIST PROGRAM
{2,4,"Ctrl+Shift+S"}, // TOOL RELEASE
{2,3,"Ctrl+Shift+T"}, // PART ZERO SET
{2,5,"Ctrl+Shift+U"}, // ATC REV
{2,6,"Ctrl+Shift+V"}, // ATC FWD
{3,11,"Ctrl+Shift+W"}, // CHIP FWD
{4,11,"Ctrl+Shift+X"}, // CHIP STOP
{5,11,"Ctrl+Shift+Y"}, // CHIP REV
{10,11,"Ctrl+Shift+Z"}, // RAPID 5%

// ─────────────── Alt+Shift+ LETTERS (Unified A–Z Safe Layer) ───────────────
{3,8,"Alt+Shift+B"}, // JOG STEP .0001 (was Ctrl+Alt+E)
{5,8,"Alt+Shift+C"}, // SELECT PROGRAM (was Ctrl+Alt+F)
{0,4,"Alt+Shift+E"}, // POWER UP / RESTART (was Ctrl+Alt+L)
{5,2,"Alt+Shift+F"}, // AUX CLNT (Mist)
{10,4,"Alt+Shift+G"}, // RAPID 50%
{1,8,"Alt+Shift+H"}, // SINGLE BLOCK (was Ctrl+Alt+P)
{9,4,"Alt+Shift+I"}, // SPINDLE CCW
{1,7,"Alt+Shift+J"}, // DRY RUN (was Ctrl+Alt+Q)
{4,2,"Alt+Shift+K"}, // CLNT DOWN
{3,7,"Alt+Shift+L"}, // JOG STEP .001 (was Ctrl+Alt+T)
{3,2,"Alt+Shift+M"}, // CLNT UP
{1,10,"Alt+Shift+N"}, // HELP / CALCULATOR
{2,7,"Alt+Shift+O"}, // ORIENT SPINDLE (was Ctrl+Alt+W)
{2,8,"Alt+Shift+Q"}, // FLOOD COOLANT
{9,11,"Alt+Shift+R"}, // SPINDLE CW
{9,12,"Alt+Shift+S"}, // SPINDLE STOP
{8,3,"Alt+Shift+T"}, // HAND SPINDLE
{10,12,"Alt+Shift+U"}, // RAPID 25%
{8,12,"Alt+Shift+V"}, // SPINDLE 100%
{10,3,"Alt+Shift+Z"}, // RAPID 100%

// ─────────────── Alt+Shift+ (Extra  axis selection Directions) ───────────────
{4,3,"Alt+Shift+1"}, // -X
{4,12,"Alt+Shift+2"}, // +X
{3,3,"Alt+Shift+3"}, // -Y
{5,12,"Alt+Shift+4"}, // +Y
{5,4,"Alt+Shift+5"}, // -Z
{3,4,"Alt+Shift+6"}, // +z
//{5,3,"Alt+Shift+7"}, // ROTARY -A
//{3,12,"Alt+Shift+8"}, // ROTARY +A


///needs to be tested. rotary axis -A/+A combos above will need to be commented out for B axis.
// ─────────────── Alt+Shift+ ( 4th/5th axis selection direction) ───────────────
{5,3,"ROTARY-"},  // Rotary -A / -B (handled in sendKey)
//                  //  Shift OFF → Alt+Shift+7  =  -A
//                  //  Shift ON  → Alt+Shift+9  =  -B
//
{3,12,"ROTARY+"}, // Rotary +A / +B (handled in sendKey)
//                  //  Shift OFF → Alt+Shift+8  =  +A
//                  //  Shift ON  → Alt+Shift+0  =  +B
// handled by Rotary Axis Handling (A/B via Haas Shift) below,


};
const size_t MAP_LEN = sizeof(MAP)/sizeof(MAP[0]);

char getHaasShiftedChar(char c){
  switch(c){
    case'1':return'*';case'2':return'\'';case'3':return'?';
    case'4':return'%';case'5':return'$';case'6':return'!';
    case'7':return'&';case'8':return'@';case'9':return':';
    case'0':return'=';case'-':return'+';case'.':return'#';
    default:return c;
  }
}

const char*findKey(uint8_t r,uint8_t c){
  for(size_t i=0; i<MAP_LEN; i++) {
    if(MAP[i].row == r && MAP[i].col == c)
      return MAP[i].key;
  }
  return nullptr;
}

// toggles the Haas panel SHIFT state so numbers/punctuation get remapped
void toggleShift() {
  shiftActive = !shiftActive;
  delay(200);   // small debounce so it doesn't flicker
}

// ────────────────────────────────────────────────────────────────
//  Keyboard.h version — sendCombo / sendKey / setup / loop
//  (Replaces TinyUSB section, uses Pico SDK stack)
// ────────────────────────────────────────────────────────────────

// ───────── Safe Ctrl+Alt combo sender (human-timed) ─────────
void sendCombo(const char* seq) {
  if (!seq) return;

  bool ctrl  = strstr(seq, "Ctrl+")  != nullptr;
  bool alt   = strstr(seq, "Alt+")   != nullptr;
  bool shift = strstr(seq, "Shift+") != nullptr;

  const char* final = strrchr(seq, '+');
  if (!final || !*(final + 1)) return;
  char keych = *(final + 1);

  // map to printable char
  char ascii = 0;
  if (isalpha(keych)) ascii = (char)tolower(keych);
  else if (isdigit(keych)) ascii = keych;
  else if (keych == '-') ascii = '-';
  else if (keych == '.') ascii = '.';
  else return;

  if (ctrl)  Keyboard.press(KEY_LEFT_CTRL);
  delay(10);
  if (alt)   Keyboard.press(KEY_LEFT_ALT);
  delay(10);
  if (shift) Keyboard.press(KEY_LEFT_SHIFT);
  delay(10);

  Keyboard.press(ascii);
  delay(50);
  Keyboard.release(ascii);

  delay(10);
  if (shift) Keyboard.release(KEY_LEFT_SHIFT);
  delay(10);
  if (alt)   Keyboard.release(KEY_LEFT_ALT);
  delay(10);
  if (ctrl)  Keyboard.release(KEY_LEFT_CTRL);
  delay(20);
}

// ───────── Main sendKey logic (with full combo support) ─────────
void sendKey(const char* key) {
  if (!key) return;

  // Handle any combo that uses Ctrl, Alt, or Shift together
  if (strstr(key, "Ctrl+") || strstr(key, "Alt+") || strstr(key, "Shift+")) {
    sendCombo(key);
    return;
  }

    // ───────── Rotary Axis Handling (A/B via Haas Shift) ─────────
  if (!strcmp(key, "ROTARY+")) {
    if (shiftActive)
      sendCombo("Alt+Shift+0");  // +B when Haas Shift is active
    else
      sendCombo("Alt+Shift+8");  // +A when Haas Shift is off
    return;
  }

  if (!strcmp(key, "ROTARY-")) {
    if (shiftActive)
      sendCombo("Alt+Shift+9");  // -B when Haas Shift is active
    else
      sendCombo("Alt+Shift+7");  // -A when Haas Shift is off
    return;
  }


  // Basic control keys
  if      (!strcmp(key, "CANCEL")) { Keyboard.press(KEY_BACKSPACE); delay(5); Keyboard.releaseAll(); return; }
  else if (!strcmp(key, "SPACE"))  { Keyboard.press(' '); delay(5); Keyboard.releaseAll(); return; }
  else if (!strcmp(key, "ENTER"))  { Keyboard.press(KEY_RETURN); delay(5); Keyboard.releaseAll(); return; }
  else if (!strcmp(key, "EOB"))    { Keyboard.press(shiftActive ? '/' : ';'); delay(5); Keyboard.releaseAll(); return; }

  // Arrow and jog keys
  else if (!strcmp(key, "LEFT"))   { Keyboard.press(KEY_LEFT_ARROW); delay(5); Keyboard.releaseAll(); return; }
  else if (!strcmp(key, "RIGHT"))  { Keyboard.press(KEY_RIGHT_ARROW); delay(5); Keyboard.releaseAll(); return; }
  else if (!strcmp(key, "UP"))     { Keyboard.press(KEY_UP_ARROW); delay(5); Keyboard.releaseAll(); return; }
  else if (!strcmp(key, "DOWN"))   { Keyboard.press(KEY_DOWN_ARROW); delay(5); Keyboard.releaseAll(); return; }
  else if (!strcmp(key, "PGUP"))   { Keyboard.press(KEY_PAGE_UP); delay(5); Keyboard.releaseAll(); return; }
  else if (!strcmp(key, "PGDN"))   { Keyboard.press(KEY_PAGE_DOWN); delay(5); Keyboard.releaseAll(); return; }

  // Softkeys
  else if (!strcmp(key, "F1"))     { Keyboard.press(KEY_F1); delay(5); Keyboard.releaseAll(); return; }
  else if (!strcmp(key, "F2"))     { Keyboard.press(KEY_F2); delay(5); Keyboard.releaseAll(); return; }
  else if (!strcmp(key, "F3"))     { Keyboard.press(KEY_F3); delay(5); Keyboard.releaseAll(); return; }
  else if (!strcmp(key, "F4"))     { Keyboard.press(KEY_F4); delay(5); Keyboard.releaseAll(); return; }

  // Normal single characters
  else if (strlen(key) == 1) {
    char c = key[0];
    if (shiftActive) {
      if (isdigit(c) || c == '-' || c == '.') c = getHaasShiftedChar(c);
      else if (c == '(') c = '[';
      else if (c == ')') c = ']';
      else if (isalpha(c)) c = toupper(c);
    }
    Keyboard.press(c);
    delay(5);
    Keyboard.release(c);
  }
}


// ───────── Setup and main loop ─────────
void setup() {
  delay(800);
  Keyboard.begin();

  for (uint8_t r = 0; r < ROWS; r++) {
    pinMode(rowPins[r], OUTPUT);
    digitalWrite(rowPins[r], HIGH);
  }
  for (uint8_t c = 0; c < COLS; c++) pinMode(colPins[c], INPUT_PULLUP);
}

void loop() {
  for (uint8_t r = 0; r < ROWS; r++) {
    digitalWrite(rowPins[r], LOW);
    for (uint8_t c = 0; c < COLS; c++) {
      if (digitalRead(colPins[c]) == LOW) {
        if (r == SHIFT_ROW && c == SHIFT_COL) {
          toggleShift();
          while (digitalRead(colPins[c]) == LOW) delay(5);
        } else {
          const char* key = findKey(r, c);
          sendKey(key);
          delay(10);
          while (digitalRead(colPins[c]) == LOW) delay(5);
        }
      }
    }
    digitalWrite(rowPins[r], HIGH);
  }
}
