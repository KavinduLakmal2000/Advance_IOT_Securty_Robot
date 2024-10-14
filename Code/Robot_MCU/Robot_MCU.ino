#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "SLT_Fiber_Optic";           
const char* password = "Life1Mal7i";   
const char* targetIP = "192.168.1.11";  

WiFiClient client;  

void setup() {
 
  Serial.begin(9600);          
  Serial.setTimeout(10); 

  WiFi.begin(ssid, password);  
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


}

void loop() {

  Serial.println("ON");
  delay(1000); 

  Serial.println("OFF");
  delay(1000); 



  sendHTTPRequest("/cut230On");
  delay(1000);  
  
  sendHTTPRequest("/cut230Off");
  delay(1000);  

  sendHTTPRequest("/ArlamOn");
  delay(1000);  

  sendHTTPRequest("/ArlamOff");
  delay(1000);  

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
