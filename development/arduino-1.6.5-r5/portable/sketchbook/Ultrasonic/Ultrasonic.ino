#include <Arduino.h>

#define ECHO_PIN 34
#define TRIG_PIN 25

void setup()
{
  Serial.begin(115200);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  uint32_t duration = pulseIn(ECHO_PIN, HIGH);
  
  float distance = duration * 0.034f / 2.f;
  
  Serial.print("Distance: ");
  Serial.println(distance);

  delay(1000);
}
