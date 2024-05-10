#include <Servo.h>
#include <Wire.h>
#include <U8glib.h>

// OLED stuff
U8GLIB_SSD1306_128X64 display(U8G_I2C_OPT_NONE);

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

Servo servo_A0;  //create servo object to control a servo A0
Servo servo_A1;  //create servo object to control a servo A1
Servo servo_A2;  //create servo object to control a servo A2
Servo servo_A3;  //create servo object to control a servo A3
Servo servo_A6;  //create servo object to control a servo A6

int servo_A0_position = 0;   // Start in the 0 degree position (facing forward)
int servo_A1_position = 90;  // Start in the 90 degree position (upright)
int servo_A2_position = 90;  // Start in the 90 degree position (upright)
int servo_A3_position = 90;  // Start in the 90 degree position (center)
int servo_A6_position = 96;  // Start in the 96 degree position (jaws closed)

const int arduino_button = 4;  // the number of the pushbutton pin
const int laser_pointer = 10;
bool PLAYER = true;

enum Program {
  NIL,
  PASS_BLUNT,
  LASER_POINTER,
  CAT_TOY,
  DANCE,
};
enum Program program = Program::NIL;

void setup() {

  Serial.begin(9600);

  servo_A0.attach(9);   //attachs the servo on pin 9 to servo object
  servo_A1.attach(6);   //attachs the servo on pin 6 to servo object
  servo_A2.attach(5);   //attachs the servo on pin 5 to servo object
  servo_A3.attach(3);   //attachs the servo on pin 3 to servo object
  servo_A6.attach(11);  //attachs the servo on pin 11 to servo object

  // Uncomment to see a silly robot dance.
  // startupDance();

  // Reset to normalized motor positions
  resetPosition();

  // initialize the pushbutton pin as an input:
  pinMode(arduino_button, INPUT);

  pinMode(laser_pointer, OUTPUT); // Set D10 as an OUTPUT


  // initialize the display
  attachDisplay();

  program = programMenu();
}



void loop() {
  switch (program) {
    case 0:
      printText("NiL");
      break;
    case 1:
      passBluntLoop();
      break;
    case 2:
      laserPointerLoop();
      break;
    case 3:
      catToyLoop();
      break;
    case 4:
      printText("Dance Demo()");
      break;
  }
}

void attachDisplay() {
  if (display.getMode() == U8G_MODE_R3G3B2)
    display.setColorIndex(255);  // white
  else if (display.getMode() == U8G_MODE_GRAY2BIT)
    display.setColorIndex(3);  // max intensity
  else if (display.getMode() == U8G_MODE_BW)
    display.setColorIndex(1);  // pixel on
  display.setFont(u8g_font_6x10);
  display.setFontRefHeightExtendedText();
  display.setDefaultForegroundColor();
  display.setFontPosTop();
}

Program programMenu() {
  char* menuItems[] = { "Blunt Rotation", "Laser Pointer", "Cat Toy", "Dance Demo" };
  int numItems = sizeof(menuItems) / sizeof(menuItems[0]);

  while (true) {
    displayMenu(menuItems, numItems);

    // Select the program based on the button click and position of A0 potentiometer
    if (digitalRead(arduino_button) == LOW) {
      while (digitalRead(arduino_button) == LOW) {
        delay(100);
        int selectedItem = menuSelector(menuItems, numItems);

        switch (selectedItem) {
          case 0:
            return Program::PASS_BLUNT;
          case 1:
            return Program::LASER_POINTER;
          case 2:
            return Program::CAT_TOY;
          case 3:
            return Program::DANCE;
        }
      }
    }
  }

  return Program::NIL;
}

void displayMenu(char* menuItems[], int numItems) {
  int potentiometer_A0 = menuSelector(menuItems, numItems);

  display.firstPage();
  do {
    display.setFont(u8g_font_6x10);  // Set the font

    char title_buffer[32];
    sprintf(title_buffer, "Select a program: ", numItems);
    display.drawStr(0, 0, title_buffer);  // Draw the title at the top

    for (int i = 0; i < numItems; i++) {
      char buffer[32];
      // Format each string with a leading item number
      if (i == potentiometer_A0) {
        sprintf(buffer, "-> %d. %s", i + 1, menuItems[i]);
      } else {
        sprintf(buffer, "%d. %s", i + 1, menuItems[i]);
      }
      // Draw each string, adjust y-coordinate accordingly
      display.drawStr(0, 11 + i * 10, buffer);
    }
  } while (display.nextPage());
}

int menuSelector(char* menuItems[], int numItems) {
  int potentiometer_A0 = map(analogRead(0), 0, 1023, 0, numItems - 1);
  return potentiometer_A0;
}

void passBluntLoop() {
  printText("Pass Blunt Program");
  if (digitalRead(arduino_button) == LOW) {
    while (digitalRead(arduino_button) == LOW) {
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

void laserPointerLoop() {
  bool on = true;
  digitalWrite(laser_pointer, HIGH);
  while (true) {
    if (on) {
      digitalWrite(laser_pointer, HIGH);
    } else {
      digitalWrite(laser_pointer, LOW);
    }

    if (digitalRead(arduino_button) == LOW) {
        on = !on;
    }
    servo_A0_position = map(analogRead(0), 0, 1023, 0, 180);
    servo_A0.write(servo_A0_position);
    servo_A1_position = map(analogRead(1), 0, 1023, 0, 180);
    servo_A1.write(servo_A1_position);
    servo_A2_position = map(analogRead(2), 0, 1023, 0, 180);
    servo_A2.write(servo_A2_position);
    servo_A3_position = map(analogRead(3), 0, 1023, 0, 180);
    servo_A3.write(servo_A3_position);
    servo_A6_position = map(analogRead(6), 0, 1023, 35, 90);
    servo_A6.write(servo_A6_position);
    displayServoPositions();
  }

}

void catToyLoop() {
  printText("Cat Toy Program");
  while (true) {
    digitalWrite(laser_pointer, HIGH); // Turn the LED on
    delay(1000);            // Wait for a second
    digitalWrite(laser_pointer, LOW);  // Turn the LED off
    delay(1000);            // Wait for a second
  }
}


void printText(const char* text) {
  display.firstPage();
  do {
    display.setFont(u8g_font_6x10);  // Set the font
    display.drawStr(0, 0, text);     // Draw the title at the top
  } while (display.nextPage());
}

void displayServoPositions() {
  char buffer[40];  // Buffer for formatted strings

  display.firstPage();  // Begin the drawing loop
  do {
    display.setFont(u8g_font_6x10);  // Set the font size

    // Draw servo position
    sprintf(buffer, "A0: %d", servo_A0.read());
    display.drawStr(0, 0, buffer);

    sprintf(buffer, "A1: %d", servo_A1.read());
    display.drawStr(0, 10, buffer);

    sprintf(buffer, "A2: %d", servo_A2.read());
    display.drawStr(0, 20, buffer);

    sprintf(buffer, "A3: %d", servo_A3.read());
    display.drawStr(0, 30, buffer);

    sprintf(buffer, "A6: %d", servo_A6.read());
    display.drawStr(0, 40, buffer);

  } while (display.nextPage());  // Loop through all pages
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