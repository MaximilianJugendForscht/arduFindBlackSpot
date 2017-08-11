#include <Servo_Manual.h>
#include <Robot_Motor.h>
#include <SR04_Ultrasonic.h>
#include <LiquidCrystal_I2C.h>
  //initalizing
  Robot_Motor *flash = new Robot_Motor (5, 6, 9, 10);
  SR04_Ultrasonic *sonic = new SR04_Ultrasonic (13, A0);
  Servo_Manual *serv = new Servo_Manual (11);
  LiquidCrystal_I2C *lcd = new LiquidCrystal_I2C (0x27, 2, 1, 0, 4, 5, 6, 7);
  //variable Colors
  const short sensL = 2;
  const short sensR = 3;


  int timetoCollide;
  int startedTimer;
  int endedTimer;

  const int turnTime = 300;

  int toDrive;

  bool left = false;
  bool right = false;

  void goToNewPlace ();
  void getColors ();
  void findLeftBlack ();
  void findRightBlack ();
  void goToNewPlace ();
  void spiralDrehung();
  void findBestDirection();
  bool isColliding();

  //variable colliding
  const short drehZeit = 25;
  const short longTurn = 200;
  const float collEntf = 20.0; //collisions-entfernung in cm

  int getDriveTime ();

void setup() {
  flash->setSpeed(215);
  serv ->setPosition (255, 40);
  lcd -> setBacklightPin (3, POSITIVE);
  lcd -> setBacklight (HIGH);
  lcd -> begin(16, 2);
}

void loop() {
  if (!(left) && !(right)) {
    serv -> setPosition (255, 40);
    spiralDrehung();
  }
  lcd -> setCursor (10, 0);
  lcd -> print ("HALLO");
  /*getColors();
  findRightBlack();
  findLeftBlack ();*/
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
    flash->turnLeft (10);
    getColors ();
  }
  flash->drive (100);
}

void findRightBlack () {
  while (left == false) {
    flash->turnRight (10);
    getColors ();
  }
  flash->drive (100);
}


int getDriveTime () {
  return endedTimer - startedTimer ;
}

void startTimer () {
  startedTimer = millis();
}

void endTimer () {
  endedTimer = millis();
}

void spiralDrehung () {
  serv-> setPosition (50, 50);
  int counter = 40;
  int whileCounter = 0;
  bool lookLeft = false;
  while (!(isColliding())) {
    flash->turnLeft(50);
    flash->drive (counter);
    counter += 1;
    if (lookLeft = false) {
      serv -> setPosition (190, 10);
    } else  {
      serv -> setPosition (30, 10);
    }
  }
  flash -> turnLeft (turnTime);
  goToNewPlace ();
}

void goToNewPlace () {
  serv -> setPosition (90, 10);
  startTimer ();
  while (!(isColliding())) {
    checkServo();
  }
  flash->stop();
  flash->turnRight(turnTime);
  endTimer ();
  toDrive = getDriveTime();
  toDrive = toDrive / 2;
  
  int start = millis ();
  lcd->clear();
  lcd -> home();
  lcd->print (start);
  lcd -> setCursor (1, 2);
  lcd->print (toDrive);
  startTimer ();
  while (millis () -start <= toDrive) {
    lcd -> setCursor(10, 1);
    lcd -> print ("DEB");
    checkServo();
    if (isColliding()) {
      endTimer ();
      toDrive = getDriveTime ();
      toDrive = toDrive * (2/3);
      startTimer ();
    }
  }
  flash->stop ();
}

bool isColliding () { 
   if (sonic-> measureDistance () < collEntf) { 
      return true; 
   } 
   else { 
      return false; 
   } 
} 

void checkServo () { 
   serv->setPosition (0, 20); 
   if (sonic->measureDistance() > collEntf){ 
         flash ->forward (); 
      } 
      else { 
        flash->stop(); 
        findBestDirection();
        toDrive = toDrive * (2/3); 
      } 
   serv->setPosition (90, 15); 
   if (!(isColliding())){ 
         flash->forward (); 
      } 
      else { 
        flash->stop(); 
        findBestDirection();
        toDrive = toDrive * (2/3); 
      } 
  serv->setPosition (255, 20); 
   if (sonic->measureDistance() > collEntf){ 
         flash->forward (); 
      } 
      else { 
        flash->stop(); 
        findBestDirection();
        toDrive = toDrive * (2/3); 
      } 
 
   serv->setPosition (90, 15); 
   if (!(isColliding())){ 
         flash->forward (); 
      } 
      else { 
        flash->stop(); 
        findBestDirection();
        toDrive = toDrive * (2/3); 
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
  leftDist = sonic-> measureDistance(); 
  serv->setPosition (0, 10); 
  rightDist = sonic-> measureDistance(); 
  bool rightTurn = evalDist (leftDist, rightDist); 
 
  if (rightTurn && rightDist < collEntf) { 
    flash->turnRight (600); 
    delay (600); 
  } else if (!(rightTurn) && leftDist < collEntf) { 
    flash->turnLeft (600); 
    delay(600); }
} 
 

