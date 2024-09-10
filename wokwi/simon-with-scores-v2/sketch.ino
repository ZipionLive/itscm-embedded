#include "dual-seven-seg.h"

// PINOUT CONSTANTS
const int buttonPlay = A1;
const int buzzer = 2;
const int clock = 3;
const int latch = 4;
const int data = 5;
const int ledRed = 6;
const int ledYellow = 7;
const int ledGreen = 8;
const int ledBlue = 9;
const int buttonRed = 10;
const int buttonYellow = 11;
const int buttonGreen = 12;
const int buttonBlue = 13;

// VALUE CONSTANTS
const int duration = 500;
const int red = 0;
const int yellow = 1;
const int green = 2;
const int blue = 3;

// Determines the game's target score for victory
const int sequenceLength = 100;

const byte digits[] {
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

const byte digitDance[] = {
// ABCDEFGDp
// ||||||||
  B10010000,
  B01001000,
  B00000010,
  B00100100
};

// GLOBAL VARIABLES
int sequence[sequenceLength];
int noteCount;
bool play;
bool success;

// DEFINES
#define NOTE_C  131
#define NOTE_D  147
#define NOTE_E  165
#define NOTE_F  175
#define NOTE_G  196
#define NOTE_A  220
#define NOTE_B  247

DualSevenSeg DSS = DualSevenSeg(data, latch, clock, true);

void setHit(String color, int ledPin, int note, int durationOpt = duration);

void setup() {
  Serial.begin(9600);
  Serial.println("SETUP STARTED");
  pinMode(buttonPlay, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(buttonRed, INPUT);
  pinMode(buttonYellow, INPUT);
  pinMode(buttonGreen, INPUT);
  pinMode(buttonBlue, INPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  play = false;
  displayScore();
  Serial.println("SETUP DONE");
}

void loop() {
  if (digitalRead(buttonPlay) == HIGH) {
    noteCount = 0;
    play = true;
  	success = true;
    delay(duration);
  }
  if (play) {
    Serial.println("GAME START");
    playLoop();
  }
}

int getRandom(int max) {
  randomSeed(analogRead(0));
  return random(max);
}

void playLoop() {
  while(success) {
    displayScore();
    if (noteCount < sequenceLength) {
      Serial.print("noteCount : ");
      Serial.println(noteCount);
      sequence[noteCount] = getRandom(4);
      playSequence();
      userTurn();
      noteCount++;
    } else {
      Serial.println("WINNER WINNER CHICKEN DINNER");
      showVictory();
      success = false;
      play = false;
      noteCount = 0;
    }
  }
}

void playSequence() {
  for (int i = 0; i <= noteCount; i++) {
    switch (sequence[i]) {
	  case red: 
	    setHit("RED", ledRed, NOTE_E);
        break;
      case yellow:
	setHit("YELLOW", ledYellow, NOTE_F);
	break;
      case green:
	setHit("GREEN", ledGreen, NOTE_G);
	break;
      case blue:
	setHit("BLUE", ledBlue, NOTE_A);
	break;
    }
    delay(100);
  }
}

void userTurn() {
  int i = 0;
  while(i <= noteCount && success == true) {
    Serial.print("Value at sequence[");
    Serial.print(i);
    Serial.print("] : ");
    Serial.println(sequence[0]);
    if (digitalRead(buttonRed) == HIGH && sequence[i] == red) {
      setHit("RED", ledRed, NOTE_E);
      i++;
    } else if(digitalRead(buttonYellow) == HIGH && sequence[i] == yellow) {
      setHit("YELLOW", ledYellow, NOTE_F);
      i++;
    } else if(digitalRead(buttonGreen) == HIGH && sequence[i] == green) {
      setHit("GREEN", ledGreen, NOTE_G);
      i++;
    } else if(digitalRead(buttonBlue) == HIGH && sequence[i] == blue) {
      setHit("BLUE", ledBlue, NOTE_A);
      i++;
    } else if(digitalRead(buttonRed) == HIGH
             || digitalRead(buttonYellow) == HIGH
             || digitalRead(buttonGreen) == HIGH
             || digitalRead(buttonBlue) == HIGH) {
      showError();
      noteCount = 0;
      displayScore();
      success = false;
    }
  }
}

void setHit(String color, int ledPin, int note, int durationOpt) {
  Serial.println(color);
  digitalWrite(ledPin, HIGH);
  tone(buzzer, note, durationOpt);
  delay(duration);
  digitalWrite(ledPin, LOW);
}

void displayScore() {
  Serial.print("Score : ");
  Serial.println(noteCount);
  DSS.displayNumber(noteCount);
}

void showError() {
  digitalWrite(ledRed, HIGH);
  digitalWrite(ledYellow, HIGH);
  digitalWrite(ledGreen, HIGH);
  digitalWrite(ledBlue, HIGH);
  tone(buzzer, NOTE_C, duration);
  delay(duration);
  digitalWrite(ledRed, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledBlue, LOW);
}

void showVictory() {
  setHit("RED", ledRed, NOTE_E, 100);
  setHit("YELLOW", ledYellow, NOTE_F, 100);
  setHit("GREEN", ledGreen, NOTE_G, 100);
  setHit("BLUE", ledBlue, NOTE_A, 100);

  setHit("BLUE", ledBlue, NOTE_A, 100);
  setHit("GREEN", ledGreen, NOTE_G, 100);
  setHit("YELLOW", ledYellow, NOTE_F, 100);
  setHit("RED", ledRed, NOTE_E, 100);

  setHit("RED", ledRed, NOTE_E, 100);
  setHit("YELLOW", ledYellow, NOTE_F, 100);
  setHit("GREEN", ledGreen, NOTE_G, 100);
  setHit("BLUE", ledBlue, NOTE_A, 100);

  setHit("BLUE", ledBlue, NOTE_A, 100);
  setHit("GREEN", ledGreen, NOTE_G, 100);
  setHit("YELLOW", ledYellow, NOTE_F, 100);
  setHit("RED", ledRed, NOTE_E, 100);
}
