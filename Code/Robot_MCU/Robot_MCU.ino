void setup() {
  // Initialize Serial communication with Arduino Pro Mini
  Serial.begin(9600);          
  Serial.setTimeout(10);  // Set timeout for serial reading
}

void loop() {
  // Send "ON" command to Arduino Pro Mini
  Serial.println("ON");
  delay(1000); // Wait for 1 second

  // Send "OFF" command to Arduino Pro Mini
  Serial.println("OFF");
  delay(1000); // Wait for 1 second
}
