int values[10];
int idx = 0;

void setup() {
  Serial.begin(256000);
}

void loop() {
  if (Serial.available()) {
    values[idx] = Serial.read();
    idx = (idx + 1) % 10;
    int sum = 0;
    for (int i = 0; i < 10; i++) sum += values[i];
    Serial.println(sum / 10);
  }
}
