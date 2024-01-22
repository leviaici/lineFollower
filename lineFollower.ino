// Leventiu Adrian-George, Ionescu Mihai-Leonard, Ioja Denis - 362
// Line Follower - Los Valores

#include <QTRSensors.h>

const int m11Pin = 7;
const int m12Pin = 6;
const int m21Pin = 5;
const int m22Pin = 4;
const int m1Enable = 11;
const int m2Enable = 10;

const int zero = 0;

int m1Speed = zero;
int m2Speed = zero;

float kp = 15;
float ki = zero;
float kd = zero;

int p = 1;
int i = zero;
int d = zero;
int error = zero;
int lastError = zero;

const int maxSpeed = 255;
const int minSpeed = -255;
const int baseSpeed = 180;

const int startDelay = 500;

const int calibrationDuration = 400;

QTRSensors qtr;

const int sensorCount = 6;
int sensorValues[sensorCount];
int sensors[sensorCount] = { zero, zero, zero, zero, zero, zero };

void setup() {

  pinMode(m11Pin, OUTPUT);
  pinMode(m12Pin, OUTPUT);
  pinMode(m21Pin, OUTPUT);
  pinMode(m22Pin, OUTPUT);
  pinMode(m1Enable, OUTPUT);
  pinMode(m2Enable, OUTPUT);
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){ A0, A1, A2, A3, A4, A5 }, sensorCount);
  delay(startDelay);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  calibrate();

  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  pidControl(kp, ki, kd);
  motors();
  
}

void calibrate(){
  const int calibrateDuration = 20; 
  int baseSpeed = 105;

  for (uint16_t i = zero; i < calibrationDuration; i++) {
    qtr.calibrate();
    
    if(i % calibrateDuration == zero) {
      setMotorSpeed(baseSpeed, zero);
      baseSpeed = baseSpeed * -1;
    }
  }
  setMotorSpeed(zero, zero);
}

void motors(){
  int motorSpeed = kp * p + ki * i + kd * d;  
  m1Speed = baseSpeed;
  m2Speed = baseSpeed;

  if (error < zero) {
    m1Speed += motorSpeed;
  } else if (error > zero) {
    m2Speed -= motorSpeed;
  }

  m1Speed = constrain(m1Speed, minSpeed, maxSpeed);
  m2Speed = constrain(m2Speed, minSpeed, maxSpeed);
  setMotorSpeed(m1Speed, m2Speed);
}

void pidControl(float kp, float ki, float kd) {
  int error = map(qtr.readLineBlack(sensorValues), zero, 5000, -50, 50);
  p = error;
  i = i + error;
  d = error - lastError;
}

void setMotorSpeed(int motor1Speed, int motor2Speed) {
  if (motor1Speed == zero) {
    digitalWrite(m11Pin, LOW);
    digitalWrite(m12Pin, LOW);
    analogWrite(m1Enable, motor1Speed);
  } else {
    if (motor1Speed > zero) {
      digitalWrite(m11Pin, HIGH);
      digitalWrite(m12Pin, LOW);
      analogWrite(m1Enable, motor1Speed);
    }
    if (motor1Speed < zero) {
      digitalWrite(m11Pin, LOW);
      digitalWrite(m12Pin, HIGH);
      analogWrite(m1Enable, -motor1Speed);
    }
  }
  if (motor2Speed == zero) {
    digitalWrite(m21Pin, LOW);
    digitalWrite(m22Pin, LOW);
    analogWrite(m2Enable, motor2Speed);
  } else {
    if (motor2Speed > zero) {
      digitalWrite(m21Pin, HIGH);
      digitalWrite(m22Pin, LOW);
      analogWrite(m2Enable, motor2Speed);
    }
    if (motor2Speed < zero) {
      digitalWrite(m21Pin, LOW);
      digitalWrite(m22Pin, HIGH);
      analogWrite(m2Enable, -motor2Speed);
    }
  }
}