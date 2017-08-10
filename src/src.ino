#include <Servo_Manual.h>
#include <IR_Keys.h>
#include <IRremote.h>
#include <Robot_Motor.h>
#include <SR04_Ultrasonic.h>
#include <LiquidCrystal.h>

  //hallo

  //initalizing
  Robot_Motor *flash = new Robot_Motor (5, 6, 9, 10);
  SR04_Ultrasonic *sonic = new SR04_Ultrasonic (13, A0);
  Servo_Manual *serv = new Servo_Manual (11);
  LiquidCrystal *lcd = new LiquidCrystal (0x27, 2, 1, 0, 4, 5, 6, 7);

  //variable Colors
  const short sensL = 2;
  const short sensR = 3;


  int timetoCollide;

  bool left = false;
  bool right = false;

  void getColors ();
  void findLeftBack ();
  void findRight ();

  //variable colliding
  const short drehZeit = 25;
  const short longTurn = 200;
  const float collEntf = 10.0; //collisions-entfernung in cm

  bool isColliding ();
  bool evalDist ();

  void findBestDirection ();
  void checkServo ();
  void leaveAngel ();
  void voidcollision ();


void setup() {
  flash->setSpeed(150);
}

void loop() {
  voidcollision ();
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

  if (left == right == true) {
    exit (0);
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
      return false;
   }
}

bool evalDist (int a, int b) {
  if (a > b) {
    return false;
  } if (a < b) {
    return true;
  }
  if (a == b) {
    return true;
  }
}

void findBestDirection () {
  int leftDist;
  int rightDist;

  serv->setPosition (255, 10);
  leftDist = sens-> measureDistance();
  serv->setPosition (0, 10);
  rightDist = sens-> measureDistance();
  bool rightTurn = evalDist (leftDist, rightDist);

  if (rightTurn && rightDist < collEntf) {
    motor.turnRight (600);
    delay (600);
  } else if (!(rightTurn) && leftDist < collEntf) {
    motor.turnLeft (600);
    delay(600);
  } else {
    leaveAngle();
  }
}

void checkServo () {
   serv->setPosition (30, 15);
   if (sens->measureDistance() > collEntf){
         motor.forward ();
      }
      else {
        motor.stop();
        findBestDirection();
      }
   serv->setPosition (90, 15);
   if (!(isColliding())){
         motor.forward ();
      }
      else {
        motor.stop();
        findBestDirection();
      }
  serv->setPosition (150, 15);
   if (sens->measureDistance() > collEntf){
         motor.forward ();
      }
      else {
        motor.stop();
        findBestDirection();
      }

   serv->setPosition (90, 15);
   if (!(isColliding())){
         motor.forward ();
      }
      else {
        motor.stop();
        findBestDirection();
      }
}

void leaveAngle() {
  while (sens->measureDistance() < collEntf) {
    motor.turnRight (200);
    delay (200);
  }
  motor.turnRight (400);
}

void voidcollision () {
  checkServo ();
}
