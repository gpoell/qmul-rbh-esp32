#pragma once

#include <array>
#include <iostream>
#include <WiFi.h>
#include "L9110HMotor.h"
#include "TactileSensor.h"

using namespace std;

const char* create_buffer_message(string& message, int size);

class Gripper {
public:
	Gripper();
	~Gripper();
	bool open();
	bool close();
	void calibrate();
	vector3 getTactileData();
    void send_tactile_data(WiFiClient& client);
private:
	L9110HMotor motor;
	TactileSensor sensor{1};
    const int motor_duration = 500;
    const int calibrate_sample = 10;
};