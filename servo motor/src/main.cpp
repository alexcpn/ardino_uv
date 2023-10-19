#include <AFMotor.h>
#include <Arduino.h>
#include <Servo.h>

#define echoPin A1 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin A2 // attach pin D3 Arduino to pin Trig of HC-SR04

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
  for (int i = 0; i < 50; i++)
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
  //Serial.print("Distance is: ");
  //Serial.print(distance);
  //Serial.println(" cm");

  delay(2);

  return distance;
}
// servo moving towards left

// void look_left()
// {

//   for (int pos = 0; pos <= 180; pos += 1)
//   { // goes from 0 degrees to 180 degrees
//     // in steps of 1 degree
//     myservo.write(pos); // tell servo to go to position in variable 'pos'
//     delay(20);           // waits 15ms for the servo to reach the position
//   }
//   for (int pos = 180; pos >= 0; pos -= 1)
//   {                     // goes from 180 degrees to 0 degrees
//     myservo.write(pos); // tell servo to go to position in variable 'pos'
//     delay(20);
//     //Serial.print("hi");     // waits 15ms for the servo to reach the position
//   }
// }

int changehead(int *prev_incr)
{
    int current_pos, increment;
    
    // Get the current position of the servo
    current_pos = myservo.read();

    if (current_pos >= 180 && *prev_incr == 1)
    {
        increment = -1;
    }
    else if (current_pos <= 0 && *prev_incr == -1)
    {
        increment = 1;
    }
    else
    {
        increment = *prev_incr;
    }

    // Update the value the pointer is pointing to
    *prev_incr = increment;

    myservo.write(current_pos + increment);
    delay(8);

    return current_pos;
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
  delay(10);
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
  delay(10);
}
/**
 * Move left by turning right side motors forward and left side motors backward
 */
void move_left()
{
  Serial.print("Move left ");

  motor_back_left.run(BACKWARD);
  motor_front_left.run(BACKWARD);
  motor_back_right.run(FORWARD);
  motor_front_right.run(FORWARD);
  delay(10);
}

/**
 * Move right by turning left side motors forward and right side motors backward
 */
void move_right()
{
  Serial.print("Move right ");

  motor_back_left.run(FORWARD);
  motor_front_left.run(FORWARD);
  motor_back_right.run(BACKWARD);
  motor_front_right.run(BACKWARD);
  delay(10);
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
  delay(10);
}

const int GO_FORWARD = 11;
const int STOP = 12;
const int GO_LEFT = 13;
const int GO_RIGHT = 14;
const int CHECK = 15;
const int GO_BACK = 16;

int STATE = GO_FORWARD;

int out = 1;  // variable to detect distance
int increment = 1;
bool stopped =false;
int pos = 0;

void loop()
{

  switch (STATE)
  {

 
  case GO_FORWARD:
    Serial.print("GO_FORWARD ");
    moveforward();
    STATE = CHECK;
    break;

  case GO_LEFT:
    Serial.print("GO_LEFT ");
    move_left();
    STATE = CHECK;
    break;

  case GO_RIGHT:
    Serial.print("GO_RIGHT ");
    move_right();
    STATE = CHECK;
    break;

  case GO_BACK:
    Serial.print("GO_BACK ");
    move_backwards();
    STATE = CHECK;
    break;

  case CHECK:
    
    pos = changehead(&increment);
    
    int distance = checkObstacles();
    //Serial.print(distance);
    //Serial.println(" centimeters");

    if (distance <= 5)
    {
      stopped = true;
      stop();
      STATE = CHECK;
      Serial.print("State set as STOP ");
      Serial.print(pos);
      Serial.println(" degress");

    }
   if (distance > 5 && stopped)
    {
        stopped = false;
        Serial.print(pos);
        Serial.println(" degrees");
        Serial.print("State set as GO_FORWARD ");

        if (pos >= 0 && pos <= 45)
        {
            STATE = GO_RIGHT;
            Serial.print("State set as GO_RIGHT ");
        }
        else if (pos >= 45 && pos <= 135)
        {
            STATE = GO_FORWARD;
            Serial.print("State set as GO_FORWARD ");
        }
        else if (pos >= 135 && pos <= 185)
        {
            STATE = GO_LEFT;
            Serial.print("State set as GO_LEFT ");
        }
    }
    delay(10);
    break;
  }
}
