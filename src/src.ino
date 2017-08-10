#include <Servo_Manual.h>
#include <IR_Keys.h>
#include <IRremote.h>
#include <Robot_Motor.h>
#include <SR04_Ultrasonic.h>

  //inital
  Robot_Motor *flash = new Robot_Motor (5, 6, 9, 10);
  SR04_Ultrasonic *sonic = new SR04_Ultrasonic (13, A0);
  Servo_Manual *serv = new Servo_Manual (11);

  //variabel Colors
  const short sensL = 2;
  const short sensR = 3;

  bool left = false;
  bool right = false;

  void getColors ();
  void findLeftBack ();
  void findRight ();
  
  //variabel colliding
  const short drehZeit = 25;
  const short longTurn = 200;
  const float collEntf = 20.0; //collisions-entfernung in cm

  bool isColliding ();
  
  
void setup() {
  flash->setSpeed(150);

}

void loop() {

  //Colors

}

//functions
void getColors () {
  if (digitalRead (sensL) == LOW) {
    left = true;
  } else {
    left = false;
  }

  if (digitalRead (sensR) == LOW) {
    right = true;
  } else {
    right = false;
  }
}

void findLeftBlack () {
  while (right == false) {
    flash->turnRight (10);
    getColors ();
  }
  flash->drive (100);
}

void findRightBlack () {
  while (left == false) {
    flash->turnLeft (10);
    getColors ();
  }
  flash->drive (100);
}

// colliding
bool isColliding () {
   if (sens-> measureDistance () < collEntf) {
      writeCollisionMessage();
      return true;
   }
   else {
      lcd -> clear();
      return false;
   }
}







