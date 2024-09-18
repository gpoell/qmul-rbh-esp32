#include "L9110HMotor.h"

using namespace std;

L9110HMotor::L9110HMotor() {};

L9110HMotor::L9110HMotor(const int p1, const int p2)
    : pin_1(p1), pin_2(p2) {};

L9110HMotor::~L9110HMotor() {};

void L9110HMotor::init() const{
    while (pin_1 != 18 || pin_2 != 19) {
        cerr << "[WARNING] Motor pin values are not supported." << endl;
        delay(3000);
    }
    pinMode(pin_1, OUTPUT);
    pinMode(pin_2, OUTPUT);
};

void L9110HMotor::open_gripper() const {
    cout << "[INFO] Opening gripper..." << endl;
    analogWrite(pin_2, 200);
    analogWrite(pin_1, 0);
};

void L9110HMotor::close_gripper() const {
    cout << "[INFO] Closing gripper..." << endl;
    analogWrite(pin_1, 200);
    analogWrite(pin_2, 0);
};

void L9110HMotor::stop_gripper() const {
    cout << "[INFO] Gripper stopped..." << endl;
    analogWrite(pin_1, 0);
    analogWrite(pin_2, 0);
    digitalWrite(pin_1, LOW);
    digitalWrite(pin_2, LOW);
}