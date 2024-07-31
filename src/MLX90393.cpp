#include "HardwareSerial.h"
#include "Arduino.h"
#include "MLX90393.h"
#include "vector3.h"

MLX90393::MLX90393(byte address){
  this->address = address;
}

void MLX90393::init(){
  calibrate(5);
};

void MLX90393::calibrate(int nSamples) {
/**
 * Calibrate the sensor by taking the average of the nSamples measured
 * as the XYZ offsets.
 * 
 * @param nSamples  integer number of samples to collect for calibration
 * 
 */

  // Reset baseline so current baseline does not impact readings
  xOffset = 0.0;
  yOffset = 0.0;
  zOffset = 0.0;

  long long int xSum = 0;
  long long int ySum = 0;
  long long int zSum = 0;
  
  for (int i=0; i < nSamples; i++) {
    vector3 data;
    readData(&data.x, &data.y, &data.z);
    xSum += data.x;
    ySum += data.y;
    zSum += data.z;
  };

  xOffset = xSum / nSamples;
  yOffset = ySum / nSamples;
  zOffset = zSum / nSamples;
};

bool MLX90393::checkStatus() {
  /* Checks the Status Byte after starting a mode */

    Wire.requestFrom(address, byte(1));
    int bytesAvailable = Wire.available();
    if (bytesAvailable == 1) {
      byte statusByte = Wire.read();

      //Check for error bit (bit 4) in status byte
      if (statusByte & 0b00010000) {
        cout << "Error in Set Mode request" << endl;
        return false;
      };
    };

    return true;
};

bool MLX90393::readData(float *x, float *y, float *z) {
/**
 * Reads data from sensor and updates x, y, z values
 * 
 * @param x   Pointer to store x value
 * @param y   Pointer to store y value
 * @param z   Pointer to store z value
 * 
 */

  // Start Single Measurement Mode and check status byte
  if (!startMode(MLX90393_SM_XYZ)) return false;
  if (!checkStatus()) return false;

  // Delay 15ms as specified by datasheet -- only for HS command?
  delay(15);

  return readMeasurement(x, y, z);
};

bool MLX90393::readMeasurement(float *x, float *y, float *z) {
/**
 * Reads XYZ Measurements from Sensor (7 bytes)
 * 7 bytes: status, xMSB, xLSB, yMSB, yLSB, zMSB, zLSB
 * 
 * @param x   Pointer to set x value in a vector result array
 * @param y   Pointer to set y value in a vector result array
 * @param z   Pointer to set z value in a vector result array
 * 
 */

  // Start Read Measurement Mode
  if (!startMode(MLX90393_RM_XYZ)) {
    return false;
  };

  Wire.requestFrom(address, byte(7));
  
  // Validate 7 bytes available to raed
  int bytesAvailable = Wire.available(); 
  if (bytesAvailable != 7) return false;

  byte readings[7];
  for (int i = 0;  i < 7; i++) {
    readings[i] = Wire.read();
  };

  // Check the status byte for an error
  if (readings[0] & 0b00010000) {
    Serial.print("Error in RMxyz measurement");
    return false;
  };

  *x = ((readings[1]<<8)|readings[2])*gain_sel.x - xOffset;
  *y = ((readings[3]<<8)|readings[4])*gain_sel.y - yOffset;
  *z = ((readings[5]<<8)|readings[6])*gain_sel.z - zOffset;

  return true;
};

bool MLX90393::startMode(byte command) {
/**
 * Sets the Mode for the MLX90393. Writes the Mode command to the sensor and
 * verifies the status byte response.
 * 
 * @param command   command set specified in 15.1 Command List
 * 
 */

  Wire.beginTransmission(address);
  Wire.write(command);
  int modeStatus = Wire.endTransmission();

  if (modeStatus != 0) {
    cout << "Read SMxyz Transmission Failed, error: " << modeStatus << endl;
    return false;
  };

  return true;
}

void MLX90393::reset(){
  /*Resets the sensor by sending exit (EX) command followed by reset (RT) command.
  
  :return: None
  */
  //Initiate I2C communication and send EXIT then RESET command to sensor
  Wire.beginTransmission(address);
  Wire.write(0x80); //EX command
  int c = Wire.endTransmission();
  if(c != 0){
    Serial.print("EX Transmission Failed, error: ");
    Serial.println(c);
  }

  delay(1);
  
  Wire.beginTransmission(address);
  Wire.write(0xF0); //RT command
  c = Wire.endTransmission();
  
  if(c != 0){
    Serial.print("RT Transmission Failed, error: ");
    Serial.println(c);
  }

  //Request status byte from sensor
  Wire.requestFrom(address, byte(1));

  if(Wire.available() == 1){
    byte statusByte = Wire.read();
    //Check for error bit in status byte
    if(statusByte & 0b00010000){
      Serial.print("Error in RT");
    }
  }

  delay(2);
}

String MLX90393::str(){
  return "Adr: " + String(address, BIN) + 
    "\nOffsets: " + String(xOffset) + ", " + String(yOffset) + ", " + String(zOffset);
}
