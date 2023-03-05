// Add keyboard for button presses
#include "Keyboard.h"

// Button pins
const int buttonBlue = 10;
const int buttonRed = 16;
const int buttonYellow = 14;
const int buttonWhite = 15;
const int buttonBlack = 18;

// LED pins
int ledPins[6] = { 2, 3, 4, 5, 7, 9 };

const int ledButtonBlue = 5;
const int ledButtonRed = 7;
const int ledButtonYellow = 4;
const int ledButtonWhite = 2;
const int ledButtonBlack = 3;
const int ledAutoWarn = 9;

// Track button states: pressed (LOW) or not pressed (HIGH)
int buttonStateBlue = HIGH;
int buttonStateRed = HIGH;
int buttonStateYellow = HIGH;
int buttonStateWhite = HIGH;
int buttonStateBlack = HIGH;

// Last state of the buttons
int buttonLastStateBlue = HIGH;
int buttonLastStateRed = HIGH;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTimeBlue = 0;  // Last time the blue button was registered
unsigned long lastDebounceTimeRed = 0;   // Red button
unsigned long lastSteerTimeout = 0;      // Timing for steering autowarn

unsigned long debounceDelay = 50;        // the debounce time; increase if the output flickers
unsigned long steeringTimeout = 110000;  // Warn after 110 seconds that steering will time out
//unsigned long steeringTimeout = 5000;    // 5 second timeout for testing - uncomment to shorten for debugging
unsigned long steeringTimeoutWarnTime = 10000;  // Warn for 10 seconds - steering will time out after 120 seconds
bool isAutowarnActive = true;


void setup() {
  // put your setup code here, to run once:

  pinMode(buttonBlue, INPUT);
  pinMode(buttonRed, INPUT);
  pinMode(buttonYellow, INPUT);
  pinMode(buttonWhite, INPUT);
  pinMode(buttonBlack, INPUT);


  pinMode(ledButtonBlue, OUTPUT);
  pinMode(ledButtonRed, OUTPUT);
  pinMode(ledButtonYellow, OUTPUT);
  pinMode(ledButtonWhite, OUTPUT);
  pinMode(ledButtonBlack, OUTPUT);
  pinMode(ledAutoWarn, OUTPUT);

  Keyboard.begin();

  Serial.begin(9600);  // open the serial port at 9600 bps:

  Keyboard.releaseAll();

  lastSteerTimeout = millis();
}

// Handles a left (a) or right (d) button press
void steeringButton(int buttonState, int &buttonLastState, int ledPin, char keyToSend, unsigned long &lastDebounceTime) {

  unsigned long currentMillis;
  currentMillis = millis();

  // Button was pressed or released:
  if (buttonState != buttonLastState) {

    lastSteerTimeout = millis();  // reset steering timeout

    if (currentMillis - lastDebounceTime > debounceDelay) {


      if (buttonState == LOW) {
        // turn LED on:
        digitalWrite(ledPin, HIGH);

        Keyboard.press(keyToSend);

        Serial.print(keyToSend);
        Serial.print(" Press LOW\n");


      } else {
        // turn LED off:

        digitalWrite(ledPin, LOW);

        Keyboard.release(keyToSend);

        Serial.print(keyToSend);
        Serial.print(" Release HIGH\n");

      }

      buttonLastState = buttonState;

      lastDebounceTime = currentMillis;

      isAutowarnActive = true;         // Reactivate auto warning
      digitalWrite(ledAutoWarn, LOW);  // Turn off LED if it was still blinking


    } else {

      Serial.println("Debouncing");
    }
  }

}

// Begin main loop

void loop() {
  // put your main code here, to run repeatedly:

  buttonStateBlue = digitalRead(buttonBlue);
  buttonStateRed = digitalRead(buttonRed);
  buttonStateYellow = digitalRead(buttonYellow);
  buttonStateWhite = digitalRead(buttonWhite);
  buttonStateBlack = digitalRead(buttonBlack);

  unsigned long currentMillis;
  currentMillis = millis();

  // Prevent a steering timeout from occuring:
  if (isAutowarnActive && (currentMillis - lastSteerTimeout > steeringTimeout)) {

    // Blink the light every second during warning phase:
    int currentSeconds = (currentMillis / 1000);
    int newLedState = (currentSeconds % 2 == 0) ? LOW : HIGH;

    if (digitalRead(ledAutoWarn) != newLedState) {

      digitalWrite(ledAutoWarn, newLedState);

    }

    // Turn off after warn phase is over - it's too late, autosteer is active
    if (currentMillis - lastSteerTimeout > steeringTimeout + steeringTimeoutWarnTime) {

      digitalWrite(ledAutoWarn, LOW);

      isAutowarnActive = false;

    }
  }

  // Steer right (d) or left (a)

  steeringButton(buttonStateBlue, buttonLastStateBlue, ledButtonBlue, 'd', lastDebounceTimeBlue);
  steeringButton(buttonStateRed, buttonLastStateRed, ledButtonRed, 'a', lastDebounceTimeRed);

  // Wave
  if (buttonStateYellow == LOW) {

    digitalWrite(ledButtonYellow, HIGH);

    Keyboard.write('t');
    delay(150);
    Keyboard.write(KEY_KP_SLASH);
    Keyboard.write('w');
    Keyboard.write('a');
    Keyboard.write('v');
    Keyboard.write('e');
    Keyboard.write(KEY_RETURN);

    delay(250);

  } else {
    digitalWrite(ledButtonYellow, LOW);
  }

  // Win
  if (buttonStateWhite == LOW) {

    digitalWrite(ledButtonWhite, HIGH);

    Keyboard.write('t');
    delay(150);
    Keyboard.write(KEY_KP_SLASH);
    Keyboard.write('w');
    Keyboard.write('i');
    Keyboard.write('n');
    Keyboard.write(KEY_RETURN);

    delay(250);

  } else {
    digitalWrite(ledButtonWhite, LOW);
  }

  
  // Lose
  if (buttonStateBlack == LOW) {

    digitalWrite(ledButtonBlack, HIGH);

    Keyboard.write('t');
    delay(150);
    Keyboard.write(KEY_KP_SLASH);
    Keyboard.write('l');
    Keyboard.write('o');
    Keyboard.write('s');
    Keyboard.write('e');
    Keyboard.write(KEY_RETURN);

    delay(250);

  } else {
    digitalWrite(ledButtonBlack, LOW);
  }
}
