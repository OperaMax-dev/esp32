void setup() {
  Serial.begin(256000);
}

void loop() {
  if (Serial.available()) {
    int data = Serial.read();
    if (data > 0 && data < 255) {
      Serial.println("Valid packet");
    } else {
      Serial.println("Checksum error");
    }
  }
}
