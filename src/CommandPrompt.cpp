#include "CommandPrompt.h"

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
