#ifndef MLX90393_h
#define MLX90393_h

#include "Wire.h"
#include "Arduino.h"
#include "vector3.h"
#include <iostream>

using namespace std;

/* MLX90393 Status Types */
enum mlx90393_status {
  MLX90393_SUCCESS,
  MLX90393_WRITE_FAIL,
  MLX90393_READ_FAIL,
};

/* Mode Commands */
enum {
  MLX90393_SM_XYZ = (0x3E),   // Single Measurement XYZ
  MLX90393_RM_XYZ = (0x4E),   // Read Measurement XYZ
};

/* Datasheet Gain Selection (Default is 7x0) */
struct mlx90393_gain_sel {
  float x = 0.150;
  float y = 0.150;
  float z = 0.242;
};

class MLX90393{
  private:
    byte address;
    float xOffset {0.0};
    float yOffset {0.0};
    float zOffset {0.0};
    mlx90393_gain_sel gain_sel;
  
  public:
    MLX90393(byte address);
    void init();
    vector3 read();
    void calibrate(int nSamples);
    void reset();
    bool startMode(byte command);
    bool checkStatus();
    bool readMeasurement(float *x, float *y, float *z);
    bool readData(float *x, float *y, float *z);
    String str();
};

#endif