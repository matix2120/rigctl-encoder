#include <Encoder.h>

#define FREQ_ENCODER_A 2
#define FREQ_ENCODER_B 3
#define FREQ_TOGGLE_BUTTON 6
#define NB_BUTTON 7
#define NR_BUTTON 8
#define COMP_BUTTON 9
#define PTT_BUTTON 10

Encoder freqEncoder(FREQ_ENCODER_A, FREQ_ENCODER_B);
long freqStep = 1000; // Hz

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

String serialInput = "";

void updateFrequency();
void toggleStep();
void toggleNoiseBlanker();
void toggleNoiseReduction();
void toggleCompressor();
void updatePTT();

void setup() {
  pinMode(FREQ_TOGGLE_BUTTON, INPUT_PULLUP);
  pinMode(NB_BUTTON, INPUT_PULLUP);
  pinMode(NR_BUTTON, INPUT_PULLUP);
  pinMode(COMP_BUTTON, INPUT_PULLUP);
  pinMode(PTT_BUTTON, INPUT_PULLUP);

  Serial.begin(115200);
}

void loop() {
  updateFrequency();
  toggleStep();
  toggleNoiseBlanker();
  toggleNoiseReduction();
  toggleCompressor();
  updatePTT();
}

void updateFrequency()
{
  static long lastEncoderValue = 0;
  long encoderValue = freqEncoder.read();
  long diff = encoderValue - lastEncoderValue;

  if (diff >= 4)
  {
    Serial.println("f+" + String(freqStep));
    lastEncoderValue = encoderValue;
  }
  else if (diff <= -4)
  {
    Serial.println("f-" + String(freqStep));
    lastEncoderValue = encoderValue;
  }

}

void toggleStep()
{
  bool buttonState = digitalRead(FREQ_TOGGLE_BUTTON);
  static bool lastButtonState = HIGH;
  if (buttonState == LOW && lastButtonState == HIGH && millis() - lastDebounceTime > debounceDelay)
  {
    freqStep = (freqStep == 100) ? 1000 : 100;
    Serial.println("step+" + String(freqStep));
  }

  lastButtonState = buttonState;
}

void toggleNoiseBlanker()
{
  bool buttonState = digitalRead(NB_BUTTON);
  static bool lastButtonState = LOW;
  if (buttonState == LOW && lastButtonState == HIGH && millis() - lastDebounceTime > debounceDelay)
  {
    Serial.println("nb");
    lastDebounceTime = millis();
  }
  lastButtonState = buttonState;
}

void toggleNoiseReduction()
{
  bool buttonState = digitalRead(NR_BUTTON);
  static bool lastButtonState = LOW;
  if (buttonState == LOW && lastButtonState == HIGH && millis() - lastDebounceTime > debounceDelay)
  {
    Serial.println("nr");
    lastDebounceTime = millis();
  }
  lastButtonState = buttonState;
}

void toggleCompressor()
{
  bool buttonState = digitalRead(COMP_BUTTON);
  static bool lastButtonState = LOW;
  if (buttonState == LOW && lastButtonState == HIGH && millis() - lastDebounceTime > debounceDelay)
  {
    Serial.println("comp");
    lastDebounceTime = millis();
  }
  lastButtonState = buttonState;
}

void updatePTT() {
  static bool pttState = false;
  bool pttButtonState = digitalRead(PTT_BUTTON);
  if (pttButtonState == LOW && !pttState)
  {
    pttState = true;
    Serial.println("ptt+1");
  }
  else if (pttButtonState == HIGH && pttState)
  {
    pttState = false;
    Serial.println("ptt+0");
  }
}
