/*
 * The L9110H is the H-Bridge driver for the RS PRO Brushed 1.5v - 3v DC Motor 
 * used to open and close the gripper.
 *      
 * Attributes
 *      pin_1:              positive shaft terminal GPIO pin (default 18 (ESP32))
 *      pin_2:              negative shaft terminal GPIO pin (default 19 (ESP32))
 * 
 * Methods
 *      
 *      init:               sets the pins to an output state
 *      open_gripper:       sets pin outputs to low and high to open gripper
 *      close_gripper:      sets pin outputs to high and low to close gripper
 *      stop_gripper:       resets pins to stop motor
 */

#ifndef L9110HMotor_h
#define L9110HMotor_h

#include <Arduino.h>
#include <iostream>

class L9110HMotor {
    private:
        const int pin_1 {18};
        const int pin_2 {19};
    public:
        L9110HMotor();
        L9110HMotor(const int r_pin1, const int r_pin2);
        ~L9110HMotor();
        void init() const;
        void open_gripper() const;
        void close_gripper() const;
        void stop_gripper() const;
};

#endif