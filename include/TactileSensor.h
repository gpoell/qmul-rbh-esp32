#ifndef TactileSensor_h
#define TactileSensor_h

#include "Arduino.h"
#include "MLX90393.h"
#include "vector3.h"
#include <array>
#include <algorithm>

using namespace std;

struct sensorReadings {
  float x[3], y[3], z[3];
};

class TactileSensor{
  public:
    TactileSensor(byte pin);
    TactileSensor(byte pin, array<uint8_t, 3> mlxAddress);
    TactileSensor(const TactileSensor& rhs);
    ~TactileSensor();
    void init();
    void calibrate(int nSamples);
    bool readData(sensorReadings&);
    vector3 readDataMaxZ();
    vector3Double readDataAverage();
    String str();
  private:
    const byte mplxrPin;
    array<MLX90393, 3> sensors = {
      MLX90393(0b0001100),
      // MLX90393(0b0001101), // issue with this taxel chip or magnet
      MLX90393(0b0001110),
      MLX90393(0b0001111)
    };
    const int sensorsSize = sensors.size();

};

#endif