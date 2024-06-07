/*
 * The L9110H is the H-Bridge driver for the RS PRO Brushed 1.5v - 3v DC Motor 
 * used to open and close the gripper.
 *      PARAMS
 *      pin_1:          positive shaft terminal GPIO pin (default 18 (ESP32))
 *      pin_2:          negative shaft terminal GPIO pin (default 19 (ESP32))
 *      rotor_duration: length of time to turn motor (default 50ms)
 */

#ifndef L9110HMotor_h
#define L9110HMotor_h

#include <Arduino.h>
#include <iostream>

class L9110HMotor {
    private:
        const int pin_1 {18};
        const int pin_2 {19};
        const int enable_1 {16};
        const int rotor_duration {150};
    public:
        L9110HMotor();
        L9110HMotor(const int r_pin1, const int r_pin2);
        L9110HMotor(const int r_pin1, const int r_pin2, const int duration);
        ~L9110HMotor();
        void init() const;
        void open() const;
        void close() const;
        void reset_pins() const;
        void get_vals() const;
        // pulse functionality later
};

#endif