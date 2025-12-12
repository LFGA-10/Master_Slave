#define DEVICE_ROLE 1

#include <SoftwareSerial.h>

SoftwareSerial BT(10, 11);

const int buttonPin = 2;
bool lastButton = HIGH;
bool ledState = false;

const int ledPin = 7;

void setup() {
  Serial.begin(9600);
  BT.begin(9600);

  if (DEVICE_ROLE == 1) {
    pinMode(buttonPin, INPUT_PULLUP);
    Serial.println("MASTER READY");
  } else {
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    Serial.println("SLAVE READY");
  }
}

void loop() {

  if (DEVICE_ROLE == 1) {
    bool reading = digitalRead(buttonPin);

    if (lastButton == HIGH && reading == LOW) {
      ledState = !ledState;

      if (ledState) {
        BT.println("LED:ON");
        Serial.println("Sent → LED:ON");
      } else {
        BT.println("LED:OFF");
        Serial.println("Sent → LED:OFF");
      }
    }

    lastButton = reading;

    if (BT.available()) {
      String feedback = BT.readStringUntil('\n');
      Serial.print("Slave says → ");
      Serial.println(feedback);
    }
  }

  else if (DEVICE_ROLE == 2) {
    if (BT.available()) {
      String command = BT.readStringUntil('\n');
      command.trim();

      Serial.print("Received → ");
      Serial.println(command);

      if (command == "LED:ON") {
        digitalWrite(ledPin, HIGH);
        BT.println("OK:LED_ON");
      }
      else if (command == "LED:OFF") {
        digitalWrite(ledPin, LOW);
        BT.println("OK:LED_OFF");
      }
      else if (command == "STATUS?") {
        String state = digitalRead(ledPin) ? "ON" : "OFF";
        BT.println("STATUS:" + state);
      }
      else {
        BT.println("ERR:UNKNOWN_CMD");
      }
    }
  }
}
