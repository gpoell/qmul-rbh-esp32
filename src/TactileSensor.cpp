#include "HardwareSerial.h"
#include "Arduino.h"
#include "TactileSensor.h"
#include "vector3.h"

TactileSensor::TactileSensor(byte pin = 1)
  : mplxrPin(pin) {};

TactileSensor::TactileSensor(byte pin, array<uint8_t, 3> mlxAddress)
  : mplxrPin(pin),
    sensors {
      MLX90393(mlxAddress[0]),
      MLX90393(mlxAddress[1]),
      MLX90393(mlxAddress[2]),
    }
  {};

TactileSensor::TactileSensor(const TactileSensor& rhs)
  : mplxrPin(rhs.mplxrPin), sensors(rhs.sensors), sensorsSize(rhs.sensorsSize) {};

TactileSensor::~TactileSensor() {};

void TactileSensor::init() {
  for (int i = 0; i < sensorsSize; i++) {
    sensors[i].init();
  }
};

void TactileSensor::calibrate(int nSamples) {
  /*Call calibrate method in all 4 MLX90393 chips.

  :param nSamples: Number of samples to be taken when calculating zero offset
  :returns: None
  */
  cout << "Calibrating sensor chips with " << nSamples << " samples: ";
  for (int i = 0; i < sensorsSize; i++) {
    cout << " " << i + 1;
    sensors[i].calibrate(nSamples);
  };
  cout << endl << "Sensor calibration is complete..." << endl;
};

bool TactileSensor::readData(sensorReadings& readings) {
  /* Read tactile data from each taxel and assign to data pointer */

  for (int i; i<sensorsSize; i++) {
    sensors[i].readData(&readings.x[i], &readings.y[i], &readings.z[i]);
  };

  return true;
};

vector3 TactileSensor::readDataMaxZ() {
  /* Determines the taxel with the highest Z value and returns its values .*/

  sensorReadings readings;
  readData(readings);
  float maxZ = max({readings.z[0], readings.z[1], readings.z[2]});
  vector3 readingMax;

  for (int i=0; i<sensorsSize; i++) {
    if (readings.z[i] == maxZ) {
      readingMax.x = readings.x[i];
      readingMax.y = readings.y[i];
      readingMax.z = readings.z[i];
    };
  };
  
  return readingMax;
};

vector3Double TactileSensor::readDataAverage() {
  /*Calculate data average for X, Y, and Z measurements.*/

  sensorReadings readings;
  readData(readings);
  vector3Double readingAvg;

  readingAvg.x = (readings.x[0] + readings.x[1] + readings.x[2]) / double(sensorsSize);
  readingAvg.y = (readings.y[0] + readings.y[1] + readings.y[2]) / double(sensorsSize);
  readingAvg.z = (readings.z[0] + readings.z[1] + readings.z[2]) / double(sensorsSize);

  return readingAvg;
};

String TactileSensor::str() {
  return "ID:" + String(mplxrPin);
};