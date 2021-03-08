#include <Servo.h>
#include <LiquidCrystal.h>


int bLeft = A0;
int bRight = A5;

//Ultrasonic sensor
int echoPin = 6;
int trigPin = 7;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 10, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Servo motorX, motorY;

int green = 3;
int yellow = 9;

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.print("Distance: ");
  lcd.setCursor(0, 1);
  lcd.print("Angle: ");

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT

  motorX.attach(2);
  motorX.write(0);

  Serial.println("Startup");

  pinMode(green, INPUT);
  pinMode(yellow, INPUT);

  motorY.attach(13);

}

float distanceOut;
String dist;
String angle;
long duration;

String inString;

bool simple, vertical, complete;

bool greenIn = false, yellowIn = false;

bool pastGreen = false, pastYellow = false;

int vertDelay = 100;
int startTime, currTime;

void loop() {
  greenIn = digitalRead(green);
  yellowIn = digitalRead(yellow);

  if (greenIn == true && simple == false) {
    simple = true;
  }
  if (yellowIn == true && vertical == false) {
    vertical = true;
  }

  if (Serial.available() > 0) {
    inString = Serial.readString();
  }

  if (inString == "Simple Sweep\n") {
    simple = true;
  } else if (inString == "testing connection\n") {
    Serial.println("Recieved\n");
  }
  if (simple == true) {
    simple = false;
    inString = "";
    for (int i = 0; i <= 360; i++) {
      int x = i;
      if (i > 180) {
        x = 360 - i;
      }
      motorX.write(x);
      dist = String(int(distance()));
      angle = String(x);

      if (dist.toInt() < 100) {
        if (angle.toInt() < 75) {
          analogWrite(bRight, 200);
          analogWrite(bLeft, LOW);
        } else if (angle.toInt() > 115) {
          analogWrite(bLeft, 200);
          analogWrite(bRight, LOW);
        } else {
          analogWrite(bRight, 200);
          analogWrite(bLeft, 200);
        }
      } else {
        analogWrite(bRight, LOW);
        analogWrite(bLeft, LOW);
      }

      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("Distance: " + dist);

      lcd.setCursor(0, 1);
      lcd.print("Angle: " + angle);

      Serial.println("D" + dist + "A" + angle + "\n");
      delay(40);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sweep Done");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Distance: ");
    lcd.setCursor(0, 1);
    lcd.print("Angle: ");

  }
  analogWrite(bRight, LOW);
  analogWrite(bLeft, LOW);

  if (vertical == true) {
    motorY.write(95);
    startTime = millis();
    while (currTime < vertDelay) {
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("Distance: " + dist);
      Serial.println("D" + dist + "A" + angle + "\n");
      currTime = millis - startTime;

    }
    motorY.write(90);
  }

}



double distance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceOut = duration * 0.034 / 2;
  return distanceOut;
}
