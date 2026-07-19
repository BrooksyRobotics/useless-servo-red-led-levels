#include <Servo.h>

// PINS 
const int SERVO_A_PIN = 9;   // right side — wants OFF
const int SERVO_B_PIN = 10;  // left side  — wants HIGH
const int LED_PIN     = 6;

// Angles for the regular positions used throughout code 
const int A_HOME  = 95;   // Center rest / staredown
const int A_PRESS = 150;  // Reaches button             ← calibrate
const int A_LED   = 70;   // Glaring at LED
const int A_READY = 130;  // Poised to strike

const int B_HOME  = 87;   // Center rest / staredown
const int B_PRESS = 35;   // Reaches button             ← calibrate
const int B_LED   = 115;  // Admiring LED
const int B_DROOP = 75;   // Dejected droop
const int B_READY = 60;   // Poised to press

// LED cycle  (Off=0 → Low=1 → Mid=2 → High=3 → wraps to Off)
const int LED_VALS[4] = { 0, 30, 85, 250 };

// Globals 
Servo servoA;
Servo servoB;
int   ledLevel = 0;   // 0–3 index into LED_VALS[]

// Servo movement functions for consistent style and LED control
void applyLED() {
  analogWrite(LED_PIN, LED_VALS[ledLevel]);
}

// Smooth sweep — stepDelay controls timing: 5=quick  15=normal  25=slower (any timing can be used)
void sweep(Servo &srv, int fromAngle, int toAngle, int stepDelay) {
  int step = (fromAngle < toAngle) ? 1 : -1;
  for (int pos = fromAngle; pos != toAngle + step; pos += step) {
    srv.write(pos);
    delay(stepDelay);
  }
}

// Instant snap with optional settle
void snap(Servo &srv, int angle, int settleMs = 150) {
  srv.write(angle);
  delay(settleMs);
}

// Emotion wiggle at a position  
void wiggle(Servo &srv, int center, int amount, int reps, int speed = 70) {
  for (int i = 0; i < reps; i++) {
    srv.write(center + amount);
    delay(speed);
    srv.write(center - amount);
    delay(speed);
  }
  srv.write(center);
  delay(80);
}

// Physical button press + advance LED one step
void pressButton(Servo &srv, int pressAngle, int returnAngle,
                 int holdMs = 100, int returnMs = 220) {
  srv.write(pressAngle);
  delay(holdMs);
  ledLevel = (ledLevel + 1) % 4;   // Off→Low→Mid→High→Off→…
  applyLED();
  srv.write(returnAngle);
  delay(returnMs);
}

// Animation Scenes for servo movements and LED control
void scene_1() {
  pressButton(servoA, A_PRESS, A_LED, 100, 150);  // OFF
  wiggle(servoA, A_LED, 5, 3);
  delay(300);
  sweep(servoB, B_HOME, B_READY, 5);  

  pressButton(servoB, B_PRESS, B_READY, 100, 120);   
  pressButton(servoB, B_PRESS, B_READY, 100, 120);   
  snap(servoA, 30);
  pressButton(servoB, B_PRESS, B_READY, 100, 120);   

  sweep(servoB, B_READY, B_LED, 5);
  delay(200);
  wiggle(servoB, B_LED, 7, 2, 52);
  sweep(servoB, B_LED, B_HOME, 5);
  delay(200);
  snap(servoA, A_HOME, 50);
  wiggle(servoA, A_HOME, 3, 5);
  delay(100);
  pressButton(servoA, A_PRESS, A_HOME, 100, 200);  
  wiggle(servoA, A_HOME, 20, 1, 100);
}

void scene_2() {
  wiggle(servoB, B_LED, 3, 3);
  sweep(servoB, B_LED, B_READY, 10);
  delay(180);

  pressButton(servoB, B_PRESS, B_READY);     
  pressButton(servoB, B_PRESS, B_READY);     
  pressButton(servoA, A_PRESS, A_HOME);      
  pressButton(servoB, B_PRESS, B_LED);       
  sweep(servoB, B_LED, B_HOME, 5);
  wiggle(servoB, B_HOME, 5, 3);
  wiggle(servoB, B_HOME, 20, 2);
  delay(300);

  pressButton(servoB, B_PRESS, B_READY);    
  pressButton(servoB, B_PRESS, B_READY);    
  delay(50);
  wiggle(servoB, B_LED, 5, 3);
  delay(100);
  snap(servoB, B_HOME);
  delay(200);
  sweep(servoA, A_HOME, A_READY, 10);
  pressButton(servoB, B_PRESS, B_LED);    
  delay(200);
  wiggle(servoB, B_LED, 5, 4);
  pressButton(servoA, A_PRESS, A_HOME);     
  delay(200);
  snap(servoB, B_HOME); 
  delay(100); 
  wiggle(servoB, B_HOME, 5, 3);
  delay(200);
  snap(servoB, B_LED);
  delay(100);
  wiggle(servoB, B_HOME, 3, 5);
}

void scene_3() {
  pressButton(servoB, B_PRESS, B_READY);    
  snap(servoA, A_LED);
  delay(100);
  pressButton(servoB, B_PRESS, B_READY);    
  wiggle(servoA, A_HOME, 4, 3);
  delay(400);
  pressButton(servoA, A_PRESS, A_READY);    
  pressButton(servoB, B_PRESS, B_READY);    
  sweep(servoA, A_READY, A_HOME, 30);
  wiggle(servoA, A_HOME, 20, 3, 120); 
  delay(300);
  pressButton(servoB, B_PRESS, B_READY);    
  pressButton(servoB, B_PRESS, B_READY);    
  delay(50);

  pressButton(servoB, B_PRESS, B_READY);    
  snap(servoA, A_READY);  
  pressButton(servoB, B_PRESS, B_READY);    
  snap(servoB, B_LED);
  snap(servoB, B_HOME);
  delay(200);
  wiggle(servoB, B_HOME, 5, 3);
  delay(200);
  sweep(servoA, A_HOME, A_LED, 15);
  wiggle(servoB, B_LED, 30, 3, 100);
  wiggle(servoA, A_LED, 5, 3);
  delay(300);
  sweep(servoB, B_LED, B_HOME, 40);   
  delay(300);
  wiggle(servoB, B_HOME, 3, 4);
  wiggle(servoB, B_LED, 5, 2);
}

void scene_4() {
  for (int i = 0; i <= 5; i++){
  pressButton(servoB, B_PRESS, B_READY, 80, 200 - (i*20)); 
  pressButton(servoB, B_PRESS, B_READY, 80, 200 - (i*20));   
  pressButton(servoB, B_PRESS, B_READY, 80, 200 - (i*20));      
  pressButton(servoA, A_PRESS, A_READY, 80, 200 - (i*20));
  }
  servoA.write(A_HOME);
  for (int i = 0; i <= 10; i++){
  pressButton(servoB, B_PRESS, B_READY, 80, 100 - (i*5));    
  }
  servoA.write(A_LED);
  for (int i = 0; i <= 11; i++){
  pressButton(servoB, B_PRESS, B_READY, 80, 50 + (i*20));    
  }  
  sweep(servoB, B_READY, B_LED, 30);
  delay(300);
  sweep(servoA, A_LED, A_HOME, 3);
  sweep(servoB, B_LED, B_HOME, 20);
}

void setup() {
  servoA.attach(SERVO_A_PIN);
  servoB.attach(SERVO_B_PIN);
  pinMode(LED_PIN, OUTPUT);

  servoA.write(A_HOME);
  servoB.write(B_HOME);
  ledLevel = 3;
  applyLED();
  delay(1500);
  
  scene_1();
  scene_2();
  scene_3();
  scene_4();
}

void loop() {

}
