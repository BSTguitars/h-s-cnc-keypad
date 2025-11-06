// Haas Panel Matrix Mapper - 11x13 version for Raspberry Pi Pico (RP2040)
// Uses GPIO 0–22 and 26 (24 total pins)
// Rows: GP0–GP10  (11 total)
// Cols: GP11–GP22 + GP26 (13 total)

const uint8_t rowPins[11] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const uint8_t colPins[13] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 26};

void setup() {
  Serial.begin(115200);
  delay(1000); // small pause to let serial settle
  Serial.println("Haas matrix mapper ready...");

  // setup rows as outputs, default HIGH
  for (uint8_t r = 0; r < 11; r++) {
    pinMode(rowPins[r], OUTPUT);
    digitalWrite(rowPins[r], HIGH);
  }

  // setup columns as inputs with pull-ups
  for (uint8_t c = 0; c < 13; c++) {
    pinMode(colPins[c], INPUT_PULLUP);
  }
}

void loop() {
  for (uint8_t r = 0; r < 11; r++) {
    digitalWrite(rowPins[r], LOW); // activate one row
    for (uint8_t c = 0; c < 13; c++) {
      if (digitalRead(colPins[c]) == LOW) {
        Serial.print("Key: Row ");
        Serial.print(r);
        Serial.print("  Col ");
        Serial.println(c);
        delay(200); // debounce
        while (digitalRead(colPins[c]) == LOW); // wait for release
      }
    }
    digitalWrite(rowPins[r], HIGH); // deactivate row
  }
}
