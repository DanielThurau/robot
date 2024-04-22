#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED stuff
#define OLED_RESET     4
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

Servo servo_A0;//create servo object to control a servo A0
Servo servo_A1;//create servo object to control a servo A1
Servo servo_A2;//create servo object to control a servo A2
Servo servo_A3;//create servo object to control a servo A3
Servo servo_A6;//create servo object to control a servo A6

int servo_A0_position = 0; // Start in the 0 degree position (facing forward)
int servo_A1_position = 90; // Start in the 90 degree position (upright)
int servo_A2_position = 90; // Start in the 90 degree position (upright)
int servo_A3_position = 90; // Start in the 90 degree position (center)
int servo_A6_position = 96; // Start in the 96 degree position (jaws closed)

const int arduino_button = 4;     // the number of the pushbutton pin

int PA6;

bool PLAYER = true;

enum Program {
  NIL,
  PASS_BLUNT,
  LASER_POINTER,
};

enum Program program = Program::NIL;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);


  servo_A0.attach(9); //attachs the servo on pin 9 to servo object
  servo_A1.attach(6); //attachs the servo on pin 6 to servo object
  servo_A2.attach(5); //attachs the servo on pin 5 to servo object
  servo_A3.attach(3); //attachs the servo on pin 3 to servo object
  servo_A6.attach(11); //attachs the servo on pin 11 to servo object

  // Uncomment to see a silly robot dance.
  // startupDance();

  resetPosition();

  // initialize the pushbutton pin as an input:
  pinMode(arduino_button, INPUT);

  // setup the display 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);//Sets the font display color
  display.clearDisplay();//cls

  program = programMenu();
}

void loop() {
  switch (program) {
    case 0:
      printText("NiL");
      break;
    case 1:
      printText("Pass blunt()");
      break;
    case 2: 
      printText("Lase pointer()");
      break;
  }
  // displayServoPositions();
}

Program programMenu() {
  while (true) {
    displayMenu();
    // if(digitalRead(arduino_button)==LOW){
    //   while(digitalRead(arduino_button)==LOW){
    //     delay(100);
    //   }

    // }
  }
  return Program::LASER_POINTER;
}

void displayMenu() {
  char *strings[] = {"Blunt Rotation", "Laser Pointer", "Cat Toy", "Dance Demo"};
  display.clearDisplay();//cls
  //Set the font size
  display.setTextSize(1);
  //Set the display location
  display.setCursor(0,0);
  //String displayed
  display.println("Select a program to run");
  for (int i=0; i < 4; i++) {
      display.println("%s", strings[i]);
  }

  //Began to show
  display.display();
}


void passBlunt() {
    if(digitalRead(arduino_button)==LOW){
      while(digitalRead(arduino_button)==LOW){
        delay(100);
      }
      PLAYER = !PLAYER;
      moveToPositionDouble(servo_A1, servo_A2, 85, 85);
      servo_A1_position = 90;
      servo_A2_position = 90;
      delay(1000);
      if (PLAYER == 1) {
        servo_A0_position = 0;
        servo_A0.write(servo_A0_position);
      } else {
        servo_A0_position = 180;
        servo_A0.write(servo_A0_position);
      }
      delay(1000);
      moveToPositionDouble(servo_A1, servo_A2, 45, 45);
      servo_A1_position = 45;
      servo_A2_position = 45;
      delay(1000);
    }
}


void printText(String text) {
  display.clearDisplay();//cls
  //Set the font size
  display.setTextSize(1);
  //Set the display location
  display.setCursor(0,0);
  //String displayed
  display.println(text);

  //Began to show
  display.display();
}

void displayServoPositions() {
  display.clearDisplay();//cls
  //Set the font size
  display.setTextSize(1);
  //Set the display location
  display.setCursor(0,0);
  //String displayed
  display.println("Blunt Rotation");
  display.print("A0: "); display.println(servo_A0.read());
  display.print("A1: "); display.println(servo_A1.read());
  display.print("A2: "); display.println(servo_A2.read());
  display.print("A3: "); display.println(servo_A3.read());
  display.print("A6: "); display.println(servo_A6.read());

  display.print("PLAYER: "); display.println(PLAYER);

  //Began to show
  display.display();

}

void startupDance() {
  // Sets the Servos to their default postion (forward/upright/closed)
  servo_A0_position = moveToPosition(servo_A0, servo_A0_position);
  servo_A1_position = moveToPosition(servo_A1, servo_A1_position);
  servo_A2_position = moveToPosition(servo_A2, servo_A2_position);
  servo_A3_position = moveToPosition(servo_A3, servo_A3_position);
  servo_A6_position = moveToPosition(servo_A6, servo_A6_position);

  servo_A0.write(servo_A0_position);
  servo_A1.write(servo_A1_position);
  servo_A2.write(servo_A2_position);
  servo_A3.write(servo_A3_position);
  servo_A6.write(servo_A6_position);

  delay(2000);

  servo_A0_position = moveToPosition(servo_A0, 180);
  servo_A0_position = moveToPosition(servo_A0, 0);
  servo_A0_position = moveToPosition(servo_A0, 180);
  servo_A0_position = moveToPosition(servo_A0, 0);

  servo_A1_position = moveToPosition(servo_A1, 180);
  servo_A1_position = moveToPosition(servo_A1, 0);
  servo_A1_position = moveToPosition(servo_A1, 180);
  servo_A1_position = moveToPosition(servo_A1, 90);

  servo_A2_position = moveToPosition(servo_A2, 180);
  servo_A2_position = moveToPosition(servo_A2, 0);
  servo_A2_position = moveToPosition(servo_A2, 180);
  servo_A2_position = moveToPosition(servo_A2, 90);

  servo_A3_position = moveToPosition(servo_A3, 180);
  servo_A3_position = moveToPosition(servo_A3, 0);
  servo_A3_position = moveToPosition(servo_A3, 180);
  servo_A3_position = moveToPosition(servo_A3, 90);

  servo_A6_position = moveToPosition(servo_A6, 97);
  servo_A6_position = moveToPosition(servo_A6, 0);
  servo_A6_position = moveToPosition(servo_A6, 97);
  servo_A6_position = moveToPosition(servo_A6, 0);
  servo_A6_position = moveToPosition(servo_A6, 97);
}

void resetPosition() {
  servo_A0_position = moveToPosition(servo_A0, 0);
  servo_A1_position = moveToPosition(servo_A1, 90);
  servo_A2_position = moveToPosition(servo_A2, 90);
  servo_A3_position = moveToPosition(servo_A3, 90);
  servo_A6_position = moveToPosition(servo_A6, 97);

}

void moveToPositionDouble(Servo servoA, Servo servoB, int positionA, int positionB) {
  int cpa = servoA.read();
  int cpb = servoB.read();

  while (cpa != positionA && cpb != positionB) {
    cpa = absoluteApproach(servoA, cpa, positionA);
    cpb = absoluteApproach(servoB, cpb, positionB);
    delay(10);
  }
}


int moveToPosition(Servo servo, int position) {
  int cp = servo.read();
  while (cp != position) {
    cp = absoluteApproach(servo, cp, position);
    delay(10);
  }
  return cp;
}

int absoluteApproach(Servo servo, int current, int destination) {
  int step = 1;
  if (current < destination) {
    current += step;
  } else if (current > destination) {
    current -= step;
  }
  servo.write(current);
  return current;
}