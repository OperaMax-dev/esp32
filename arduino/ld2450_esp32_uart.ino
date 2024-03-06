HardwareSerial mySerial(1);

void setup() {
  mySerial.begin(256000, SERIAL_8N1, 16, 17);
  Serial.begin(115200);
}

void loop() {
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}
