#include <Servo.h>
#include <LiquidCrystal.h>

int echoPin = 6;
int trigPin = 7;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 10, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Servo motorX, motorY;


void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.print("Distance: ");
  lcd.setCursor(0, 1);
  lcd.print("Angle: ");

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 

  motorX.attach(2);
  motorX.write(0);

  Serial.println("Startup");

}

float distanceOut;
String dist;
String angle;
long duration;

String inString;

bool simple, vertical, complete;

void loop() {
  if (Serial.available() > 0) {
    inString = Serial.readString();
  }

  if (inString == "Simple Sweep\n") {
    simple = true;
  }else if (inString == "testing connection\n") {
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

      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("Distance: " + dist);

      lcd.setCursor(0, 1);
      lcd.print("Angle: " + angle);

      Serial.println("D" + dist + "A" + angle+"\n");
      delay(40);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("N/A");

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
