
#include "Arduino.h"


const int FLOOD_LIGHTS = 22; // Flood lights attached to pin 22
const int MOTION_SENSOR = 23; // PIR motion sensor pin
const uint8_t ALARM_PIN = 24; // Active buzzer simulating our alarm speakers

const uint8_t ON = HIGH;
const uint8_t OFF = LOW;

bool previous_motion = false; // previous state of motion sensor's pin

void setup() {
  Serial.begin(9600); // initialize serial
  pinMode(MOTION_SENSOR, INPUT); // set arduino pin to input mode
  pinMode(FLOOD_LIGHTS, OUTPUT); // set arduino pin to output mode
  pinMode(ALARM_PIN, OUTPUT); // Set Alarm pin to OUTPUT
}

unsigned long start_time;
unsigned long delay_start = 0;
const uint8_t DELAY_SECONDS = 1;



// How long to delay while beeping the buzzer (in milliseconds).
const int BUZZER_DELAY = 300; // Delay this much while beeping the buzzer

unsigned long buzzer_delay_start = 0; // when did our last buzzer state change

const int TONES[] = {880, 698, 587};
bool playing_tones = false;
int current_tone;


void loop() {
  bool motion_detected = digitalRead(MOTION_SENSOR); // read new state

  if (motion_detected && !previous_motion) { // New motion detected
    Serial.println("Motion detected!");
    playing_tones = true;
    current_tone = 0;
    delay_start = 0; // stop delay timer when motion detected
    start_time = millis();

    if (playing_tones) {
      int current_millis = millis(); // Get current millis() count
      if (current_millis - buzzer_delay_start > BUZZER_DELAY) {
        if (current_tone < sizeof(TONES) / sizeof(int)) {
          tone(ALARM_PIN, TONES[current_tone++]);
          Serial.print("play tone ");
          Serial.println(current_tone);
          buzzer_delay_start = current_millis; // Save start time
        } else {
          Serial.println("stop playing");
          noTone(ALARM_PIN);
          playing_tones = false;
        }
      }
    }
  } else if (!motion_detected && previous_motion) { // motion delay expired
    float seconds = (millis() - start_time) / 1000.0;
    Serial.print("Motion stopped after ");
    Serial.print(seconds);
    Serial.println(" seconds.");
    delay_start = millis(); // Start delay timer
  }

  // If flood lights are on and our timer expires then turn off the lights
  if (delay_start && (millis() - delay_start) / 1000 >= DELAY_SECONDS) {
    float seconds = (millis() - start_time) / 1000.0;
    Serial.print("Stop Alarm ");
    Serial.print(seconds);
    Serial.println(" seconds.");
    noTone(ALARM_PIN);
    delay_start = 0;
  }

  previous_motion = motion_detected;
  delay(10);
}