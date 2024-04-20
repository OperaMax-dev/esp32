void setup() {
  Serial.begin(115200);
  esp_task_wdt_init(5, true); // 5 seconds timeout
  esp_task_wdt_add(NULL);
}

void loop() {
  if (Serial.available()) {
    esp_task_wdt_reset();  // Feed watchdog
    Serial.println("UART active");
  } else {
    delay(100);
  }
}
