#include "HardwareSerial.h"
#include "Arduino.h"
#include "MLX90393.h"
#include "vector3.h"

MLX90393::MLX90393(byte address){
  this->address = address;
}

void MLX90393::init(){
  calibrate(5);
}

vector3 MLX90393::read(){
  /*Read sensor data. Must be called after calibrate() for accurate measurements.

  :return: vector of measured data in X,Y,Z axes.
  */
  byte readings[7];
  vector3 data;

  // Set Sensor Mode: Single Measurement Mode (3) for XYZ values (E(14))
  Wire.beginTransmission(address);
  Wire.write(0x3E); //SMxyz command
  int modeStatus = Wire.endTransmission();

  if(modeStatus != 0){
    Serial.print("Read SMxyz Transmission Failed, error: ");
    Serial.println(modeStatus);
  }

  // Request status byte from sensor
  Wire.requestFrom(address, byte(1));

  // Read status byte from sensor
  int bytesAvailable = Wire.available();
  if(bytesAvailable == 1) {
    byte statusByte = Wire.read();

    //Check for error bit (bit 4) in status byte
    if(statusByte & 0b00010000){
      Serial.print("Error in SMxyz request");
    }
  };

  // Delay 15ms specified by datasheet
  delay(15);

  // Set Sensor Mode: Read Measurement (4) for XYZ values (E(14))
  Wire.beginTransmission(address);
  Wire.write(0x4E); //RMxyz command
  modeStatus = Wire.endTransmission();

  if (modeStatus != 0) {
    Serial.print("Read RMxyz Transmission Failed, error: ");
    Serial.println(modeStatus);
  }

  // Request Data (7 bytes): status, xMSB, xLSB, yMSB, yLSB, zMSB, zLSB
  Wire.requestFrom(address, byte(7));
  
  bytesAvailable = Wire.available();
  if (bytesAvailable == 7) {
    for (int i = 0;  i < 7; i++) {
      readings[i] = Wire.read();
    };
  };

  //Check for error bit in status byte
  if (readings[0] & 0b00010000) {
    Serial.print("Error in measure RMxyz");
  };

  data.x = ((readings[1]<<8)|readings[2]) - xOffset;
  data.y = ((readings[3]<<8)|readings[4]) - yOffset;
  data.z = ((readings[5]<<8)|readings[6]) - zOffset;

  return data;
}

void MLX90393::calibrate(int nSamples){
  /*Calibrate the sensor by taking the average of the nSamples measured
  as the XYZ offsets.*/

  // Reset baseline so current baseline does not impact readings
  xOffset = 0;
  yOffset = 0;
  zOffset = 0;

  long long int xSum = 0;
  long long int ySum = 0;
  long long int zSum = 0;
  
  for (int i=0; i < nSamples; i++) {
    vector3 data = read();
    xSum += data.x;
    ySum += data.y;
    zSum += data.z;
  };

  xOffset = xSum / nSamples;
  yOffset = ySum / nSamples;
  zOffset = zSum / nSamples;

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
