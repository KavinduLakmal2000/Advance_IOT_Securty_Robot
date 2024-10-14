//------------------------------------------receiver

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "SLT_Fiber_Optic";        
const char* password = "Life1Mal7i"; 

ESP8266WebServer server(80);   

const int MainCutOff = D2;
const int Arlam = D3;                  

void setup() {

  pinMode(MainCutOff, OUTPUT);
  pinMode(Arlam, OUTPUT);

  digitalWrite(MainCutOff, LOW);  
  digitalWrite(Arlam, LOW);  

  Serial.begin(9600);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");
  Serial.println(WiFi.localIP());  

  
  server.on("/cut230On", []() {
    digitalWrite(MainCutOff, HIGH);     
    server.send(200, "text/plain", "ON");
    Serial.println("Power is cut!");
  });

  server.on("/cut230Off", []() {
    digitalWrite(MainCutOff, LOW);    
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
}

void loop() {
  server.handleClient();  
}
