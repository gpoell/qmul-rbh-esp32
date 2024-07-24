#include "HardwareSerial.h"
#include "Arduino.h"
#include "TactileSensor.h"
#include "vector3.h"

TactileSensor::TactileSensor(byte mplxrPin) {
  this->mplxrPin = mplxrPin;
}

void TactileSensor::init() {
  for (int i = 0; i < sensorsSize; i++) {
    sensors[i].init();
  }
}

void TactileSensor::calibrate(int nSamples) {
    /*Call calibrate method in all 4 MLX90393 chips.

    :param nSamples: Number of samples to be taken when calculating zero offset
    :returns: None
    */
    Serial.print("Calibrating sensor chips with ");
    Serial.print(nSamples);
    Serial.print(" samples:");
    for (int i = 0; i < sensorsSize; i++) {
        Serial.print(" ");
        Serial.print(i + 1);
        sensors[i].calibrate(nSamples);
    }
    Serial.println();
    Serial.println("Sensor calibration is complete...");
    Serial.println();
}

sensorReadings TactileSensor::readData() {
  /*Read measurement data of the 4 attached MLX90393 chips.*/

  sensorReadings readings;
  vector3 rawData;

  for (int i = 0; i < sensorsSize; i++) {
    rawData = sensors[i].read();
    readings.x[i] = rawData.x;
    readings.y[i] = rawData.y;
    readings.z[i] = rawData.z;
  }

  return readings;
}

vector3Double TactileSensor::readDataAverage() {
  /*Calculate data average for X, Y, and Z measurements.*/

  sensorReadings readings = readData();
  vector3Double readingAvg;

  readingAvg.x = (readings.x[0] + readings.x[1] + readings.x[2]) / double(sensorsSize);
  readingAvg.y = (readings.y[0] + readings.y[1] + readings.y[2]) / double(sensorsSize);
  readingAvg.z = (readings.z[0] + readings.z[1] + readings.z[2]) / double(sensorsSize);

  return readingAvg;
}

String TactileSensor::str() {
  return "ID:" + String(mplxrPin);
}