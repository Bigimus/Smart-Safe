#include <Servo.h>

Servo L_Servo;
Servo R_Servo;

const int buttonPin = 2;
const int U_LED = 51;
const int L_LED = 53;

volatile int buttonState = 0;
int L_ServoState = 0;
int R_ServoState = 0;
int lastButtonState = LOW;

volatile bool Interrupt = false;

void setup() {
  Serial.begin(9600);

  L_Servo.attach(52);
  L_Servo.write(180);
  R_Servo.attach(50);
  R_Servo.write(180);

  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), buttonPress, CHANGE);

  pinMode(U_LED, OUTPUT);
  pinMode(L_LED, OUTPUT);
  digitalWrite(L_LED, HIGH);
  digitalWrite(U_LED, LOW);
  
}

void loop() {
  if (Interrupt) {
    rotateServo();
    Interrupt = false;
  }
  
}

void buttonPress() {
  Interrupt = true;
}

void rotateServo() {
  L_ServoState = L_Servo.read();
  R_ServoState = R_Servo.read();
  if (L_ServoState == 0 && R_ServoState == 0){
    L_Servo.write(180);
    R_Servo.write(180);
    digitalWrite(L_LED, HIGH);
    digitalWrite(U_LED, LOW);
  } else if (L_ServoState == 180 && R_ServoState == 180){
    L_Servo.write(0);
    R_Servo.write(0);
    digitalWrite(L_LED, LOW);
    digitalWrite(U_LED, HIGH);
  } else {
    
  }
  delay(500);
}
