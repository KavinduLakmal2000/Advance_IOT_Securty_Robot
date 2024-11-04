//------------------------------------------receiver

// D5 green
// D6 Red

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define greenLed D5
#define redLed D6

const char* ssid = "SLT_Fiber_Optic";        
const char* password = "Life1Mal7i"; 

ESP8266WebServer server(80);   

const int MainCutOff = D2;
const int Arlam = D3;   
int count = 0;               

void setup() {

  pinMode(MainCutOff, OUTPUT);
  pinMode(Arlam, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);

  digitalWrite(MainCutOff, LOW);  
  digitalWrite(Arlam, LOW);  

  Serial.begin(9600);
  WiFi.begin(ssid, password);
  unsigned long startAttemptTime = millis();
  bool connected = false;

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(greenLed, HIGH);
    delay(250);
    digitalWrite(greenLed, LOW);
    delay(250);
    
    if (millis() - startAttemptTime >= 10000) {
      Serial.println("Failed to connect to WiFi.");
      digitalWrite(redLed, HIGH);  
      connected = false;
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    connected = true;
    digitalWrite(greenLed, HIGH);  
  }

  
  server.on("/cut230On", []() {
    digitalWrite(MainCutOff, HIGH);     
    server.send(200, "text/plain", "ON");
    digitalWrite(redLed, HIGH);  
    Serial.println("Power is cut!");
  });

  server.on("/cut230Off", []() {
    digitalWrite(MainCutOff, LOW); 
    digitalWrite(redLed, LOW);     
    server.send(200, "text/plain", "OFF");
    Serial.println("turned off the power cut switch!");
  });
////////////////////////////////////////////////////
    server.on("/ArlamOn", []() {
    digitalWrite(Arlam, HIGH);     
    server.send(200, "text/plain", "AON");
    Serial.println("Arlam is On!");
  });

    server.on("/ArlamOff", []() {
    digitalWrite(Arlam, LOW);     
    server.send(200, "text/plain", "AOff");
    Serial.println("Arlam is Off!");
  });


  server.begin();
  Serial.println("Server started");
  delay(2000);
  digitalWrite(greenLed, LOW);
}

void loop() {
  server.handleClient();
  Indicator_Led(100000, 110000);
}


void Indicator_Led(int C_start, int C_end){

if (count == C_start){
  digitalWrite(greenLed, HIGH);
}

else if (count == C_end){
  digitalWrite(greenLed, LOW);
  count = 0;
}


count++;

}
