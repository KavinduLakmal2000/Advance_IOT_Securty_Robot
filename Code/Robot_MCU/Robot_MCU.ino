// PIR - D5
// MQ2 - D3
// servo - D8
// DHT11 - D4
// Fire sensor - D6
// battery voltage - A0

#define BLYNK_TEMPLATE_ID "test"
#define BLYNK_TEMPLATE_NAME "Spider"
#define BLYNK_AUTH_TOKEN "##########################################"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define BLYNK_PRINT Serial

#define DHTPIN D4
#define PIR D5
#define MQ2 D3
#define IR D6
#define led D0

const float maxBatteryVoltage = 8.4;
const float minBatteryVoltage = 7; 
const int maxADCValue = 1023;
int count = 0;
int currentPosition = 90;
int randomAngle;

const char* ssid = "SLT_Fiber_Optic";           
const char* pass = "Life1Mal7i"; 

// char ssid[] = "4G-MIFI-IOT";
// char pass[] = "Kl1234567890";

const char* targetIP = "192.168.1.16";  

boolean sleepMode = true;
boolean securtyMode = false;
boolean powerSave = false;

WiFiClient client;  
Servo FaceServo;
#define DHTTYPE DHT11 

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

DHT dht(DHTPIN, DHTTYPE);

int c1 = 0;
int c2 = 0;

void setup() {
 
  Serial.begin(9600);          
  Serial.setTimeout(10); 

  FaceServo.attach(D8);
  FaceServo.write(currentPosition);

  dht.begin();

  pinMode(PIR, INPUT);
  pinMode(IR, INPUT);
  pinMode(MQ2, INPUT);
  pinMode(led, OUTPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500); 
    Serial.println("Connecting to WiFi...");
    digitalWrite(led, LOW);
  }

  Serial.println("Connected to WiFi!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());  
  digitalWrite(led, HIGH);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;);
  }
  display.clearDisplay();
  display.display();
  delay(1000);
}

void loop() {
  //Blynk.logEvent("full_charged");
  Blynk.run();
  Indicator_Led(20,21);

  happyEyes();
  servoMoveRandom(90, 180);
  delay(50);
  blinkEyes();
  servoMoveRandom(0, 90);
  delay(50);
  surprisedEyes();
  servoMoveRandom(90, 180);
  delay(50);
  servoMoveRandom(0, 90);

  if (powerSave){///////////////////////////////////////////////////////// power save mode (all sensors working stop moving)
    sleepMode = false;
    securtyMode = false;
    sensorRead();
    securty();
    Blynk.virtualWrite(V7, 0); 
    Blynk.virtualWrite(V6, 0);
    Serial.println("stop");
  }


 if(securtyMode) { ////////////////////////////////////////////////////////// securty mode (all sensors and robot moving)
    sleepMode = false;
    powerSave = false; 
    sensorRead(); 
    securty();          
    Blynk.virtualWrite(V6, 0); 
    Blynk.virtualWrite(V9, 0);
    Serial.println("move");
  } 
 
 

  if (sleepMode) { ///////////////////////////////////////////////////////// sleep mode (call only sensorRead();)
    securtyMode = false;
    powerSave = false;  
    sleepModee();
    Blynk.virtualWrite(V7, 0); 
    Blynk.virtualWrite(V9, 0); 
    Serial.println("stop");
  } 
  


}

void sleepModee(){
  int adcValue = analogRead(A0);
  float batteryVoltage = adcValue * (maxBatteryVoltage / maxADCValue);
  int batteryPercentage = map(batteryVoltage * 100, minBatteryVoltage * 100, maxBatteryVoltage * 100, 0, 100);

  Blynk.virtualWrite(V0, 0);
  Blynk.virtualWrite(V1, 0);
  Blynk.virtualWrite(V2, 0);
  Blynk.virtualWrite(V4, 0);
  Blynk.virtualWrite(V5, 0);
  Blynk.virtualWrite(V3, 0);
  Blynk.virtualWrite(V8, batteryPercentage);
}

void securty(){

  if (digitalRead(PIR) == HIGH){
    Blynk.virtualWrite(V4, 1);
    Blynk.logEvent("m_detect", "Motion");
    //sendPowerCutOff(3);
  }
  else{
    Blynk.virtualWrite(V4, 0);
    //sendPowerCutOff(4);
  }

  if (digitalRead(IR) == LOW){
    Blynk.virtualWrite(V5, 1);
    //sendPowerCutOff(2);
    Blynk.logEvent("ir_detect","Fire");
  }
  else{
    Blynk.virtualWrite(V5, 0);
    //sendPowerCutOff(1);
  }


  if (digitalRead(MQ2) == LOW){
    Blynk.virtualWrite(V3, 1);
    Blynk.logEvent("gas_detect", "gas");
    //sendPowerCutOff(2);
  }
  else{
    Blynk.virtualWrite(V3, 0);
    //sendPowerCutOff(1);
  }

} 

void sendPowerCutOff(int data){
  if (data == 2){
    sendHTTPRequest("/cut230On");
    delay(1000);  
    sendHTTPRequest("/ArlamOn");
    delay(1000);
  }

  if (data == 1){
  sendHTTPRequest("/cut230Off");
  delay(1000);
  sendHTTPRequest("/ArlamOff");
  delay(1000);
  }

  if (data == 3){
    sendHTTPRequest("/ArlamOn");
  }

  if (data == 4){
    sendHTTPRequest("/ArlamOff");
  }

}

void sensorRead(){
  float humidity = dht.readHumidity();         
  float temperature = dht.readTemperature();  

  int adcValue = analogRead(A0);
  float batteryVoltage = adcValue * (maxBatteryVoltage / maxADCValue);
  int batteryPercentage = map(batteryVoltage * 100, minBatteryVoltage * 100, maxBatteryVoltage * 100, 0, 100);

  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, batteryVoltage);
  Blynk.virtualWrite(V8, batteryPercentage);

  }


void sendHTTPRequest(String command) {
  if (WiFi.status() == WL_CONNECTED) {  
    HTTPClient http;
    String url = "http://" + String(targetIP) + command;  

    http.begin(client, url); 
    int httpCode = http.GET(); 

    if (httpCode > 0) {
      String payload = http.getString();  
      Serial.println(payload);  
    } else {
      Serial.println("Error in HTTP request");
    }
    http.end();  
  } else {
    Serial.println("WiFi not connected");
  }
}



BLYNK_WRITE(V6) {
  sleepMode = param.asInt(); 
}

BLYNK_WRITE(V7) { 
  securtyMode = param.asInt(); 
}

BLYNK_WRITE(V9) { 
  powerSave = param.asInt(); 
}

void drawEyes(int eyeSize, int eyeGap) {
  display.fillRect(25, 20, eyeSize, eyeSize, SSD1306_WHITE);
  display.fillRect(75 + eyeGap, 20, eyeSize, eyeSize, SSD1306_WHITE);
}

void clearEyes(int eyeSize, int eyeGap) {
  display.fillRect(25, 20, eyeSize, eyeSize, SSD1306_BLACK);
  display.fillRect(75 + eyeGap, 20, eyeSize, eyeSize, SSD1306_BLACK);
  display.display();
}

void blinkEyes() {
  clearEyes(20, 10);
  delay(100);
  drawEyes(20, 10);
  display.display();
  delay(100);
}

void happyEyes() {
  clearEyes(20, 10);
  drawEyes(15, 10);
  display.display();
  delay(200);
}

void surprisedEyes() {
  clearEyes(20, 10);
  drawEyes(25, 10);
  display.display();
  delay(200);
}

void servoMoveRandom(int minAngle, int maxAngle) {
  randomAngle = random(minAngle, maxAngle + 1);
  FaceServo.write(randomAngle);
  delay(200);
  FaceServo.write(90);
  delay(200);
}


void Indicator_Led(int C_start, int C_end){

if (count == C_start){
  digitalWrite(led, LOW);
}

else if (count == C_end){
  digitalWrite(led, HIGH);
  count = 0;
}


count++;

}

