/*
 * Command Prompt displays informative messages to the user based on commands
 * sent by the GUI or the Serial Monitor interface.
 */

#ifndef TactileLogger_h
#define TactileLogger_h

#include <Arduino.h>

class CommandPrompt {
    public:
        CommandPrompt() {};
        ~CommandPrompt() {};
        void prompt();
        void clear();
        void help();
        void invalid();
};

#endif