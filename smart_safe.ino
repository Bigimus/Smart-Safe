#include <Servo.h>
#include <RTClib.h>
#include <Wire.h>

//SERVO(S)
Servo Servo;
int servoState = 0;
const int servoMin = 30;
const int servoMax = 180;

//RTC
RTC_DS3231 rtc;

//BUTTON(S)
const int button = 2; 
int buttonState = 0;
volatile bool Interrupt = false;
unsigned long lastInterrupt;

//LED(S)
const int greenLED = 4;
const int redLED = 3;

//MISC
char receivedChar;
int lockState;
const int unlockTime = 25;
const int lockTime = 0;
const int locked = 1;
const int unlocked = 0;

void setup() {
  Serial.begin(9600);
  Servo.attach(5);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(button, INPUT);
  attachInterrupt(digitalPinToInterrupt(button), buttonPress, FALLING);
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  startUp();
}

void loop() {
  DateTime now = rtc.now();

  while (now.hour() >= unlockTime || now.hour() < lockTime){
    if (Interrupt){
      rotateServo();
      Interrupt = false;
      break;
    } 

    else if (digitalRead(greenLED) == LOW){
      blinkLED(redLED);
      break;
    }

    else if (digitalRead(greenLED) == HIGH){
      blinkLED(greenLED);
      break;
    }
  }

  while (now.hour() < unlockTime && now.hour() > lockTime){
    if (Serial.available() > 0) {
    receivedChar = Serial.read();

      //Reset
      if (receivedChar == 'r'){
        startUp();
        break;
      }

      //Lock / Unlock
      if (receivedChar == 'u' || receivedChar == 'l'){
        rotateServo();
        break;
      }
    }
    if (lockState = unlocked) {
      if (Interrupt) {
        rotateServo();
        Interrupt = false;
        break;
      }
    }
  }
}

void blinkLED(int pin){
  digitalWrite(pin, LOW);
  delay(500);
  digitalWrite(pin, HIGH);
  delay(500);
}

void rotateServo() {
  servoState = Servo.read();

  if (servoState <= servoMin){
    //UNLOCKING
    Servo.write(servoMax);
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
    lockState = unlocked;
  } 

  else if (servoState == servoMax){
    //LOCKING
    Servo.write(servoMin);
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);
    lockState = locked;
  }
  Serial.println(lockState);
  delay(500);
}

void buttonPress() {
  if (millis() - lastInterrupt > 500 && Interrupt != true){
    Interrupt = true;
    lastInterrupt = millis();
  }
}

void startUp(){
  //LOCKING
  Servo.write(servoMin);
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, HIGH);
  lockState = locked;
}
