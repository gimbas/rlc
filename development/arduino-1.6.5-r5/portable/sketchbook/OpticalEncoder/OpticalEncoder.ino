#include <Arduino.h>

#define ENCODER_LEFT_PIN 26
#define MOTOR_LEFT_FWD_PIN 32
#define MOTOR_LEFT_BWD_PIN 33
#define ENCODER_RIGHT_PIN  27
#define MOTOR_RIGHT_FWD_PIN  23
#define MOTOR_RIGHT_BWD_PIN  21

#define MOTOR_RIGHT_FWD_CHAN  0
#define MOTOR_RIGHT_BWD_CHAN  1
#define MOTOR_LEFT_FWD_CHAN   2
#define MOTOR_LEFT_BWD_CHAN   3

#define DELTA_TIME          100

#define ENCODER_STEPS       40
#define TRACK_WIDTH         135.f
#define TRACK_PERIMETER     (TRACK_WIDTH * 3.14159f)
#define WHEEL_DIAMETER      67.5f
#define WHEEL_PERIMETER     (WHEEL_DIAMETER * 3.14159f)
#define WHEEL_STEP          (WHEEL_PERIMETER / ENCODER_STEPS)

enum MotorSide
{
  MOTOR_LEFT,
  MOTOR_RIGHT
};

int8_t encoder_right_dir = 0;
int8_t encoder_left_dir = 0;
uint32_t encoder_right_count = 0;
uint32_t encoder_left_count = 0;

void IRAM_ATTR encoder_right_isr(void *pArg)
{
  if(encoder_right_dir == -1 && !encoder_right_count)
    return;

  encoder_right_count += encoder_right_dir;
}
void IRAM_ATTR encoder_left_isr(void *pArg)
{
  if(encoder_left_dir == -1 && !encoder_left_count)
    return;

  encoder_left_count += encoder_left_dir;
}

void setMotorSpeed(MotorSide motor, int32_t duty)
{
  if(motor == MOTOR_RIGHT)
  {
    if(duty < 0)
    {
      ledcWrite(MOTOR_RIGHT_FWD_CHAN, 0);
      ledcWrite(MOTOR_RIGHT_BWD_CHAN, -duty);

      encoder_right_dir = -1;
    }
    else if(duty > 0)
    {
      ledcWrite(MOTOR_RIGHT_BWD_CHAN, 0);
      ledcWrite(MOTOR_RIGHT_FWD_CHAN, duty);

      encoder_right_dir = 1;
    }
    else
    {
      ledcWrite(MOTOR_RIGHT_BWD_CHAN, 0);
      ledcWrite(MOTOR_RIGHT_FWD_CHAN, 0);

      encoder_right_dir = 0;
    }
  }
  else if(motor == MOTOR_LEFT)
  {
    if(duty < 0)
    {
      ledcWrite(MOTOR_LEFT_FWD_CHAN, 0);
      ledcWrite(MOTOR_LEFT_BWD_CHAN, -duty);

      encoder_left_dir = -1;
    }
    else if(duty > 0)
    {
      ledcWrite(MOTOR_LEFT_BWD_CHAN, 0);
      ledcWrite(MOTOR_LEFT_FWD_CHAN, duty);

      encoder_left_dir = 1;
    }
    else
    {
      ledcWrite(MOTOR_LEFT_FWD_CHAN, 0);
      ledcWrite(MOTOR_LEFT_BWD_CHAN, 0);

      encoder_left_dir = 0;
    }
  }
}
void turn(float ang, uint32_t duty)
{
  if(!ang)
    return;

  // Normalize angle
  while(ang > PI)
    ang -= 2.f * PI;

  while(ang < -PI)
    ang += 2.f * PI;

  float arc_length = TRACK_PERIMETER * ((ang > 0 ? ang : -ang) / (2.f * PI));

  uint32_t num_steps = arc_length / WHEEL_STEP;

  uint8_t right_duty;
  uint8_t left_duty;

  if(ang > 0)
  {
    right_duty = duty;
    left_duty = -duty;
  }
  else
  {
    right_duty = -duty;
    left_duty = duty;
  }

  uint8_t done = 0;

  setMotorSpeed(MOTOR_LEFT, 0);
  setMotorSpeed(MOTOR_RIGHT, 0);

  encoder_right_count = 0;
  encoder_left_count = 0;

  while(done != 0x3)
  {
    if (encoder_right_count < num_steps && !(done & 0x1))
    {
      setMotorSpeed(MOTOR_RIGHT, right_duty);
    }
    else
    {
      done |= 0x1;
      setMotorSpeed(MOTOR_RIGHT, 0);
    }

    if (encoder_left_count < num_steps && !(done & 0x2))
    {
      setMotorSpeed(MOTOR_LEFT, left_duty);
    }
    else
    {
      done |= 0x2;
      setMotorSpeed(MOTOR_LEFT, 0);
    }
  }

  setMotorSpeed(MOTOR_LEFT, 0);
  setMotorSpeed(MOTOR_RIGHT, 0);

  encoder_right_count = 0;
  encoder_left_count = 0;
}

void setup()
{
  Serial.begin(115200);

  pinMode(MOTOR_RIGHT_FWD_PIN, OUTPUT);
  pinMode(MOTOR_RIGHT_BWD_PIN, OUTPUT);
  pinMode(MOTOR_LEFT_FWD_PIN, OUTPUT);
  pinMode(MOTOR_LEFT_BWD_PIN, OUTPUT);

  digitalWrite(MOTOR_RIGHT_FWD_PIN, LOW);
  digitalWrite(MOTOR_RIGHT_BWD_PIN, LOW);
  digitalWrite(MOTOR_LEFT_FWD_PIN, LOW);
  digitalWrite(MOTOR_LEFT_BWD_PIN, LOW);

  ledcSetup(MOTOR_RIGHT_FWD_CHAN, 50, 16);
  ledcSetup(MOTOR_RIGHT_BWD_CHAN, 50, 16);
  ledcSetup(MOTOR_LEFT_FWD_CHAN, 50, 16);
  ledcSetup(MOTOR_LEFT_BWD_CHAN, 50, 16);

  ledcAttachPin(MOTOR_RIGHT_FWD_PIN, MOTOR_RIGHT_FWD_CHAN);
  ledcAttachPin(MOTOR_RIGHT_BWD_PIN, MOTOR_RIGHT_BWD_CHAN);
  ledcAttachPin(MOTOR_LEFT_FWD_PIN, MOTOR_LEFT_FWD_CHAN);
  ledcAttachPin(MOTOR_LEFT_BWD_PIN, MOTOR_LEFT_BWD_CHAN);

  ledcWrite(MOTOR_RIGHT_FWD_CHAN, 0);
  ledcWrite(MOTOR_RIGHT_BWD_CHAN, 0);
  ledcWrite(MOTOR_LEFT_FWD_CHAN, 0);
  ledcWrite(MOTOR_LEFT_BWD_CHAN, 0);

  pinMode(ENCODER_RIGHT_PIN, INPUT_PULLUP);
  pinMode(ENCODER_LEFT_PIN, INPUT_PULLUP);

  attachInterruptArg(ENCODER_RIGHT_PIN, encoder_right_isr, NULL, CHANGE);
  attachInterruptArg(ENCODER_LEFT_PIN, encoder_left_isr, NULL, CHANGE);

  //delay(1000);
  //turn(90);
  //delay(1000);
  //turn(-90);
  //delay(1000);
  //turn(180);
  //delay(1000);
  //turn(-180);
  //delay(1000);
  //turn(360);
}
void loop()
{
  static int32_t right_duty = 65535;
  static int32_t left_duty = 65535;

  static float target_speed = 300.f; // mm/s

  uint32_t now = millis();
  static uint32_t prev_calc = 0;

  float time_delta = now - prev_calc;

  if(time_delta > DELTA_TIME)
  {
    prev_calc = now;

    static uint32_t prev_encoder_right_count = 0;
    static uint32_t prev_encoder_left_count = 0;

    uint32_t encoder_right_delta = encoder_right_count - prev_encoder_right_count;
    uint32_t encoder_left_delta = encoder_left_count - prev_encoder_left_count;

    prev_encoder_right_count = encoder_right_count;
    prev_encoder_left_count = encoder_left_count;

    // Distance travelled and speed calc
    float right_distance = encoder_right_delta * WHEEL_STEP;
    float left_distance = encoder_left_delta * WHEEL_STEP;

    float right_speed = right_distance / (time_delta / 1000);
    float left_speed = left_distance / (time_delta / 1000);

    // Odometry
    static float pos_x = 0;
    static float pos_y = 0;
    static float theta_z = 0;

    float center_distance = (right_distance + left_distance) / 2; // Error is small if delta time is small

    theta_z += (right_distance - left_distance) / TRACK_WIDTH;
    pos_x += center_distance * cos(theta_z);
    pos_y += center_distance * sin(theta_z);

    // Normalize theta
    while(theta_z > PI)
      theta_z -= 2.f * PI;

    while(theta_z < -PI)
      theta_z += 2.f * PI;

    Serial.print("X: ");
    Serial.println(pos_x);
    Serial.print("Y: ");
    Serial.println(pos_y);
    Serial.print("Theta Z: ");
    Serial.println(theta_z * (180.f / PI));

    Serial.print("Left speed: ");
    Serial.println(left_speed);
    Serial.print("Right speed: ");
    Serial.println(right_speed);

    Serial.print("Left pulse count: ");
    Serial.println(encoder_left_count);
    Serial.print("Right pulse count: ");
    Serial.println(encoder_right_count);

    Serial.print("Left duty: ");
    Serial.println(left_duty);
    Serial.print("Right duty: ");
    Serial.println(right_duty);
  }

  setMotorSpeed(MOTOR_LEFT, left_duty);
  setMotorSpeed(MOTOR_RIGHT, right_duty);
}
