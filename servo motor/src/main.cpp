#include <AFMotor.h>
#include <Arduino.h>
#include <Servo.h>

#define echoPin A1 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin A2 //attach pin D3 Arduino to pin Trig of HC-SR04

// defines variables
long duration; // variable for the duration of sound wave travel
int distance;  // variable for the distance measurement

Servo myservo; // create servo object to control a servo
// twelve servo objects can be created on most boards
AF_DCMotor motor_back_left(1, MOTOR12_1KHZ);
AF_DCMotor motor_front_right(2, MOTOR34_1KHZ);
AF_DCMotor motor_front_left(3, MOTOR34_1KHZ);
AF_DCMotor motor_back_right(4, MOTOR12_1KHZ);

void moveforward();
void look_left();
void setup()
{

  //------------------------Settinng uo Infra Sensor----------------
  pinMode(trigPin, OUTPUT);                         // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT);                          // Sets the echoPin as an INPUT
  Serial.begin(9600);                               // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
  //------------------------END-------------------------------------

  myservo.attach(9); // attaches the servo on pin 9 to the servo object
  
  //-------------------------------- turn on motor--------------------------
  Serial.println("Motor test!");
  for (int i = 0; i < 255; i++)
  {
    motor_front_right.setSpeed(i);
    motor_front_left.setSpeed(i);
    motor_back_left.setSpeed(i);
    motor_back_right.setSpeed(i);
    delay(10);
  }
 
}

void moveServo(int &pos)
{
  //-------------------------------------------
  // Move the servo
  //-------------------------------------------
  if (pos == 180)
  {
    pos = 90;
  }
  myservo.write(pos);
  pos++;
}

/**
 * Use ultrasonic to check obstacles
 * return 0 if there is a problem
 * return 1 if there is enough way
 */
int checkObstacles()
{
   // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance is: ");
  Serial.print(distance);
  Serial.println(" cm");
  if (distance < 5)
  {
    Serial.print("it's too small!!");
    return 0;
  }
  return 1;
}
//servo moving towards left

void look_left()
{

  for (int pos = 0; pos <= 180; pos += 1)
  { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos); // tell servo to go to position in variable 'pos'
    delay(20);           // waits 15ms for the servo to reach the position
  }
  for (int pos = 180; pos >= 0; pos -= 1)
  {                     // goes from 180 degrees to 0 degrees
    myservo.write(pos); // tell servo to go to position in variable 'pos'
    delay(20);  
    //Serial.print("hi");     // waits 15ms for the servo to reach the position
  }
}

void look(bool toLeft)
{
  int start, end, increment;

  if (toLeft) 
  {
    start = 90;
    end = 180;
    increment = 1;
  } 
  else 
  {
    start = 90;
    end = 0;
    increment = -1;
  }

  for (int pos = start; (increment > 0) ? pos <= end : pos >= end; pos += increment) 
  {
    myservo.write(pos);
    delay(8);
    if (!toLeft) 
    {
      Serial.print("hi");
    }
  }
}

/**
 * Move forwards
 */
void moveforward()
{

  motor_back_left.run(FORWARD);
  motor_front_right.run(FORWARD);
  motor_front_left.run(FORWARD);
  motor_back_right.run(FORWARD);
}

/**
 * Move backwards
 */
void move_backwards()
{
  Serial.print("Move back ");

  motor_back_left.run(BACKWARD);
  motor_front_right.run(BACKWARD);
  motor_front_left.run(BACKWARD);
  motor_back_right.run(BACKWARD);


}

/**
 * Stop the motor
 */

void stop()
{
  Serial.print("Stopping the motor  ");
  motor_back_left.run(RELEASE);
  motor_front_right.run(RELEASE);
  motor_front_left.run(RELEASE);
  motor_back_right.run(RELEASE);
}

const int RUNNING = 11;
const int STOP = 12;
const int LOOK_LEFT = 13;
const int LOOK_RIGHT = 14;
const int CHECK = 15;
const int BACK = 16;

int STATE = RUNNING;

int pos = 90; // variable to store the servo position
int out = 1;  // variable to detect distance

void loop()
{

  look_left();

  switch (STATE)
  {
  case RUNNING:
    moveforward();
    STATE = CHECK;
    break;

  // case LOOK_LEFT:
  //   look(true);
  //   STATE = LOOK_RIGHT;
  //   break;

  // case LOOK_RIGHT:
  //   look(false);
  //   STATE = LOOK_LEFT;
  //   break;

  case CHECK:
    Serial.print("Check");
    out = checkObstacles();
    if (out == 0)
    {
      STATE = STOP;
      Serial.print("State set as STOP ");
    }
    if (out == 1)
    {
      STATE = RUNNING;
      Serial.print("State set as RUNNING ");
    }
    break;

  case STOP:
    Serial.print("Stop motor");
    stop();
    STATE = CHECK;

  case BACK:
    Serial.print("Back up ");
    move_backwards();
    STATE = CHECK;
    break;
  }
}
