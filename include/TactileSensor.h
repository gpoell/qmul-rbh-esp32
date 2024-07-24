#ifndef TactileSensor_h
#define TactileSensor_h

#include "Arduino.h"
#include "MLX90393.h"
#include "vector3.h"
#include <array>

using namespace std;

struct sensorReadings {
  int x[3], y[3], z[3];
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
    int sensorsSize = sensors.size();
  public:
    TactileSensor(byte mplxrPin);
    void init();
    void calibrate(int nSamples);
    sensorReadings readData();
    vector3Double readDataAverage();
    String str();
};

#endif