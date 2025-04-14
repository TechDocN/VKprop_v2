#include <math.h>

// ----------- RGB LED Pins (XIAO pinout) -----------
#define RED_PIN     1   // D1 / A1
#define GREEN_PIN   2   // D2 / A2
#define BLUE_PIN    3   // D3 / A3

// ----------- Button Pins --------------------------
#define BUTTON_IDLE_RESET  4   // D4 / A4
#define BUTTON_AMBER       5   // D5 / A5
#define BUTTON_RED         6   // D6 / A6
#define BUTTON_GREEN       7   // D7 / A7

// ----------- State Machine -------------------------
enum State {
  IDLE,
  BREATH_AMBER,
  BREATH_GREEN,
  ALERT_RED
};

State currentState = IDLE;

// ----------- Timing Variables ----------------------
unsigned long lastRedFlashToggle = 0;
bool redFlashState = false;
bool amberJustFlashed = false;

// ----------- Setup ----------------------------------
void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  pinMode(BUTTON_IDLE_RESET, INPUT_PULLUP);
  pinMode(BUTTON_AMBER, INPUT_PULLUP);
  pinMode(BUTTON_RED, INPUT_PULLUP);
  pinMode(BUTTON_GREEN, INPUT_PULLUP);

  randomSeed(analogRead(0));
  setColor(0, 0, 0);  // All off
}

// ----------- Main Loop ------------------------------
void loop() {
  checkButtons();

  switch (currentState) {
    case IDLE:
      runIdleBluePulse();
      break;

    case BREATH_AMBER:
      breathingEffectWithFlash(200, 255, 0, 0.5);
      break;

    case BREATH_GREEN:
      breathingEffect(0, 255, 0, 0.5);
      break;

    case ALERT_RED:
      alertRedFlash();
      break;
  }
}

// ----------- Button Handling ------------------------
void checkButtons() {
  if (digitalRead(BUTTON_IDLE_RESET) == LOW) {
    currentState = IDLE;
    setColor(0, 0, 0);
  } else if (digitalRead(BUTTON_AMBER) == LOW && currentState == IDLE) {
    currentState = BREATH_AMBER;
  } else if (digitalRead(BUTTON_GREEN) == LOW && currentState == IDLE) {
    currentState = BREATH_GREEN;
  } else if (digitalRead(BUTTON_RED) == LOW && currentState == IDLE) {
    currentState = ALERT_RED;
    redFlashState = false;
    lastRedFlashToggle = millis();
  }
}

// ----------- Color Control (Common Cathode) ----------
void setColor(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(RED_PIN,   r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN,  b);
}

// ----------- Blue Idle Pulse ------------------------
void runIdleBluePulse() {
  static unsigned long phaseStart = 0;
  static int phase = 0;

  unsigned long now = millis();
  unsigned long elapsed = now - phaseStart;

  switch (phase) {
    case 0: // Fade in
      if (elapsed >= 500) {
        phase = 1;
        phaseStart = now;
        setColor(0, 0, 255);
      } else {
        float t = (float)elapsed / 500.0;
        uint8_t r = 20 * (1.0 - t);
        uint8_t g = 40 * (1.0 - t);
        uint8_t b = 100 + (155 * t);
        setColor(r, g, b);
      }
      break;

    case 1: // Hold
      if (elapsed >= 500) {
        phase = 2;
        phaseStart = now;
      } else {
        setColor(0, 0, 255);
      }
      break;

    case 2: // Fade out
      if (elapsed >= 500) {
        phase = 0;
        phaseStart = now;
      } else {
        float t = (float)elapsed / 500.0;
        uint8_t r = 20 * t;
        uint8_t g = 40 * t;
        uint8_t b = 255 - (155 * t);
        setColor(r, g, b);
      }
      break;
  }
}

// ----------- Amber Breathing + Flash ---------------
void breathingEffectWithFlash(uint8_t r, uint8_t g, uint8_t b, float cap) {
  static unsigned long breathStart = 0;
  if (breathStart == 0) breathStart = millis();

  float t = (millis() - breathStart) / 1200.0;
  float wave = (sin(t * PI) + 1.0) / 2.0;
  float brightness = wave * cap;

  if (wave > 0.98 && !amberJustFlashed) {
    setColor(200, 255, 0);
    amberJustFlashed = true;
    delay(50);
  } else {
    if (wave < 0.95) amberJustFlashed = false;
    setColor(r * brightness, g * brightness, b * brightness);
  }
}

// ----------- Green Breathing ------------------------
void breathingEffect(uint8_t r, uint8_t g, uint8_t b, float cap) {
  static unsigned long breathStart = 0;
  if (breathStart == 0) breathStart = millis();

  float t = (millis() - breathStart) / 1500.0;
  float brightness = (sin(t * PI) + 1.0) / 2.0 * cap;
  setColor(r * brightness, g * brightness, b * brightness);
}

// ----------- Red Flash Alert ------------------------
void alertRedFlash() {
  unsigned long now = millis();
  unsigned long interval = redFlashState ? 75 : 925;  // ON pulse for 75ms, 70% red for 925ms

  if (now - lastRedFlashToggle >= interval) {
    lastRedFlashToggle = now;
    redFlashState = !redFlashState;

    if (redFlashState) {
      setColor(255, 0, 0);  // Full red pulse
    } else {
      setColor(80, 0, 0);  // Dim red (70%)
    }
  }
}