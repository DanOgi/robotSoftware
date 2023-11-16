#include <SoftwareSerial.h>

#define RX 2
#define TX 3

#define IN1 8
#define IN2 7
#define IN3 5
#define IN4 4

#define ENA 9
#define ENB 6

#define TRIG_PIN_BACK 11
#define ECHO_PIN_BACK 10

#define TRIG_PIN_FRONT 12
#define ECHO_PIN_FRONT 13

SoftwareSerial mySerial(RX, TX);
String str;

const int voltageSensorPin = A0;
const float factor = 5.128;
const float vCC = 4.00;
float voltageSensorVal;
float vOut;
float vIn;

String direction;
String speedStr;
int speed;

int distance_back;
int distance_front;

void setup()

{

  Serial.begin(9600);
  mySerial.begin(9600);
  mySerial.setTimeout(25);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  pinMode(TRIG_PIN_BACK, OUTPUT);
  pinMode(ECHO_PIN_BACK, INPUT);

  pinMode(TRIG_PIN_FRONT, OUTPUT);
  pinMode(ECHO_PIN_FRONT, INPUT);

  direction = "SS";
  speedStr = "0";
  speed = speedStr.toInt();
}


void loop()

{

  voltageSensorVal = analogRead(voltageSensorPin);
  vOut = (voltageSensorVal / 1024) * vCC;
  vIn = vOut * factor;

  if (mySerial.available()) {
    str = mySerial.readString();
    Serial.print(str);
    mySerial.println(vIn);

    direction = str.substring(0, 2);
    speedStr = str.substring(2);
    speed = speedStr.toInt();
  }

  distance_back = ultrasonic(TRIG_PIN_BACK, ECHO_PIN_BACK);
  distance_front = ultrasonic(TRIG_PIN_FRONT, ECHO_PIN_FRONT);

  //Serial.println(distance_front);

  if (direction == "SS") {
    //Serial.println("STOP");
    stop();
  } else if (direction == "FF" && distance_front > 10) {
    //Serial.println("FORWARD");
    forward(speed);
  } else if (direction == "BB" && distance_back > 10) {
    //Serial.println("BACKWARD");
    backward(speed);
  } else if (direction == "LL") {
    //Serial.println("LEFT");
    left(speed);
  } else if (direction == "RR") {
    //Serial.println("RIGHT");
    right(speed);
  } else if (direction == "FL" && distance_front > 10) {
    //Serial.println("LEFT-FORWARD");
    left_forward(speed);
  } else if (direction == "FR" && distance_front > 10) {
    //Serial.println("RIGHT-FORWARD");
    right_forward(speed);
  } else if (direction == "BL" && distance_back > 10) {
    //Serial.println("LEFT-BACKWARD");
    left_backward(speed);
  } else if (direction == "BR" && distance_back > 10) {
    //Serial.println("RIGHT-BACKWARD");
    right_backward(speed);
  } else {
    //Serial.println("STOP");
    stop();
  }
}

void stop() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void forward(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void left(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void right(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void left_forward(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed / 2);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void right_forward(int speed) {
  analogWrite(ENA, speed / 2);
  analogWrite(ENB, speed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void left_backward(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed / 2);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void right_backward(int speed) {
  analogWrite(ENA, speed / 2);
  analogWrite(ENB, speed);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

int ultrasonic(int trig_pin, int echo_pin) {
  long duration;
  int distance;

  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);

  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trig_pin, LOW);

  duration = pulseIn(echo_pin, HIGH);
  distance = duration * 0.034 / 2;

  return distance;
}