/*
 * Command Prompt displays informative messages to the user based on commands
 * sent by the GUI or the Serial Monitor interface.
 */

#ifndef CommandPrompt_h
#define CommandPrompt_h

#include <Arduino.h>

class CommandPrompt {
    public:
        CommandPrompt() {};
        ~CommandPrompt() {};
        static void prompt();
        static void clear();
        static void help();
        static void invalid();
};

#endif