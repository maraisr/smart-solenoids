#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define DEBUG true

// Water pressure sensor config
#define PRESSURE_SENSOR_PIN 1 // Analog pin
#define PRESSURE_SENSOR_OFFSET 0.488 // volts to offset

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

  radio.openWritingPipe(radioAddress[0]);
}

void loop()
{
  float pressure = getPressure();

  radio.stopListening();
  bool maybeSent = radio.write(&pressure, sizeof(pressure));

  if (DEBUG && !maybeSent) Serial.println("[ERROR]: Failed to send packet");

  delay(500);
}

// Gets the current pressure from the sensor
float getPressure()
{
  float V = analogRead(PRESSURE_SENSOR_PIN) * 5.00 / 1024; // Fancy math here, but think its read * 5vdc / <something>
  float P = (V - PRESSURE_SENSOR_OFFSET) * 40;             // increase this "40" if you wan't to see a "larger" number ie 0.01 -> 0.2

  if (DEBUG)
    Serial.println("[INFO]: Pressure: " + String(P) + " KPa");

  return P;
}
