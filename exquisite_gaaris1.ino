// Dante
// Nov 3, 2023

// audio
#define PIEZO_PIN 2

// const value telling the compiler it won't change
const int buttonPin = 3;

// bool to keep track of roulette wheel
bool isPlaying = false;

// custom delay
int startTime;

// global ints
int numSpins;
int stopPinIndex;
int spin;

// keep track of pins
int pins[] = {13, 12, 11, 10, 9, 8, 7, 6, 5, 4};

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(PIEZO_PIN, OUTPUT);
  DDRD = B11111111; // set port D to output
  DDRB = B11111111; // set port B to output
  attachInterrupt(digitalPinToInterrupt(buttonPin), playing, RISING);
  Serial.begin(9600);
}

void loop() {
  if (isPlaying == true) {
    // determine the number of spins (1 to 3)
    int numSpins = random(1, 4);

    // randomly select a pin to stop on
    int stopPinIndex = random(0, sizeof(pins) / sizeof(pins[0]));

    // spin the LEDs
    for (int spin = 0; spin < numSpins; ++spin) {
      for (int i = 0; i < sizeof(pins) / sizeof(pins[0]); ++i) {
        digitalWrite(pins[i], HIGH);
        tone(PIEZO_PIN, 1000);
        delaySecond(70);
        noTone(PIEZO_PIN);
        digitalWrite(pins[i], LOW);
        delaySecond(70);

        // check if it's the last spin and the random pin is reached
        if (spin == numSpins - 1 && i == stopPinIndex) {
          // flash the selected pin four times
          flashPin(pins[i]);

          // set isPlaying to false to stop the loop
          isPlaying = false;
          break; // break out of the loop
        }
      }
    }
  } else {
    // cleanup
    startTime = 0;
    numSpins = 0;
    stopPinIndex = 0;
    spin = 0;
    PORTB = B00000000;
    PORTD = B00000000;
  }
}

void playing() {
  if (digitalRead(buttonPin) == HIGH) {
    delay(50); // debounce delay
    isPlaying = true;
  }
}

// makes use of functions parameters/arguments
// faster replacement for delay
void delaySecond(int interval) {
  startTime = millis();
  while (millis() - startTime < interval);
}

// function to make a pin flash four times
void flashPin(int pin) {
  for (int i = 0; i < 4; ++i) {
    digitalWrite(pin, HIGH);
    tone(PIEZO_PIN, 1000);
    delaySecond(170);
    noTone(PIEZO_PIN);
    digitalWrite(pin, LOW);
    delaySecond(170);
  }
}
