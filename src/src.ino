#include <Servo_Manual.h>
#include <Robot_Motor.h>
#include <SR04_Ultrasonic.h>
#include <LiquidCrystal.h>

  //initalizing
  Robot_Motor *flash = new Robot_Motor (5, 6, 9, 10);
  SR04_Ultrasonic *sonic = new SR04_Ultrasonic (13, A0);
  Servo_Manual *serv = new Servo_Manual (11);
  LiquidCrystal *lcd = new LiquidCrystal (0x27, 2, 1, 0, 4, 5, 6, 7);

  //variable Colors
  const short sensL = 2;
  const short sensR = 3;


  int timetoCollide;
  int startedTimer;
  int endedTimer;

  int toDrive;

  bool left = false;
  bool right = false;

  void goToNewPlace ();
  void getColors ();
  void findLeftBack ();
  void findRight ();
  void goToNewPlace ();

  //variable colliding
  const short drehZeit = 25;
  const short longTurn = 200;
  const float collEntf = 10.0; //collisions-entfernung in cm

  int getDriveTime ();

void setup() {
  flash->setSpeed(150);
  serv ->setPosition (255, 40);
}

void loop() {
  if (!findRightBlack && !findLeftBlack) {
    spiralDrehung();
  }
  getColors();
  findRightBlack();
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


int getDriveTime () {
  return timetoCollide;
}

void startTimer () {
  startedTimer = millis();
}

void endTimer () {
  endedTimer = millis();
}

void SpiralDrehung () {
  startTimer ();
  while (sonic->measureDistance() > collEntf) {
    motor.turnLeft(5);
    motor.drive(10);
  }
  endTimer ();
  toDrive = getDriveTime() / 2;
  goToNewPlace ();
}

void goToNewPlace () {
  int start = millis ();
  startTimer ();
  while (start - millis <= toDrive) {
    motor.forward();
    if (sonic-> measureDistance > collEntf) {
      motor.stop;
      endTimer();
      break;
    }
  }
  motor.stop ();
}
