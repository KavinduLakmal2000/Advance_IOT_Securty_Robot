// PIR - D5
// MQ2 - D3
// servo - D8
// DHT11 - D4
// Fire sensor - D6
// battery voltage - A0

#define BLYNK_TEMPLATE_ID "TMPL6GV8veXRR"
#define BLYNK_TEMPLATE_NAME "Spider"
#define BLYNK_AUTH_TOKEN "y0HfuxqIifEr-WLh6XruakCrKcT1TW-n"

// #define BLYNK_TEMPLATE_ID "TMPLln-Vhydm"
// #define BLYNK_DEVICE_NAME "test"
// #define BLYNK_AUTH_TOKEN "b9tvGjv-42Mytf7RtFeo-GgPrCZZo6Dd"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#include <DHT.h>


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

const char* ssid = "SLT_Fiber_Optic";           
const char* pass = "Life1Mal7i";   
const char* targetIP = "192.168.1.11";  

boolean sleepMode = true;
boolean securtyMode = false;
boolean powerSave = false;

WiFiClient client;  
Servo FaceServo;
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);

void setup() {
 
  Serial.begin(9600);          
  Serial.setTimeout(10); 

  FaceServo.attach(D8);
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
}

void loop() {
  //Blynk.logEvent("full_charged");
  Blynk.run();
  Indicator_Led(20,21);
  
  FaceServo.write(90);


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
    sensorRead();              
    Blynk.virtualWrite(V7, 0); 
    Blynk.virtualWrite(V9, 0); 
    Serial.println("stop");
  } 
  


}



void securty(){

  if (digitalRead(PIR) == HIGH){
    Blynk.virtualWrite(V4, 1);
    //sendPowerCutOff(3);
  }
  else{
    Blynk.virtualWrite(V4, 0);
    //sendPowerCutOff(4);
  }

  if (digitalRead(IR) == LOW){
    Blynk.virtualWrite(V5, 1);
    //sendPowerCutOff(2);
  }
  else{
    Blynk.virtualWrite(V5, 0);
    //sendPowerCutOff(1);
  }


  if (digitalRead(MQ2) == LOW){
    Blynk.virtualWrite(V3, 1);
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

