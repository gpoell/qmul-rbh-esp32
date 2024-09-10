#include "gripper.h"

Gripper::Gripper() {
	cout << "Calling Gripper constructor..." << endl;
};

Gripper::~Gripper() {
	cout << "Calling Gripper destructor..." << endl;
};

bool Gripper::open() {
    motor.open_gripper();
    delay(motor_duration);
    motor.stop_gripper();
    return true;
};

bool Gripper::close() {
	motor.close_gripper();
    delay(motor_duration);
    motor.stop_gripper();
    return true;
}

vector3 Gripper::getTactileData() {
	return sensor.readDataMaxZ();
}

void Gripper::calibrate() {
	sensor.calibrate(calibrate_sample);
}

void Gripper::send_tactile_data(WiFiClient& client) {
    vector3 data = sensor.readDataMaxZ();
    string message = to_string(data.x) + "," + to_string(data.y) + "," + to_string(data.z) + ",";
    const char* result = create_buffer_message(message, 65);
    client.print(result);
};

const char* create_buffer_message(string& message, int size) {
    int remainder = size - message.length() - 1;
    string filler(remainder, '0');
    message.append(filler);
    const char* p = message.c_str();
    return p;
};