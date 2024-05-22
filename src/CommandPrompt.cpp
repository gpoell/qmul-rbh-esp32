#include "CommandPrompt.h"
#include "Arduino.h"

void CommandPrompt::prompt() {
    Serial.println("----------------------------------------------------------------------------------------------------");
    Serial.println("Awaiting command. Type help for list of commands.");
    Serial.println("----------------------------------------------------------------------------------------------------");
}

void CommandPrompt::clear() {
    Serial.println("clearing...[function in progress]");
}

void CommandPrompt::help() {
    Serial.println();
    Serial.println("    >> calibrate:     Runs Sensor Calibration. Takes 1 arg, number of samples, of type int. Default val 150");
    Serial.println("    >> read_sens:     Toggles sensor data collection");
    Serial.println("    >> run_col:       Collects a set amount of data points and writes to serial. Takes 1 arg, number of samples, of type int");
    Serial.println();
};

void CommandPrompt::invalid() {
    Serial.println("Invalid command. Type help for list of commands:");
};

void CommandPrompt::setOption(String cmd) {
    if (cmd.equals("help")) { option = 0; }                     // CMD: help
    else if (cmd.equals("clear")) { option = 1; }               // CMD: clear
    else if (cmd.startsWith("calibrate")) { option = 2; }       // CMD: calibrate
    else if (cmd.equals("read")) { option = 3; }                // CMD: read
    else if (cmd.equals("stop")) { option = 4; }                // CMD: stop
    else if (cmd.startsWith("collect")) { option = 5; }         // CMD: collect
    else { option = 9; }                                        // Default (nothing)
}

int CommandPrompt::getOption() {
    return option;
}