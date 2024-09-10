#include "dual-seven-seg.h"
#include <Arduino.h>

const byte decimalDigits[] {
// ABCDEFGDp
// ||||||||
  B11111100, // 0
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110  // 9
};

const byte hexadecimalDigits[] {
// ABCDEFGDp
// ||||||||
  B11111100, // 0
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11101110, // A - 10
  B00111110, // B - 11
  B10011100, // C - 12
  B01111010, // D - 13
  B10011110, // E - 14
  B10001110  // F - 15
};

const byte danceDigits[] = {
// ABCDEFGDp
// ||||||||
  B10010000,
  B01001000,
  B00000010,
  B00100100
};

const byte errorDigit = B00000010;

// variables

int _dataPin;
int _latchPin;
int _clockPin;
int _dance;

// constructor

DualSevenSeg::DualSevenSeg(int dataPin, int latchPin, int clockPin, bool dance) {
  _dataPin = dataPin;
  _latchPin = latchPin;
  _clockPin = clockPin;
  pinMode(_dataPin, OUTPUT);
  pinMode(_latchPin, OUTPUT);
  pinMode(_clockPin, OUTPUT);
  _dance = dance;
}

// public methods

void DualSevenSeg::displayNumber(byte number) {
  if (_dance) {
    dance();
  }

  if (number < 100) {
    byte tens = number % 100 / 10;
    byte units = number % 10;
    sendDigits(decimalDigits[tens], decimalDigits[units]);
  } else {
    sendDigits(errorDigit, errorDigit);
  }
}

void DualSevenSeg::displayNumberHex(byte number) {
  if (_dance) {
    dance();
  }

  byte tens = number % 256 / 16;
  byte units = number % 16;
  sendDigits(hexadecimalDigits[tens], hexadecimalDigits[units]);
}

// private methods

void DualSevenSeg::sendDigits(byte tens, byte units) {
  digitalWrite(_latchPin, LOW);
  shiftOut(_dataPin, _clockPin, LSBFIRST, units);
  shiftOut(_dataPin, _clockPin, LSBFIRST, tens);
  digitalWrite(_latchPin, HIGH);
}

void DualSevenSeg::dance() {
  for (int i = 0; i < 8; i++) {
    byte digit = danceDigits[i % 4];
    sendDigits(digit, digit);
    delay(100);
  }
}
