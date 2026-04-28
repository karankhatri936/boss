void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT); // LED
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');

    cmd.trim(); // remove spaces/newline

    if (cmd == "LED_ON") {
      digitalWrite(13, HIGH);
    }
    else if (cmd == "LED_OFF") {
      digitalWrite(13, LOW);
    }
  }
}   