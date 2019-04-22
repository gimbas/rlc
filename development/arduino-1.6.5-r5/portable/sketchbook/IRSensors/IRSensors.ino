#include <Arduino.h>

#define LIMSWOTCH_PIN   12
#define FRONT_PIN       2
#define MIDDLE_PIN      18
#define OUTER_LEFT_PIN  17
#define INNER_LEFT_PIN  5
#define OUTER_RIGHT_PIN 22
#define INNER_RIGHT_PIN 19

void setup()
{
  Serial.begin(115200);
  
  pinMode(LIMSWOTCH_PIN, INPUT);
  pinMode(FRONT_PIN, INPUT);
  pinMode(MIDDLE_PIN, INPUT);
  pinMode(OUTER_LEFT_PIN, INPUT);
  pinMode(INNER_LEFT_PIN, INPUT);
  pinMode(OUTER_RIGHT_PIN, INPUT);
  pinMode(INNER_RIGHT_PIN, INPUT);
}

void loop()
{
  bool road[5] = { 0 };
  bool near = 0;
  bool collision = 0;

  road[0] = !digitalRead(MIDDLE_PIN);
  road[1] = !digitalRead(OUTER_LEFT_PIN);
  road[2] = !digitalRead(INNER_LEFT_PIN);
  road[3] = !digitalRead(OUTER_RIGHT_PIN);
  road[4] = !digitalRead(INNER_RIGHT_PIN);

  near = !digitalRead(FRONT_PIN);

  collision = digitalRead(LIMSWOTCH_PIN);
  
  Serial.print("Road: ");
  Serial.print(road[0]);
  Serial.print(", ");
  Serial.print(road[1]);
  Serial.print(", ");
  Serial.print(road[2]);
  Serial.print(", ");
  Serial.print(road[3]);
  Serial.print(", ");
  Serial.println(road[4]);
  
  Serial.print("Near: ");
  Serial.println(near);
  
  Serial.print("Collision: ");
  Serial.println(collision);

  delay(1000);
}
