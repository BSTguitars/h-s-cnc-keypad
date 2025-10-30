// Haas Panel Matrix Mapper - 11x13 version for Mega
// Rows = even pins 22-42 (11 total)
// Columns = odd pins 23-45 plus 44 (13 total)

const byte rowPins[11] = {22,24,26,28,30,32,34,36,38,40,42};
const byte colPins[13] = {23,25,27,29,31,33,35,37,39,41,43,44,45};

void setup() {
  Serial.begin(115200);
  Serial.println("Haas matrix mapper ready...");

  // setup rows
  for (byte r = 0; r < 11; r++) {     // ✅ match array length
    pinMode(rowPins[r], OUTPUT);
    digitalWrite(rowPins[r], HIGH);
  }

  // setup columns
  for (byte c = 0; c < 13; c++) {     // ✅ match array length
    pinMode(colPins[c], INPUT_PULLUP);
  }
}

void loop() {
  for (byte r = 0; r < 11; r++) {     // ✅ match array length
    digitalWrite(rowPins[r], LOW);
    for (byte c = 0; c < 13; c++) {   // ✅ match array length
      if (digitalRead(colPins[c]) == LOW) {
        Serial.print("Key: Row ");
        Serial.print(r);
        Serial.print("  Col ");
        Serial.println(c);
        delay(200);
        while (digitalRead(colPins[c]) == LOW); // wait for release
      }
    }
    digitalWrite(rowPins[r], HIGH);
  }
}
