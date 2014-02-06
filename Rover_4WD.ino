#include <Servo.h> 
#include <AFMotor.h>

#define echo A4
#define trig A5

Servo s1;

AF_DCMotor m1(1);
AF_DCMotor m2(2);
AF_DCMotor m3(3);
AF_DCMotor m4(4);

const int LEFT = 45;
const int AHEAD = 90;
const int RIGHT = 135;


void setup_serial() {
  Serial.begin(9600);  
}

void setup_hc_sr04() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}

void setup_motors() {
  s1.attach(9);
  look_ahead(); 
}

void setup() {
  setup_serial();
  setup_hc_sr04();
  setup_motors();
}

void stop() {
  m1.run(RELEASE);
  m2.run(RELEASE);
  m3.run(RELEASE);
  m4.run(RELEASE);
}

void set_speed(int speed) {
  m1.setSpeed(speed);
  m2.setSpeed(speed);
  m3.setSpeed(speed);
  m4.setSpeed(speed); 
}

void move_forward(int speed) {
  set_speed(speed);
  
  m1.run(FORWARD);
  m2.run(BACKWARD);
  m3.run(FORWARD);
  m4.run(BACKWARD);
}

void move_backward(int speed) {
  set_speed(speed);
  
  m1.run(BACKWARD);
  m2.run(FORWARD);
  m3.run(BACKWARD);
  m4.run(FORWARD);
}

void turn(int direction) {
  set_speed(255);
    
  if (direction == RIGHT) 
  {  
    m1.run(FORWARD);
    m2.run(FORWARD);
    m3.run(BACKWARD);
    m4.run(BACKWARD);
  }
  else
  {
    m1.run(BACKWARD);
    m2.run(BACKWARD);
    m3.run(FORWARD);
    m4.run(FORWARD);
  }    
  delay(250);
}

long check_distance() {
  long duration = 0;
  
  digitalWrite(trig, LOW); 
  delayMicroseconds(2); 

  digitalWrite(trig, HIGH);
  delayMicroseconds(10); 
 
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  
  return duration/58.2;
}

void look(int where) {
  s1.write(where);
}

void look_ahead() {
  s1.write(AHEAD);
}  

void look_right() {
  s1.write(RIGHT);
}

void look_left() {
  s1.write(LEFT);  
}

int fdist = 0;
int ldist = 0;
int rdist = 0;

void loop() {

  look_ahead();
  
  Serial.print("Distance from nearest object is: ");
  fdist = check_distance();
  Serial.println(fdist);

  if (fdist >= 30)
  {
    Serial.println("Looking ahead");
    look(AHEAD);
    
    Serial.println("Moving forward");
    move_forward(200);
  }
  else
  {
    Serial.println("Stopping");
    stop();
    
    delay(500);
    
    Serial.println("Looking left");
    look(LEFT);
    
    delay(250);
    
    ldist = check_distance();
    Serial.print("Left distance is: ");
    Serial.println(ldist);

    delay(250);

    Serial.println("Looking right");
    look(RIGHT);
    
    delay(250);
    
    rdist = check_distance();
    Serial.print("Right distance is: ");
    Serial.println(rdist);
    
    if (ldist > rdist) 
    {
      Serial.println("Turning left");
      turn(LEFT);
      stop();
    }
    else
    {
      Serial.println("Turning right");
      turn(RIGHT);
      stop();
    }
  }
}
