/*
  Button Box HID
  Author: Daniel Mota

  Description:
  Button box implemented using Arduino as a USB HID device.
  Each physical button is mapped to a joystick button.
  Includes software debounce and clean code structure.

  Hardware:
  - Arduino Leonardo / Micro / Pro Micro (ATmega32u4)
  - Push buttons connected to GND
*/

#include <Joystick.h>

// ================= CONFIGURATION =================

// Number of buttons used
constexpr uint8_t BUTTON_COUNT = 12;

// Arduino pins used for buttons
constexpr uint8_t buttonPins[BUTTON_COUNT] = {
  2, 3, 4, 5, 6, 7,
  8, 9, 10, 14, 15, 16
};

// Debounce time (milliseconds)
constexpr unsigned long DEBOUNCE_TIME = 25;

// =================================================

// Create Joystick instance
Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID,
  JOYSTICK_TYPE_JOYSTICK,
  BUTTON_COUNT,   // Number of buttons
  0,              // No hat switches
  false, false, false, // X, Y, Z axis
  false, false, false, // Rx, Ry, Rz
  false, false,       // Rudder, Throttle
  false, false, false // Accelerator, Brake, Steering
);

// Button states
bool lastButtonState[BUTTON_COUNT];
bool currentButtonState[BUTTON_COUNT];
unsigned long lastDebounceTime[BUTTON_COUNT];

// ================= INITIALIZATION =================

void setup() {
  for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    lastButtonState[i] = HIGH;
    currentButtonState[i] = HIGH;
    lastDebounceTime[i] = 0;
  }

  Joystick.begin();
}

// ================= MAIN LOOP ======================

void loop() {
  readButtons();
}

// ================= FUNCTIONS ======================

void readButtons() {
  unsigned long currentTime = millis();

  for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
    bool reading = digitalRead(buttonPins[i]);

    if (reading != lastButtonState[i]) {
      lastDebounceTime[i] = currentTime;
    }

    if ((currentTime - lastDebounceTime[i]) > DEBOUNCE_TIME) {
      if (reading != currentButtonState[i]) {
        currentButtonState[i] = reading;

        // Button pressed (LOW because of INPUT_PULLUP)
        if (currentButtonState[i] == LOW) {
          Joystick.setButton(i, 1);
        } else {
          Joystick.setButton(i, 0);
        }
      }
    }

    lastButtonState[i] = reading;
  }
}
