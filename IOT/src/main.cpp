// MQTT Topics (data that comes/goes to MQTT broker):
/// EventLog/Device      device events like errors etc         (uploads)
/// sensorData/Device    data from an arbitrary sensor/device  (uploads)
/// devicePayload/Device gives data to esp32 to act on         (downloads)

// the MQTT recieves EventLog and sensorData and all its subtopics and sends that to the MariaDB, it receives the payload from MariaDB
// the website recieves EventLog and sensorData and all its subtopics from MariaDB to the website
// Set a unique identifier for your device before importing comms.h
// red thingo is called a temperature featherwing, figure out how to read the data off of it and put it into the serial monitor

// MQTT client name
// TODO - Change the name to the specific module name.
const char *mqttClient = "ESP32_Lexi"; // This should be unique for each ESP32, e.g: "ESP32_Servo", "ESP32_Piezo", etc

// MQTT Topic
const char *mqttTopic;

#include <Arduino.h>
#include "comms.h"
#include <Wire.h>
#include "Adafruit_ADT7410.h"

// Create the ADT7410 temperature sensor object
Adafruit_ADT7410 tempsensor = Adafruit_ADT7410();


void performActionBasedOnPayload(String payload)
{
    Serial.print("Payload: ");
    Serial.println(payload);
    if ((char)payload[0] == '1')
    {
        Serial.println("LED ON");
        digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
        digitalWrite(LED_BUILTIN, LOW);
    }
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    wifiSetup();
    mqttSetup();
    while (!Serial)
    {
        delay(10);
    }
    delay(1000);

    randomSeed(analogRead(A0));   // Seed using an unconnected analog pin for real randomness

    // Make sure the sensor is found, you can also pass in a different i2c
    // address with tempsensor.begin(0x49) for example
    if (!tempsensor.begin()) {
        Serial.println("Couldn't find ADT7410!");
        while (1);
    }
}

void loop()
{
    // 1. Handle Connection Persistence
    mqttConnect(); // Ensure we are connected to the MQTT broker. If not, this will attempt to reconnect.

    // 2. Generate and send a random number periodically
    int randomNumber = random(1, 2147483647);

    // Read and print out the temperature, then convert to *F
    float tempC = tempsensor.readTempC();
    float tempF = tempC * 9.0 / 5.0 + 32;

    String tempFormatted;
    tempFormatted += tempC; tempFormatted += " C,\t"; tempFormatted += tempF; tempFormatted += " F";

    sendPeriodicUpdate("sensorData", String(tempFormatted));    

    client.loop(); // Check for incoming messages and keep the connection alive
    delay(100);



delay(1000);
}