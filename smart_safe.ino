#include <Servo.h>
#include <Keypad.h>

Servo L_Servo;
Servo R_Servo;

const int buttonPin = 2;
const int U_LED = 51;
const int L_LED = 53;
const byte ROWS = 4; // Four rows
const byte COLS = 4;
const String PW = "1021";

char keys[ROWS][COLS] = {
   {'1', '2', '3', 'A'},
   {'4', '5', '6', 'B'},
   {'7', '8', '9', 'C'},
   {'*', '0', '#', 'D'} 
};

byte rowPins[ROWS] = {22, 24, 26, 28};
byte colPins[COLS] = {30, 32, 34, 36};

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String test_PW = "";
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
  char key = kpd.getKey();
  switch (key) {

    case '*':
      test_PW = "";
      Serial.println("Test PW Reset");
      break;

    case '#':
      if (test_PW == PW) {
        Serial.println("Safe Unlocked!");
        rotateServo();
        test_PW = "";
        Serial.println("Test PW Reset");
      }
      break;

    case NO_KEY:
      break;

    default: 
      test_PW += key;
      Serial.println(test_PW + " KEY = " + key);
      break;
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
  }
  delay(500);
}

