/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */

#include <IRremote.h>
//Beginning of Auto generated function prototypes by Atmel Studio
void channelUp();
void channelDown();
void reduceBrightness();
void increaseBrightness();
void turnOn();
void turnOff();
void cycleColors();
void resetForColorToggle();
void resetToWhite();
void updateLEDs();
void setAllLEDs(bool state);
void setWhiteLEDs(int count);
void setRGBLED(int ledPin);
//End of Auto generated function prototypes by Atmel Studio



// Pin definitions for LED connections
#define RED_LED_PIN   11
#define GREEN_LED_PIN 13
#define BLUE_LED_PIN  12
#define WHITE_LED_1   5
#define WHITE_LED_2   6
#define WHITE_LED_3   9
#define WHITE_LED_4   10

// Pin definitions for IR receiver
#define IR_RECEIVE_PIN 2

// IR Remote button codes
#define VOLUME_UP_CODE 21 // Update the correct code for Volume Up
#define VOLUME_DOWN_CODE 7 // Update the correct code for Volume Down
#define CHANNEL_DOWN_CODE 69 // Update the correct code for Channel Down
#define CHANNEL_UP_CODE 71 // Update the correct code for Channel Down


// State variables
int ledState = 0; // Tracks the current state of LEDs
bool toggleColors = false; // Tracks whether the color toggle mode is active
int colorCounter = 1; // Tracks the current color when in color toggle mode
int led_brightness[4] = {255, 255, 255, 255}; // Stores the brightness levels for each LED
int iterate = 0; // Tracks the number of iterations in color toggle mode

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN);

  // Initialize white LEDs as outputs
  pinMode(WHITE_LED_1, OUTPUT);
  pinMode(WHITE_LED_2, OUTPUT);
  pinMode(WHITE_LED_3, OUTPUT);
  pinMode(WHITE_LED_4, OUTPUT);

  // Initialize RGB LEDs as outputs
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
}


void loop() {
  if (IrReceiver.decode()) { // Check if there is an incoming IR signal
    IrReceiver.resume();  // Resume decoding for the next IR signal
    int irCode = IrReceiver.decodedIRData.command; // Get the decoded IR code
    Serial.println(irCode); // Print the IR code to serial monitor for debugging

     // Check the type of IR code received and take appropriate action
    if (irCode == CHANNEL_UP_CODE) {
      channelUp();
    } else if (irCode == CHANNEL_DOWN_CODE) {
      channelDown();
    } else if (irCode == VOLUME_DOWN_CODE) {
      reduceBrightness();
    } else if (irCode == VOLUME_UP_CODE) {
      increaseBrightness();
    }
  }
}

void channelUp() {
  if (toggleColors) {
    cycleColors();
  } else {
    turnOn();
  }
  updateLEDs();
  delay(500); // Debounce delay
}

void channelDown() {
  if (ledState > 4 && !toggleColors){
    resetForColorToggle();
  } else if (toggleColors) {
    resetToWhite();
  } else {
    turnOff();
  }
  updateLEDs();
  delay(500); // Debounce delay
}

void reduceBrightness() {
  int i = ledState - 1;
  for(i = 0; i < 4; i++){
  led_brightness[i] -= 20;
  if (led_brightness[i] < 10) {
    led_brightness[i] = 10;
  }
  updateLEDs();
  }
}


void increaseBrightness() {
  int i = ledState - 1;
  for(i = 0; i < 4; i++){
  led_brightness[i] += 20; // Increase brightness by 10 units
  if (led_brightness[i] > 255) { // Assuming maximum brightness is 255
    led_brightness[i] = 255; // Cap brightness at 255
  }
  updateLEDs();
  }
}

void turnOn() {
  ledState++;
  if (ledState > 7) {
    iterate++;
    ledState = 7;
    toggleColors = true;
    colorCounter = 1;
  }
}

void turnOff() {
  ledState--;
  led_brightness[ledState] = 255;
  if (ledState < 0) {
    ledState = 0;
  }
}

void cycleColors() {
  colorCounter++;
  if (colorCounter > 3) {
    colorCounter = 1;
    iterate++;
  }
}

void resetForColorToggle() {
	ledState--;
	if (ledState <= 0) {
		ledState = 0;
	}
}

void resetToWhite() {
	colorCounter--;
	if (colorCounter < 1 && iterate > 0) {
		colorCounter = 3;
		iterate--;
		if (iterate <= 0) {
			toggleColors = false;	
			iterate = 0;	
		}
	}
}
void updateLEDs() {
  if (!toggleColors) {
    // Handle white LEDs and RGB LEDs for non-color toggle mode
    switch (ledState) {
      case 0: setAllLEDs(LOW); break; // All off
      case 1: setWhiteLEDs(1); break;
      case 2: setWhiteLEDs(2); break;
      case 3: setWhiteLEDs(3); break;
      case 4: setWhiteLEDs(4); break;
      case 5: setRGBLED(RED_LED_PIN); break; // Red LED on
      case 6: setRGBLED(GREEN_LED_PIN); break; // Green LED on
      case 7: setRGBLED(BLUE_LED_PIN); break; // Blue LED on
    }
  } else {
    // Handle RGB LED for color toggle mode
    switch (colorCounter) {
      case 0: setWhiteLEDs(4); break; // All off
      case 1: setRGBLED(RED_LED_PIN); break; // Red LED on
      case 2: setRGBLED(GREEN_LED_PIN); break; // Green LED on
      case 3: setRGBLED(BLUE_LED_PIN); break; // Blue LED on
    }
  }
}

void setAllLEDs(bool state) {
  digitalWrite(WHITE_LED_1, state);
  digitalWrite(WHITE_LED_2, state);
  digitalWrite(WHITE_LED_3, state);
  digitalWrite(WHITE_LED_4, state);
  digitalWrite(RED_LED_PIN, state);
  digitalWrite(GREEN_LED_PIN, state);
  digitalWrite(BLUE_LED_PIN, state);
}


void setWhiteLEDs(int count) {
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);

  if (count >= 1) {
    analogWrite(WHITE_LED_1, led_brightness[0]);
  } else {
    digitalWrite(WHITE_LED_1, LOW);
  }

  if (count >= 2) {
    analogWrite(WHITE_LED_2, led_brightness[1]);
  } else {
    digitalWrite(WHITE_LED_2, LOW);
  }

  if (count >= 3) {
    analogWrite(WHITE_LED_3, led_brightness[2]);
  } else {
    digitalWrite(WHITE_LED_3, LOW);
  }

  if (count >= 4) {
    analogWrite(WHITE_LED_4, led_brightness[3]);
  } else {
    digitalWrite(WHITE_LED_4, LOW);
  }

}

void setRGBLED(int ledPin) {
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);
  digitalWrite(ledPin, HIGH);
}
