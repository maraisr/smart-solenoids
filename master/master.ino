#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define DEBUG true

// RF radio config
#define RADIO_CHANNEL 124

// Sets up the rf radio, SPI buss (pins 10, 11, 12, 13, 7, 8)
RF24 radio(7, 8);

byte radioAddress[][6] = {"node1"};

void setup()
{
  if (DEBUG)
    Serial.begin(9600);

  // Setup the rf radio
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.setRetries(15, 15);
  radio.setChannel(RADIO_CHANNEL);

  radio.openReadingPipe(0, radioAddress[0]);
}

void loop() {
  radio.startListening();

  if ( radio.available()) {
    if (DEBUG) Serial.println("[INFO]: Radio here");

    float pressure;
    radio.read(&pressure, sizeof(pressure));

    Serial.println("Pressure " + String(pressure));
  } else {
    if (DEBUG) Serial.println("[ERROR]: No radio");
  }

  delay(500);
}
