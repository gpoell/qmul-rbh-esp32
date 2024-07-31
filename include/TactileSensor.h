#ifndef TactileSensor_h
#define TactileSensor_h

#include "Arduino.h"
#include "MLX90393.h"
#include "vector3.h"
#include <array>
#include <algorithm>

using namespace std;

struct sensorReadings {
  float x[3] = { 10.00 };
  float y[3] = { 10.00 };
  float z[3] = { 10.00 };
};

struct sensorReadingsNew {
  array<float, 3> x, y, z;
};

class TactileSensor{
  private:
    byte mplxrPin;
    array<MLX90393, 3> sensors = {
      MLX90393(0b0001100), 
  //  MLX90393(0b0001101),
      MLX90393(0b0001110),
      MLX90393(0b0001111)
    };
    const int sensorsSize = sensors.size();
  public:
    TactileSensor(byte mplxrPin);
    void init();
    void calibrate(int nSamples);
    sensorReadings readData();
    bool readData2(sensorReadings&);
    vector3Double readDataAverage();
    vector3Float readDataMaxZ();
    String str();
};

#endif