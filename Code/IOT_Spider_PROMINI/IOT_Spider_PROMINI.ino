// leg movement for left body motors
#include <Servo.h>
#include <NewPing.h>

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

#define TRIGGER_PIN_1 A2 
#define ECHO_PIN_1    A3  

#define TRIGGER_PIN_2 A0 
#define ECHO_PIN_2    A1  

#define MAX_DISTANCE 200  

NewPing sonar1(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE); //for walls
NewPing sonar2(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE);  // for sters 

#define LED A4
int servoSpeed = 10;
int count = 0;
//String command;

void serialReadMCU(){


}

void setup() {

  Serial.begin(9600);

  pinMode(LED, OUTPUT);

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
}

void loop() {
//serialReadMCU();

Indicator_Led(40, 45);


 if (Serial.available()) {
    String command = Serial.readStringUntil('\n');  
    command.trim();  

     if(command == "stop") {
    standUp();
    } 
    if(command == "move") {
     robotMove();
    }

  }


 delay(30);

}


void robotMove(){
  unsigned int distance1 = sonar1.ping_cm();
  unsigned int distance2 = sonar2.ping_cm();
    if (distance1 < 40) {
      //turn
      turn();
    } 

    else {
      //go forward
      walkForward();
    }

}

void turn(){
  LA1.write(LA1pullUp);
  RA3.write(RA3pullUp);

  delay(200);

  LB1.write(LB1goMid);
  RB3.write(RB3goMid);
  
  delay(100);

  LA1.write(LA1pullDown);
  RA3.write(RA3pullDown);

  delay(200);

  LA2.write(LA2pullDown);
  RA2.write(RA2pullDown);

  delay(500);

  LB1.write(LB1goFront);
  RB3.write(RB3goBack);

  LB3.write(LB3goMid);
  RB1.write(RB1goMid);

  LB2.write(LB2goFront);
  RB2.write(RB2goBack);
///////////////////////////////////////////////////////////////
  delay(200);

  LA2.write(LA2pullUp);
  RA2.write(RA2pullUp);

  delay(200);

  LB2.write(LB2goMid);
  RB2.write(RB2goMid);

  delay(100);

  LA2.write(LA2pullDown);
  RA2.write(RA2pullDown);
  
  delay(200);

  RA1.write(RA1pullUp);
  LA3.write(LA3pullUp);

  delay(100);

  RB1.write(RB1goFront);
  LB3.write(LB3goBack);

  delay(200);


}

void walkForward() {

LA1.write(LA1pullUp);
LA3.write(LA3pullUp);
RA2.write(RA2pullUp);
delay(500);

RB1.write(RB1goMid);
RB3.write(RB3goBack);
LB2.write(LB2goBack);
LA1.write(LA1pullDown);

delay(500);

LA3.write(LA3pullDown);
RA2.write(RA2pullDown);

delay(500);

LA2.write(LA2pullUp);
RA1.write(RA1pullUp);
RA3.write(RA3pullUp);

delay(100);

RB1.write(RB1goFront);
RB3.write(RB3goMid);
LB2.write(LB3goMid);

delay(500);

LB1.write(LB1goMid);
RB2.write(RB2goBack);
LB3.write(LB3goBack);

RA1.write(RA1pullDown);

delay(500);

LA2.write(LA2pullDown);
RB2.write(RB2goMid);
RA2.write(RA2pullUp);
RA3.write(RA3pullDown);

delay(100);

LA1.write(LA1pullUp);
LA3.write(LA3pullUp);

LB3.write(LB3goMid);
LB1.write(LB1goFront);
RB2.write(RB2goMid);

delay(100);


}


void standUp(){
  LB1.write(81);
  RB1.write(107);

  LB2.write(93);
  RB2.write(89);

  LB3.write(83);
  RB3.write(84);

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

void Indicator_Led(int C_start, int C_end){

if (count == C_start){
  digitalWrite(LED, HIGH);
}

else if (count == C_end){
  digitalWrite(LED, LOW);
  count = 0;
}


count++;

}





