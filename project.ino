// Two FSR sensors read independently + 10-second continuous pressure check
// LED1 (pin 8) = pressure detected now
// LED2 (pin 9) = pressure continuous for 10 sec

const int fsrA = A0;
const int fsrB = A1;
const int fsrC = A2;
const int fsrD = A3;
const int ledPresence = 8;
const int ledContinuity = 9;

const int THRESHOLD = 10;
const unsigned long REQUIRED_CONTINUITY_MS = 10000UL; // 10 seconds in ms

bool continuousPresence = false;
unsigned long continuityStartTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPresence, OUTPUT);
  pinMode(ledContinuity, OUTPUT);
  digitalWrite(ledContinuity, LOW);
}

void loop() {
  unsigned long now = millis();

  int vA = analogRead(fsrA);
  int vB = analogRead(fsrB);
  int vC = analogRead(fsrC);
  int vD = analogRead(fsrD);

  Serial.print("FSR_A = "); Serial.print(vA);
  Serial.print("  |  FSR_B = "); Serial.print(vB);
  Serial.print("  |  FSR_C = "); Serial.print(vC);
  Serial.print("  |  FSR_D = "); Serial.println(vD);

  bool pressureNow = (vA > THRESHOLD || vB > THRESHOLD || vC > THRESHOLD || vD > THRESHOLD );
  digitalWrite(ledPresence, pressureNow ? HIGH : LOW);

  // ===== HERE IS THE 10-SECOND CRITERIA CHECK 👇 =====
  if (pressureNow) {
    if (!continuousPresence) {
      continuousPresence = true;
      continuityStartTime = now; // mark when pressure began
    }
    if (now - continuityStartTime >= REQUIRED_CONTINUITY_MS) {
      digitalWrite(ledContinuity, HIGH); // LED2 ON after 10 sec continuous pressure
      
    }
  } else {
    continuousPresence = false;
    continuityStartTime = 0;
    digitalWrite(ledContinuity, LOW); // reset if pressure lost
  }
}