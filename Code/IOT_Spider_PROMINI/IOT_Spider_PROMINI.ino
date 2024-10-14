#include "arduino_secrets.h"

// leg movement for left body motors
#include <Servo.h>

#define LB1goFront 81
#define LB2goFront 51
#define LB3goFront 120

#define LB1goMid 110
#define LB2goMid 93
#define LB3goMid 93

#define LB1goBack 149
#define LB2goBack 126
#define LB3goBack 58

// leg movement for right body motors
#define RB1goFront 107
#define RB2goFront 120
#define RB3goFront 45

#define RB1goMid 77
#define RB2goMid 89
#define RB3goMid 74

#define RB1goBack 36
#define RB2goBack 52
#define RB3goBack 109

// leg movement for left arm moters
#define LA1pullDown 60
#define LA1pullUp 155

#define LA2pullDown 78
#define LA2pullUp 162

#define LA3pullDown 128
#define LA3pullUp 40


// leg movemt for right arm motors
#define RA1pullUp 15
#define RA1pullDown 125

#define RA2pullUp 10
#define RA2pullDown 113

#define RA3pullUp 100
#define RA3pullDown 7

Servo LB1; 
Servo LB2;  
Servo LB3;  
Servo RB1;  
Servo RB2; 
Servo RB3;  
Servo LA1; 
Servo LA2;  
Servo LA3; 
Servo RA1;  
Servo RA2; 
Servo RA3; 

void setup() {

  Serial.begin(9600);

  LB1.attach(4);
  LB2.attach(3);
  LB3.attach(2);
  RB1.attach(5);
  RB2.attach(6);
  RB3.attach(7);
  LA1.attach(10);
  LA2.attach(9);
  LA3.attach(8);
  RA1.attach(11);
  RA2.attach(12);
  RA3.attach(13);
  Serial.begin(9600);  
}

void loop() {
walkForward();


  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');  
    command.trim();  

    if(command == "ON") {
      Serial.println("LED ON");
    } 
    else if(command == "OFF") {
      Serial.println("LED OFF"); 
    }

  }


// standUp();
// delay(2000);
// standDown();
// delay(2000);
// Center();
}



void walkForward() {

}


void standUp(){
  LB1.write(81);
  RB1.write(107);

  LB2.write(93);
  RB2.write(89);

  LB3.write(58);
  RB3.write(109);

  LA1.write(60);
  LA2.write(78);
  LA3.write(128);

  RA1.write(125);
  RA2.write(113);
  RA3.write(7);
}

void standDown(){
  LB1.write(81);
  RB1.write(107);

  LB2.write(93);
  RB2.write(89);

  LB3.write(58);
  RB3.write(109);

  LA1.write(155);
  LA2.write(162);
  LA3.write(40);

  RA1.write(15);
  RA2.write(10);
  RA3.write(100);
}