#ifndef TactileLogger_h
#define TactileLogger_h
#include "Arduino.h"

class CommandPrompt {
    private:
        int option;
    public:
        void prompt();
        void help();
        void invalid();
        void clear();
        void stop();
        void readCommand(String cmd);
        void setOption(String cmd);
        int getOption();
};

#endif