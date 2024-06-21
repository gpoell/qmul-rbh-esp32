#include "L9110HMotor.h"

using namespace std;

L9110HMotor::L9110HMotor() {};

L9110HMotor::L9110HMotor(const int p1, const int p2)
    : pin_1(p1), pin_2(p2) {};

L9110HMotor::L9110HMotor(const int p1, const int p2, const int rd)
    : pin_1(p1), pin_2(p2), rotor_duration(rd) {};

L9110HMotor::~L9110HMotor() {};

void L9110HMotor::init() const{
    while (pin_1 != 18 || pin_2 != 19) {
        cerr << "[WARNING] Motor pin values are not supported." << endl;
        delay(3000);
    }
    pinMode(pin_1, OUTPUT);
    pinMode(pin_2, OUTPUT);
};

void L9110HMotor::open() const {
    cout << "[INFO] Opening gripper..." << endl;
    digitalWrite(pin_1, LOW);
    digitalWrite(pin_2, HIGH);
    reset_pins();
};

void L9110HMotor::close() const {
    cout << "[INFO] Closing gripper..." << endl;
    digitalWrite(pin_1, HIGH);
    digitalWrite(pin_2, LOW);
    reset_pins();
};

void L9110HMotor::reset_pins() const {
    delay(rotor_duration);
    digitalWrite(pin_1, LOW);
    digitalWrite(pin_2, LOW);
}