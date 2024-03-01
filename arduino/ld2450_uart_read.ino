void setup() {
  Serial.begin(256000);
}

void loop() {
  while (Serial.available()) {
    Serial.write(Serial.read());
  }
}
